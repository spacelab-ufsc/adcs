/*
 * sun_pos.h
 *
 *  Created on: May 23, 2016
 *      Author: azisi
 */

#ifndef INC_SUN_POS_H_
#define INC_SUN_POS_H_

#include "../adcs_frame.h"

typedef struct {
    double JD_epoch;    // julian date, days from 4713 bc
    xyz_t sun_pos_eci;  // Position of Sun in ECI AU
    xyz_t sun_pos_ned;  // Position of Sun in NED AU
    double norm;
    double rtasc;
    double decl;
} _sun_vector;

extern _sun_vector sun_vector;

void update_sun(_sun_vector *sStr);

#endif /* INC_SUN_POS_H_ */

