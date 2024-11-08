#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(3000, 49, 4, 5); 

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
    uint64_t data;
    uint8_t datalen = sizeof(data);

    if (driver.recv((uint8_t*)&data, &datalen)) // Non-blocking
    {
	    // Message with a good checksum received, dump it.  
	    
      Serial.println((unsigned long)data);
    }
}
