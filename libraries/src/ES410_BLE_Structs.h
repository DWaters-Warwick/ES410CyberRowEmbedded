#include <stdint.h>

#ifndef _ES410_BLE_TRANS_STRUCTS_H_
#define _ES410_BLE_TRANS_STRUCTS_H_

struct ES410_BLE_Struct_Seat {
    int32_t tSample;

    float aCoM[2];
    float mTotal;

};

struct ES410_BLE_Struct_Trolly {
    int32_t tSample;

    float x;
    float v;
    float a;

};

struct ES410_BLE_Struct_Footplate {
    int32_t tSample;

    float x;
    float v;
    float a;

    float aCoF[2];
    float FTotal;
    
};

struct ES410_BLE_Struct_Handle {
    int32_t tSample;

    float a;

    float CoF;
    float FTotal;
    
};



#endif //_ES410_BLE_TRANS_STRUCTS_H_