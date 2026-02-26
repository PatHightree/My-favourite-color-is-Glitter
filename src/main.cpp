#include <arduino.h>
#include <math.h>
#include <MotorDriver.h>
#include <ultrasonic.h>

// Configuration
int MotorID = 1;
int MotorSpeed = 255;
int Led1ID = 2;
int Led2ID = 3;
int Led3ID = 4;
int LedMinValue = 10; // Avoid totally switching off leds, it feels 'jerky'
int LedMaxValue = 255;
float LedFadeOutSeconds = 2;  // Fade leds over to sine after spooling up

// States
const int State_SpoolingUp = 0; // Bring the fluid up to speed
const int State_MotorStop = 1;  // Let the fluid coast
const int State_MotorPulse = 2; // Pulse to bring the fluid back up to speed
// State timing
int PeriodS = 20;
int PeriodMs = PeriodS * 1000;
// State timing
int MotorPulse_DurationMs = 2000;
int MotorPulse_PulseLengthMs = 200;
int MotorStop_DurationMs = PeriodMs - MotorPulse_DurationMs;

// Variables
MotorDriver MD;
int State = -1;
unsigned long StateStartTime;

void SetState(int _state)
{
  State = _state;
  StateStartTime = millis();
}

void SetStateAfterMs(int _durationMs, int _state)
{
  if (millis() > StateStartTime + _durationMs) SetState(_state);
}

float Remap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float WaveS(float _periodSeconds)
{
  return sin(millis() * 2*PI / 1000 / _periodSeconds);
}

float WaveMs(unsigned long _startMs, float _periodMs)
{
  return sin((millis() - _startMs) * 2*PI / _periodMs);
}

unsigned long SpoolingEnd;
void LedStateMachine()
{
  if (State == State_SpoolingUp)
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
    // Fade leds with a sine wave, one inverted to the other
    // Keep it in step with the motor pulse, so that the bottom leds are on when the motor pulses
    MD.motor(Led2ID, FORWARD, max(fadeOut, Remap(WaveS(PeriodS/2), 1, -1, LedMinValue, LedMaxValue)));
    MD.motor(Led3ID, FORWARD, max(fadeOut, Remap(WaveS(PeriodS/2), -1, 1, LedMinValue, LedMaxValue)));
  }
}

void MotorStateMachine()
{
  switch(State)
  {
    case State_SpoolingUp:
      // Pulse motor for 5 seconds, pulse because we don't want the stick to bounce around
      if (WaveS(1) > 0)
        MD.motor(MotorID, FORWARD, MotorSpeed);
      else
        MD.motor(MotorID, FORWARD, 0);

      SetStateAfterMs(5000, State_MotorStop);
      break;
    case State_MotorStop:
      // Stop motor
      MD.motor(MotorID, FORWARD, 0);

      SetStateAfterMs(MotorStop_DurationMs, State_MotorPulse);
      break;
    case State_MotorPulse:
      // Pulse to agitate the fluid
      if (WaveMs(StateStartTime, MotorPulse_PulseLengthMs) > 0)
        MD.motor(MotorID, FORWARD, MotorSpeed);
      else
        MD.motor(MotorID, FORWARD, 0);

      SetStateAfterMs(MotorPulse_DurationMs, State_MotorStop);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  SetState(State_SpoolingUp);
}

void loop() 
{
  LedStateMachine();
  MotorStateMachine();
}