// Using 
// Arduino Nano
// L298N motor shield, powered at 12V, driving a motor at 5V

#include <arduino.h>

// Configuration
int motor1pin1 = 7;
int motor1pin2 = 8;
int enablepin1 = 9;
int enablepin2 = 10;

void setup() {
  Serial.begin(9600);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enablepin1, OUTPUT);
  pinMode(enablepin2, OUTPUT);
  analogWrite(enablepin1, 0);
  analogWrite(enablepin2, 0);
}

void Forward()
{
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
}

void Reverse()
{
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
}

int run = 200;
int stop = 0;
int runTime = 3000;
int stopTime = 15000;

void loop() 
{
  Forward();
  analogWrite(enablepin1, run); //Controlling speed (0 = off and 255 = max speed)
  delay(runTime);

  analogWrite(enablepin1, stop);
  delay(stopTime);

  // Reverse();
  // analogWrite(enablepin1, run);
  // delay(runTime);

  // analogWrite(enablepin1, stop);
  // delay(stopTime);
}