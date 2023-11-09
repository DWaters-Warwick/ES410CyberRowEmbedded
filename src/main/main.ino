#include <Wire.h>
#include "ES410_BNO055.h"

#define BAUD_RATE_SERIAL    9600  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;
ES410_BNO055 sensor1 = ES410_BNO055();

void setup(void) 
{
  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!sensor1.initialise())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

}

void loop(void) 
{
  tOn = millis();
  sensor1.UpdateEvents();

  if((tOn-tOutputLast) > SERIAL_OUTPUT_RATE) {
    Serial.print(sensor1.OutputString());
    tOutputLast = tOn;
  }
  

}