import copy
import unittest

import treemap_layout as treemap


class LayoutTests(unittest.TestCase):
    def test_preserves_report_order_and_halfway_crossing(self):
        self.assertEqual(treemap.layout([3, 3, 4]), [
            [0, 0, 0, 300, 500],
            [1, 300, 0, 300, 500],
            [2, 600, 0, 400, 500],
        ])
        self.assertEqual([r[0] for r in treemap.layout([1, 9, 2])], [0, 1, 2])
        self.assertEqual(treemap.layout([3, 3, 3, 7]), [
            [0, 0, 0, 375, 250],
            [1, 0, 250, 375, 250],
            [2, 375, 0, 187.5, 500],
            [3, 562.5, 0, 437.5, 500],
        ])

    def test_square_splits_top_to_bottom(self):
        self.assertEqual(treemap.layout([1, 1, 2]), [
            [0, 0, 0, 500, 250],
            [1, 0, 250, 500, 250],
            [2, 500, 0, 500, 500],
        ])

    def test_empty_and_zero_weights_preserve_original_indexes(self):
        self.assertEqual(treemap.layout([]), [])
        self.assertEqual(treemap.layout([0, 0]), [])
        self.assertEqual(treemap.layout([0, 4, 0]), [[1, 0, 0, 1000, 500]])
        self.assertEqual(treemap.layout([0, 1, 0, 1]), [
            [1, 0, 0, 500, 500],
            [3, 500, 0, 500, 500],
        ])

    def test_bounds_coverage_and_proportional_areas(self):
        for weights in ([1], [9, 1, 1, 1], [1, 2, 3, 5, 8, 13], list(range(1, 80))):
            with self.subTest(weights=weights):
                boxes = treemap.layout(weights)
                self.assertEqual(len(boxes), len(weights))
                self.assertAlmostEqual(sum(w * h for _, _, _, w, h in boxes), 500000, delta=0.01)
                for index, x, y, width, height in boxes:
                    self.assertGreaterEqual(x, 0)
                    self.assertGreaterEqual(y, 0)
                    self.assertGreater(width, 0)
                    self.assertGreater(height, 0)
                    self.assertLessEqual(x + width, 1000.000001)
                    self.assertLessEqual(y + height, 500.000001)
                    self.assertAlmostEqual(width * height, 500000 * weights[index] / sum(weights), delta=0.001)
                for i, (_, x, y, width, height) in enumerate(boxes):
                    for _, other_x, other_y, other_width, other_height in boxes[i + 1:]:
                        overlap_x = min(x + width, other_x + other_width) - max(x, other_x)
                        overlap_y = min(y + height, other_y + other_height) - max(y, other_y)
                        self.assertTrue(overlap_x <= 0.000001 or overlap_y <= 0.000001)


class ColorTests(unittest.TestCase):
    def test_palette_stops_and_clamping(self):
        for percent, expected in ((-1, "#ebe2b9"), (0, "#ebe2b9"), (50, "#c3d27e"),
                                  (100, "#62ad72"), (101, "#62ad72")):
            self.assertEqual(treemap.ramp(percent), expected)

    def test_oklab_interpolation(self):
        self.assertEqual(treemap.ramp(25), "#d7da9c")
        self.assertEqual(treemap.ramp(75), "#95c078")

    def test_status_takes_precedence_over_percent_or_complete(self):
        self.assertEqual(treemap.unit_color({"status": "auto", "fuzzy": 100, "complete": True}), "#e4e4e1")
        self.assertEqual(treemap.unit_color({"status": "linked", "fuzzy": 0, "complete": False}), "#1f5f4b")
        self.assertEqual(treemap.unit_color({"status": "fuzzy", "fuzzy": 100, "complete": True}), "#62ad72")
        self.assertEqual(treemap.unit_color({"status": "named", "fuzzy": 0}), "#ebe2b9")
        self.assertEqual(treemap.function_color(100), "#1f5f4b")
        self.assertEqual(treemap.function_color(99), treemap.ramp(99))


class PrepareTests(unittest.TestCase):
    def test_indexes_colors_and_idempotence(self):
        payload = {"schema": 1, "versions": [{"version": "GGTE01", "units": [
            {"group": "game", "code": 0, "status": "named", "fuzzy": 0, "funcs": []},
            {"group": "SDK", "code": 10, "status": "linked", "fuzzy": 100,
             "funcs": [["empty", 0, 0, 0], ["done", 4, 10, 100]]},
            {"group": "game", "code": 20, "status": "fuzzy", "fuzzy": 50,
             "funcs": [["started", 8, 20, 50]]},
        ]}]}
        original = copy.deepcopy(payload)
        treemap.prepare(payload)
        version = payload["versions"][0]
        self.assertEqual(set(version["layouts"]), {"all", "SDK", "game"})
        self.assertEqual([r[0] for r in version["layouts"]["all"]], [1, 2])
        self.assertEqual(version["layouts"]["SDK"], [[1, 0, 0, 1000, 500]])
        self.assertEqual(version["layouts"]["game"], [[2, 0, 0, 1000, 500]])
        self.assertEqual(version["units"][1]["function_map"], [[1, 0, 0, 1000, 500]])
        self.assertEqual(version["units"][1]["color"], "#1f5f4b")
        self.assertEqual(version["units"][1]["funcs"][1][4], "#1f5f4b")
        self.assertEqual(version["units"][2]["funcs"][0][4], "#c3d27e")
        for before, after in zip(original["versions"][0]["units"], version["units"]):
            self.assertEqual(before["funcs"], [function[:4] for function in after["funcs"]])
        prepared = copy.deepcopy(payload)
        treemap.prepare(payload)
        self.assertEqual(payload, prepared)


if __name__ == "__main__":
    unittest.main()
