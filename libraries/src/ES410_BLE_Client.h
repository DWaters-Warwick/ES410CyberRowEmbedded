#ifndef _ES410_BLE_CLIENT_H_
#define _ES410_BLE_CLIENT_H_

#include <stdint.h>
#include <sstream>
#include "Wire.h"

#include "BLEDevice.h"

#include "ES410_BLE_Structs.h"
#include "ES410_BLE_UUID.h"

static bool doConnect = false;
static bool connected = false;
static bool doScan = false;

static BLEAdvertisedDevice* myDevice;

static BLEUUID serviceUUID1(ES410_BLE_UUID_SERVICE_TROLLY);


static void notifyCallback(
        BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, 
        size_t length, bool isNotify);
 

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    //Serial.println("onDisconnect");
  }
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    //Serial.print("BLE Advertised Device found: ");
    //Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID1)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

class ES410_BLE_Client{
private:

    const char * serviceUUID;
    const char * charUUID;

    BLERemoteCharacteristic* pRemoteCharacteristic;

   bool connectToServer(const char * serviceUUID, const char * charUUID);
    
public:

    ES410_BLE_Client(const char * _serviceUUID, const char * _charUUID);
    int tryConnect();
    int setup();
    int Update(uint8_t * pData, int size);


};

#endif//_ES410_BLE_CLIENT_H_