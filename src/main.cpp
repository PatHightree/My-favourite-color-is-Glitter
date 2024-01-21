// Using 
// Arduino Nano
// L298N motor shield, powered at 12V, driving a PSU fan motor at 10V

#include <arduino.h>
#include <GOTStateMachine.h>
#include <wave.h>

GOTStateMachine stateMachine(50); // execute every 50 milliseconds
void StatePurge();
void StateWave();
void StateWait();

// Configuration
int lightPin1 = 5;
int lightPin2 = 6;
int motorPin1 = 7;
int motorPin2 = 8;
int motorPwmPin = 9;
int lightPwmPin = 10;

void Forward()
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void Reverse()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void setup() {
  Serial.begin(9600);
  
  // Turn on light
  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
  pinMode(lightPwmPin, OUTPUT);
  digitalWrite(lightPin1, HIGH);
  digitalWrite(lightPin2, LOW);
  analogWrite(lightPwmPin, 255);
  // Initialize motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPwmPin, OUTPUT);
  analogWrite(motorPwmPin, 0);
  Forward();

  // stateMachine.setStartState(StatePurge);
  stateMachine.setStartState(StateWave);
}

void loop() 
{
  stateMachine.execute();
}

void StatePurge()
{
    analogWrite(motorPwmPin, 255);

    if (stateMachine.isDelayComplete(5000))
      stateMachine.changeState(StateWait);
}

void StateWave()
{
    float durationMs = 2000;
    float amplitudeTwentheMotor = 60;
    float amplitudePSUMotor = 255;

    int amplitude = amplitudePSUMotor;
    float progress = (millis() - stateMachine.getCurrentStateStartTime()) / durationMs;
    float value = sin(progress * PI); // [0, pi] -> [-1,1]
    value = value * 0.5 + 0.5; // [0,1]
    value *= amplitude;
    Serial.println(value);
    analogWrite(motorPwmPin, value); //Controlling speed (0 = off and 255 = max speed)

    if (stateMachine.isDelayComplete(durationMs))
      stateMachine.changeState(StateWait);
}

void StateWait()
{
  if(stateMachine.isFirstTime())
    analogWrite(motorPwmPin, 0);
  
  if (stateMachine.isDelayComplete(10000))
    stateMachine.changeState(StateWave);
}