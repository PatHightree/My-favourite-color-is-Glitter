#include <arduino.h>
#include <math.h>

#define PotPin1 A0
#define PotPin2 A1
#define LedPin 13
#define VUPin1 5
#define VUPin2 6
void setup() {
  Serial.begin(9600);
  pinMode(PotPin1, INPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(VUPin1, OUTPUT);
  pinMode(VUPin2, OUTPUT);
}

float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float GenerateWave()
{
  float SinePeriod = 2;
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);

  return wave;
}
void loop() {
  digitalWrite(LedPin, 1);

  float voltageScale = 0.15/5.0; // VU input range / default pin voltage output range

  // Display 2 sine waves on the VU meters
  // analogWrite(VUPin1, MapFloat(GenerateWave(), -1, 1, 0, 255) * voltageScale);
  // analogWrite(VUPin2, MapFloat(GenerateWave(), 1, -1, 0, 255) * voltageScale);

  int value;

  // Read potmeter2 and display on VU1
  value = analogRead(PotPin2);
  value = map(value, -100, 1023, 0, 255);
  analogWrite(VUPin1, value * voltageScale);

  // Display sine wave on VU2 with amplitude specified by potmeter2
  analogWrite(VUPin2, MapFloat(GenerateWave() * value, 255, -255, 0, 255) * voltageScale);

  delay(10);
}