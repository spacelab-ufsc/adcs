<h1 align="center">
    ADCS Bdot FIRMWARE PROJECT
    <br>
</h1>

<h4 align="center">Firmware project of the ADCS `Bdot Version` module.</h4>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#dependencies">Dependencies</a> •
    <a href="#compiling">Compiling and building</a> •
    <a href="#flashing">Flashing</a> •
</p>

## Overview

The product tree of the firmware can be seen below:

## Dependencies

* RTEMS Project
- st-link tools

### Installation on Ubuntu

```
sudo apt install gcc-arm-none-eabi stlink-tools
```

### Installation on Fedora

```
sudo dnf install gcc-arm-linux-gnu stlink
```

## Compiling and building

```make```

## Flashing

```make flash```
