<h1 align="center">
    ADCS Bdot FIRMWARE PROJECT
    <br>
</h1>

<h4 align="center">Firmware project of the ADCS `Bdot Version` module.</h4>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#dependencies">Dependencies</a> •
    <a href="#rtems-toolchain">RTEMS toolchain</a> •
    <a href="#compiling-and-building">Compiling and building</a> •
    <a href="#flashing">Flashing</a>
</p>

## Overview

The product tree of the firmware can be seen below:

```text
rtems_project/
├── patches/            # Custom .patch files for the kernel
├── rtems-source/       # Original source code cloned from RTEMS
└── build_dir/          # Output directory for the compiled firmware
```

Compiling this RTEMS-based firmware requires applying patches to the base kernel to add support for specific hardware (BSPs) or to modify real-time behaviors before building.

## Dependencies

* RTEMS Project Toolchain
* st-link tools

### Installation on Ubuntu

```bash
sudo apt install gcc-arm-none-eabi stlink-tools
```

### Installation on Fedora

```bash
sudo dnf install gcc-arm-linux-gnu stlink
```

## RTEMS toolchain

`scripts/install-toolchain.sh` builds the RTEMS 6.2 `arm-rtems6` cross-compiler and the `arm/stm32f4` BSP from source via the RTEMS Source Builder, and installs them to `toolchain/` in this directory (git-ignored). This is a one-time setup step, separate from the app build in the next section.

```bash
./scripts/install-toolchain.sh
```

Override the install location with `RTEMS_PREFIX` (also update `RTEMS_PREFIX`/`RTEMS_PKG_CONFIG_PATH` passed to CMake if you do):

```bash
RTEMS_PREFIX=/opt/rtems/6 ./scripts/install-toolchain.sh
```

The build needs `curl`, `tar`, and a native C/C++/Python toolchain (`gcc`, `g++`, `make`, `bison`, `flex`, `texinfo`, `python3-dev`, `git`), plus `pax` if `--with-rtems-tests` is enabled (it is, by default). Package names differ per distro — see the [RTEMS POSIX host guide](https://ftp.rtems.org/pub/rtems/releases/6/6.2/docs/html/user/hosts/posix.html) for the exact list for your system. Expect the build to take a while (it builds gcc, newlib, and gdb from source).

## Compiling and building

The build process uses strictly relative paths. Follow the sequence below to download the source, apply patches, and compile:

**1. Download the RTEMS Source Code**
```bash
git clone git://git.rtems.org/rtems.git rtems-source
```

**2. Navigate to the Source Tree**
```bash
cd rtems-source
```

**3. Apply the Patch to the Kernel**
Apply the patch fetching the file from the sibling directory:
```bash
patch -p1 < ../patches/custom_bsp_fix.patch
```

**4. Configure the Build Environment**
Initialize the build context using the Waf build system targeting the STM32F4 BSP:
```bash
./waf configure --prefix=../build_dir --rtems-bsp=arm/stm32f4 --rtems-tools=../../opt/rtems/6
```

**5. Compile and Install**
Build the modified kernel and install the final artifacts into the output directory:
```bash
./waf build
./waf install
```

## Flashing

```bash
make flash
```
