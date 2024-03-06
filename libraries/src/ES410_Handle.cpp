#include "ES410_Handle.h"
#include "ES410_Serial.h"
    
int ES410_Handle::initialise(){
    LoadCell_1.begin(LOADCELL_1_DOUT_PIN, LOADCELL_1_SCK_PIN);
    LoadCell_2.begin(LOADCELL_2_DOUT_PIN, LOADCELL_2_SCK_PIN);
    LoadCell_3.begin(LOADCELL_3_DOUT_PIN, LOADCELL_3_SCK_PIN);
    delay(100);
    LoadCell_1.set_scale(ES410_HANDLE_SCALE);
    LoadCell_2.set_scale(ES410_HANDLE_SCALE);
    LoadCell_3.set_scale(ES410_HANDLE_SCALE);
    LoadCell_1.set_offset(ES410_HANDLE_OFFSET);
    LoadCell_2.set_offset(ES410_HANDLE_OFFSET);
    LoadCell_3.set_offset(ES410_HANDLE_OFFSET);
    delay(200);
    LoadCell_1.tare();
    LoadCell_2.tare();
    LoadCell_3.tare();
    delay(1000);
    return 0;
}

int ES410_Handle::Update(){
    tSample = millis();

    F1 = LoadCell_1.get_units();
    F2 = LoadCell_2.get_units();
    F3 = LoadCell_3.get_units() ;

    PullForce = F1+F2;

    Torque = F1-F2;
    
    return 0;
}

const char * ES410_Handle::OutputPlot(){
    std::ostringstream strOutput;
    strOutput << "F1"<< nodeName <<":" << F1 << ",F2"<< nodeName <<":" << F2 << ",F3"<< nodeName <<":" << F3;
    strOutput << ",Torque"<< nodeName <<":" << Torque << ",PullForce"<< nodeName <<":" << PullForce << ",";
    strOutput << "tFPSample" << nodeName <<":" << tSample;

    return strOutput.str().c_str();
}

void ES410_Handle::setNodeName(const char * _name){
    nodeName = _name;
}