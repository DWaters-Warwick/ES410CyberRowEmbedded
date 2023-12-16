#include <Wire.h>
#include "ES410_BNO055.h"
#include "ES410_VL53L5CX.h"

#define BAUD_RATE_SERIAL    9600  //Baud rate for standard serial updates
#define SERIAL_OUTPUT_RATE  1000  //Time in ms between serial outputs

int32_t tOn;
int32_t tOutputLast;
ES410_BNO055 IMU = ES410_BNO055(0x28,&Wire);
ES410_VL53L5CX ToF = ES410_VL53L5CX(0x29,&Wire);


void setup(void) 
{

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 

  //IMU = new ES410_BNO055(0x28,Wire);
  //ToF = new ES410_VL53L5CX(0x29,Wire);

  Serial.begin(BAUD_RATE_SERIAL);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!IMU.initialise())
  {
    /* There was a problem detecting the IMU ... check your connections */
    Serial.print("No IMU detected!");
    while(1);
  }
  if(!ToF.initialise()){
    Serial.print("No ToF detected!");
    while(1);
  }

}

void loop(void) 
{
  tOn = millis();
  IMU.UpdateEvents();
  ToF.UpdateEvents();

  if((tOn-tOutputLast) > SERIAL_OUTPUT_RATE) {
    Serial.print(IMU.OutputString());
    Serial.print(ToF.OutputString());
    tOutputLast = tOn;
  }
  

}