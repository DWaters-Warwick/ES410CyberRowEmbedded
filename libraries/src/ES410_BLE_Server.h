#ifndef _ES410_BLE_SERVER_H_
#define _ES410_BLE_SERVER_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <sstream>
#include "Wire.h"

#include "ES410_BLE_UUID.h"
#include "ES410_BLE_Structs.h"

static bool deviceConnected = false;
static bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class ES410_BLE_Server{
private:
    


    BLEServer* pServer = NULL;
    BLECharacteristic* pCharSeat = NULL;
    BLECharacteristic* pCharTrolly = NULL;

    ES410_BLE_Struct_Seat seatData;
    ES410_BLE_Struct_Trolly trollyData;
    
public:

    ES410_BLE_Server();
    int setup();
    int Update();
    const char * OutputPlot();


};

#endif//_ES410_BLE_SERVER_H_