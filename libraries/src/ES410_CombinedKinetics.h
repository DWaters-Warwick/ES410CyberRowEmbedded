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
#include <Kalman.h>
//#include <utility/imumaths.h>

#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define ES410_COMBINEDKINETICS_IMU_I2CADDR 0x28
#define ES410_COMBINEDKINETICS_TOF_I2CADDR 0x29

#define ES410_COMBINEDKINETICS_TOF_RESOLUTION 4*4

#define ES410_COMBINEDKINETICS_KALMAN_NSTATE    3
#define ES410_COMBINEDKINETICS_KALMAN_NOBS      2
#define ES410_COMBINEDKINETICS_KALMAN_NOISE_P   0.3
#define ES410_COMBINEDKINETICS_KALMAN_NOISE_A   5.0

#define ES410_COMBINEDKINETICS_CALIBRATION_TIME 1000
#define ES410_COMBINEDKINETICS_CALIBRATION_TIMESTEP 100

#define ES410_COMBINEDKINETICS_TOF_SAMPLERATE   10
#define ES410_COMBINEDKINETICS_IMU_SAMPLERATE   1
#define ES410_COMBINEDKINETICS_TOF_TIMEOUT      10000


#define m_p 0.1
#define m_s 0.1
#define m_a 0.8



class ES410_CombinedKinetics {
public:
    /* i2c parameters and variables */;
    TwoWire *wirePort;
    
    /* Timestamp of last sample */
    int32_t  tIMUSample;
    int32_t  tToFSample;
    int32_t  dtIMUSample;
    int32_t  dtToFSample;
    
    SparkFun_VL53L5CX   *ToFSensor;
    Adafruit_BNO055     *IMUSensor;

    /* Data extracted directly from VL53L5CX sensor as */
    VL53L5CX_ResultsData    ToFMeasurementData;
    imu::Vector<3>          IMULinearAcceleration;

    double ZeroCentreToFMeas;//[ES410_COMBINEDKINETICS_TOF_RESOLUTION];
    double ZeroCentreIMUMeas;

    KALMAN<ES410_COMBINEDKINETICS_KALMAN_NSTATE,ES410_COMBINEDKINETICS_KALMAN_NOBS>
        KFilter;


    /* Class Methods */
    ES410_CombinedKinetics();
    
    int initialise(TwoWire *wirePort_init);
    int initialiseKalman();
    int ZeroCalibration();

    int Update();
    int UpdateMeasurements();
    int UpdateKalman();
    
    const char * OutputString();
    const char * OutputPlot();

};

#endif //_ES410_COMBINEDKINETICS_H_