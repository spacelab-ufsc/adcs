<h1 align="center">
    ADCS HARDWARE
    <br>
</h1>

<h4 align="center">Attitude determination and control system hardware project (sources, outputs, and documentation).</h4>

<p align="center"
    <a a href="https://github.com/spacelab-ufsc/spacelab#versioning">
        <img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/adcs/releases">
        <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/adcs?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/adcs/releases">
        <img alt="GitHub commits since latest release (by date) for a branch" src="https://img.shields.io/github/commits-since/spacelab-ufsc/adcs/latest/dev_hardware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/adcs/commits/master">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/last-commit/spacelab-ufsc/adcs/dev_hardware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/adcs/issues">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/issues/spacelab-ufsc/adcs/hardware?style=for-the-badge">
    </a>
    <a href="">
        <img src="https://img.shields.io/badge/CAD%20tool-altium%20v19.2-yellow?style=for-the-badge">
    </a>
    <a href="#license">
        <img src="https://img.shields.io/badge/license-cern%20ohl%202-yellow?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#architecture">Architecture</a> •
    <a href="#development">Development</a> •
    <a href="#version">Version</a> •
    <a href="#license">License</a> •
    <a href="#notes">Notes</a>
</p>

## Overview


## Architecture

The board design is a microcontroller based hardware connecting different interfaces and peripherals. For more information refer to the [documentation](https://github.com/spacelab-ufsc/adcs/tree/master/doc) in the "Hardware" chapter.

<p align="center">
    <img width="70%" src="https://github.com/spacelab-ufsc/adcs/tree/dev_hardware/hardware/images/hardware_architecture_v01.png">
</p>

## Development


#### Manufacture

The folder [fabrication](https://github.com/spacelab-ufsc/adcs/tree/master/hardware/fabrication) contain 3 "ready to go" files: the gerbers and nc_drills for manufacturing the board, the BOM with all required components, and the pick_place file for automated assembly. Additional files are avaliable in the [outputs](https://github.com/spacelab-ufsc/adcs/tree/master/hardware/outputs) folder, which contain several useful files and documents, such as: 3D models, bill of materials, schematics, layout prints, and draftsman.

#### Assembly

The board has components that should not be soldered simultaneously. Refer to the [documentation](https://github.com/spacelab-ufsc/adcs/tree/master/doc) in the "Assembly" chapter.

#### Testing

Automated testing is not avaliable yet.

## Version

Refer to the [releases](https://github.com/spacelab-ufsc/adcs/releases) page.

## License

This repository containing hardware files is licensed under CERN Open Hardware License, version 2.

## Notes

Project under development!
