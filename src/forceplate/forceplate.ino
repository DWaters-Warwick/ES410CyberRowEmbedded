#include <Wire.h>
#include "ES410_BNO055.h"
#include "ES410_ForcePlate.h"

#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs


int32_t tOn;
int32_t tOutputLast;

ES410_ForcePlate ForcePlate;

void setup(void) 
{
  Serial.begin(BAUD_RATE_SERIAL);
  //Serial.println("Orientation Sensor Test"); Serial.println("");

  ForcePlate.initialise();

}

void loop(void) 
{
  tOn = millis();
  ForcePlate.Update();
  

  if((tOn-tOutputLast) > SERIAL_OUTPUT_RATE) {
    Serial.print(ForcePlate.OutputPlot());
    tOutputLast = tOn;
  }
  

}