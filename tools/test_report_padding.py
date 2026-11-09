import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from report_padding import pad_object


class ReportPaddingTest(unittest.TestCase):
    def test_linker_layout(self):
        binaries = Path(__file__).resolve().parents[1] / "build" / "binutils"
        assembler = shutil.which("powerpc-eabi-as", path=str(binaries))
        if not assembler:
            self.skipTest("PowerPC binutils required")
        objcopy = Path(assembler).with_name(Path(assembler).name.replace("-as", "-objcopy"))
        objdump = Path(assembler).with_name(Path(assembler).name.replace("-as", "-objdump"))
        with tempfile.TemporaryDirectory() as temp:
            retail, ours, out = (Path(temp) / f"{n}.o" for n in ("retail", "ours", "out"))
            # dtk's split: the string's symbol runs into retail's '0' padding, the next word has a label
            subprocess.run([assembler, "-o", retail], text=True, check=True, input=(
                '.section .rodata,"a"\n.type text,@object\ntext: .asciz "abc"\n.ascii "0000"\n.size text,8\n'
                ".type lbl_8,@object\nlbl_8: .long 0x01020304\n.size lbl_8,4\n"
                '.data\n.type table,@object\ntable: .long callback\n.ascii "0000"\n.size table,8\n'))
            # our compiler: the exact string, zero alignment, its own name for the word
            subprocess.run([assembler, "-o", ours], text=True, check=True, input=(
                '.section .rodata,"a"\n.type text,@object\ntext: .asciz "abc"\n.size text,4\n.balign 8\n'
                ".type word,@object\nword: .long 0x01020304\n.size word,4\n"
                ".data\n.type table,@object\ntable: .long callback\n.size table,4\n"))
            subprocess.run([objcopy, "--redefine-sym", "word=@12", ours], check=True)

            mapping = pad_object(ours, retail, "small_data+rodata", True, out)
            self.assertEqual(mapping, {"lbl_8": "@12"})
            dump = subprocess.check_output([objdump, "-s", "-j", ".rodata", "-t", out], text=True)
            self.assertIn("61626300 30303030 01020304", dump)
            self.assertRegex(dump, r"\.rodata\s+00000008 text")
            # a section that grows keeps its relocations
            data = subprocess.check_output([objdump, "-rs", "-j", ".data", out], text=True)
            self.assertRegex(data, r"0+ R_PPC_ADDR32\s+callback")
            self.assertIn("00000000 30303030", data)

            # PAL doesn't fill, its gaps stay zero
            pad_object(ours, retail, "small_data+rodata", False, out)
            self.assertIn("61626300 00000000 01020304",
                          subprocess.check_output([objdump, "-s", "-j", ".rodata", out], text=True))


if __name__ == "__main__":
    unittest.main()
