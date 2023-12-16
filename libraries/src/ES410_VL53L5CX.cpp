/*!
 * @file VL53L5CX.cpp
 *
 *  @mainpage ES410 VL53L5CX Addaptation
 *
 *  @section intro_sec Introduction
 *
 *  Designed specifically to work with the pimoroni VL53L5CX Breakout.
 * 
 *
 *  @section author Author
 *
 *  D.Waters
 */
#include "ES410_VL53L5CX.h"

/*!
 *  @brief  Instantiates a new Sparkfun_VL53L5CX class
 */
//ES410_VL53L5CX::ES410_VL53L5CX();
ES410_VL53L5CX::ES410_VL53L5CX(int8_t i2cAddr, TwoWire *wirePort) : SparkFun_VL53L5CX(){
    this->i2cAddr = i2cAddr;
    this->wirePort = wirePort;
}

/*!
 *  @brief  Initialise sensor, to be called within startup
 *          function before first use.
 */
int ES410_VL53L5CX::initialise(){
    if(begin(0x29, *wirePort)){
        delay(1000);   
        setResolution(64);

        startRanging();
        
    } else {
        return 0;
    }
    return 1;    
}

/*!
 *  @brief  Update all 6 measurement data vectors and 2 integrated data vectors.
 */
int ES410_VL53L5CX::UpdateEvents(){

    int32_t tSampleLast = tSample;
    tSample = millis();
    float tSampleDelta = 0.001 * (tSample - tSampleLast);

    if(isDataReady()){
        VL53L5CX_ResultsData measurementData;
        getRangingData(&measurementData);

        memcpy(Distance, measurementData.distance_mm, 64);
    }


    return 1;
}

/*!
 *  @brief  Output last sensor measurement as a string.
 *  
 *  @return C string representation of last measurement event.
 */
const char * ES410_VL53L5CX::OutputString(){
    std::ostringstream strOutput;

    //The ST library returns the data transposed from zone mapping shown in datasheet
    //Pretty-print data with increasing y, decreasing x to reflect reality
    for (int y = 0 ; y <= 8 * (8 - 1) ; y += 8)
    {
        for (int x = 8 - 1 ; x >= 0 ; x--)
        {
            strOutput << "\t";
            strOutput << Distance[x + y];
        }
        strOutput << "\n";
    }
    strOutput << "\n";
    return strOutput.str().c_str();
    }