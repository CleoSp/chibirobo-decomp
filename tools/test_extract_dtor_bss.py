import unittest

from extract_dtor_bss import local_static, owner_and_node


class DestructorNodeTests(unittest.TestCase):
    def test_local_static_requires_a_unique_compiler_suffix(self):
        symbols = "frame$12 b 40 4\nframe$13 T 80 8"
        self.assertEqual(local_static(symbols, "frame"), "frame$12")
        with self.assertRaises(ValueError):
            local_static(symbols + "\nframe$97 b 98 4", "frame")

    def test_selects_node_immediately_before_owner(self):
        symbols = "...bss.0 b 0\n@7 b 0 c\n@31 b 194 c\nStageTable B 1a0 1380\n"
        self.assertEqual(owner_and_node(symbols, "StageTable"), ("StageTable", "@31"))

    def test_rejects_missing_or_ambiguous_node(self):
        for nodes in ("", "@7 b 4 c", "@7 b 0 8", "@7 b 0 c\n@8 b 0 c"):
            with self.subTest(nodes=nodes), self.assertRaises(ValueError):
                owner_and_node(nodes + "\nCamera B c 23d8", "Camera")

    def test_local_static_node_follows_array(self):
        symbols = "sChars$1490 b 0 41000\n@1492 b 41000 c\n"
        self.assertEqual(owner_and_node(symbols, "sChars"), ("sChars$1490", "@1492"))
