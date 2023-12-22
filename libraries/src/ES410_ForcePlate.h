/*!
 *  @file ES410_ForcePlate.h
 *
 *  This is a custom library for the BNO055 orientation sensor
 *
 *  This class is specificaly to encapsulate the needs of the 
 *  Warwick University ES410 project "Warwick Cyber Row"
 * 
 *  This class adds aditional functionality to this sensor such
 *  as storing data integrating acceleration.
 */

#ifndef _ES410_FORCEPLATE_H_
#define _ES410_FORCEPLATE_H_

#include "HX711.h"
#include <string>
#include <sstream>

#define LOADCELL_1_DOUT_PIN 13
#define LOADCELL_1_SCK_PIN 12

#define LOADCELL_2_DOUT_PIN 27
#define LOADCELL_2_SCK_PIN 26

#define LOADCELL_3_DOUT_PIN LOADCELL_1_DOUT_PIN
#define LOADCELL_3_SCK_PIN LOADCELL_1_SCK_PIN

#define LOADCELL_4_DOUT_PIN LOADCELL_2_DOUT_PIN
#define LOADCELL_4_SCK_PIN LOADCELL_2_SCK_PIN

#define ES410_FORCEPLATE_FX19_SCALE 770000
#define ES410_FORCEPLATE_FX29_SCALE 900000
#define ES410_FORCEPLATE_LC1_SCALE ES410_FORCEPLATE_FX19_SCALE
#define ES410_FORCEPLATE_LC2_SCALE ES410_FORCEPLATE_FX29_SCALE
#define ES410_FORCEPLATE_LC3_SCALE ES410_FORCEPLATE_FX19_SCALE
#define ES410_FORCEPLATE_LC4_SCALE ES410_FORCEPLATE_FX29_SCALE

#define ES410_FORCEPLATE_COM_MINMASSLIMIT 0.1

class ES410_ForcePlate {
public:
    /* Timestamp of last sample */
    int32_t  tSample;

    HX711 LoadCell_1;
    HX711 LoadCell_2;
    HX711 LoadCell_3;
    HX711 LoadCell_4;

    float m1;
    float m2;
    float m3;
    float m4;

    float CoM[2];
    float MassTotal;

    ES410_ForcePlate();
    int initialise();
    int Update();
    const char * OutputPlot();


};

#endif //_ES410_FORCEPLATE_H_