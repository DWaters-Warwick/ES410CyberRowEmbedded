#include <Wire.h>
#include "ES410_CombinedKinetics.h"

#define BAUD_RATE_SERIAL    115200  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  100  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;
ES410_CombinedKinetics CombinedKinetics;


void setup(void) 
{

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 

  //IMU = new ES410_BNO055(0x28,Wire);
  //ToF = new ES410_VL53L5CX(0x29,Wire);

  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Combined Kinetics Test \n Initialising ...");
  
  /* Initialise the sensor */
  int initError = CombinedKinetics.initialise(&Wire);
  if(not(initError)){
    Serial.println("Initialisation Complete");
  } else {
    Serial.println("Initialisation Failed. Code:");
    Serial.println(initError);
    while(1){}
  }
}

void loop(void) 
{
  tOn = millis();
  bool bUpdateError = CombinedKinetics.Update();
  while(bUpdateError){}

  if((tOn-tOutputLast) > SERIAL_OUTPUT_RATE) {
    Serial.print(CombinedKinetics.OutputPlot());
    tOutputLast = tOn;
  }
  

}