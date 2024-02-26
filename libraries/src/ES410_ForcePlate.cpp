#include "ES410_ForcePlate.h"

ES410_ForcePlate::ES410_ForcePlate(){

}

int ES410_ForcePlate::initialise(){

    LoadCell_1.begin(LOADCELL_1_DOUT_PIN, LOADCELL_1_SCK_PIN);
    LoadCell_2.begin(LOADCELL_2_DOUT_PIN, LOADCELL_2_SCK_PIN);
    LoadCell_3.begin(LOADCELL_3_DOUT_PIN, LOADCELL_3_SCK_PIN);
    LoadCell_4.begin(LOADCELL_4_DOUT_PIN, LOADCELL_4_SCK_PIN);
    Serial.println(LoadCell_1.is_ready());
    delay(100);
    LoadCell_1.set_scale(ES410_FORCEPLATE_LC1_SCALE);
    LoadCell_2.set_scale(ES410_FORCEPLATE_LC2_SCALE);
    LoadCell_3.set_scale(ES410_FORCEPLATE_LC3_SCALE);
    LoadCell_4.set_scale(ES410_FORCEPLATE_LC4_SCALE);
    delay(200);
    LoadCell_1.tare();
    LoadCell_2.tare();
    LoadCell_3.tare();
    LoadCell_4.tare();
    delay(1000);
    return 0;

}

int ES410_ForcePlate::Update(){
    tSample = millis();

    m1 = LoadCell_1.get_units();
    m2 = LoadCell_2.get_units();
    m3 = LoadCell_3.get_units();
    m4 = LoadCell_4.get_units();

    MassTotal = m1+m2+m3+m4;

    if(MassTotal > ES410_FORCEPLATE_COM_MINMASSLIMIT){
        CoM[0] = ((m1+m2)-(m3+m4))/MassTotal;
        CoM[1] = ((m1+m3)-(m2+m4))/MassTotal;
    } else {
        CoM[0] = 0;
        CoM[1] = 0;
    }

    
    return 0;

}

const char * ES410_ForcePlate::OutputPlot(){
    std::ostringstream strOutput;

    strOutput << "M1:" << m1 << ",M2:" << m2 << ", M3:" << m3 << ",M4:" << m4 << ",";
    strOutput << "X:" << CoM[0] << ",Y:" << CoM[1] << ",";
    strOutput << "Total:" << MassTotal;


    return strOutput.str().c_str();
}