/*!
 *  @file ES410_BNO055.h
 *
 *  This is a custom library for the BNO055 orientation sensor
 *
 *  This class is specificaly to encapsulate the needs of the 
 *  Warwick University ES410 project "Warwick Cyber Row"
 * 
 *  This class adds aditional functionality to this sensor such
 *  as storing data integrating acceleration.
 */

#ifndef _ES410_BNO055_H_
#define _ES410_BNO055_H_

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h>
#include <string>
#include <sstream>

class ES410_BNO055 : public Adafruit_BNO055 {
public:
    /* i2c parameters and variables */
    int8_t i2cAddr;
    TwoWire *wirePort;
    
    /* Timestamp of last sample */
    int32_t  tSample;
    
    /* Data extracted directly from BNO055 sensor as imu::vector<3> (x,y,z) */
    imu::Vector<3> vOrientation;
    imu::Vector<3> vAcceleration;
    imu::Vector<3> vMagnemometer;
    imu::Vector<3> vGyroscope;
    imu::Vector<3> vGravity;
    imu::Vector<3> vLinearAccel;

    /* Calculated values from imu data as imu::vector<3> (x,y,z) */    
    imu::Vector<3> vVelocity;
    imu::Vector<3> vOffset;

    /* Class Methods */
    //ES410_BNO055();
    ES410_BNO055(uint8_t i2cAddr, TwoWire *wirePort);
    int initialise();
    int UpdateEvents();
    const char * OutputString();

};

#endif //_ES410_BNO055_H_