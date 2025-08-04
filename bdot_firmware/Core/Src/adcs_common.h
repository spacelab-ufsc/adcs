/*
 * adcs_common.h
 *
 *  Created on: Jun 17, 2016
 *      Author: azisi
 */

#ifndef INC_ADCS_COMMON_H_
#define INC_ADCS_COMMON_H_

#define PI      3.141592654
#define TWOPI   6.283185308
#define HALFPI  1.570796327
#define DEG2RAD 0.017453293
#define RAD2DEG 57.295779506
#define RAD2RPM 60.0/(2*PI)

#define GEO_NAN log(-1.0)

#define SOLAR_DAY_MIN   1440.0                              // Minutes per 24 hours
#define SIDERIAL_DAY    (23.0*60.0 + 56.0 + 4.09054/60.0)   // Against stars
#define SOLAR_DAY_SEC   86400.0                             // Seconds per 24 hours
#define SOLAR_DAY_HOURS 24                                  // Hours for one day
#define JULIAN_GPS_TIME 2444244.5                           // Julian Date of GPS starting date
#define LEAP_SECOND     17                                  // Leap seconds of 2016
#define JD1900          2415020.5                           // Julian day number for Jan 1st, 00:00 hours 1900

#define EQRAD   6378.137                   // Earth radius at equator, km
#define LATCON  (1.0/298.257)               // Latitude radius constant
#define ECON    ((1.0-LATCON)*(1.0-LATCON))

#define MU          398600.5    // Earth gravitational constant for wgs-84 in km3 / s2
#define EARTH_RADII 6378E3      // Unit earth radii


/* Coordinate conversion macros */
#define DEG(x) ((x)/DEG2RAD)
#define RAD(x) ((x)*DEG2RAD)
#define GEOC(x) (atan(ECON*tan(x))) /* Geographic to geocentric. */
#define GEOG(x) (atan(tan(x)/ECON))

/* TLE update */
#define CD      2.3         // Drag Coefficient clear number
#define RHO     5.75E-13    // Atmospheric Density Kg/m^3 in 400 km
#define MASS    2.3         // Mass of cubesat in Kg
#define AREA    0.1*0.1     // Cross-sectional effective Area m^2

#endif /* INC_ADCS_COMMON_H_ */

