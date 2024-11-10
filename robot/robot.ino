#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

bool running = true;

// Radio/Controller Settings
RH_ASK driver(4000, 49, 2, 4); 
short mask = 0b1111111111;

// Motor settings
double motorDeadzone = 0.55;
double topSpeed = 128; //relative to 256 being max

// Motor ports
struct MotorPorts
{
  short a;
  short b;
  short pwm;
};

struct MotorPorts front_left {23, 22, 2};
struct MotorPorts front_right {52, 53, 6};
struct MotorPorts back_left {25, 24, 3};
struct MotorPorts back_right {51, 50, 5};

// Controller inputs
unsigned long controlExpirationTimer = 0;
short controlLifetime = 80; // Milliseconds until a control times out (Sets all controls to 0 after this many milliseconds)

// Joysticks
short forward = 0;
short sideways = 0;
short zRotation = 0;

void setup() 
{
  Serial.begin(9600);
  
  setPinModes();
  setMotorsZero();

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
  // === Get controller inputs from radio ===
  struct // Data from radio is packed into 32 + 16 bits
  {
    uint32_t _data1;
    uint16_t _data2;
  } packedData;
  uint8_t datalen = sizeof(packedData);

  if (driver.recv((uint8_t*)&packedData, &datalen)) // Non-blocking
  {
	  // Message with a good checksum received  

    // Reset Expiration timer
    controlExpirationTimer = millis();

    // Get sticks
    sideways = -getAxis(packedData._data1, 0);
    forward = getAxis(packedData._data1, 1);
  }

  // Reset controls to zero if the last receieved control has expired
  if(millis() - controlExpirationTimer > controlLifetime)
  {
    sideways = 0;
    forward = 0;
  }

  Serial.print("X: ");
  Serial.print(sideways);
  Serial.print(" Y: ");
  Serial.print(forward);
  Serial.print(" D: ");
  Serial.println(millis() - controlExpirationTimer);

  short frontLeftSpeed = forward - sideways + zRotation;
  short frontRightSpeed = forward + sideways - zRotation;
  short backLeftSpeed = forward + sideways + zRotation;
  short backRightSpeed = forward - sideways - zRotation;

  if(running)
  {
    setMotor(front_left, frontLeftSpeed);
    setMotor(front_right, frontRightSpeed);
    setMotor(back_left, backLeftSpeed);
    setMotor(back_right, backRightSpeed);
  }
  else
  {
    setMotor(back_left, 0);
    setMotor(back_right, 0);
    setMotor(front_left, 0);
    setMotor(front_right, 0);
  }
  delay(16);
}

short getAxis(uint32_t data, uint8_t axis)
{
  return (mask & short((data >> (10 * axis)))) - 512;
}

void setPinModes()
{
  // Radio
  pinMode(49, INPUT);

  // Drive motor PWM
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  // Drive motor direction
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
}

void setMotorsZero()
{
  analogWrite(2, 0);
  analogWrite(3, 0);
  analogWrite(4, 0);
  analogWrite(5, 0);

  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  digitalWrite(52, LOW);
  digitalWrite(53, LOW);

  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);
}

void setMotor(MotorPorts motor, short speed)
{
  if(speed > 0)
  {
    digitalWrite(motor.a, HIGH);
    digitalWrite(motor.b, LOW);
  }
  else
  {
    digitalWrite(motor.a, LOW);
    digitalWrite(motor.b, HIGH);
  }

  analogWrite(motor.pwm, abs(speed));
}

void stopAllDrivingMotors()
{
  setMotor(back_left, 0);
  setMotor(back_right, 0);
  setMotor(front_left, 0);
  setMotor(front_right, 0);
}