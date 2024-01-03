#include <arduino.h>
#include <math.h>
#include <MotorDriver.h>
#include <ultrasonic.h>

// Configuration
bool DebugLog = true;
float SinePeriod = 5;
int MinSonarDistance = 5;
int MotorID = 3;

// Variables
MotorDriver m;

void setup() {
  Serial.begin(9600);
  ultrasonic_setup();

  for (int i = 0; i < 5; i++)
  {
    m.motor(MotorID, FORWARD, 255);
    delay(500);
    m.motor(MotorID, FORWARD, 0);
    delay(500);
  }
}

float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float GenerateWave()
{
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);

  // if (DebugLog)
  //   DebugLogGraph(wave);
  return wave;
}

void loop() {
  // // Sine wave is the basic output [-1,1]
  // float wave = GenerateWave();
  
  // // Obrain sonar distance [5,50]cm => sonarMultiplier [0,1]
  // int distance = max(ultrasonic_loop()-MinSonarDistance, 0);
  // float sonarMultiplier = constrain(distance, 0.0, 50.0) / 50.0;
  
  // // Apply sonar multiplier
  // wave = wave * sonarMultiplier;
  // // Map to servo signal range
  // byte servoOutput = MapFloat(wave, -1, 1, 0, 255);

  // if (DebugLog) 
  // {
  //   Serial.print("\t");
  //   Serial.print(distance);
  //   Serial.print("\t");
  //   Serial.print(sonarMultiplier);
  //   Serial.print("\t");
  //   Serial.print(servoOutput);
  //   Serial.println();
  // }  

  // m.motor(1,FORWARD,servoOutput);
  Serial.print("On\n");
  m.motor(MotorID, FORWARD, 255);
  delay(1000);
  Serial.print("Off\n");
  m.motor(MotorID, FORWARD, 0);
  delay(20000);
}