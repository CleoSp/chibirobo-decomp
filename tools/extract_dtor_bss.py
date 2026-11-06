"""Export an MWCC global object's anonymous destructor-chain node from native BSS."""

import argparse
import subprocess
import tempfile
from pathlib import Path


def owner_and_node(symbols: str, owner: str) -> tuple[str, str]:
    rows = [fields for line in symbols.splitlines() if len(fields := line.split()) == 4]
    owners = [(name, int(addr, 16), int(size, 16)) for name, kind, addr, size in rows
              if kind in ("B", "b") and (name == owner or
                  (name.startswith(owner + "$") and name[len(owner) + 1:].isdigit()))]
    if len(owners) != 1:
        raise ValueError(f"Expected one BSS object named {owner}")
    name, start, size = owners[0]
    # MWCC puts the record after a function-local static and before a namespace-scope object.
    address = start + size if name != owner else start - 12
    nodes = [symbol for symbol, kind, addr, length in rows if kind == "b" and symbol.startswith("@")
             and int(length, 16) == 12 and int(addr, 16) == address]
    if len(nodes) != 1:
        raise ValueError(f"Expected one MWCC destructor-chain node beside {owner}")
    return name, nodes[0]


def local_static(symbols: str, prefix: str) -> str:
    matches = []
    for line in symbols.splitlines():
        fields = line.split()
        if len(fields) == 4 and fields[1] == "b":
            name = fields[0]
            base, separator, suffix = name.rpartition("$")
            if separator and base == prefix and suffix.isdigit():
                matches.append(name)
    if len(matches) != 1:
        raise ValueError(f"Expected one function-local BSS symbol named {prefix}")
    return matches[0]


def extract(objcopy: Path, nm: Path, source: Path, output: Path, exports: list[str],
            nodes: list[list[str]], locals_: list[list[str]], sbss: bool = False) -> None:
    with tempfile.TemporaryDirectory(dir=output.parent) as temp:
        bss = Path(temp) / "bss.o"
        section = ".sbss" if sbss else ".bss"
        subprocess.run([objcopy, "--only-section=" + section, source, bss], check=True)
        symbols = subprocess.check_output([nm, "--format=posix", "--print-size", bss], text=True)
        aliases = {}
        for owner, label in nodes:
            name, node = owner_and_node(symbols, owner)
            aliases[node] = label
            if name != owner:
                aliases[name] = owner
        for prefix, label in locals_:
            aliases[local_static(symbols, prefix)] = label
        result = Path(temp) / "export.o"
        subprocess.run([objcopy, *(f"--redefine-sym={name}={label}" for name, label in aliases.items()),
                        *(f"--globalize-symbol={name}" for name in [*exports, *aliases.values()]), bss, result], check=True)
        result.replace(output)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("objcopy", "nm", "source", "output"):
        parser.add_argument(name, type=Path)
    parser.add_argument("exports", nargs="*")
    parser.add_argument("--node", action="append", nargs=2, default=[])
    parser.add_argument("--local", action="append", nargs=2, default=[])
    parser.add_argument("--sbss", action="store_true")
    args = parser.parse_args()
    extract(args.objcopy, args.nm, args.source, args.output, args.exports, args.node, args.local, args.sbss)
