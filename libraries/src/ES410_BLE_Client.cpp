#include "ES410_BLE_Client.h"
#include "ES410_Serial.h"

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
    SERIAL_LOG_DEBUG("Begin Scan for BLE Server...");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    BLEScanResults AvailableDevices = pBLEScan->start(5, false);
    int NumBLEDevices = AvailableDevices.getCount();
    SERIAL_LOG_DEBUG("Scan Complete. " + String(NumBLEDevices) + " devices found.");

    SERIAL_LOG_DEBUG("Finding Service in available devices...");
    BLEAdvertisedDevice advertisedDevice;
    bool deviceFound = false;
    for (int i = 0; i < NumBLEDevices; i++) {
        advertisedDevice = AvailableDevices.getDevice(i);
        if(advertisedDevice.getServiceUUID().equals(serviceUUID)){
            SERIAL_LOG_DEBUG("FOUND ONE!!");
            serverAdvitised = new BLEAdvertisedDevice(advertisedDevice);
            deviceFound = true;
            break;
        }
    }
    if(deviceFound){
        SERIAL_LOG_DEBUG("BLE Server Found in Network");
        return true;
    } else{
        SERIAL_LOG_DEBUG("Could Not Find Suitible BLE Server");
        return false;
    }
    return true;

}

bool ES410_BLE_Client::connectToServer(){
    SERIAL_LOG_DEBUG("Forming a connection to ");
    SERIAL_LOG_DEBUG(serverAdvitised->getAddress().toString().c_str());
    
    
    SERIAL_LOG_DEBUG(" - Created client");

    // Connect to the remove BLE Server.
    pClient->connect(serverAdvitised);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    SERIAL_LOG_DEBUG(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      SERIAL_LOG_DEBUG("Failed to find our service UUID: ");
      SERIAL_LOG_DEBUG(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    SERIAL_LOG_DEBUG(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      SERIAL_LOG_DEBUG("Failed to find our characteristic UUID: ");
      SERIAL_LOG_DEBUG(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    SERIAL_LOG_DEBUG(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      SERIAL_LOG_DEBUG("The characteristic value was: ");
      SERIAL_LOG_DEBUG(value.c_str());
    }

    return true;
}

bool ES410_BLE_Client::isConnected(){
    return pClient->isConnected();
}

bool ES410_BLE_Client::writeString(std::string string){
    SERIAL_LOG_DEBUG("Setting new characteristic value to \"" + String(string.c_str()) + "\"");
    if(isConnected()){
        pRemoteCharacteristic->writeValue(string.c_str(), string.length());
        std::string CheckData = pRemoteCharacteristic->readValue();
        if(string.compare(CheckData) != 0){
            SERIAL_LOG_DEBUG("Data not written correctly: ");
            SERIAL_LOG_DEBUG(CheckData.c_str());
            return false;
        } else {
            SERIAL_LOG_DEBUG("Data written successfully, Check Data: ");
            SERIAL_LOG_DEBUG(CheckData.c_str());
            return true;
        }
    } else {
        SERIAL_LOG_DEBUG("No Server Connected!");
        SERIAL_LOG_DEBUG(isConnected());
        return true;
    }
}