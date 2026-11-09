#!/usr/bin/env python3
"""Generate the objdiff report against our objects as retail's linker laid them out.

python3 tools/report_padding.py <objdiff-cli> <objcopy> <build dir> <fill 0|1> <report.json> [report args...]

A unit linked from source is byte for byte retail in the DOL, but objdiff compares
objects, and ours lack what the linker adds: fill_padding.py's '0' bytes in gaps,
each section's padding up to the next object, and the padding dtk counts into the
retail symbol before it or labels on its own. A data section that differs only
there scores below 100 and counts as unmatched, so for the report each object is
copied with that layout applied:
  - zero gaps get fill_padding.py's fill (US and JP only, the main DOL only)
  - each data section is padded to the size of retail's split
  - retail labels pair with our compiler's names at the same offset (symbol_mappings)
  - paired data symbols get the retail symbol's size, retail labels nothing of ours
    starts at get the same label in our copy, so objdiff compares each retail
    symbol's bytes with ours at the same offset, however the two sides divide them
Only zero bytes are ever filled, and objdiff still compares every byte.
"""
import json
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from fill_padding import FILL, SECTIONS, fills_inside  # noqa: E402

DATA = {".rodata", ".data", ".sdata", ".sdata2", ".bss", ".sbss", ".sbss2", "extab", "extabindex"}
PAD = 0x20  # fill_padding.py only ever fills gaps below this


def parse(path: Path):
    """section name -> (header, bytes or None), (section, symbol) -> (value, size, symtab entry offset), raw"""
    data = path.read_bytes()
    shoff = struct.unpack_from(">I", data, 32)[0]
    stride, count, shstr = struct.unpack_from(">HHH", data, 46)
    headers = [struct.unpack_from(">10I", data, shoff + i * stride) for i in range(count)]
    names_at = headers[shstr][4]
    names = [data[names_at + h[0]:data.index(0, names_at + h[0])].decode() for h in headers]
    symbols = {}
    for h in headers:
        if h[1] != 2:
            continue
        strtab = headers[h[6]][4]
        for entry in range(h[4], h[4] + h[5], h[9]):
            name, value, size, info, _, shndx = struct.unpack_from(">IIIBBH", data, entry)
            if size and 0 < shndx < count and info & 15 != 3:
                symbol = data[strtab + name:data.index(0, strtab + name)].decode(errors="replace")
                symbols[(names[shndx], symbol)] = (value, size, entry)
    sections = {n: (h, data[h[4]:h[4] + h[5]] if h[1] != 8 else None) for n, h in zip(names, headers)}
    return sections, symbols, data


def symbol_entries(data: bytes) -> dict:
    """every symbol name -> its symtab entry offset, size 0 included"""
    shoff = struct.unpack_from(">I", data, 32)[0]
    stride, count, _ = struct.unpack_from(">HHH", data, 46)
    headers = [struct.unpack_from(">10I", data, shoff + i * stride) for i in range(count)]
    out = {}
    for h in headers:
        if h[1] != 2:
            continue
        strtab = headers[h[6]][4]
        for entry in range(h[4], h[4] + h[5], h[9]):
            name = struct.unpack_from(">I", data, entry)[0]
            out[data[strtab + name:data.index(0, strtab + name)].decode(errors="replace")] = entry
    return out


def spans(symbols, section):
    return sorted((v[0], v[0] + v[1]) for (s, _), v in symbols.items() if s == section)


