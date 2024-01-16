// Using 
// Arduino Nano
// L298N motor shield, powered at 12V, driving a motor at 5V

#include <arduino.h>

// Configuration
int motor1pin1 = 7;
int motor1pin2 = 8;
int enablepin1 = 9;
int enablepin2 = 10;

const int StateWave = 1;
int StateWaveDuration = 2000;
const int StateStop = 2;
int StateStopDuration = 15000;
int State;
unsigned long StateStartMillis;

void setup() {
  Serial.begin(9600);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enablepin1, OUTPUT);
  pinMode(enablepin2, OUTPUT);
  analogWrite(enablepin1, 0);
  analogWrite(enablepin2, 0);

  State = StateWave;
  StateStartMillis = millis();
}

void Forward()
{
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
}

void Reverse()
{
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
}

int run = 200;
int stop = 0;

void loop() 
{
  switch (State)
  {
    case StateWave:
      Forward();
      analogWrite(enablepin1, run); //Controlling speed (0 = off and 255 = max speed)
      if (millis() > StateStartMillis + StateWaveDuration)
      {
        State = StateStop;
        StateStartMillis = millis();  
      }
    break;
    case StateStop:
      analogWrite(enablepin1, stop);
      if (millis() > StateStartMillis + StateStopDuration)
      {
        State = StateWave;
        StateStartMillis = millis();
      }
      break;
  }

  // Reverse();
  // analogWrite(enablepin1, run);
  // delay(runTime);

  // analogWrite(enablepin1, stop);
  // delay(stopTime);
}