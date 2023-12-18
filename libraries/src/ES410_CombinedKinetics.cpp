#include "ES410_CombinedKinetics.h"

ES410_CombinedKinetics::ES410_CombinedKinetics(){
    
}

int ES410_CombinedKinetics::initialise(TwoWire *wirePort_init){
    wirePort = wirePort_init;

    ToFSensor = new SparkFun_VL53L5CX(); 
    IMUSensor = new Adafruit_BNO055(-1,ES410_COMBINEDKINETICS_IMU_I2CADDR, wirePort);
    
    if(IMUSensor->begin()){
        delay(1000);
        IMUSensor->setExtCrystalUse(true);
    } else {
        return 1;
    }

    if(ToFSensor->begin(ES410_COMBINEDKINETICS_TOF_I2CADDR, *wirePort)){
        delay(1000);
        ToFSensor->setResolution(ES410_COMBINEDKINETICS_TOF_RESOLUTION);
        ToFSensor->startRanging();
    } else {
        return 2;
    }

    // time evolution matrix (whatever... it will be updated inloop)
    KFilter.F = { 1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0};

    // measurement matrix n the position (e.g. GPS) and acceleration (e.g. accelerometer)
    KFilter.H = { 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0};
    // measurement covariance matrix
    KFilter.R = {   ES410_COMBINEDKINETICS_KALMAN_NOISE_P*ES410_COMBINEDKINETICS_KALMAN_NOISE_P,    0.0,
                    0.0,        ES410_COMBINEDKINETICS_KALMAN_NOISE_A*ES410_COMBINEDKINETICS_KALMAN_NOISE_A};
    // model covariance matrix
    KFilter.Q = { m_p*m_p,    0.0,        0.0,
            0.0,        m_s*m_s,    0.0,
            0.0,        0.0,        m_a*m_a};

    tSample = millis();

    return 0;
}

int ES410_CombinedKinetics::UpdateMeasurements(){
    int32_t tSampleLast = tSample;
    tSample = millis();
    dtSample = tSample - tSampleLast;
    float fdtSample = dtSample/1000.0;

    IMULinearAcceleration = IMUSensor->getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    
    if(ToFSensor->isDataReady()){
        ToFSensor->getRangingData(&ToFMeasurementData);
    }

    KFilter.F = {   1.0,    fdtSample,   fdtSample*fdtSample/2,
		            0.0,    1.0,        fdtSample,
                    0.0,    0.0,        1.0};

    BLA::Matrix<ES410_COMBINEDKINETICS_KALMAN_NOBS> observation;
    observation(0) = (ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2])/1000.0;
    observation(1) = IMULinearAcceleration.z();
    KFilter.update(observation);

    return 0;
}

const char * ES410_CombinedKinetics::OutputString(){
    std::ostringstream strOutput;

    strOutput << "Timestamp: " << tSample << "\n";
    strOutput << "Linear Acceleration:"    << " X: " << IMULinearAcceleration.x();
    strOutput << " Y: " << IMULinearAcceleration.y();
    strOutput << " Z: " << IMULinearAcceleration.z() << "\n";

    for (int y = 0 ; y <= 8 * (8 - 1) ; y += 8)
    {
        for (int x = 8 - 1 ; x >= 0 ; x--)
        {
            strOutput << "\t";
            strOutput << ToFMeasurementData.distance_mm[x + y];
        }
        strOutput << "\n";
    }
    strOutput << "\n";

    strOutput << KFilter.x(0,0) << " " << KFilter.x(0,1) << KFilter.x(0,2) << "\n";

    return strOutput.str().c_str();

    
}