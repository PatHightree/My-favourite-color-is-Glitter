#include <SpeedyStepper.h> //Simple & good stepper library, get it.

#include <TMCStepper.h>

#define DIR_PIN          3 // Direction
#define STEP_PIN         4 // Step
#define SERIAL_PORT Serial2 // HardwareSerial port pins 16 & 17
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

bool shaft = false;  // ONLY NEEDED FOR CHANGING DIRECTION VIA UART, NO NEED FOR DIR PIN FOR THIS


TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

SpeedyStepper stepper;

void setup() {

stepper.connectToPins(STEP_PIN, DIR_PIN); // INITIALIZE SpeedyStepper
  
SERIAL_PORT.begin(115200);      // INITIALIZE UART TMC2209
Serial.begin(9600);
delay(500);
Serial.println(F("Serial Initialized"));

  driver.begin();                // Initialize driver
                           
  driver.toff(5);                 // Enables driver in software

  driver.rms_current(600);       // Set motor RMS current
  driver.microsteps(2);            // Set microsteps to 1/2

  driver.pwm_autoscale(true);    // Needed for stealthChop
  driver.en_spreadCycle(true);   // false = StealthChop / true = SpreadCycle

  stepper.setCurrentPositionInSteps(0);                   // Set zero position
  stepper.setSpeedInStepsPerSecond(400);              //Set Speed
  stepper.setAccelerationInStepsPerSecondPerSecond(400);   //Set acceleration, smaller value for super smooth direction changing

}

void loop() {

uint16_t msread=driver.microsteps();
Serial.print(F("Read microsteps via UART to test UART receive : "));    Serial.println(msread); 
  
Serial.println(F("Move 6400 steps forward at 600ma"));
driver.rms_current(600); 
stepper.moveToPositionInSteps(6400);

Serial.println(F("Wait 3sec and turn current low so you can turn the motor shaft"));
driver.rms_current(10); 
delay(3000);

Serial.println(F("Move back to 0 position at 300ma"));
driver.rms_current(300); 
stepper.moveToPositionInSteps(0);

//MOVE MOTOR VIA UART AND CHANGE DIRECTION VIA SOFTWARE, IT RUNS AS LONG AS YOU LET IT... PROBABLY ONLY USEFUL WITH ENCODER. THE VALUE SETS ONLY THE SPEED.

driver.VACTUAL(16000); //SET SPEED OF MOTOR
delay(2000); // MOTOR MOVES 2 SEC THEN STOPS
driver.VACTUAL(0); //STOP MOTOR BY SETTING SPEED TO 0
shaft = !shaft; // REVERSE DIRECTION
driver.shaft(shaft); // SET DIRECTION

}