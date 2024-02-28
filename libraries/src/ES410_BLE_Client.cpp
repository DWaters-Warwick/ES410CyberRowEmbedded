#include "ES410_BLE_Client.h"


bool ES410_BLE_Client::initialise(const char * cServiceUUID, const char * cCharUUID){
    serviceUUID = BLEUUID(cServiceUUID);
    charUUID = BLEUUID(cCharUUID);

    
    BLEDevice::init("ESP32");

    bool deviceFound = scanForServer();
    if(deviceFound){
        return connectToServer();
    }
    return false;
}

bool ES410_BLE_Client::scanForServer(){
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    Serial.println("Begin Scan for BLE Server...");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    BLEScanResults AvailableDevices = pBLEScan->start(5, false);
    int NumBLEDevices = AvailableDevices.getCount();
    Serial.println("Scan Complete. " + String(NumBLEDevices) + " devices found.");

    Serial.println("Finding Service in available devices...");
    BLEAdvertisedDevice advertisedDevice;
    bool deviceFound = false;
    for (int i = 0; i < NumBLEDevices; i++) {
        advertisedDevice = AvailableDevices.getDevice(i);
        if(advertisedDevice.getServiceUUID().equals(serviceUUID)){
            Serial.println("FOUND ONE!!");
            serverAdvitised = new BLEAdvertisedDevice(advertisedDevice);
            deviceFound = true;
            break;
        }
    }
    if(deviceFound){
        Serial.println("BLE Server Found in Network");
        return true;
    } else{
        Serial.println("Could Not Find Suitible BLE Server");
        return false;
    }
    return true;

}

bool ES410_BLE_Client::connectToServer(){
    Serial.print("Forming a connection to ");
    Serial.println(serverAdvitised->getAddress().toString().c_str());
    
    
    Serial.println(" - Created client");

    // Connect to the remove BLE Server.
    pClient->connect(serverAdvitised);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    return true;
}

bool ES410_BLE_Client::isConnected(){
    return pClient->isConnected();
}

bool ES410_BLE_Client::writeString(std::string string){
    Serial.println("Setting new characteristic value to \"" + String(string.c_str()) + "\"");
    if(isConnected()){
        pRemoteCharacteristic->writeValue(string.c_str(), string.length());
        std::string CheckData = pRemoteCharacteristic->readValue();
        if(string.compare(CheckData) != 0){
            Serial.println("Data not written correctly: ");
            Serial.println(CheckData.c_str());
            return false;
        } else {
            Serial.println("Data written successfully, Check Data: ");
            Serial.println(CheckData.c_str());
            return true;
        }
    } else {
        Serial.println("No Server Connected!");
        Serial.println(isConnected());
        return true;
    }
}