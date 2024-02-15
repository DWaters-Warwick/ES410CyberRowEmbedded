/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */
#include <Wire.h>
#include "ES410_CombinedKinetics.h"
#include "ES410_BLE_Client.h"


#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;

ES410_CombinedKinetics CombinedKinetics;

BLEClient*  pClient  = BLEDevice::createClient();

static bool doConnect = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    
    Serial.println(" - Created client");

    // Connect to the remove BLE Server.
    bool connected = pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to serve: " + String(connected));
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
    Serial.println(" - MTU Set");
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

void setup() {
  Wire.begin(); //This resets to 100kHz I2C

  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  BLEScanResults AvailableDevices = pBLEScan->start(5, false);
  int NumBLEDevices = AvailableDevices.getCount();
  Serial.println("Out of scan");

  BLEAdvertisedDevice advertisedDevice;
  for (int i = 0; i < NumBLEDevices; i++) {
    advertisedDevice = AvailableDevices.getDevice(i);
    if(advertisedDevice.getServiceUUID().equals(BLEUUID(ES410_BLE_UUID_SERVICE))){
      Serial.println("FOUND ONE!!");
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      break;
    }
  }
  Serial.println("No Dice:(");

  

  

  /* Initialise the sensor */
  int initError = CombinedKinetics.initialise(&Wire);
  if(not(initError)){
    Serial.println("Initialisation Complete");
  } else {
    Serial.println("Initialisation Failed. Code:");
    Serial.println(initError);
  }
} // End of setup.


// This is the Arduino main loop function.
void loop() {
  tOn = millis();

  bool bUpdateError = CombinedKinetics.Update();
  Serial.println(CombinedKinetics.OutputPlot());


  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (pClient->isConnected()) {
    String newValue = CombinedKinetics.OutputPlot();
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }else{
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(1000); // Delay a second between loops.
} // End of loop
