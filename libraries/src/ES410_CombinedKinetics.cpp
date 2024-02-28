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

        uint8_t RangingFreq = ToFSensor->getRangingFrequency();
        SF_VL53L5CX_RANGING_MODE RangingMode = ToFSensor->getRangingMode();
        Serial.println("Ranging data initial:");
        Serial.println(RangingFreq);
        //Serial.println((RangingMode == SF_VL53L5CX_RANGING_MODE::CONTINUOUS));
        ToFSensor->setRangingFrequency(ES410_COMBINEDKINETICS_TOF_RANGING_FREQ);
        ToFSensor->setResolution(ES410_COMBINEDKINETICS_TOF_RESOLUTION);
        RangingFreq = ToFSensor->getRangingFrequency();
        Serial.println("Ranging frequency new:");
        Serial.println(RangingFreq);

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
    bool bError = UpdateMeasurements();
    if(bError==0){
        UpdateKalman();
    }

    return bError;
}
int ES410_CombinedKinetics::UpdateMeasurements(){

    int32_t t = millis();
    dtIMUSample = t - tIMUSample;
    tIMUSample = t;

    IMULinearAcceleration = IMUSensor->getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    
    //
    ToFMeasUpdated = false;
    if((t-tToFSample)>=ES410_COMBINEDKINETICS_TOF_SAMPLERATE && ToFSensor->isDataReady()){
        ToFMeasUpdated = true;
        
        dtToFSample = t - tToFSample;
        tToFSample = t;

        bool get = ToFSensor->getRangingData(&ToFMeasurementData);
        
        return 0;
    } else if ((t-tToFSample)>ES410_COMBINEDKINETICS_TOF_TIMEOUT){
        Serial.println("ToF Sensor timeout. No response within specified time.");

        return 1;
    }
    

    return 0;
}

int ES410_CombinedKinetics::UpdateKalman(){
    float fdtIMUSample = dtIMUSample/1000.0;
    float fdtTOFSample = dtToFSample/1000.0;

    // Check if ToF was sampled/Updated this time round,
    // if not update Kalman filter without P measurement
    if (ToFMeasUpdated){
        KFilter.H = { 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0};
    } else {
        KFilter.H = { 0.0, 0.0, 0.0,
                    0.0, 0.0, 1.0};    
    }

    KFilter.F = {   1.0,    fdtIMUSample,   fdtIMUSample*fdtIMUSample/2,
		            0.0,    1.0,        fdtIMUSample,
                    0.0,    0.0,        1.0};

    BLA::Matrix<ES410_COMBINEDKINETICS_KALMAN_NOBS> observation;
    //Serial.println(ZeroCentreToFMeas);
    observation(0) = -(ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2]-ZeroCentreToFMeas)/1000.0;
    observation(1) = (IMULinearAcceleration.z() - ZeroCentreIMUMeas);
    KFilter.update(observation);

    return 0;
}

const char * ES410_CombinedKinetics::OutputString(){
    std::ostringstream strOutput;

    strOutput << "Timestamp IMU: " << tIMUSample << "\n";
    strOutput << "Timestamp ToF: " << tToFSample << "\n";
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

    strOut << "P"<< nodeName <<":" << KFilter.x(0,0) << ", V"<< nodeName <<":" << KFilter.x(0,1) << ", A"<< nodeName <<":" << KFilter.x(0,2);
    strOut << ", PRaw"<< nodeName <<":" << (ToFMeasurementData.distance_mm[ES410_COMBINEDKINETICS_TOF_RESOLUTION/2] - ZeroCentreToFMeas)/1000 << ", ARaw"<< nodeName <<":" << IMULinearAcceleration.z();
    strOut << ", ToFUpdate"<< nodeName <<":" << ToFMeasUpdated << ", tIMUSample"<< nodeName <<":" << tIMUSample << ", tToFSample"<< nodeName <<":" << tToFSample;
    return strOut.str().c_str();
}

void ES410_CombinedKinetics::setNodeName(const char * _name){
    nodeName = _name;
}