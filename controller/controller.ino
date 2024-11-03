#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 4, 2, 5); 

void setup()
{
  Serial.begin(9600);
  if(!driver.init())
  {
    Serial.println("rh transmitter init failed lmao");
  }
}

void loop()
{
    uint16_t data = (uint16_t)analogRead(4);

    driver.send((uint8_t *)&data, sizeof(data));
    driver.waitPacketSent();
    delay(10);
}
