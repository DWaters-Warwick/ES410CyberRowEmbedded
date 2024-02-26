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
    BLEUUID serviceUUID;
    BLEUUID charactUUID;

    BLEClient* pClient = BLEDevice::createClient();
    BLEAdvertisedDevice* serverAdvitised;

    BLERemoteCharacteristic* pRemoteCharacteristic;

    bool initialise(const char * cServiceUUID, const char * cCharactUUID);
    bool scanForServer();
    bool connectToServer();
    bool isConnected();
    bool writeString(std::string string);

};


#endif //_ES410_BLE_CLIENT_H_