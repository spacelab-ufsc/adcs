<h1 align="center">
	ATTITUDE DETERMINATION AND CONTROL SYSTEM
	<br>
</h1>

<h4 align="center">Attitude determination and control system designed and developed by SpaceLab.</h4>

<p align="center">
	<a href="https://github.com/spacelab-ufsc/spacelab#versioning">
		<img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/adcs/releases">
		<img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/adcs?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/adcs/releases">
		<img alt="GitHub commits since latest release (by date)" src="https://img.shields.io/github/commits-since/spacelab-ufsc/adcs/latest?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/adcs/commits/master">
		<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/spacelab-ufsc/adcs?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/adcs/issues">
		<img alt="GitHub issues" src="https://img.shields.io/github/issues/spacelab-ufsc/adcs?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/adcs/graphs/contributors">
		<img alt="GitHub contributors" src="https://img.shields.io/github/contributors/spacelab-ufsc/adcs?color=yellow&style=for-the-badge">
	</a>
</p>

<p align="center">
  	<a href="#overview">Overview</a> •
  	<a href="#repository-organization">Repository Organizarion</a> •
  	<a href="#license">License</a> •
  	<a href="#releases">Releases</a> •
  	<a href="#notes">Notes</a>
</p>

## Overview

The SpaceLab ADCS (Attitude Determination and Control System) is one of the modules developed for a 3U Cubesat mission. The ADCS is responsible for the active control and determination of the cubesat orientation in space. It has 3 magnetorquers, one for each axis, that are the actuators, the interaction of the actuators with the earth's magnetic fields will produce the torque for the estabilization or pointing of the cubesat. The module has de-tumbling and pointing capabilities.

## Repository Organization
	- doc: Technical documentation (including firmware, hardware, user guide, and datasheet).
	- firmware: ADCS module firmware project (sources and configs)
	- hardware: ADCS module hardware project (sources and outputs).

## License

This project is open-source under three different licenses: GNU General Public License v3.0 for firmware sources, CERN Open Hardware License v2.0 for hardware files, and CC BY-SA 4.0 for the documentation. Some third-part files and libraries are subjected to their specific terms and licenses.

## Releases

The ADCS software and hardware releases are synchronized in order to garantee compatibility. Then, using diferent versions might lead to unpredictable behavior. Refer to the [documentation](https://github.com/spacelab-ufsc/adcs/tree/master/doc) for compatibility notes.

## Notes

More info about the SpaceLab: [GitHub](https://github.com/spacelab-ufsc/spacelab) and [Website](https://spacelab.ufsc.br/en/home/)