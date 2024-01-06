#include <arduino.h>
#include <math.h>
#include <MotorDriver.h>
#include <ultrasonic.h>

// Configuration
int MotorID = 1;
int MotorSpeed = 255; // To keep magnet from bouncing around
int Led1ID = 2;
int Led2ID = 3;
int Led3ID = 4;
int LedMinValue = 10; // Avoid totally switching off leds, it feels 'jerky'
int LedMaxValue = 255;
float LedFadeOutSeconds = 2;  // Fade leds over to sine after spooling up

// Variables
MotorDriver MD;
int State = -1;
const int StateSpoolingUp = 0;
const int StateStopMotor = 1;
const int StatePulseMotor = 2;
unsigned long StateStartTime;

void SetState(int _state)
{
  State = _state;
  StateStartTime = millis();
}

float Remap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Wave(float _periodSeconds)
{
  return sin(millis() * 2*PI / 1000 / _periodSeconds);
}

float Wave(unsigned long _startMillis, float _periodSeconds)
{
  return Wave(_periodSeconds - _startMillis);
}

float Wave255(float _periodSeconds)
{
  return (Wave(_periodSeconds) * 0.5 + 0.5) * 255;
}

float Wave255(unsigned long _startMillis, float _periodSeconds)
{
  return (Wave(_periodSeconds - _startMillis) * 0.5 + 0.5) * 255;
}

unsigned long SpoolingEnd;
void LedStateMachine()
{
  if (State == StateSpoolingUp)
  {
    // Both leds on while spooling up
    MD.motor(Led2ID, FORWARD, 255);
    MD.motor(Led3ID, FORWARD, 255);
    SpoolingEnd = millis();
  }
  else
  {
    float fadeOut = LedFadeOutSeconds - (millis() - SpoolingEnd) / (LedFadeOutSeconds * 1000);
    fadeOut = max(fadeOut, 0);
    fadeOut = fadeOut * 255 / LedFadeOutSeconds;
    Serial.println(fadeOut);
    // Fade leds with a sine wave with a period of 10 seconds, one inverted to the other
    // 10 seconds keeps it in step with the motor pulse, so that the bottom leds are on when the motor pluses
    MD.motor(Led2ID, FORWARD, max(fadeOut, Remap(Wave(10), 1, -1, LedMinValue, LedMaxValue)));
    MD.motor(Led3ID, FORWARD, max(fadeOut, Remap(Wave(10), -1, 1, LedMinValue, LedMaxValue)));
  }
}

void MotorStateMachine()
{
  switch(State)
  {
    case StateSpoolingUp:
      // Pulse motor for 5 seconds, pulse because we don't want the stick to bounce around
      if (Wave(1) > 0)
        MD.motor(MotorID, FORWARD, MotorSpeed);
      else
        MD.motor(MotorID, FORWARD, 0);

      if (millis() > StateStartTime + 5000) SetState(StateStopMotor);
      break;
    case StateStopMotor:
      // Stop motor
      MD.motor(MotorID, FORWARD, 0);

      if (millis() > StateStartTime + 19250) SetState(StatePulseMotor);
      break;
    case StatePulseMotor:
      // Pulse motor
      MD.motor(MotorID, FORWARD, MotorSpeed);

      if (millis() > StateStartTime + 750) SetState(StateStopMotor);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  SetState(StateSpoolingUp);
}

void loop() 
{
  LedStateMachine();
  MotorStateMachine();
}