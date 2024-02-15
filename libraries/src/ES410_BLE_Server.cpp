#include "ES410_BLE_Server.h"

ES410_BLE_Server::ES410_BLE_Server(){
    BLEDevice::init("ESP32");
    pServer = BLEDevice::createServer();
}

int ES410_BLE_Server::setup(){
    pServer->setCallbacks(new BLEServerCallbacks());

    // Create the BLE Service
    pService = pServer->createService(ES410_BLE_UUID_SERVICE);

    // Create a BLE Characteristic
    pCharSeat = pService->createCharacteristic(
                        ES410_BLE_UUID_CHARACT_SEAT,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharSeat->addDescriptor(new BLE2902());

    pCharTrolly = pService->createCharacteristic(
                        ES410_BLE_UUID_CHARACT_TROLLY,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharTrolly->addDescriptor(new BLE2902());
    
    
    

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ES410_BLE_UUID_SERVICE);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();

    return 0;
}

const char * ES410_BLE_Server::dataToString(){
    std::string output;
    output += pCharSeat->getValue();
    output += ",";
    output += pCharTrolly->getValue();

    return output.c_str();
}

bool ES410_BLE_Server::getConnectedCount(){
    return pServer->getConnectedCount();
}

void ES410_BLE_Server::startAdvertising(){
    pServer->startAdvertising();
}