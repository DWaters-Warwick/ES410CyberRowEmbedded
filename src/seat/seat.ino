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

#define TICK_RATE_MAIN      10
#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  100  //Time in ms between serial outputs
#define BLE_OUTPUT_RATE     1000

int32_t tOn;
int32_t tSerialLast;
int32_t tBLELast;

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

  // 
  if (pServer->getConnectedCount() == 0) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.print(pServer->getConnectedCount());
    Serial.println(" Devices Connected");
    Serial.println("start advertising");
}


std::string output = ForcePlate.OutputPlot();
output += pServer->dataToString();
if ((tOn - tSerialLast) >= SERIAL_OUTPUT_RATE){
  Serial.println(output.c_str());
  tSerialLast = tOn;
}

/* Seat is acting as server, cannot write as client
if ((tOn - tBLELast) >= BLE_OUTPUT_RATE){
  BLEClient.writeString(output);
  tBLELast = tOn;
}*/


  
  delay(TICK_RATE_MAIN); // Delay a second between loops.
} // End of loop
