#include <arduino.h>
#include <math.h>

#define PotPin A0
#define LedPin 13

void setup() {
  Serial.begin(9600);
  pinMode(PotPin, INPUT);
  pinMode(LedPin, OUTPUT);
}

float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  int value = analogRead(PotPin);
  value = map(value, 0, 1023, 0, 255);
  analogWrite(LedPin, value);
  delay(100);
}