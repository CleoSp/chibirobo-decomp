import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from extract_text import extract


class TextSectionTest(unittest.TestCase):
    def test_cross_section_relocations(self):
        binaries = Path(__file__).resolve().parents[1] / "build" / "binutils"
        assembler = shutil.which("powerpc-eabi-as", path=str(binaries))
        if not assembler:
            self.skipTest("PowerPC binutils required")
        objcopy = Path(assembler).with_name(Path(assembler).name.replace("-as", "-objcopy"))
        objdump = Path(assembler).with_name(Path(assembler).name.replace("-as", "-objdump"))
        with tempfile.TemporaryDirectory() as temp:
            source = Path(temp) / "input.o"
            output = Path(temp) / "output.o"
            header = '.data\n.global table\ntable: .long 0\n.section .text.split,"ax"\n'
            subprocess.run([assembler, "-o", source], input=header + ".long table+4\nbl callback\n",
                           text=True, check=True)
            extract(str(objcopy), ".text.split", source, output)
            symbols = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(symbols, r"\*UND\*\s+0+ table")
            self.assertRegex(symbols, r"0+ R_PPC_ADDR32\s+table\+0x0*4")
            self.assertRegex(symbols, r"0*4 R_PPC_REL24\s+callback")

            previous = output.read_bytes()
            subprocess.run([assembler, "-o", source], input=header + ".long .data\n", text=True, check=True)
            with self.assertRaisesRegex(ValueError, "local symbol"):
                extract(str(objcopy), ".text.split", source, output)
            self.assertEqual(output.read_bytes(), previous)

            pool = (
                '.bss\n.globl ...bss.0, reader\n...bss.0: .skip 16\nreader: .skip 4\n'
                '.section .text.split,"ax"\nlis 3,(...bss.0+4)@ha\naddi 3,3,(...bss.0+4)@l\n'
            )
            subprocess.run([assembler, "-o", source], input=pool, text=True, check=True)
            subprocess.run([objcopy, "--localize-symbol=...bss.0", source], check=True)
            with self.assertRaisesRegex(ValueError, "missing global BSS anchor"):
                extract(str(objcopy), ".text.split", source, output, "unknown")
            extract(str(objcopy), ".text.split", source, output, "reader")
            symbols = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(symbols, r'R_PPC_ADDR16_HA\s+reader-0x0*c')
            self.assertRegex(symbols, r'R_PPC_ADDR16_LO\s+reader-0x0*c')
