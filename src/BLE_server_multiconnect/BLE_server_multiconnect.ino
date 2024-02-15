/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ES410_BLE_Server.h"

ES410_BLE_Server* pServer = NULL;



// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

void setup() {
    Serial.println("Initialising USB Serial...");

    Serial.begin(115200);

    //BLEDevice::init("ESP32");
    //pServer = BLEDevice::createServer();
    // Create the BLE Device
    Serial.println("Initialise Bluetooth...");

    pServer = new ES410_BLE_Server();
    Serial.println("Setup Bluetooth...");

    // Create the BLE Server
    pServer->setup();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value

    if (pServer->getConnectedCount() > 0) {
        Serial.println(pServer->dataToString());
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }

    // disconnecting
    if (pServer->getConnectedCount() == 0) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
    }

}
