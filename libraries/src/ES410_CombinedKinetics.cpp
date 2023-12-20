#include "ES410_CombinedKinetics.h"

ES410_CombinedKinetics::ES410_CombinedKinetics(){
    
}

int ES410_CombinedKinetics::initialise(TwoWire *wirePort_init){
    Serial.println("Init Measurement Devices");
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
        bool start = ToFSensor->startRanging();
        Serial.println(start);
        delay(1000);
    } else {
        return 2;
    }

    Serial.println("Performing Zero Calibration");
    ZeroCalibration();
    Serial.println("Initialising Kalman Filter");
    initialiseKalman();

    return 0;
}

int ES410_CombinedKinetics::ZeroCalibration(){
    ZeroCentreIMUMeas = 0;
    ZeroCentreToFMeas = 0;

    for(int i = 0; i < ES410_COMBINEDKINETICS_CALIBRATION_TIME/ES410_COMBINEDKINETICS_CALIBRATION_TIMESTEP; i++){

        UpdateMeasurements();

        ZeroCentreIMUMeas = ZeroCentreIMUMeas + IMULinearAcceleration.z();
        ZeroCentreToFMeas = ZeroCentreToFMeas + ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2];
        //Serial.println(ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2]);
        delay(ES410_COMBINEDKINETICS_CALIBRATION_TIMESTEP);
    }
    

    ZeroCentreIMUMeas = ZeroCentreIMUMeas/(ES410_COMBINEDKINETICS_CALIBRATION_TIME/ES410_COMBINEDKINETICS_CALIBRATION_TIMESTEP);
    ZeroCentreToFMeas = ZeroCentreToFMeas/(ES410_COMBINEDKINETICS_CALIBRATION_TIME/ES410_COMBINEDKINETICS_CALIBRATION_TIMESTEP);
    Serial.print(ZeroCentreToFMeas);
    

    return 0;
}

int ES410_CombinedKinetics::initialiseKalman(){
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

    return 0;
}

int ES410_CombinedKinetics::Update(){
    UpdateMeasurements();
    UpdateKalman();

    return 0;
}
int ES410_CombinedKinetics::UpdateMeasurements(){
    int32_t tSampleLast = tSample;
    tSample = millis();
    dtSample = tSample - tSampleLast;

    IMULinearAcceleration = IMUSensor->getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    
    if(ToFSensor->isDataReady()){
        //Serial.println("Data ready");
        bool get = ToFSensor->getRangingData(&ToFMeasurementData);
        //Serial.println(get);
    } else {
        //Serial.println("NotReady");
    }

    return 0;
}

int ES410_CombinedKinetics::UpdateKalman(){
    float fdtSample = dtSample/1000.0;

    KFilter.F = {   1.0,    fdtSample,   fdtSample*fdtSample/2,
		            0.0,    1.0,        fdtSample,
                    0.0,    0.0,        1.0};

    BLA::Matrix<ES410_COMBINEDKINETICS_KALMAN_NOBS> observation;
    //Serial.println(ZeroCentreToFMeas);
    observation(0) = (ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2]-ZeroCentreToFMeas)/1000.0;
    observation(1) = IMULinearAcceleration.z() - ZeroCentreIMUMeas;
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

    strOutput << KFilter.x(0,0) << " " << KFilter.x(0,1) << " " << KFilter.x(0,2) << "\n";

    return strOutput.str().c_str();

}

const char * ES410_CombinedKinetics::OutputPlot(){
    std::ostringstream strOut;

    strOut << "P:" << KFilter.x(0,0) << ", V:" << KFilter.x(0,1) << ", A:" << KFilter.x(0,2) << "\n";

    strOut << "\n";
    return strOut.str().c_str();
}