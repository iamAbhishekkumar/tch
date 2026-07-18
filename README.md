# tch

[![Build](https://img.shields.io/badge/build-make-brightgreen)](https://github.com)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](https://isocpp.org)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)](https://www.kernel.org)

`tch` is a small C++ tool that works like `touch`.
It creates missing parent directories, creates files if needed, and updates file access/modification times.

## Features

- Creates missing directories automatically
- Creates files if they do not exist
- Updates file access and modification timestamps
- Supports `-a` to update only access time
- Supports `-m` to update only modification time

## Build

Run:

```sh
make
```

This builds `build/tch`.

## Install

Run:

```sh
sudo make install
```

This installs `tch` to `/usr/local/bin`.

You can also use `install.sh` to build, install, and add a shell wrapper that falls back to the system `touch` command for unsupported options.

```sh
./install.sh
```

## Usage

```sh
tch [OPTION]... FILE...
```

Examples:

```sh
tch file.txt
tch -a file.txt
tch -m dir/sub/file.txt
```

For help:

```sh
tch --help
```
