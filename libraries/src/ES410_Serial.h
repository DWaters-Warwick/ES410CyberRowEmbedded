#define SERIAL_LOG_DEBUG(log)\
            Serial.print(millis());\
            Serial.print(" [DBG] ");\
            Serial.println(log);
            
#define SERIAL_LOG_DATA(data)\
            Serial.print(millis());\
            Serial.print(" [DAT] ");\
            Serial.println(data);
            