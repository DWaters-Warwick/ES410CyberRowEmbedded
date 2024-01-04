#include "ES410_BLE_Server.h"


ES410_BLE_Server::ES410_BLE_Server(){};

int ES410_BLE_Server::setup(){
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pServiceSeat = pServer->createService(ES410_BLE_UUID_SERVICE_SEAT);
    BLEService *pServiceTrolly = pServer->createService(ES410_BLE_UUID_SERVICE_TROLLY);
    // Create a BLE Characteristic
    pCharSeat = pServiceSeat->createCharacteristic(
                        ES410_BLE_UUID_CHARACT_SEAT,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
    pCharTrolly = pServiceTrolly->createCharacteristic(
                        ES410_BLE_UUID_CHARACT_TROLLY,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharSeat->addDescriptor(new BLE2902());
    pCharTrolly->addDescriptor(new BLE2902());

    // Start the service
    pServiceSeat->start();
    pServiceTrolly->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ES410_BLE_UUID_SERVICE_SEAT);
    pAdvertising->addServiceUUID(ES410_BLE_UUID_SERVICE_TROLLY);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();

    return 0;
}


int ES410_BLE_Server::Update(){
    
    // notify changed value
    if (deviceConnected) {

        uint8_t *seatDataInput = pCharSeat->getData();
        uint8_t *trollyDataInput = pCharSeat->getData();

        memcpy(&seatData, seatDataInput ,sizeof(ES410_BLE_Struct_Seat));
        memcpy(&trollyData, trollyDataInput ,sizeof(ES410_BLE_Struct_Trolly));

        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        //Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    return 0;

}

const char * ES410_BLE_Server::OutputPlot(){
    std::ostringstream strOut;
    
    strOut << "tIMUSample:" << trollyData.tSample;
    strOut << ",";
    strOut << "FSeatTotal:" << seatData.mTotal << "," << "xSeatComX:" << seatData.aCoM[0] << "xSeatComY:" << seatData.aCoM[1];
    strOut << ",";
    strOut << "xTrollyEstimate:" << trollyData.x << "," << "vTrollyEstimate:" << trollyData.v << "," << "aTrollyEstimate:" << trollyData.a;
    strOut << "\n";

    return strOut.str().c_str();
};





