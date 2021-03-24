#include<Arduino.h>
#include<ultrasonic.h>
#include<pwm.h>
#include<math.h>

bool DebugLog = false;
int MaxFanPower = 50;
float SinePeriod = 20;
int MinDistanceOutput = 5;

void setup() 
{
  ultrasonic_setup();
  pwm_setup();

  Serial.begin(9600); // Starts the serial communication
}

float GenerateWave()
{
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);
  wave += 1;
  wave /= 2;
  wave *= MaxFanPower;
  
  if (DebugLog)
  {
    for(int i = -1; i<wave; i++)
      Serial.print("#");
    Serial.println();
  }
  return wave;
}

void loop() 
{
  // Sine wave is the basic output  
  byte fanPower = GenerateWave();
  
  Serial.print(fanPower);

  // Limit by distance [0,50]cm => fan power percentage [0,MaxFanPower]
  // Hand at 0 distance yields MinDistanceOutput output, so substract MinDistanceOutput
  int distance = max(ultrasonic_loop()-MinDistanceOutput, 0);
  if (distance < MaxFanPower)
    fanPower = distance;

  Serial.print("\t");
  Serial.print(distance);
  Serial.print("\t");
  Serial.print(fanPower);
  Serial.println();
  
  setPwmDuty(fanPower);
}