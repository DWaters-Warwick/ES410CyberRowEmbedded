/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */
#include <Wire.h>
#include "BLEDevice.h"
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ES410_BLE_Server.h"

#include "ES410_ForcePlate.h"

#define SENSOR_NODE_NAME "SEAT"

#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;

ES410_BLE_Server* pServer = NULL;

ES410_ForcePlate ForcePlate;

void setup() {
  Wire.begin(); //This resets to 100kHz I2C

  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Starting Arduino BLE Server application...");
  pServer = new ES410_BLE_Server();
  pServer->setup();
  Serial.println("Waiting a client connection to notify...");

  /* Initialise the sensor */
  Serial.println("Initialise Sensors...");
  int initError = ForcePlate.initialise();
  ForcePlate.setNodeName(SENSOR_NODE_NAME);
  
} // End of setup.


// This is the Arduino main loop function.
void loop() {
  tOn = millis();

  bool bUpdateError = ForcePlate.Update();
  //Serial.println(ForcePlate.OutputPlot());
  //int32_t tUpdated = millis

  //Handle BLE Server functions
  /*if (pServer->getConnectedCount() > 2) {
    /*String serialString = String("Hello ");
    serialString += tOn;
    serialString += " " + tUpdated;/
    String serialString = ForcePlate.OutputPlot();
    serialString += pServer->dataToString();
    Serial.print(serialString.c_str());
  }*/

  // disconnecting
  if (pServer->getConnectedCount() < 2) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.print(pServer->getConnectedCount());
    Serial.println(" Devices Connected");
    Serial.println("start advertising");
    //Serial.println(ForcePlate.OutputPlot());
}

String serialString = ForcePlate.OutputPlot();
serialString += pServer->dataToString();
Serial.print(serialString.c_str());
  
  delay(100); // Delay a second between loops.
} // End of loop
