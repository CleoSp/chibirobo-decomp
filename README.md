# Chibi-Robo!

A work in progress decompilation of Chibi-Robo! for the GameCube.

[![Linked](https://cleosp.github.io/chibirobo-decomp/badges/linked.svg)](https://cleosp.github.io/chibirobo-decomp/progress.html)
[![Data](https://cleosp.github.io/chibirobo-decomp/badges/data.svg)](https://cleosp.github.io/chibirobo-decomp/progress.html)

[![Decompilation progress](https://cleosp.github.io/chibirobo-decomp/progress.svg)](https://cleosp.github.io/chibirobo-decomp/progress.html)

Supported versions:

- `GGTE01`: USA rev 0
- `GGTJ01_01`: Japan rev 1
- `GGTP01`: Europe rev 0

This repository contains no game assets. You need your own copy of the game to build it.

## Dependencies

### Windows

Install [Python](https://www.python.org/downloads/) and add it to PATH, then install ninja with `py -m pip install ninja`. Use `py` instead of `python3` below.

### macOS

```sh
brew install python ninja
```

### Linux

Install Python 3.10 or later and ninja with your package manager.

The build downloads its compiler and other tools on first run.

## Building

Clone the repository:

```sh
git clone https://github.com/CleoSp/chibirobo-decomp.git
cd chibirobo-decomp
```

Put an ISO, RVZ or CISO of your game in the folder for its version: `orig/GGTE01/`, `orig/GGTJ01_01/` or `orig/GGTP01/`.

Configure and build:

```sh
python3 configure.py
ninja
```

To pick a version, add `--version GGTE01`, `--version GGTJ01_01` or `--version GGTP01` to the configure command.

The build checks the DOL and 16 RELs against the originals. Unfinished units still link original code.

## Diffing

Once the first build succeeds, open [objdiff](https://github.com/encounter/objdiff/releases), set its project directory to your clone and select a unit. objdiff reads the generated `objdiff.json` and rebuilds automatically when source, header or config files change.

Before opening a pull request, compile all source:

```sh
ninja all_source progress
```

Include your source and config changes in the pull request. Keep game files and build output out of it.

## Modding

Configure with `--non-matching` to build modified or non-matching code:

```sh
python3 configure.py --non-matching
ninja
```

The output DOL is written to `build/GGTE01/main.dol`, or to the folder for the version you built.

[Credits and license notices](NOTICE)
