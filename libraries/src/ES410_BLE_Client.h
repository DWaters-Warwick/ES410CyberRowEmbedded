#ifndef _ES410_BLE_CLIENT_H_
#define _ES410_BLE_CLIENT_H_

#include <Arduino.h>

#include "BLEDevice.h"
#include "ES410_BLE_UUID.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID(ES410_BLE_UUID_SERVICE);
static BLEUUID    charUUID(ES410_BLE_UUID_CHARACT_TROLLY);

class ES410_BLE_Client{
public:
    BLEClient* pClient;
    BLEAdvertisedDevice* myDevice;
    bool doConnect;

    BLERemoteCharacteristic* pRemoteCharacteristic;

    bool connectToServer();

};

#endif //_ES410_BLE_CLIENT_H_