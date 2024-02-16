// This is a basic TMC2209 hello world, without libraries
// For the schematic, see "TMC2209 hello world without libraries.fzz"
//
// If this code+schematic don't work for you:
// - Check the wiring (again)
// - Play with the delays in the loop (100-1000us)
// - This forum post was helpful in debugging https://forum.arduino.cc/t/using-a-tmc2209-silent-stepper-motor-driver-with-an-arduino/666992

#include <Arduino.h>

const int enPin = 2;
const int dirPin = 3;
const int stepPin = 4;

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  pinMode(enPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(enPin, HIGH);
  delayMicroseconds(1000);

  Serial.println("Enable");
  digitalWrite(enPin, LOW);
  digitalWrite(dirPin, HIGH);
}

void loop()
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(250);

  digitalWrite(stepPin, LOW);
  delayMicroseconds(250);
}