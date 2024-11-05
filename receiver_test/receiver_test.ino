#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 2, 4, 5); 

void setup()
{
  Serial.begin(9600);
  if(!driver.init())
  {
    Serial.println("rh reciever init failed");
  }
  else
  {
    Serial.println("it fucking worked");
  }
}

void loop()
{
    uint32_t data;
    uint8_t datalen = sizeof(data);

    if (driver.recv((uint8_t*)&data, &datalen)) // Non-blocking
    {
	    // Message with a good checksum received, dump it.  
	    
      Serial.println(data x);
      analogWrite(11, data / 8);
    }
}
