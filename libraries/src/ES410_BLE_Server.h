#ifndef _ES410_BLE_CLIENT_H_
#define _ES410_BLE_CLIENT_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ES410_BLE_UUID.h"

class ES410_BLE_Server{
public:

    BLEServer* pServer;
    BLEService* pService;
    BLECharacteristic* pCharSeat;
    BLECharacteristic* pCharTrolly;

    ES410_BLE_Server();
    int setup();
    const char * dataToString();
    bool getConnectedCount();
    void startAdvertising();

};

#endif //_ES410_BLE_CLIENT_H_