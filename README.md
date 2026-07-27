<h1 align="center">
    ADCS Bdot FIRMWARE PROJECT
    <br>
</h1>

<h4 align="center">Firmware project of the ADCS `Bdot Version` module.</h4>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#dependencies">Dependencies</a> •
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
Initialize the build context using the Waf build system. Adjust the toolchain relative path as needed:
```bash
./waf configure --prefix=../build_dir --rtems-bsp=arm/xilinx_zynq_a9_qemu --rtems-tools=../../opt/rtems/6
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

*(Optional) Verification*
To confirm the build was successful, you can test the binary in an emulator before flashing to the board:
```bash
cd ../build_dir
qemu-system-arm -M xilinx-zynq-a9 -m 256M -no-reboot -serial null -serial mon:stdio -nographic -kernel ../build_dir/arm-rtems6/xilinx_zynq_a9_qemu/testsuites/samples/hello.exe
```
