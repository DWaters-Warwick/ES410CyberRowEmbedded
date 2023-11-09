/*!
 * @file ES410_BNO055.cpp
 *
 *  @mainpage ES410 BNO055 Addaptation
 *
 *  @section intro_sec Introduction
 *
 *  Designed specifically to work with the Adafruit BNO055 9-DOF Breakout.
 * 
 *
 *  @section author Author
 *
 *  D.Waters
 */
#include "ES410_BNO055.h"

/*!
 *  @brief  Instantiates a new Adafruit_BNO055 class
 */
ES410_BNO055::ES410_BNO055() : Adafruit_BNO055(55){
    vVelocity   = 0;
    vOffset     = 0;
}

/*!
 *  @brief  Initialise sensor, to be called within startup
 *          function before first use.
 */
int ES410_BNO055::initialise(){
    if(begin()){
        delay(1000);   
        setExtCrystalUse(true);
        return 1;
    }
    return 0;    
}

/*!
 *  @brief  Update all 6 measurement data vectors and 2 integrated data vectors.
 */
int ES410_BNO055::UpdateEvents(){

    int32_t tSampleLast = tSample;
    tSample = millis();
    float tSampleDelta = 0.001 * (tSample - tSampleLast);

    imu::Vector<3> vAccelerationPrev = vAcceleration;
    imu::Vector<3> vVelocityPrev = vVelocity;

    vOrientation    = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_EULER);
    vAcceleration   = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_ACCELEROMETER);
    vMagnemometer   = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_MAGNETOMETER);
    vGyroscope      = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GYROSCOPE);
    vGravity        = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GRAVITY);
    vLinearAccel    = getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);

    vVelocity   = vVelocityPrev + ((vAccelerationPrev + vAcceleration)/2 * tSampleDelta);
    vOffset     = vOffset + ((vVelocityPrev + vVelocity)/2 * tSampleDelta);

    return 1;
}

/*!
 *  @brief  Output last sensor measurement as a string.
 *  
 *  @return C string representation of last measurement event.
 */
const char * ES410_BNO055::OutputString(){
    std::ostringstream strOutput;

    strOutput << "Timestamp: " << tSample;
    strOutput << "Orientation:"            << " X: " << vOrientation.x()     << " Y: " << vOrientation.y()     << " Z: " << vOrientation.z()     << "\n";
    strOutput << "Acceleration:"           << " X: " << vAcceleration.x()    << " Y: " << vAcceleration.y()    << " Z: " << vAcceleration.z()    << "\n";
    strOutput << "Magnemometer:"           << " X: " << vMagnemometer.x()    << " Y: " << vMagnemometer.y()    << " Z: " << vMagnemometer.z()    << "\n";
    strOutput << "Gyroscope:"              << " X: " << vGyroscope.x()       << " Y: " << vGyroscope.y()       << " Z: " << vGyroscope.z()       << "\n";
    strOutput << "Gravity:"                << " X: " << vGravity.x()         << " Y: " << vGravity.y()         << " Z: " << vGravity.z()         << "\n";
    strOutput << "Linear Acceleration:"    << " X: " << vLinearAccel.x()     << " Y: " << vLinearAccel.y()     << " Z: " << vLinearAccel.z()     << "\n";
    
    return strOutput.str().c_str();
}