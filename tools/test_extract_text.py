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

            # Compiler-generated vertex initializers can share one retail symbol.
            vertices = '.section .rodata,"a"\n'
            for name, word in [("first", 1), ("second", 2)]:
                vertices += f".type {name},@object\n{name}: .long {word}\n.size {name},4\n"
            subprocess.run([assembler, "-o", source], input=vertices, text=True, check=True)
            subprocess.run([objcopy, "--redefine-sym", "first=@10", "--redefine-sym", "second=@11", source], check=True)
            extract(str(objcopy), ".rodata", source, output, rodata_objects=[("vertices", 8)], data_range=(0, 8))
            self.assertRegex(subprocess.check_output([objdump, "-t", output], text=True), r"g\s+O \.rodata\s+00000008 vertices")
            subprocess.run([objcopy, "--globalize-symbol", "@11", source], check=True)
            with self.assertRaisesRegex(ValueError, "Unexpected initializer"):
                extract(str(objcopy), ".rodata", source, output, rodata_objects=[("vertices", 8)], data_range=(0, 8))

            literals = ('.section .sdata2,"a"\n.balign 8\n.type literal,@object\n'
                        'literal: .double 42.5\n.size literal,8\n')
            subprocess.run([assembler, "-o", source], input=literals, text=True, check=True)
            extract(str(objcopy), ".sdata2", source, output, data_objects=[("point_limit", 8)], data_range=(0, 8))
            result = subprocess.check_output([objdump, "-ht", output], text=True)
            self.assertRegex(result, r"\.sdata2\s+00000008[^\n]+2\*\*3")
            self.assertRegex(result, r"g\s+O \.sdata2\s+00000008 point_limit")

            aligned_table = '.data\n.balign 8\n.type table,@object\ntable: .long 1,2\n.size table,8\n'
            subprocess.run([assembler, "-o", source], input=aligned_table, text=True, check=True)
            extract(str(objcopy), ".data", source, output, data_range=(0, 8), data_alignment=8)
            result = subprocess.check_output([objdump, "-h", output], text=True)
            self.assertRegex(result, r"\.data\s+00000008[^\n]+2\*\*3")
            with self.assertRaisesRegex(ValueError, "Invalid retained data alignment"):
                extract(str(objcopy), ".data", source, output, data_range=(0, 8), data_alignment=3)

            # A native switch table and its reader refer to each other's sections.
            switch = ('.section .text.split,"ax"\nentry: lis 3,table@ha\naddi 3,3,table@l\nblr\n'
                      '.data\ntable: .long entry,entry+4\n')
            subprocess.run([assembler, "-o", source], input=switch, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, keep_data=True)
            relocs = subprocess.check_output([objdump, "-r", output], text=True)
            self.assertRegex(relocs, r"R_PPC_ADDR16_HA\s+\.data")
            self.assertRegex(relocs, r"R_PPC_ADDR32\s+\.text\+0x0*4")

            named_switch = (".global entry,table\n.type table,@object\n.size table,8\n" + switch)
            subprocess.run([assembler, "-o", source], input=named_switch, text=True, check=True)
            subprocess.run([objcopy, "--localize-symbol", "table", source], check=True)
            extract(str(objcopy), ".text.split", source, output, external_data=(0, 8, "phase_table"))
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"R_PPC_ADDR16_HA\s+phase_table")
            self.assertRegex(result, r"R_PPC_ADDR16_LO\s+phase_table")
            self.assertRegex(result, r"\*UND\*\s+0+ phase_table")
            with self.assertRaises(ValueError):
                extract(str(objcopy), ".text.split", source, output, external_data=(0, 4, "phase_table"))

            suffix = ".long discarded_target\n"
            subprocess.run([assembler, "-o", source], input=switch + suffix, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_prefix=8)
            result = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(result, r"\.data\s+00000008")
            self.assertNotIn("discarded_target", result)
            subprocess.run([assembler, "-o", source], input=switch.replace("table@ha", "(table+8)@ha") + suffix,
                           text=True, check=True)
            with self.assertRaisesRegex(ValueError, "outside retained data"):
                extract(str(objcopy), ".text.split", source, output, data_prefix=8)

            # An unrelated weak vtable can follow a retained dispatch table.
            tail = ".weak vtable\nvtable: .long callback\n"
            subprocess.run([assembler, "-o", source], input=switch + tail, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_prefix=8)
            self.assertNotIn("vtable", subprocess.check_output([objdump, "-t", output], text=True))
            referenced = switch.replace("blr", "lis 4,vtable@ha\naddi 4,4,vtable@l\nblr")
            subprocess.run([assembler, "-o", source], input=referenced + tail, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_prefix=8)
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"\*UND\*\s+0+ vtable")
            self.assertRegex(result, r"R_PPC_ADDR16_LO\s+vtable")
            subprocess.run([assembler, "-o", source], input=switch + tail.replace(".weak", ".global"), text=True, check=True)
            with self.assertRaisesRegex(ValueError, "Unexpected public symbol"):
                extract(str(objcopy), ".text.split", source, output, data_prefix=8)

            # A split switch table follows other relocated data in the compiler object.
            middle = switch.replace("table: .long", ".balign 8\n.long discarded_head\ntable: .long") + suffix
            subprocess.run([assembler, "-o", source], input=middle, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_range=(4, 12))
            result = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(result, r"\.data\s+00000008[^\n]+2\*\*2")
            self.assertRegex(result, r"00000000 R_PPC_ADDR32\s+\.text")
            self.assertRegex(result, r"00000004 R_PPC_ADDR32\s+\.text\+0x0*4")
            self.assertNotIn("discarded_", result)

            public = middle.replace(".long discarded_head", ".global floors\n.type floors,@object\nfloors: .long 0")
            public = public.replace("blr", "lis 4,floors@ha\naddi 4,4,floors@l\nblr")
            subprocess.run([assembler, "-o", source], input=public, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_range=(4, 12))
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"\*UND\*\s+0+ floors")
            self.assertRegex(result, r"R_PPC_ADDR16_LO\s+floors")
            self.assertRegex(result, r"00000004 R_PPC_ADDR32\s+\.text\+0x0*4")

            # The switch reader also refers to the original data pool through MWCC's local alias.
            alias = public.replace("floors: .long", ".global ...data.0\n...data.0:\n.size floors,4\nfloors: .long")
            alias = alias.replace("floors@ha", "...data.0@ha").replace("floors@l", "...data.0@l")
            subprocess.run([assembler, "-o", source], input=alias, text=True, check=True)
            subprocess.run([objcopy, "--localize-symbol", "...data.0", source], check=True)
            extract(str(objcopy), ".text.split", source, output, data_range=(4, 12))
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"\*UND\*\s+0+ floors")
            self.assertRegex(result, r"R_PPC_ADDR16_HA\s+floors")
            self.assertRegex(result, r"R_PPC_ADDR16_LO\s+floors")
            self.assertNotIn("...data.0", result)

            # A table can start at an eight-byte offset but link at a four-byte boundary.
            aligned = middle.replace(".long discarded_head", ".long discarded_head,discarded_head")
            subprocess.run([assembler, "-o", source], input=aligned, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, data_range=(8, 16))
            result = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(result, r"\.data\s+00000008[^\n]+2\*\*2")
            self.assertRegex(result, r"00000004 R_PPC_ADDR32\s+\.text\+0x0*4")

            # A data-only unit retains the compiler's dispatch targets at their new offsets.
            data_only = ".data\n.long discarded_head\n.long callback\n.long discarded_tail\n"
            subprocess.run([assembler, "-o", source], input=data_only, text=True, check=True)
            extract(str(objcopy), ".data", source, output, data_range=(4, 8))
            result = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(result, r"\.data\s+00000004")
            self.assertRegex(result, r"00000000 R_PPC_ADDR32\s+callback")
            self.assertNotIn("discarded_", result)

            # A data-only prefix can precede a public object owned by another unit.
            prefix = ".data\n.long callback\n.global other_table\nother_table: .long discarded_tail\n"
            subprocess.run([assembler, "-o", source], input=prefix, text=True, check=True)
            extract(str(objcopy), ".data", source, output, data_range=(0, 4))
            result = subprocess.check_output([objdump, "-hrt", output], text=True)
            self.assertRegex(result, r"00000000 R_PPC_ADDR32\s+callback")
            self.assertNotIn("other_table", result)
            self.assertNotRegex(result, r"R_PPC_ADDR32\s+discarded_tail")

            tables = (".data\n.long discarded_head\n.type local_base,@object\nlocal_base: .long descriptor\n.size local_base,4\n"
                      ".weak vtable\n.type vtable,@object\nvtable: .long callback\n.size vtable,4\n")
            subprocess.run([assembler, "-o", source], input=tables, text=True, check=True)
            extract(str(objcopy), ".data", source, output, data_range=(4, 12), data_objects=[("base", 4), ("vtable", 4)])
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"00000000 g\s+O \.data\s+00000004 base")
            self.assertRegex(result, r"00000004 g\s+O \.data\s+00000004 vtable")
            self.assertRegex(result, r"00000000 R_PPC_ADDR32\s+descriptor")
            self.assertRegex(result, r"00000004 R_PPC_ADDR32\s+callback")
            with self.assertRaisesRegex(ValueError, "layout"):
                extract(str(objcopy), ".data", source, output, data_range=(4, 12), data_objects=[("base", 8)])

            rtti = ('.section .sdata,"aw"\n.global __RTTI__5CJObj\n.type __RTTI__5CJObj,@object\n'
                    '__RTTI__5CJObj: .long type_name,0\n.size __RTTI__5CJObj,8\n.data\n.long __RTTI__5CJObj\n')
            subprocess.run([assembler, "-o", source], input=rtti, text=True, check=True)
            subprocess.run([objcopy, "--localize-symbol", "__RTTI__5CJObj", source], check=True)
            extract(str(objcopy), ".data", source, output, data_range=(0, 4), rtti_symbols={"__RTTI__5CJObj": "base_type"})
            result = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(result, r"R_PPC_ADDR32\s+base_type")
            self.assertRegex(result, r"\*UND\*\s+0+ base_type")
            with self.assertRaisesRegex(ValueError, "Missing referenced RTTI"):
                extract(str(objcopy), ".data", source, output, data_range=(0, 4), rtti_symbols={"__RTTI__4XObj": "other"})

            vectors = ('.section .rodata,"a"\n.type pos,@object\npos: .space 12\n.size pos,12\n'
                       '.type aim,@object\naim: .space 12\n.size aim,12\n'
                       '.section .text.split,"ax"\n.long pos,aim\n')
            subprocess.run([assembler, "-o", source], input=vectors, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, keep_rodata=True)
            sections = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(sections, r"\.rodata\s+00000018")
            self.assertIn("R_PPC_ADDR32", sections)
            extract(str(objcopy), ".text.split", source, output, rodata_objects=[("position", 12), ("interest", 12)])
            symbols = subprocess.check_output([objdump, "-t", output], text=True)
            self.assertRegex(symbols, r"00000000 g\s+O \.rodata\s+0000000c position")
            self.assertRegex(symbols, r"0000000c g\s+O \.rodata\s+0000000c interest")
            with self.assertRaisesRegex(ValueError, "layout"):
                extract(str(objcopy), ".text.split", source, output, rodata_objects=[("position", 24)])

            extract(str(objcopy), ".rodata", source, output, data_range=(12, 24), rodata_objects=[("interest", 12)])
            symbols = subprocess.check_output([objdump, "-t", output], text=True)
            self.assertRegex(symbols, r"00000000 g\s+O \.rodata\s+0000000c interest")
            self.assertNotRegex(symbols, r"\bpos\b")
            with self.assertRaisesRegex(ValueError, "cuts a symbol"):
                extract(str(objcopy), ".rodata", source, output, data_range=(4, 24))

            strings = '.section .rodata,"a"\n.long 0\n.type name,@object\nname: .asciz "A"\n.size name,.-name\n'
            subprocess.run([assembler, "-o", source], input=strings, text=True, check=True)
            extract(str(objcopy), ".rodata", source, output, data_range=(4, 6), rodata_objects=[("typeName", 2)])
            self.assertRegex(subprocess.check_output([objdump, "-h", output], text=True), r"\.rodata\s+00000002")
            with self.assertRaisesRegex(ValueError, "Invalid retained data range"):
                extract(str(objcopy), ".rodata", source, output, data_range=(0, 5))

            ctors = '.section .text.split,"ax"\n.type init,@function\ninit: blr\n.section .ctors,"a"\n.long init\n'
            subprocess.run([assembler, "-o", source], input=ctors, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, constructor="init")
            table = subprocess.check_output([objdump, "-r", "-j", ".ctors", output], text=True)
            exported = subprocess.check_output([objdump, "-t", output], text=True)
            self.assertRegex(exported, r"g\s+F \.text\s+0+ init")
            self.assertRegex(table, r"R_PPC_ADDR32\s+(?:init|\.text)")

            array = ('.section .text.split,"ax"\n.type init,@function\ninit: lis 3,finalizer@ha\n'
                     'addi 3,3,finalizer@l\nblr\n.size init,.-init\n.global finalizer\n'
                     '.type finalizer,@function\nfinalizer: b destroy_array\n.size finalizer,.-finalizer\n'
                     '.section .ctors,"a"\n.long init\n')
            subprocess.run([assembler, "-o", source], input=array, text=True, check=True)
            subprocess.run([objcopy, "--redefine-sym=finalizer=__arraydtor$123", source], check=True)
            subprocess.run([objcopy, "--localize-symbol=__arraydtor$123", source], check=True)
            extract(str(objcopy), ".text.split", source, output, constructor="init", array_dtor="DestroyArray")
            result = subprocess.check_output([objdump, "-hr", output], text=True)
            self.assertRegex(result, r"\.text\s+0000000c")
            self.assertRegex(result, r"R_PPC_ADDR16_HA\s+DestroyArray")
            self.assertRegex(result, r"R_PPC_ADDR16_LO\s+DestroyArray")
            self.assertNotIn("R_PPC_REL24", result)
            with self.assertRaisesRegex(ValueError, "layout"):
                extract(str(objcopy), ".text.split", source, output, constructor="wrong", array_dtor="DestroyArray")

            helper = '.weak row_ctor\n.type row_ctor,@function\nrow_ctor: blr\n.size row_ctor,.-row_ctor\n'
            for binding in (".weak", ".global"):
                assembly = array.replace(".section .ctors", helper.replace(".weak", binding) + ".section .ctors")
                subprocess.run([assembler, "-o", source], input=assembly, text=True, check=True)
                subprocess.run([objcopy, "--redefine-sym=finalizer=__arraydtor$123", source], check=True)
                if binding == ".weak":
                    extract(str(objcopy), ".text.split", source, output, constructor="init", array_dtor="DestroyArray")
                    self.assertRegex(subprocess.check_output([objdump, "-h", output], text=True), r"\.text\s+0000000c")
                else:
                    with self.assertRaisesRegex(ValueError, "after the array finalizer"):
                        extract(str(objcopy), ".text.split", source, output, constructor="init", array_dtor="DestroyArray")

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

            # MWCC assigns an anonymous object symbol to a static destructor record.
            node = ('.bss\n.global node, camera\n.type node,@object\nnode: .space 12\n.size node,12\n'
                    'camera: .space 4\n.section .text.split,"ax"\n.type init,@function\n'
                    'init: lis 3,node@ha\naddi 3,3,node@l\nblr\n.section .ctors,"a"\n.long init\n')
            for gap in (0, 4):
                subprocess.run([assembler, "-o", source], input=node.replace("camera:", ".skip 4\ncamera:") if gap else node,
                               text=True, check=True)
                subprocess.run([objcopy, "--redefine-sym=node=@123", source], check=True)
                subprocess.run([objcopy, "--localize-symbol=@123", source], check=True)
                if gap:
                    with self.assertRaisesRegex(ValueError, "local symbol"):
                        extract(str(objcopy), ".text.split", source, output, "camera", constructor="init")
                else:
                    extract(str(objcopy), ".text.split", source, output, "camera", constructor="init")
                    symbols = subprocess.check_output([objdump, "-r", output], text=True)
                    self.assertRegex(symbols, r'R_PPC_ADDR16_HA\s+camera-0x0*c')
                    self.assertRegex(symbols, r'R_PPC_ADDR16_LO\s+camera-0x0*c')

            for section, flags in [(".rodata", "a"), (".data", "aw")]:
                assembly = (
                    f'.section {section},"{flags}"\n.globl constants,..{section}.0\n.type constants,@object\n'
                    f'constants:\n..{section}.0: .long 1,2\n.size constants,8\n'
                    f'.section .text.split,"ax"\nlis 3,(..{section}.0+4)@ha\naddi 3,3,(..{section}.0+4)@l\n'
                )
                subprocess.run([assembler, "-o", source], input=assembly, text=True, check=True)
                subprocess.run([objcopy, f"--localize-symbol=..{section}.0", source], check=True)
                extract(str(objcopy), ".text.split", source, output)
                symbols = subprocess.check_output([objdump, "-rt", output], text=True)
                self.assertRegex(symbols, r'R_PPC_ADDR16_HA\s+constants\+0x0*4')
                self.assertRegex(symbols, r'R_PPC_ADDR16_LO\s+constants\+0x0*4')
                subprocess.run([objcopy, "--localize-symbol=constants", source], check=True)
                with self.assertRaisesRegex(ValueError, "local symbol"):
                    extract(str(objcopy), ".text.split", source, output)

            literal = (
                '.section .rodata,"a"\n.global value\n.type value,@object\n'
                'value: .long 0x3f800000\n.size value,4\n'
                '.section .text.split,"ax"\nlis 3,value@ha\nlfs 1,value@l(3)\n'
            )
            literal += ".data\n.long value\n"
            subprocess.run([assembler, "-o", source], input=literal, text=True, check=True)
            subprocess.run([objcopy, "--redefine-sym", "value=@123", source], check=True)
            subprocess.run([objcopy, "--localize-symbol=@123", source], check=True)
            extract(str(objcopy), ".text.split", source, output, literals={bytes.fromhex("3f800000"): "sOne"})
            symbols = subprocess.check_output([objdump, "-rt", output], text=True)
            self.assertRegex(symbols, r'R_PPC_ADDR16_HA\s+sOne')
            self.assertRegex(symbols, r'R_PPC_ADDR16_LO\s+sOne')
            extract(str(objcopy), ".text.split", source, output, keep_data=True, literals={bytes.fromhex("3f800000"): "sOne"})
            self.assertRegex(subprocess.check_output([objdump, "-r", output], text=True), r'R_PPC_ADDR32\s+sOne')
            previous = output.read_bytes()
            with self.assertRaisesRegex(ValueError, "local symbol"):
                extract(str(objcopy), ".text.split", source, output, literals={bytes.fromhex("3f80"): "sOne"})
            self.assertEqual(output.read_bytes(), previous)

            # MWCC also puts strings and relocated switch tables in the same data section.
            for word in ("0", "callback"):
                mixed = literal.replace('.section .rodata,"a"', '.data\n.long callback').replace('0x3f800000', word)
                subprocess.run([assembler, "-o", source], input=mixed, text=True, check=True)
                subprocess.run([objcopy, "--redefine-sym", "value=@123", source], check=True)
                subprocess.run([objcopy, "--localize-symbol=@123", source], check=True)
                if word == "0":
                    extract(str(objcopy), ".text.split", source, output, literals={bytes(4): "zero"})
                    symbols = subprocess.check_output([objdump, "-r", output], text=True)
                    self.assertRegex(symbols, r'R_PPC_ADDR16_LO\s+zero')
                else:
                    with self.assertRaisesRegex(ValueError, "local symbol"):
                        extract(str(objcopy), ".text.split", source, output, literals={bytes(4): "zero"})

            small_literal = literal.replace(".rodata", ".sdata2").replace(
                "lis 3,value@ha\nlfs 1,value@l(3)", "lfs 1,value@sda21(0)"
            )
            for local_name in ("@123", "__FUNCTION__$897"):
                subprocess.run([assembler, "-o", source], input=small_literal, text=True, check=True)
                subprocess.run([objcopy, "--redefine-sym", f"value={local_name}", source], check=True)
                subprocess.run([objcopy, f"--localize-symbol={local_name}", source], check=True)
                extract(str(objcopy), ".text.split", source, output, literals={bytes.fromhex("3f800000"): "sOne"})
                symbols = subprocess.check_output([objdump, "-rt", output], text=True)
                self.assertRegex(symbols, r'R_PPC_EMB_SDA21\s+sOne')

            # References to individual elements keep their offsets into the pooled array.
            for offset in (4, 8):
                pair = small_literal.replace(".long 0x3f800000", ".long 0,9").replace(".size value,4", ".size value,8")
                pair = pair.replace("value@sda21", f"(value+{offset})@sda21")
                subprocess.run([assembler, "-o", source], input=pair, text=True, check=True)
                subprocess.run([objcopy, "--redefine-sym", "value=@123", source], check=True)
                subprocess.run([objcopy, "--localize-symbol=@123", source], check=True)
                mapping = {bytes.fromhex("0000000000000009"): "jointPath"}
                if offset == 4:
                    extract(str(objcopy), ".text.split", source, output, literals=mapping)
                    self.assertRegex(subprocess.check_output([objdump, "-r", output], text=True),
                                     r'R_PPC_EMB_SDA21\s+jointPath\+0x0*4')
                else:
                    with self.assertRaisesRegex(ValueError, "local symbol"):
                        extract(str(objcopy), ".text.split", source, output, literals=mapping)

            # Zero-filled color words have no payload in the object file.
            empty_color = ('.section .sbss2,"a",@nobits\n.global color\n.type color,@object\n'
                           'color: .skip 4\n.size color,4\n.section .text.split,"ax"\nlwz 3,color@sda21(0)\n')
            for count in (1, 2):
                colors = empty_color if count == 1 else empty_color + empty_color.replace("color", "other")
                subprocess.run([assembler, "-o", source], input=colors, text=True, check=True)
                for idx, symbol in enumerate(("color", "other")[:count]):
                    subprocess.run([objcopy, "--redefine-sym", f"{symbol}=@{idx}", source], check=True)
                    subprocess.run([objcopy, f"--localize-symbol=@{idx}", source], check=True)
                if count == 1:
                    extract(str(objcopy), ".text.split", source, output, zero_word="sDisabledColor")
                    self.assertRegex(subprocess.check_output([objdump, "-r", output], text=True),
                                     r'R_PPC_EMB_SDA21\s+sDisabledColor')
                else:
                    with self.assertRaisesRegex(ValueError, "Ambiguous"):
                        extract(str(objcopy), ".text.split", source, output, zero_word="sDisabledColor")

            weak = '.section .text.split,"ax"\n.weak setter,callback\n.type setter,@function\nsetter: bl callback\n'
            subprocess.run([assembler, "-o", source], input=weak, text=True, check=True)
            extract(str(objcopy), ".text.split", source, output, strong_functions=True)
            symbols = subprocess.check_output([objdump, "-t", output], text=True)
            self.assertRegex(symbols, r'g\s+F \.text\s+0+ setter')
            self.assertRegex(symbols, r'w\s+\*UND\*\s+0+ callback')
