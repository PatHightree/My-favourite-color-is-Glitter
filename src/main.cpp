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
  myservo.attach(ServoPin);
  ultrasonic_setup();
}

float GenerateWave()
{
  float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);
  wave += 1;
  wave /= 2;
  wave *= MaxServoOutput;
  
  if (DebugLog)
  {
    for(int i = -1; i<wave; i++)
      Serial.print("#");
    Serial.println();
  }
  return wave;
}

void loop() {
  // Sine wave is the basic output  
  byte servoOutput = GenerateWave();
  
  // if (DebugLog) 
  //   Serial.print(servoOutput);

//   // Limit by distance [0,50]cm => servo output percentage [0,MaxFanPower]
//   // Hand at 0 distance yields MinDistanceOutput output, so substract MinDistanceOutput
//   int distance = max(ultrasonic_loop()-MinSonarDistance, 0);
//   if (distance < MaxServoOutput)
//     servoOutput = distance;
// if (DebugLog) 
//   {
//     Serial.print("\t");
//     Serial.print(distance);
//     Serial.print("\t");
//     Serial.print(servoOutput);
//     Serial.println();
//   }  

  myservo.write(servoOutput);
  delay(10);

  // // Tell the servo to go to a particular angle:
  // myservo.write(90);
  // delay(1000);
  // myservo.write(180);
  // delay(1000);
  // myservo.write(0);
  // delay(1000);

  // // Sweep from 0 to 180 degrees:
  // for (angle = 0; angle <= 180; angle += 1) {
  //   myservo.write(angle);
  //   delay(15);
  // }

  // // And back from 180 to 0 degrees:
  // for (angle = 180; angle >= 0; angle -= 1) {
  //   myservo.write(angle);
  //   delay(30);
  // }
  // delay(1000);
}


// #include<Arduino.h>
// #include<ultrasonic.h>
// #include<pwm.h>
// #include<math.h>

// bool DebugLog = false;
// int MaxFanPower = 50;
// float SinePeriod = 20;
// int MinDistanceOutput = 5;

// void setup() 
// {
//   ultrasonic_setup();
//   pwm_setup();

//   Serial.begin(9600); // Starts the serial communication
// }

// float GenerateWave()
// {
//   float wave = sin(millis() * (2*PI) / 1000 / SinePeriod);
//   wave += 1;
//   wave /= 2;
//   wave *= MaxFanPower;
  
//   if (DebugLog)
//   {
//     for(int i = -1; i<wave; i++)
//       Serial.print("#");
//     Serial.println();
//   }
//   return wave;
// }

// void loop() 
// {
//   // Sine wave is the basic output  
//   byte fanPower = GenerateWave();
  
//   Serial.print(fanPower);

//   // Limit by distance [0,50]cm => fan power percentage [0,MaxFanPower]
//   // Hand at 0 distance yields MinDistanceOutput output, so substract MinDistanceOutput
//   int distance = max(ultrasonic_loop()-MinDistanceOutput, 0);
//   if (distance < MaxFanPower)
//     fanPower = distance;

//   Serial.print("\t");
//   Serial.print(distance);
//   Serial.print("\t");
//   Serial.print(fanPower);
//   Serial.println();
  
//   setPwmDuty(fanPower);
// }