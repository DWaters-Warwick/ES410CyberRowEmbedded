/*!
 *  @file ES410_VL53L5CX.h
 *
 *  This is a custom library for the VL53L5CX ToF distance sensor.
 *
 *  This class is specificaly to encapsulate the needs of the 
 *  Warwick University ES410 project "Warwick Cyber Row"
 * 
 *  This class adds aditional functionality to this sensor such
 *  as storing data.
 */

#ifndef _ES410_VL53L5CX_H_
#define _ES410_VL53L5CX_H_

#include <Adafruit_Sensor.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX#include <utility/imumaths.h>
#include <Wire.h>
#include <string>
#include <sstream>

class ES410_VL53L5CX : public SparkFun_VL53L5CX {
public:
    /* i2c parameters and variables */
    int8_t i2cAddr;
    TwoWire *wirePort;
    
    /* Timestamp of last sample */
    int32_t  tSample;
    
    /* Data extracted directly from VL53L5CX sensor as */
    int16_t Distance[8][8];


    /* Class Methods */
    //ES410_VL53L5CX();
    ES410_VL53L5CX(int8_t i2cAddr, TwoWire *wirePort);
    int initialise();
    int UpdateEvents();
    const char * OutputString();

};

#endif //_ES410_VL53L5CX_H_