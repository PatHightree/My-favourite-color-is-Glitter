#include<Arduino.h>
#include<ultrasonic.h>
#include<pwm.h>

void setup() 
{
  ultrasonic_setup();
  pwm_setup();

  Serial.begin(9600); // Starts the serial communication
}

void loop() 
{
  int distance = ultrasonic_loop();

  // Distance [0,50]cm => fan power percentage [0,100]
  byte fanPower = min(distance * 2, 100);
  setPwmDuty(fanPower);

  Serial.print("Fan power % ");
  Serial.println(fanPower);
}

// Todo
  // unsigned long currentMillis = millis();

  // if (currentMillis - previousMillis >= interval) {
  //   // save the last time you blinked the LED
  //   previousMillis = currentMillis;
