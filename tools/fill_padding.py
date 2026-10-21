#!/usr/bin/env python3
"""Put retail's ASCII '0' padding back into a linked DOL.

python3 tools/fill_padding.py <main.dol> <main.elf.MAP> [source_compilers.json]

Our linker pads with zeros, retail used 0x30, so any unit linked from source
breaks the hash without this. Retail fills:
  - alignment gaps between objects, in .text and the data sections
  - gaps between symbols inside an object, depending on how it was built.
    configure.py's "padding" option says which, otherwise it goes by compiler
    (GC/1.2.5n is small_data, everything else small_sections):
      small_sections     any section 0x80 or smaller (MSL, Runtime, TRK)
      small_data         .sdata and .sdata2 only (SDK, HSD)
      small_data+data    plus .data (a few HSD objects)
      small_data+rodata  plus .rodata (tobj)
  - every gap in extab, there's only two, after a table in Gecko_ExceptionPPC

PAL doesn't do any of this, config.fill_padding turns it off there. Only
zero bytes in our own DOL get touched, nothing comes from the retail one.
"""
import json
import re
import struct
import sys
from pathlib import Path

# sections where retail shows the fill, add more only with evidence
SECTIONS = {".text", ".rodata", ".data", ".sdata", ".sdata2", "extab"}
FILL = 0x30
# small_sections: biggest section that still gets 0x30 inside
SMALL_SECTION = 0x80
SMALL_DATA = {".sdata", ".sdata2"}


def fills_inside(rule: str, section: str, size: int) -> bool:
    if section == "extab":
        return True
    if rule == "GC/1.2.5n":
        rule = "small_data"
    elif rule.startswith("GC/"):
        rule = "small_sections"
    if rule == "small_sections":
        return size <= SMALL_SECTION
    if rule == "small_data":
        return section in SMALL_DATA
    if rule == "small_data+data":
        return section in SMALL_DATA or section == ".data"
    if rule == "small_data+rodata":
        return section in SMALL_DATA or section == ".rodata"
    sys.exit(f"fill_padding: unknown padding rule {rule}")


RE_LAYOUT = re.compile(r"^(\S+) section layout$")
RE_ENTRY = re.compile(r"^  [0-9a-f]{8} ([0-9a-f]{6,8}) ([0-9a-f]{8}) +\d+ (\S+) \t(\S+)")


def object_ranges(map_text: str):
    """section -> [(start, end, object, [(sym_start, sym_end)])] for each object's piece of that section."""
    out = {}
    current = None
    for line in map_text.splitlines():
        m = RE_LAYOUT.match(line)
        if m:
            current = m.group(1)
            continue
        if current not in SECTIONS:
            continue
        m = RE_ENTRY.match(line)
        if not m:
            continue
        size, addr, name, obj = int(m.group(1), 16), int(m.group(2), 16), m.group(3), m.group(4)
        # an object's section entry carries the section name as its symbol,
        # its symbols follow it
        if name == current:
            out.setdefault(current, []).append((addr, addr + size, obj, []))
        elif out.get(current) and out[current][-1][2] == obj and size:
            out[current][-1][3].append((addr, addr + size))
    return out


def main() -> None:
    dol_path, map_path = Path(sys.argv[1]), Path(sys.argv[2])
    compilers = {}
    if len(sys.argv) > 3:
        compilers = json.loads(Path(sys.argv[3]).read_text())
    dol = bytearray(dol_path.read_bytes())
    offs = struct.unpack(">18I", dol[0:72])
    addrs = struct.unpack(">18I", dol[72:144])
    sizes = struct.unpack(">18I", dol[144:216])

    def file_offset(addr: int) -> int:
        for i in range(18):
            if sizes[i] and addrs[i] <= addr < addrs[i] + sizes[i]:
                return offs[i] + addr - addrs[i]
        raise ValueError(f"0x{addr:08X} is outside the DOL")

    filled = 0

    def fill(start: int, end: int, what: str) -> None:
        nonlocal filled
        o = file_offset(start)
        gap = dol[o : o + end - start]
        if any(gap):
            sys.exit(f"fill_padding: nonzero padding {what} at 0x{start:08X}")
        dol[o : o + end - start] = bytes([FILL]) * len(gap)
        filled += 1

    for section, ranges in object_ranges(map_path.read_text(errors="replace")).items():
        ranges.sort()
        # rule 1, between objects. the linker only pads up to the next object's alignment.
        # the map gives an object's size before dead stripping, so its last
        # symbol is where it really ends
        for (_, end, obj, syms), (start, _, _, _) in zip(ranges, ranges[1:]):
            if syms:
                end = max(e for _, e in syms)
            if 0 < start - end < 0x20:
                fill(end, start, f"after {obj} in {section}")
        # rule 2, inside objects built from source
        for start, end, obj, syms in ranges:
            compiler = compilers.get(obj)
            if not compiler or not fills_inside(compiler, section, end - start):
                continue
            syms.sort()
            for (_, a_end), (b_start, _) in zip(syms, syms[1:]):
                if 0 < b_start - a_end < 0x20:
                    fill(a_end, b_start, f"inside {obj} {section}")
    dol_path.write_bytes(dol)
    if filled:
        print(f"fill_padding: filled {filled} gap(s)")


if __name__ == "__main__":
    main()
