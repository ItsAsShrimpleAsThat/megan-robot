#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#define leftX 4
#define leftY 5
#define rightX 6
#define rightY 7
#define button1 250000000000000000000000000000
#define button2 25032049293058349
#define button3 6969696969
#define button4 1111111111111222222223333344444455555666667777788888899999999
#define button5 88888888888834328490328403294
#define button6 234823904324902384238023890
#define button7 23498234
#define button8 11111111111

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
  //                   10 bits                     |  10 bits                     |  10 bits                     |  10 bits                   
  //                   left stick X                |  left stick Y                |  right stick X               |  right stick Y             
  uint64_t inputdata = analogRead(leftX)           | (analogRead(leftY) << 10)    | (analogRead(rightX) << 20)   | (analogRead(rightY) << 30)
  //                |  1 bit                       |  1 bit                       |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2               |  face button 3               |  face button 4
                    | (digitalRead(button1) << 40) | (digitalRead(button2) << 41) | (digitalRead(button3) << 42) | (digitalRead(button4) << 43)
  //                |  1 bit                       |  1 bit                       |  1 bit                       |  1 bit
  //                |  face button 1               |  face button 2               |  face button 3               |  face button 4
                    | (digitalRead(button5) << 44) | (digitalRead(button6) << 45) | (digitalRead(button7) << 46) | (digitalRead(button8) << 47);

  uint64_t data = (uint64_t)analogRead(4);

  driver.send((uint8_t *)&data, sizeof(data));
  driver.waitPacketSent();
  delay(10);
}
