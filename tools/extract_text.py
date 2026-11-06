"""Extract one native MWCC code section while keeping its PowerPC relocations."""

import argparse
import struct
import subprocess
import tempfile
from pathlib import Path


def extract(objcopy: str, name: str, source: Path, output: Path, bss_anchor: str | None = None,
            literals: dict[bytes, str] | None = None, strong_functions: bool = False, constructor: str | None = None,
            keep_data: bool = False, array_dtor: str | None = None, keep_extab: bool = False, data_prefix: int | None = None,
            rodata_objects: list[tuple[str, int]] | None = None, data_range: tuple[int, int] | None = None, keep_bss: bool = False,
            data_objects: list[tuple[str, int]] | None = None, zero_word: str | None = None, keep_rodata: bool = False,
            external_data: tuple[int, int, str] | None = None, rtti_symbols: dict[str, str] | None = None, data_alignment: int | None = None) -> None:
    data = bytearray(source.read_bytes())
    if data[:6] != b"\x7fELF\x01\x02" or struct.unpack_from(">HH", data, 16) != (1, 20):
        raise ValueError(f"{source}: expected a big-endian PowerPC ELF32 relocatable object")
    section_offset = struct.unpack_from(">I", data, 32)[0]
    stride, count, names_index = struct.unpack_from(">HHH", data, 46)
    headers = [struct.unpack_from(">10I", data, section_offset + i * stride) for i in range(count)]
    names = headers[names_index][4]
    section_names = [data[names + h[0]:data.index(0, names + h[0])].decode() for h in headers]
    if constructor and ".ctors" not in section_names:
        raise ValueError("Missing static constructor table")
    target = section_names.index(name)
    data_only = name in (".data", ".rodata", ".sdata", ".sdata2") and data_range is not None
    range_section = name if data_only else ".data"
    if headers[target][1] != 1 or not (data_only or headers[target][2] & 4) or not headers[target][5]:
        raise ValueError(f"{name}: expected a nonempty code section")

    if data_prefix is not None:
        if data_range is not None:
            raise ValueError("Conflicting data ranges")
        data_range = (0, data_prefix)
    retained = {target}
    if keep_rodata:
        retained.add(section_names.index(".rodata"))
    if keep_bss:
        retained.add(section_names.index(".bss"))
    if keep_data:
        retained.add(section_names.index(".data"))
    if data_range is not None:
        retained.add(section_names.index(range_section))

    object_args = []
    if data_objects and not data_only:
        raise ValueError("Named data objects require a data-only unit")
    for section_name, layout in ((".rodata", rodata_objects), (range_section, data_objects)):
        if not layout:
            continue
        # Keep distinct addresses for identical compiler-generated initializers.
        section = section_names.index(section_name)
        sym = next(h for h in headers if h[1] == 2)
        objects = []
        for entry in range(sym[4], sym[4] + sym[5], sym[9]):
            string, value, size, info, _, owner = struct.unpack_from(">IIIBBH", data, entry)
            if owner == section and info & 15 == 1:
                if data_range is not None and section_name == range_section and not data_range[0] <= value < data_range[1]:
                    continue
                objects.append((value, size, string, entry))
        objects.sort()
        # MWCC emits a separate anonymous constant for each aggregate element.
        # A retail symbol may span several such initializers.
        if data_only and section_name == ".rodata" and len(layout) == 1 and len(objects) > 1:
            cursor, end = data_range
            for value, size, string, entry in objects:
                start = headers[sym[6]][4] + string
                local_name = data[start:data.index(0, start)].decode()
                if (value != cursor or not size or data[entry + 12] != 1
                        or not local_name.startswith("@") or not local_name[1:].isdigit()):
                    raise ValueError("Unexpected initializer in pooled rodata")
                cursor += size
                if value != data_range[0]:
                    object_args.extend(["--strip-symbol", local_name])
            if cursor != end or layout[0][1] != end - data_range[0]:
                raise ValueError("Unexpected pooled rodata size")
            value, _, string, entry = objects[0]
            struct.pack_into(">I", data, entry + 8, layout[0][1])
            objects = [(value, layout[0][1], string, entry)]
        if [size for _, size, _, _ in objects] != [size for _, size in layout]:
            raise ValueError(f"Unexpected {section_name} object layout")
        retained.add(section)
        object_args.extend(["-j", section_name])
        for (_, _, string, entry), (symbol, _) in zip(objects, layout):
            # objcopy globalizes local symbols but leaves weak vtables weak.
            if data[entry + 12] == 0x21:
                data[entry + 12] = 0x11
            start = headers[sym[6]][4] + string
            old = data[start:data.index(0, start)].decode()
            if old != symbol:
                object_args.extend(["--redefine-sym", f"{old}={symbol}"])
            object_args.extend(["--globalize-symbol", symbol])

    if keep_extab:
        retained.update(section_names.index(section) for section in ("extab", "extabindex"))

    # The callee's vtables can live in the same compiler object but be supplied
    # by a different link unit. Keep the symbol and addend on each relocation.
    external = set()
    pooled = {}
    if array_dtor:
        # MWCC emits the array finalizer directly after the initializer. The original
        # linker placed it elsewhere; its reconstructed implementation owns that slot.
        symtab = next(h for h in headers if h[1] == 2)
        strings = headers[symtab[6]][4]
        functions = []
        for entry in range(symtab[4], symtab[4] + symtab[5], symtab[9]):
            offset, value, length, info, _, section = struct.unpack_from(">IIIBBH", data, entry)
            if section == target and info & 15 == 2:
                start = strings + offset
                symbol = data[start:data.index(0, start)].decode()
                functions.append((value, length, symbol, entry))
        functions.sort()
        if len(functions) < 2:
            raise ValueError("Expected exactly an initializer and an array finalizer")
        (start, size, init, _), (boundary, tail, finalizer, entry) = functions[:2]
        if (init != constructor or start or not size or size != boundary or not tail
                or not finalizer.startswith("__arraydtor$")
                or not finalizer[len("__arraydtor$"):].isdigit()):
            raise ValueError("Unexpected initializer or array finalizer layout")
        # MWCC may append weak inline helpers also supplied by the owning link unit.
        end = boundary + tail
        for offset, length, _, helper in functions[2:]:
            if offset != end or not length or data[helper + 12] != 0x22:
                raise ValueError("Unexpected function after the array finalizer")
            external.add(helper)
            end += length
        if end != headers[target][5]:
            raise ValueError("Unexpected bytes after the array finalizer")
        pooled[finalizer] = array_dtor
        external.add(entry)
        for index, h in enumerate(headers):
            if index == target:
                length = boundary
            elif h[1] in (4, 9) and h[7] == target:
                records = [data[pos:pos + h[9]] for pos in range(h[4], h[4] + h[5], h[9])
                           if struct.unpack_from(">I", data, pos)[0] < boundary]
                kept = b"".join(records)
                length = len(kept)
                data[h[4]:h[4] + length] = kept
            else:
                continue
            struct.pack_into(">I", data, section_offset + index * stride + 20, length)
            headers[index] = (*h[:5], length, *h[6:])
    strip_symbols = set()
    discarded_data = set()
    if data_range is not None:
        # Preserve symbol and relocation offsets relative to the selected data.
        section = section_names.index(range_section)
        begin, end = data_range
        # A final string may end before the next word boundary.
        unaligned_end = end % 4 and not (data_only and end == headers[section][5])
        if begin % 4 or unaligned_end or not 0 <= begin < end <= headers[section][5]:
            raise ValueError("Invalid retained data range")
        sym = next(h for h in headers if h[1] == 2)
        strings = headers[sym[6]][4]
        for entry in range(sym[4], sym[4] + sym[5], sym[9]):
            string, value, length, info, _, owner = struct.unpack_from(">IIIBBH", data, entry)
            if owner != section:
                continue
            if info & 15 == 3:
                continue
            if value < begin < value + length or value < end < value + length:
                raise ValueError("Retained data range cuts a symbol")
            if begin <= value < end:
                struct.pack_into(">I", data, entry + 4, value - begin)
            else:
                # Weak definitions (such as inline vtables) can be emitted by other units.
                if info >> 4 not in (0, 2) and not begin and not data_only:
                    raise ValueError("Unexpected public symbol in discarded data")
                discarded_data.add(entry)
                start = strings + string
                strip_symbols.add(data[start:data.index(0, start)].decode())
        for i, h in enumerate(headers):
            if i == section:
                length = end - begin
                data[h[4]:h[4] + length] = data[h[4] + begin:h[4] + end]
            elif h[1] in (4, 9) and h[7] == section:
                records = bytearray()
                for pos in range(h[4], h[4] + h[5], h[9]):
                    off = struct.unpack_from(">I", data, pos)[0]
                    if begin <= off < end:
                        records.extend(struct.pack(">I", off - begin) + data[pos + 4:pos + h[9]])
                length = len(records)
                data[h[4]:h[4] + length] = records
            else:
                continue
            struct.pack_into(">I", data, section_offset + i * stride + 20, length)
            headers[i] = (*h[:5], length, *h[6:])
    if keep_extab and name != ".text":
        # Keep unwind records for the selected code, excluding trailing inline functions.
        table, index = (section_names.index(n) for n in ("extab", "extabindex"))
        rel = next(h for h in headers if h[1] == 4 and h[7] == index)
        sym = headers[rel[6]]
        refs = {}
        for pos in range(rel[4], rel[4] + rel[5], rel[9]):
            off, info, addend = struct.unpack_from(">IIi", data, pos)
            if info & 255 != 1 or addend or off in refs:
                raise ValueError("Unexpected unwind index relocation")
            refs[off] = struct.unpack_from(">IIIBBH", data, sym[4] + (info >> 8) * sym[9])
        size = headers[index][5]
        if size % 12 or set(refs) != {i + delta for i in range(0, size, 12) for delta in (0, 8)}:
            raise ValueError("Incomplete unwind index")
        # MWCC can interleave unwind records for inline copies from other sections.
        spans = {index: [], table: []}
        cursor = 0
        for off in range(0, size, 12):
            func, unwind = refs[off], refs[off + 8]
            if unwind[5] != table or unwind[1] != cursor or not unwind[2]:
                raise ValueError("Unexpected unwind table layout")
            if func[5] == target:
                spans[index].append((off, off + 12))
                spans[table].append((cursor, cursor + unwind[2]))
            cursor += unwind[2]
        if not spans[index] or cursor != headers[table][5]:
            raise ValueError("Incomplete unwind table")

        def compact_offset(section, value):
            dest = 0
            for begin, end in spans[section]:
                if begin <= value < end:
                    return dest + value - begin
                dest += end - begin
            return None

        for entry in range(sym[4], sym[4] + sym[5], sym[9]):
            string, value, _, info, _, section = struct.unpack_from(">IIIBBH", data, entry)
            if section not in spans:
                continue
            new_value = compact_offset(section, value)
            if new_value is not None:
                struct.pack_into(">I", data, entry + 4, new_value)
            elif info == 1:
                start = headers[sym[6]][4] + string
                strip_symbols.add(data[start:data.index(0, start)].decode())
            elif info != 3 or value != 0:
                raise ValueError("Unexpected symbol in discarded unwind records")
        for i, h in enumerate(headers):
            if i in spans:
                kept = b"".join(data[h[4] + begin:h[4] + end] for begin, end in spans[i])
                length = len(kept)
                data[h[4]:h[4]+length] = kept
            elif h[1] in (4, 9) and h[7] in spans:
                records = bytearray()
                for pos in range(h[4], h[4] + h[5], h[9]):
                    new_off = compact_offset(h[7], struct.unpack_from(">I", data, pos)[0])
                    if new_off is not None:
                        records.extend(struct.pack(">I", new_off) + data[pos + 4:pos + h[9]])
                length = len(records)
                data[h[4]:h[4]+length] = records
            else:
                continue
            struct.pack_into(">I", data, section_offset + i * stride + 20, length)
            headers[i] = (*h[:5], length, *h[6:])
    relocation_offsets = {}
    for h in headers:
        if h[1] in (4, 9):
            relocation_offsets.setdefault(h[7], []).extend(
                struct.unpack_from(">I", data, pos)[0] for pos in range(h[4], h[4] + h[5], h[9])
            )
    external_data_entry = None
    zero_entry = None
    for _, kind, _, _, offset, size, symbols, relocated, _, entry_size in headers:
        if relocated not in retained or kind not in (4, 9):  # SHT_RELA, SHT_REL
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

        # MWCC addresses a shared data pool through its local base symbol.
        # A public object at that exact address remains linkable from a split reader.
        data_bases = {}
        for index in range(symtab[5] // symtab[9]):
            entry = symtab[4] + index * symtab[9]
            value, length, info, _, section = struct.unpack_from(">IIBBH", data, entry + 4)
            if section < len(headers) and section_names[section] in (".data", ".rodata") and info == 0x11 and length:
                data_bases.setdefault((section, value, entry in discarded_data), []).append((index, entry))

        for pos in range(offset, offset + size, entry_size):
            reloc = struct.unpack_from(">I", data, pos + 4)[0]
            entry = symtab[4] + (reloc >> 8) * symtab[9]
            value, length, info, _, section = struct.unpack_from(">IIBBH", data, entry + 4)
            if keep_extab and section_names[relocated] == "extabindex" and section not in (target, section_names.index("extab")):
                raise ValueError("Exception index refers outside retained code and exception tables")
            if info >> 4 == 0 and (section not in retained or entry in discarded_data):
                if kind == 4 and symbol_name(entry) in ("...data.0", "...rodata.0") and (reloc & 0xff) in (4, 6):
                    bases = data_bases.get((section, value, entry in discarded_data), [])
                    if len(bases) == 1:
                        index, public_entry = bases[0]
                        struct.pack_into(">I", data, pos + 4, (index << 8) | (reloc & 0xff))
                        strip_symbols.discard(symbol_name(public_entry))
                        external.add(public_entry)
                        continue
            if data_range is not None and section < len(headers) and section_names[section] == range_section:
                if entry in discarded_data:
                    if info == 0x11 or info >> 4 == 2 or entry in external:
                        # A retained reader needs the canonical definition from the unit owning this object.
                        external.add(entry)
                        strip_symbols.discard(symbol_name(entry))
                        data[entry + 12] = 0x10 | (info & 15)
                        continue
                    raise ValueError("Relocation references discarded data symbol")
                if kind == 4 and begin and info & 15 == 3:
                    addend = struct.unpack_from(">i", data, pos + 8)[0]
                    struct.pack_into(">i", data, pos + 8, addend - begin)
                if kind != 4 or not 0 <= value + struct.unpack_from(">i", data, pos + 8)[0] < end - begin:
                    raise ValueError("Relocation outside retained data prefix")
            if section in retained or section in (0, 0xFFF1):  # undefined, retained or absolute
                continue
            if info >> 4 not in (1, 2):
                # The compiler emits a local table whose definition belongs to a separate data unit.
                if (external_data and info == 1 and section_names[section] == ".data"
                        and (value, length) == external_data[:2] and kind == 4 and (reloc & 255) in (4, 6)
                        and struct.unpack_from(">i", data, pos + 8)[0] == 0):
                    if external_data_entry is not None and external_data_entry != entry:
                        raise ValueError("Ambiguous external data object")
                    external_data_entry = entry
                    pooled[symbol_name(entry)] = external_data[2]
                    external.add(entry)
                    continue
                if (zero_word and kind == 4 and (reloc & 255) == 109 and info == 1 and length == 4
                        and section_names[section] == ".sbss2" and headers[section][1] == 8
                        and symbol_name(entry).startswith("@") and symbol_name(entry)[1:].isdigit()
                        and struct.unpack_from(">i", data, pos + 8)[0] == 0):
                    if zero_entry is not None and entry != zero_entry:
                        raise ValueError("Ambiguous four-byte literal in .sbss2")
                    zero_entry = entry
                    pooled[symbol_name(entry)] = zero_word
                    external.add(entry)
                    continue
                # A split MWCC vtable uses the canonical RTTI descriptor emitted by its type-info unit.
                rtti = symbol_name(entry)
                if (data_only and section_names[section] == ".sdata"
                        and info == 1 and length == 8 and rtti.startswith("__RTTI__") and kind == 4
                        and (reloc & 255) == 1 and struct.unpack_from(">i", data, pos + 8)[0] == 0):
                    if rtti_symbols and rtti in rtti_symbols:
                        pooled[rtti] = rtti_symbols[rtti]
                    data[entry + 12] = 0x11
                    external.add(entry)
                    continue
                # Pool only explicitly mapped, relocation-free MWCC literals, including their full size.
                if (literals and section < len(headers)
                        and info == 1 and kind == 4
                        and ((section_names[section] in (".rodata", ".data") and (reloc & 0xff) in (4, 6))
                             or (section_names[relocated] == ".data" and section_names[section] == ".rodata" and (reloc & 0xff) == 1)
                             or (section_names[section] in (".sdata", ".sdata2") and (reloc & 0xff) == 109))
                        and 0 <= struct.unpack_from(">i", data, pos + 8)[0] < length
                        and not any(value - 3 <= off < value + length for off in relocation_offsets.get(section, ()))):
                    size = struct.unpack_from(">I", data, entry + 8)[0]
                    start = headers[section][4] + value
                    original = symbol_name(entry)
                    numbered = original.startswith("@") and original[1:].isdigit()
                    function_name = original.startswith("__FUNCTION__$") and original.removeprefix("__FUNCTION__$").isdigit()
                    if (numbered or function_name) and 0 < size <= headers[section][5] - value:
                        shared = literals.get(bytes(data[start:start + size]))
                        if shared:
                            pooled[original] = shared
                            external.add(entry)
                            continue
                # A split reader still addresses the original BSS pool through a public member.
                local = symbol_name(entry)
                # A namespace-scope object has its 12-byte destructor node immediately before it.
                dtor_node = (constructor and anchor and info == 1 and length == 12 and value + length == anchor[2]
                             and local.startswith("@") and local[1:].isdigit())
                if not (anchor and kind == 4 and section == anchor[3] and (local.startswith("...bss.") or dtor_node)
                        and (reloc & 0xff) in (4, 6)):
                    raise ValueError(f"{name}: relocation to a local symbol in a different section")
                addend = struct.unpack_from(">i", data, pos + 8)[0]
                struct.pack_into(">Ii", data, pos + 4, (anchor[0] << 8) | (reloc & 0xff), addend + value - anchor[2])
                entry = anchor[1]
            external.add(entry)
    if rtti_symbols and rtti_symbols.keys() - pooled.keys():
        raise ValueError("Missing referenced RTTI descriptor")
    if external_data and external_data_entry is None:
        raise ValueError("Missing referenced external data object")
    if zero_word and zero_entry is None:
        raise ValueError("Missing four-byte literal in .sbss2")
    if strong_functions:
        # This unit owns the retail definitions even when other objects emit inline copies.
        for _, kind, _, _, offset, size, _, _, _, entry_size in headers:
            if kind == 2:  # SHT_SYMTAB
                for entry in range(offset, offset + size, entry_size):
                    info, _, section = struct.unpack_from(">BBH", data, entry + 12)
                    if info == 0x22 and section == target:  # STB_WEAK, STT_FUNC
                        data[entry + 12] = 0x12  # STB_GLOBAL, STT_FUNC

    for entry in external:
        struct.pack_into(">II", data, entry + 4, 0, 0)
        struct.pack_into(">H", data, entry + 14, 0)

    with tempfile.TemporaryDirectory(dir=output.parent) as temp:
        obj = Path(temp) / "source.o"
        result = Path(temp) / "text.o"
        obj.write_bytes(data)
        renames = [arg for old, new in pooled.items() for arg in
                   ("--redefine-sym", f"{old}={new}", "--globalize-symbol", new)]
        bss_args = ["-j", ".bss"] if keep_bss else []
        rodata_args = ["-j", ".rodata"] if keep_rodata else []
        ctor_args = ["-j", ".ctors", "--globalize-symbol", constructor] if constructor else []
        extab_args = ["-j", "extab", "-j", "extabindex"] if keep_extab else []
        data_args = ["-j", ".data", "--set-section-alignment", ".data=4"] if keep_data else []
        if data_range is not None:
            data_args = ["-j", range_section]
            if data_only:
                alignment = 4
                if range_section in (".sdata", ".sdata2"):
                    alignment = headers[target][8]
                    if data_range[0]:
                        alignment = min(alignment, data_range[0] & -data_range[0])
                if data_alignment is not None:
                    if data_alignment < 1 or data_alignment & (data_alignment - 1) or begin % data_alignment:
                        raise ValueError("Invalid retained data alignment")
                    alignment = data_alignment
                data_args.extend(["--set-section-alignment", f"{range_section}={alignment}"])
            elif data_range[0]:
                alignment = min(headers[section_names.index(".data")][8], data_range[0] & -data_range[0])
                # A switch table made entirely of pointers into the retained code is word-aligned.
                section = section_names.index(".data")
                jump_offsets = set()
                for h in headers:
                    if h[1] == 4 and h[7] == section:
                        sym = headers[h[6]]
                        for pos in range(h[4], h[4] + h[5], h[9]):
                            off, info = struct.unpack_from(">II", data, pos)
                            owner = struct.unpack_from(">H", data, sym[4] + (info >> 8) * sym[9] + 14)[0]
                            if info & 255 == 1 and owner == target:
                                jump_offsets.add(off)
                if jump_offsets == set(range(0, headers[section][5], 4)):
                    alignment = min(alignment, 4)
                data_args.extend(["--set-section-alignment", f".data={alignment}"])
        strip_args = [arg for symbol in sorted(strip_symbols) for arg in ("--strip-symbol", symbol)]
        subprocess.run([objcopy, *strip_args, *renames, "-j", name, *data_args, *object_args, *extab_args, *bss_args, *rodata_args, *ctor_args, "-j", "*UND*", "-j", "*ABS*",
                        *([] if data_only else ["--rename-section", f"{name}=.text"]), obj, result], check=True)
        result.replace(output)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("objcopy")
    parser.add_argument("section")
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("anchor", nargs="?")
    parser.add_argument("--literal", nargs=2, action="append", default=[], metavar=("HEX", "SYMBOL"))
    parser.add_argument("--zero-word", help="symbol for a single referenced four-byte .sbss2 literal")
    parser.add_argument("--strong-functions", action="store_true")
    parser.add_argument("--constructor", metavar="SYMBOL")
    parser.add_argument("--keep-extab", action="store_true", help="retain the exception tables for this code")
    parser.add_argument("--rodata-object", nargs=2, action="append", default=[], metavar=("SYMBOL", "SIZE"))
    parser.add_argument("--data-object", nargs=2, action="append", default=[], metavar=("SYMBOL", "SIZE"))
    parser.add_argument("--data-range", nargs=2, type=int, metavar=("START", "END"))
    parser.add_argument("--data-alignment", type=int, help="alignment of the extracted data section")
    parser.add_argument("--data-prefix", type=int, help="retain the leading bytes of .data at native alignment")
    parser.add_argument("--keep-rodata", action="store_true", help="retain local read-only data referenced by the selected code")
    parser.add_argument("--keep-bss", action="store_true", help="retain native zero-initialized objects with their code")
    parser.add_argument("--keep-data", action="store_true", help="retain native .data and its relocations with the code")
    parser.add_argument("--array-dtor", metavar="SYMBOL", help="link the trailing array finalizer from its own unit")
    parser.add_argument("--external-data", nargs=3, metavar=("OFFSET", "SIZE", "SYMBOL"))
    parser.add_argument("--rtti-symbol", nargs=2, action="append", default=[], metavar=("LOCAL", "EXTERNAL"))
    args = parser.parse_args()
    extract(args.objcopy, args.section, args.source, args.output, args.anchor,
            {bytes.fromhex(bits): name for bits, name in args.literal}, args.strong_functions, args.constructor, args.keep_data, args.array_dtor, args.keep_extab, args.data_prefix,
            [(symbol, int(size)) for symbol, size in args.rodata_object], args.data_range, args.keep_bss,
            [(symbol, int(size)) for symbol, size in args.data_object], args.zero_word, args.keep_rodata,
            (int(args.external_data[0]), int(args.external_data[1]), args.external_data[2]) if args.external_data else None,
            dict(args.rtti_symbol), args.data_alignment)
