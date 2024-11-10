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
  // Pack data into 48 bits
  struct
  {
    uint32_t _data1;
    uint16_t _data2;
  } packedData;

  //                   10 bits                     |  10 bits                               |  10 bits
  //                   left stick X                |  left stick Y                          |  right stick X
  packedData._data1  = analogRead(leftX)           | ((uint64_t)analogRead(leftY) << 10)    | (uint64_t)(analogRead(rightX) << 20)
  //                |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2               
                    | (digitalRead(button1) << 30) | (digitalRead(button2) << 31);
  //           --------------------------------------------------------------------------------------------------------------------------
  //                   10 bits                     |  1 bit                                 |  1 bit
  //                   right stick Y               |  face button 3                         |  face button 4
  packedData._data2  = analogRead(rightY)          | (digitalRead(button3) << 10)           | (digitalRead(button4) << 11)
  //                |  1 bit                       |  1 bit                                 |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2                         |  face button 3               |  face button 4
                    | (digitalRead(button5) << 12) | (digitalRead(button6) << 13)           | (digitalRead(button7) << 14) | (digitalRead(button8) << 15);

  driver.send((uint8_t *)&packedData, sizeof(packedData));  
  driver.waitPacketSent();
}
