"""Extract one native MWCC code section while keeping its PowerPC relocations."""

import struct
import subprocess
import sys
import tempfile
from pathlib import Path


def extract(objcopy: str, name: str, source: Path, output: Path, bss_anchor: str | None = None) -> None:
    data = bytearray(source.read_bytes())
    if data[:6] != b"\x7fELF\x01\x02" or struct.unpack_from(">HH", data, 16) != (1, 20):
        raise ValueError(f"{source}: expected a big-endian PowerPC ELF32 relocatable object")
    section_offset = struct.unpack_from(">I", data, 32)[0]
    stride, count, names_index = struct.unpack_from(">HHH", data, 46)
    headers = [struct.unpack_from(">10I", data, section_offset + i * stride) for i in range(count)]
    names = headers[names_index][4]
    section_names = [data[names + h[0]:data.index(0, names + h[0])].decode() for h in headers]
    target = section_names.index(name)
    if headers[target][1] != 1 or not headers[target][2] & 4 or not headers[target][5]:
        raise ValueError(f"{name}: expected a nonempty code section")

    # The callee's vtables can live in the same compiler object but be supplied
    # by a different link unit. Keep the symbol and addend on each relocation.
    external = set()
    for _, kind, _, _, offset, size, symbols, relocated, _, entry_size in headers:
        if relocated != target or kind not in (4, 9):  # SHT_RELA, SHT_REL
            continue
        symtab = headers[symbols]
        strings = headers[symtab[6]][4]

        def symbol_name(entry):
            begin = strings + struct.unpack_from(">I", data, entry)[0]
            return data[begin:data.index(0, begin)].decode()

        anchor = None
        if bss_anchor:
            for index in range(symtab[5] // symtab[9]):
                entry = symtab[4] + index * symtab[9]
                value, _, info, _, section = struct.unpack_from(">IIBBH", data, entry + 4)
                if (section < len(headers) and section_names[section] == ".bss"
                        and info >> 4 in (1, 2) and symbol_name(entry) == bss_anchor):
                    anchor = (index, entry, value, section)
                    break
            if anchor is None:
                raise ValueError(f"{name}: missing global BSS anchor {bss_anchor}")

        for pos in range(offset, offset + size, entry_size):
            reloc = struct.unpack_from(">I", data, pos + 4)[0]
            entry = symtab[4] + (reloc >> 8) * symtab[9]
            value, _, info, _, section = struct.unpack_from(">IIBBH", data, entry + 4)
            if section in (0, target, 0xFFF1):  # undefined, retained or absolute
                continue
            if info >> 4 not in (1, 2):
                # A split reader still addresses the original BSS pool through a public member.
                if not (anchor and kind == 4 and section == anchor[3] and symbol_name(entry).startswith("...bss.")
                        and (reloc & 0xff) in (4, 6)):
                    raise ValueError(f"{name}: relocation to a local symbol in a different section")
                addend = struct.unpack_from(">i", data, pos + 8)[0]
                struct.pack_into(">Ii", data, pos + 4, (anchor[0] << 8) | (reloc & 0xff), addend + value - anchor[2])
                entry = anchor[1]
            external.add(entry)
    for entry in external:
        struct.pack_into(">II", data, entry + 4, 0, 0)
        struct.pack_into(">H", data, entry + 14, 0)

    with tempfile.TemporaryDirectory(dir=output.parent) as temp:
        obj = Path(temp) / "source.o"
        result = Path(temp) / "text.o"
        obj.write_bytes(data)
        subprocess.run([objcopy, "-j", name, "-j", "*UND*", "-j", "*ABS*",
                        "--rename-section", f"{name}=.text", obj, result], check=True)
        result.replace(output)


if __name__ == "__main__":
    extract(sys.argv[1], sys.argv[2], Path(sys.argv[3]), Path(sys.argv[4]), *sys.argv[5:])
