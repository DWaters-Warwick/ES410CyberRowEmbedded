/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */
#include <Wire.h>
#include "ES410_Handle.h"
#include "ES410_BLE_Client.h"


#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;

ES410_Handle Handle;

ES410_BLE_Client BLEClient;

void setup() {
  Wire.begin(); //This resets to 100kHz I2C

  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Starting Arduino BLE Client application...");
  
  BLEClient.initialise(ES410_BLE_UUID_SERVICE, ES410_BLE_UUID_CHARACT_TROLLY);

  /* Initialise the sensor */
  int initError = Handle.initialise(&Wire);
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

  bool bUpdateError = Handle.Update();

  BLEClient.writeString(Handle.OutputPlot());
  if (BLEClient.pClient->isConnected() == false) {
    if (BLEClient.connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
  }

  
  
  delay(1000); // Delay a second between loops.
} // End of loop