/*
 * adcs_transformations.h
 *
 *  Created on: Jun 13, 2016
 *      Author: azisi
 */

#ifndef INC_ADCS_FRAME_H_
#define INC_ADCS_FRAME_H_

typedef struct {
    double x;
    double y;
    double z;
} xyz_t;

typedef struct {
    double lat;
    double lon;
    double alt;
} llh_t;

void ECEF2ECI(double jd, xyz_t from, xyz_t *to);
void ECI2ECEF(double jd, xyz_t from, xyz_t *to);
void ECI2NED(xyz_t from, xyz_t *to, double w, double i, double u);
void cart2spher(xyz_t from, llh_t *to);
void spher2cart(llh_t from, xyz_t *to);

double norm(double x1, double x2, double x3);

#endif /* INC_ADCS_FRAME_H_ */

