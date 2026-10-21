"""Shared geometry and colors for the interactive treemap and README image."""

import math

PALETTE = {
    "unnamed": "#e4e4e1",
    "r0": "#ebe2b9",
    "r1": "#c3d27e",
    "r2": "#62ad72",
    "linked": "#1f5f4b",
}


def layout(weights):
    items = [(index, weight) for index, weight in enumerate(weights) if weight > 0]
    if not items:
        return []
    scale = 2 / sum(weight for _, weight in items)
    prefix = [0]
    for _, weight in items:
        prefix.append(prefix[-1] + weight * scale)
    result = []

    def split(lo, hi, x, y, width, height):
        if hi - lo == 1:
            result.append([items[lo][0], *[round(n * 500, 6) for n in (x, y, width, height)]])
            return
        value = prefix[hi] - prefix[lo]
        target = prefix[lo] + value / 2
        a, b = lo, hi - 1
        while a < b:
            mid = (a + b) // 2
            if prefix[mid + 1] > target:
                b = mid
            else:
                a = mid + 1
        mid = max(a, lo + 1)
        left = prefix[mid] - prefix[lo]
        if width > height:
            cut = width * left / value
            split(lo, mid, x, y, cut, height)
            split(mid, hi, x + cut, y, width - cut, height)
        else:
            cut = height * left / value
            split(lo, mid, x, y, width, cut)
            split(mid, hi, x, y + cut, width, height - cut)

    split(0, len(items), 0, 0, 2, 1)
    return result


def _multiply(matrix, vector):
    return [sum(a * b for a, b in zip(row, vector)) for row in matrix]


# https://www.w3.org/TR/css-color-4/#color-conversion-code
_RGB_TO_XYZ = (
    (506752 / 1228815, 87881 / 245763, 12673 / 70218),
    (87098 / 409605, 175762 / 245763, 12673 / 175545),
    (7918 / 409605, 87881 / 737289, 1001167 / 1053270),
)
_XYZ_TO_LMS = (
    (0.8190224379967030, 0.3619062600528904, -0.1288737815209879),
    (0.0329836539323885, 0.9292868615863434, 0.0361446663506424),
    (0.0481771893596242, 0.2642395317527308, 0.6335478284694309),
)
_LMS_TO_LAB = (
    (0.2104542683093140, 0.7936177747023054, -0.0040720430116193),
    (1.9779985324311684, -2.4285922420485799, 0.4505937096174110),
    (0.0259040424655478, 0.7827717124575296, -0.8086757549230774),
)
_LAB_TO_LMS = (
    (1, 0.3963377773761749, 0.2158037573099136),
    (1, -0.1055613458156586, -0.0638541728258133),
    (1, -0.0894841775298119, -1.2914855480194092),
)
_LMS_TO_XYZ = (
    (1.2268798758459243, -0.5578149944602171, 0.2813910456659647),
    (-0.0405757452148008, 1.1122868032803170, -0.0717110580655164),
    (-0.0763729366746601, -0.4214933324022432, 1.5869240198367816),
)
_XYZ_TO_RGB = (
    (12831 / 3959, -329 / 214, -1974 / 3959),
    (-851781 / 878810, 1648619 / 878810, 36519 / 878810),
    (705 / 12673, -2585 / 12673, 705 / 667),
)


def _oklab(color):
    rgb = [int(color[i:i + 2], 16) / 255 for i in (1, 3, 5)]
    linear = [v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4 for v in rgb]
    lms = _multiply(_XYZ_TO_LMS, _multiply(_RGB_TO_XYZ, linear))
    return _multiply(_LMS_TO_LAB, [math.copysign(abs(v) ** (1 / 3), v) for v in lms])


def _hex(lab):
    lms = [v ** 3 for v in _multiply(_LAB_TO_LMS, lab)]
    rgb = _multiply(_XYZ_TO_RGB, _multiply(_LMS_TO_XYZ, lms))
    channels = []
    for v in rgb:
        v = max(0, min(1, v))
        v = 12.92 * v if v <= 0.0031308 else 1.055 * v ** (1 / 2.4) - 0.055
        channels.append(f"{int(v * 255 + 0.5):02x}")
    return "#" + "".join(channels)


_STOPS = [_oklab(PALETTE[key]) for key in ("r0", "r1", "r2")]


def ramp(percent):
    percent = max(0, min(100, percent))
    if percent in (0, 50, 100):
        return PALETTE[f"r{int(percent / 50)}"]
    stop = 0 if percent < 50 else 1
    amount = percent / 50 - stop
    return _hex([a + (b - a) * amount for a, b in zip(_STOPS[stop], _STOPS[stop + 1])])


def unit_color(unit):
    if unit["status"] == "auto":
        return PALETTE["unnamed"]
    if unit["status"] == "linked":
        return PALETTE["linked"]
    return ramp(unit["fuzzy"])


def function_color(percent):
    return PALETTE["linked"] if percent >= 100 else ramp(percent)


def prepare(payload):
    for version in payload["versions"]:
        units = version["units"]
        groups = dict.fromkeys(unit["group"] for unit in units)
        version["layouts"] = {"all": layout(unit["code"] for unit in units)}
        for group in groups:
            version["layouts"][group] = layout(unit["code"] if unit["group"] == group else 0 for unit in units)
        for unit in units:
            unit["color"] = unit_color(unit)
            unit["function_map"] = layout(function[2] for function in unit["funcs"])
            unit["funcs"] = [function[:4] + [function_color(function[3])] for function in unit["funcs"]]
