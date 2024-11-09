#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#define leftX 4
#define leftY 5
#define rightX 3
#define rightY 3
#define button1 3
#define button2 3
#define button3 3
#define button4 3
#define button5 3
#define button6 3
#define button7 3
#define button8 3

RH_ASK driver(4000, 4, 2, 5); 

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
  //                   10 bits                     |  10 bits                               |  10 bits                                  |  10 bits                   
  //                   left stick X                |  left stick Y                          |  right stick X                            |  right stick Y             
  uint32_t      data = analogRead(leftX)           | ((uint64_t)analogRead(leftY) << 10)    | (uint64_t)(analogRead(rightX) << 20);   //| ((uint64_t)analogRead(rightY) << 30)
  //                |  1 bit                       |  1 bit                       |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2               |  face button 3               |  face button 4
//                    | (digitalRead(button1) << 40) | (digitalRead(button2) << 41) | (digitalRead(button3) << 42) | (digitalRead(button4) << 43)
  //                |  1 bit                       |  1 bit                       |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2               |  face button 3               |  face button 4
//                    | (digitalRead(button5) << 44) | (digitalRead(button6) << 45) | (digitalRead(button7) << 46) | (digitalRead(button8) << 47);

  uint32_t grink = (data >> 10) & 0b1111111111;
  Serial.println((unsigned long)grink);
  driver.send((uint8_t *)&data, sizeof(data));  
  driver.waitPacketSent();
}
