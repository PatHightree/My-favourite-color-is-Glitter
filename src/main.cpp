// Using 
// Arduino Nano
// L298N motor shield, powered at 12V, driving a PSU fan motor at 10V

#include <arduino.h>
#include <GOTStateMachine.h>
#include <wave.h>

GOTStateMachine stateMachine(50); // execute every 50 milliseconds
void StateWave();
void StateWait();

// Configuration
int motor1pin1 = 7;
int motor1pin2 = 8;
int enablepin1 = 9;

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

void setup() {
  Serial.begin(9600);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enablepin1, OUTPUT);
  analogWrite(enablepin1, 0);
  Forward();

  stateMachine.setStartState(StateWave);
}

void loop() 
{
  stateMachine.execute();
}

void StateWave()
{
    int durationMs = 1000;
    int period = durationMs * 2;  // we want the first PI of the sine wave
    int amplitudeTwentheMotor = 60;
    int amplitudePSUMotor = 200;
    int amplitude = amplitudePSUMotor;
    float value = sin((millis() - stateMachine.getCurrentStateStartTime()) * 2*PI / period); // [-1,1]
    value = value * 0.5 + 0.5; // [0,1]
    value *= amplitude;
    Serial.println(value);
    analogWrite(enablepin1, value); //Controlling speed (0 = off and 255 = max speed)

    if (stateMachine.isDelayComplete(durationMs))
      stateMachine.changeState(StateWait);
}

void StateWait()
{
  if(stateMachine.isFirstTime())
    analogWrite(enablepin1, 0);
  
  if (stateMachine.isDelayComplete(10000))
    stateMachine.changeState(StateWave);
}