def pad_object(objcopy: str, ours: Path, target: Path, rule: str | None, fill: bool, out: Path) -> dict:
    """Write the padded copy of ours to out, return the retail -> ours symbol mapping."""
    sections, mine, raw = parse(ours)
    retail_sections, theirs, _ = parse(target)
    retail_size = {n: h[5] for n, (h, _) in retail_sections.items()}
    compiled = dict(mine)  # the layout our compiler emitted, the linker pads around this

    by_offset = {(s, v[0]): n for (s, n), v in mine.items() if (s, n) not in theirs}
    our_names = {n for _, n in mine}
    pairs, mapping = {}, {}
    for (s, n), v in theirs.items():
        if s not in DATA:
            continue
        if (s, n) in mine:
            pairs[(s, n)] = n
        elif n not in our_names and (s, v[0]) in by_offset:
            pairs[(s, n)] = mapping[n] = by_offset[(s, v[0])]

    raw = bytearray(raw)
    for (s, n), o in pairs.items():
        value, size, entry = mine[(s, o)]
        retail_value, retail_symbol_size, _ = theirs[(s, n)]
        end = value + retail_symbol_size
        if retail_value == value and retail_symbol_size != size and end <= retail_size.get(s, 0):
            struct.pack_into(">I", raw, entry + 8, retail_symbol_size)
            mine[(s, o)] = (value, retail_symbol_size, entry)
    out.write_bytes(raw)

    args = []
    temp = Path(tempfile.mkdtemp())
    for name in SECTIONS - {".text"}:
        if name not in sections or name not in retail_sections or sections[name][1] is None:
            continue
        content = bytearray(sections[name][1])
        want = retail_size[name]
        layout = spans(compiled, name)
        last = max((e for _, e in layout), default=0)
        if fill and rule and fills_inside(rule, name, len(content)):
            for (_, a_end), (b_start, _) in zip(layout, layout[1:]):
                if 0 < b_start - a_end < PAD and not any(content[a_end:b_start]):
                    content[a_end:b_start] = bytes([FILL]) * (b_start - a_end)
        # a split inside a retail string pool keeps the last string's zero-filled word
        if name in (".data", ".rodata") and rule == f"small_data+{name[1:]}_word_tail":
            last = (last + 3) & ~3
        if want > len(content):
            content += bytes(want - len(content))
        if fill and 0 < want - last < PAD and not any(content[last:want]):
            content[last:want] = bytes([FILL]) * (want - last)
        if content != sections[name][1]:
            path = temp / name.strip(".")
            path.write_bytes(content)
            args += ["--update-section", f"{name}={path}"]

    added = {}
    if ".comment" in sections:
        for (s, n), (value, size, _) in theirs.items():
            if s not in DATA or (s, n) in pairs or s not in sections:
                continue
            if value + size <= retail_size.get(s, 0):
                # global, so objcopy appends it after every existing symbol and keeps their indices
                args += ["--add-symbol", f"{n}={s}:{value:#x},global,object"]
                added[n] = size
    if added:
        # MWCC's .comment has one 8-byte entry per symbol, add ours (alignment 1)
        path = temp / "comment"
        path.write_bytes(sections[".comment"][1] + b"\0\0\0\1\0\0\0\0" * len(added))
        args += ["--update-section", f".comment={path}"]
    if args:
        subprocess.run([objcopy, *args, str(out)], check=True)
    if added:
        data = bytearray(out.read_bytes())
        entries = symbol_entries(data)
        for n, size in added.items():
            struct.pack_into(">I", data, entries[n] + 8, size)
        out.write_bytes(data)
    return mapping


def main() -> None:
    objdiff, objcopy, build_dir, fill, report = sys.argv[1:6]
    root = Path.cwd()
    build = root / build_dir
    project = json.loads((root / "objdiff.json").read_text())
    rules = json.loads((build / "source_compilers.json").read_text())
    copies = build / "report_objs"

    def rule_for(base: Path) -> str | None:
        stem = base.name[:-2]
        for name in (base.name, f"{stem}.aligned.o", f"{stem}.text.o", f"{stem}.text.aligned.o"):
            if rules.get(name):
                return rules[name]
        return None

    def copy(unit: dict) -> None:
        base, target = unit.get("base_path"), unit.get("target_path")
        for key in ("base_path", "target_path"):
            if unit.get(key):
                unit[key] = str(root / unit[key])
        if not base or not target or not (root / base).is_file() or not (root / target).is_file():
            return
        # units can share one base object against different retail splits
        out = copies / f"{unit['name']}.o"
        out.parent.mkdir(parents=True, exist_ok=True)
        dol = unit["name"].startswith("main/")
        mapping = pad_object(objcopy, root / base, root / target, rule_for(Path(base)) if dol else None,
                             fill == "1" and dol, out)
        unit["base_path"] = str(out)
        if mapping:
            unit["symbol_mappings"] = {**unit.get("symbol_mappings", {}), **mapping}

    with ThreadPoolExecutor() as pool:
        list(pool.map(copy, project["units"]))
    project_dir = build / "report_project"
    project_dir.mkdir(parents=True, exist_ok=True)
    (project_dir / "objdiff.json").write_text(json.dumps(project))
    subprocess.run([objdiff, "report", "generate", "-p", str(project_dir), *sys.argv[6:], "-o", report], check=True)


if __name__ == "__main__":
    main()
