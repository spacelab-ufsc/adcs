#include <math.h>
#include <stdlib.h>

#include "sun_pos.h"
#include "../adcs_common.h"

_sun_vector sun_vector = { .JD_epoch = 0, .sun_pos_eci.x = 0, .sun_pos_eci.y = 0,
                .sun_pos_eci.z = 0, .sun_pos_ned.x = 0, .sun_pos_ned.y = 0,
                .sun_pos_ned.z = 0, .norm = 0, .decl = 0, .rtasc = 0 };

/* ------------------------------------------------------------------------------
 *
 *                           function sun
 *
 *  this function calculates the geocentric equatorial position vector
 *    the sun given the julian date.  this is the low precision formula and
 *    is valid for years from 1950 to 2050.  accuaracy of apparent coordinates
 *    is 0.01  degrees.  notice many of the calculations are performed in
 *    degrees, and are not changed until later.  this is due to the fact that
 *    the almanac uses degrees exclusively in their formulations.
 *
 *  author        : david vallado                  719-573-2600   27 may 2002
 *
 *  revisions
 *    vallado     - fix mean lon of sun                            7 mat 2004
 *
 *  inputs          description                    range / units
 *    jd          - julian date                    days from 4713 bc
 *
 *  outputs       :
 *    rsun        - ijk position vector of the sun AU
 *    rtasc       - right ascension                rad
 *    decl        - declination                    rad
 *
 *  locals        :
 *    meanlong    - mean longitude
 *    meananomaly - mean anomaly
 *    eclplong    - ecliptic longitude
 *    obliquity   - mean obliquity of the ecliptic
 *    tut1        - julian centuries of ut1 from
 *                  jan 1, 2000 12h
 *    ttdb        - julian centuries of tdb from
 *                  jan 1, 2000 12h
 *    hr          - hours                          0 .. 24              10
 *    min         - minutes                        0 .. 59              15
 *    sec         - seconds                        0.0  .. 59.99          30.00
 *    temp        - temporary variable
 *    deg         - degrees
 *
 *  coupling      :
 *    none.
 *
 *  references    :
 *    vallado       2007, 281, alg 29, ex 5-1
 * --------------------------------------------------------------------------- */

void update_sun(_sun_vector *sStr) {
    double tut1, meanlong, ttdb, meananomaly, eclplong, obliquity, magr;

    // -------------------------  implementation   -----------------
    // -------------------  initialize values   --------------------
    tut1 = (sStr->JD_epoch - 2451545.0) / 36525.0;

    meanlong = 280.460 + 36000.77 * tut1;
    meanlong = fmod(meanlong, 360.0); //deg

    ttdb = tut1;
    meananomaly = 357.5277233 + 35999.05034 * ttdb;
    meananomaly = fmod(meananomaly * DEG2RAD, TWOPI); //rad
    if (meananomaly < 0.0) {
        meananomaly = TWOPI + meananomaly;
    }
    eclplong = meanlong + 1.914666471 * sin(meananomaly)
            + 0.019994643 * sin(2.0 * meananomaly); //deg
    obliquity = 23.439291 - 0.0130042 * ttdb; //deg
    meanlong = meanlong * DEG2RAD;
    if (meanlong < 0.0) {
        meanlong = TWOPI + meanlong;
    }
    eclplong = eclplong * DEG2RAD;
    obliquity = obliquity * DEG2RAD;

    // --------- find magnitude of sun vector, )   components ------
    magr = 1.000140612 - 0.016708617 * cos(meananomaly)
            - 0.000139589 * cos(2.0 * meananomaly); // in au's

    sStr->sun_pos_eci.x = magr * cos(eclplong);
    sStr->sun_pos_eci.y = magr * cos(obliquity) * sin(eclplong);
    sStr->sun_pos_eci.z = magr * sin(obliquity) * sin(eclplong);

    sStr->rtasc = atan(cos(obliquity) * tan(eclplong));

    // --- check that rtasc is in the same quadrant as eclplong ----
    if (eclplong < 0.0) {
        eclplong = eclplong + TWOPI;    // make sure it's in 0 to 2pi range
    }
    if ((abs(eclplong - sStr->rtasc)) > PI * 0.5) {
        sStr->rtasc = sStr->rtasc
                + 0.5 * PI * round((eclplong - sStr->rtasc) / (0.5 * PI));
    }
    sStr->decl = asin(sin(obliquity) * sin(eclplong));
}

