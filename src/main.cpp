#include <arduino.h>
#include <math.h>

#define PotPin1 A0
#define LedPin 13
#define VUPin 5
void setup() {
  Serial.begin(9600);
  pinMode(PotPin1, INPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(VUPin, OUTPUT);
}

float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float GenerateWave()
{
  float SinePeriod = 5;
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);

  return wave;
}
void loop() {
  float voltageScale = 0.15/5.0; // VU input range / default pin voltage output range

  analogWrite(5, MapFloat(GenerateWave(), -1, 1, 0, 255) * voltageScale);

  // int value = analogRead(PotPin);
  // value = map(value, 0, 1023, 0, 255);
  // analogWrite(5, value * voltageScale);

  // for(int i=0; i<255; i++)
  // {
  //   analogWrite(5, i * voltageScale);
  //   delay(20);
  // }
  // for(int i=255; i>0; i--)
  // {
  //   analogWrite(5, i * voltageScale);
  //   delay(20);
  // }

  delay(10);
}