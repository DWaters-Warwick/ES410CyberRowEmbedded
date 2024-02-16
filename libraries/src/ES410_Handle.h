#ifndef _ES410_HANDLE_H_
#define _ES410_HANDLE_H_

#include "HX711.h"
#include <string>
#include <sstream>

#define LOADCELL_1_DOUT_PIN 13
#define LOADCELL_1_SCK_PIN 12

#define LOADCELL_2_DOUT_PIN 27
#define LOADCELL_2_SCK_PIN 26

#define LOADCELL_3_DOUT_PIN LOADCELL_1_DOUT_PIN
#define LOADCELL_3_SCK_PIN LOADCELL_1_SCK_PIN

#define ES410_FORCEPLATE_FX19_SCALE 770000
#define ES410_FORCEPLATE_FX29_SCALE 900000

#define ES410_FORCEPLATE_COM_MINMASSLIMIT 0.1

class ES410_Handle {
public:
    int32_t  tSample;

    HX711 LoadCell_1;
    HX711 LoadCell_2;
    HX711 LoadCell_3;
    
    float F1;
    float F2;
    float F3;

    float PullForce;
    float Torque;

    int initialise();
    int Update();
    const char * OutputPlot();


};





#endif