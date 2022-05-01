#include <arduino.h>
#include <math.h>
#include <Servo.h>
#include <ultrasonic.h>

#define ServoPin 9

// Configuration
bool DebugLog = true;
float SinePeriod = 5;
int MaxServoOutput = 180;
int MinSonarDistance = 5;

// Variables
Servo myservo;
int angle = 0;

void setup() {
  Serial.begin(9600);
  ultrasonic_setup();
  myservo.attach(ServoPin);
}

float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DebugLogGraph(float value)
{
    int graphWidth = 20;

    Serial.print('|');
    for(int i = 0; i < graphWidth; i++)
      if (value * graphWidth / MaxServoOutput > i && 
          value * graphWidth / MaxServoOutput < i+1)
        Serial.print("#");
      else
        Serial.print(" " );
      Serial.println('|');
}

float GenerateWave()
{
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);

  // if (DebugLog)
  //   DebugLogGraph(wave);
  return wave;
}

void loop() {
  // Sine wave is the basic output [-1,1]
  float wave = GenerateWave();
  
  // Obrain sonar distance [5,50]cm => sonarMultiplier [0,1]
  int distance = max(ultrasonic_loop()-MinSonarDistance, 0);
  float sonarMultiplier = constrain(distance, 0.0, 50.0) / 50.0;
  
  // Apply sonar multiplier
  wave = wave * sonarMultiplier;
  // Map to servo signal range
  byte servoOutput = MapFloat(wave, -1, 1, 0, MaxServoOutput);

  if (DebugLog) 
  {
    Serial.print("\t");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(sonarMultiplier);
    Serial.print("\t");
    Serial.print(servoOutput);
    Serial.println();
  }  

  myservo.write(servoOutput);
}