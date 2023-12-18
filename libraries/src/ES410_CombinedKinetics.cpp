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

    return 0;
}

int ES410_CombinedKinetics::UpdateMeasurements(){
    tSample = millis();

    IMULinearAcceleration = IMUSensor->getVector(Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    
    if(ToFSensor->isDataReady()){
        ToFSensor->getRangingData(&ToFMeasurementData);
    }

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

    return strOutput.str().c_str();
}