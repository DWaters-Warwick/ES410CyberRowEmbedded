/*!
 *  @file ES410_CombinedKinetics.h
 *
 *  This is a custom library for the VL53L5CX ToF distance sensor.
 *  And the BNO055 9-DOF IMU. As a combined Kinetics module deriving Acceleration, Velocity, and Position.
 *
 *  This class is specificaly to encapsulate the needs of the 
 *  Warwick University ES410 project "Warwick Cyber Row"
 * 
 */

#ifndef _ES410_COMBINEDKINETICS_H_
#define _ES410_COMBINEDKINETICS_H_

#include <Wire.h>
#include <string>
#include <sstream>
//#include <utility/imumaths.h>

#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define ES410_COMBINEDKINETICS_IMU_I2CADDR 0x28
#define ES410_COMBINEDKINETICS_TOF_I2CADDR 0x29

#define ES410_COMBINEDKINETICS_TOF_RESOLUTION 8*8

class ES410_CombinedKinetics {
public:
    /* i2c parameters and variables */;
    TwoWire *wirePort;
    
    /* Timestamp of last sample */
    int32_t  tSample;
    
    SparkFun_VL53L5CX   *ToFSensor;
    Adafruit_BNO055     *IMUSensor;

    /* Data extracted directly from VL53L5CX sensor as */
    VL53L5CX_ResultsData    ToFMeasurementData;
    imu::Vector<3>          IMULinearAcceleration;


    /* Class Methods */
    ES410_CombinedKinetics();
    int initialise(TwoWire *wirePort_init);
    int UpdateMeasurements();
    const char * OutputString();

};

#endif //_ES410_COMBINEDKINETICS_H_