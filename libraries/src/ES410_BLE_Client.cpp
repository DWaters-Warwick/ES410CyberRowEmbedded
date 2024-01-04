#include "ES410_BLE_Client.h"

ES410_BLE_Client::ES410_BLE_Client(const char * _serviceUUID, const char * _charUUID){
    serviceUUID = _serviceUUID;
    charUUID = _charUUID;

}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    //Serial.print("Notify callback for characteristic ");
    //Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    //Serial.print(" of data length ");
    //Serial.println(length);
    //Serial.print("data: ");
    //Serial.write(pData, length);
    //Serial.println();
}

bool ES410_BLE_Client::connectToServer(const char * serviceUUID, const char * charUUID) {
    //Serial.print("Forming a connection to ");
    //Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    //Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    //Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      //Serial.print("Failed to find our service UUID: ");
      //Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    //Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      //Serial.print("Failed to find our characteristic UUID: ");
      //Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    //Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      //Serial.print("The characteristic value was: ");
      //Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}


int ES410_BLE_Client::setup(){
    BLEDevice::init("");

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);

    return 0;
}

int ES410_BLE_Client::tryConnect(){
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
    // connected we set the connected flag to be true.
    if (doConnect == true) {
        if (connectToServer(serviceUUID, charUUID)) {
        //Serial.println("We are now connected to the BLE Server.");
        } else {
        //Serial.println("We have failed to connect to the server; there is nothin more we will do.");
        }
        doConnect = false;
        
    }
    return 0;
}    

int ES410_BLE_Client::Update(uint8_t * pData, int size){
        // If we are connected to a peer BLE Server, update the characteristic each time we are reached
        // with the current time since boot.
        if (connected) {
            // Set the characteristic's value to be the array of bytes that is actually a string.
            pRemoteCharacteristic->writeValue(pData, size, 0);
        }else if(doScan){
            BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
        }
        
        
        return 0;
    }
