#ifndef _ES410_BLE_CLIENT_H_
#define _ES410_BLE_CLIENT_H_

#include <Arduino.h>

#include "BLEDevice.h"
#include "ES410_BLE_UUID.h"


class ES410_BLE_Client{
public:
    BLEUUID serviceUUID;
    BLEUUID charUUID;

    BLEClient* pClient = BLEDevice::createClient();
    BLEAdvertisedDevice* serverAdvitised;

    BLERemoteCharacteristic* pRemoteCharacteristic;

    bool initialise(const char * cServiceUUID, const char * cCharUUID);
    bool scanForServer();
    bool connectToServer();
    bool isConnected();
    bool writeString(std::string string);

};


#endif //_ES410_BLE_CLIENT_H_