
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.

 */

#include <Stepper.h>

const int stepsPerRevolution = 50;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper1(stepsPerRevolution, 8, 9, 10, 11);
// initialize the stepper library on pins 4 through 7:
Stepper myStepper2(stepsPerRevolution, 4, 5, 6, 7);

#define LDRpin1 A0
#define LDRpin2 A1
#define LDRpin3 A2
#define LDRpin4 A3
int x_LDRValue = 0;
int y_LDRValue = 0;
int x_maxLDR = 0;
int y_maxLDR = 0;
// Initialize step count
int x_steps = 0;
int y_steps = 0;
// Define max step count
int max_x_steps = 10;
int max_y_steps = 10;
// Array to store: max_ldr, step_count
int OptimalX[2];
int OptimalY[2];

bool set_position = false;


void setup() {
  // set the speed at 60 rpm:
	myStepper1.setSpeed(30);
  myStepper2.setSpeed(30);
	// initialize the serial port:
	Serial.begin(9600);

}

int avgLDR(){
  int a = analogRead(LDRpin1);
  int b = analogRead(LDRpin2);
  int c = analogRead(LDRpin3);
  int d = analogRead(LDRpin4);
  int result = (a + b + c + d)/4;
  return result;
}

void loop() {
  while (set_position != true){
    Serial.println("Finding best x_step");
    while (x_steps < max_x_steps) {
      // Serial.println("clockwise");
      myStepper1.step(stepsPerRevolution);
      int LDRValue = avgLDR();
      Serial.println(LDRValue);
      if (LDRValue >= x_maxLDR) {
        x_maxLDR = LDRValue;
        OptimalX[0] = x_maxLDR;
        OptimalX[1] = x_steps;
      }
      x_steps++;
      Serial.println("x step count: " + x_steps);
      delay(500); 
    }
    // Set Optimal stepper motor location
    Serial.println("Setting best x_step");
    while (x_steps != OptimalX[1]) {
      // Serial.println("counterclockwise");
      myStepper1.step(-stepsPerRevolution);
      x_steps--;
      Serial.println("x step count: " + x_steps);
      delay(250);
    }

    Serial.println("Finding best y_step");
    while (y_steps < max_y_steps) {
      // Serial.println("clockwise");
      myStepper2.step(stepsPerRevolution);
      int LDRValue = avgLDR();
      Serial.println(y_LDRValue);
      if (LDRValue >= y_maxLDR) {
        y_maxLDR = LDRValue;
        OptimalY[0] = y_maxLDR;
        OptimalY[1] = y_steps;
      }
      y_steps++;
      Serial.println("y step count: " + y_steps);
      delay(500); 
    }
    // Set Optimal stepper motor location
    Serial.println("Setting best y_step");
    while (y_steps != OptimalY[1]) {
      // Serial.println("counterclockwise");
      myStepper2.step(-stepsPerRevolution);
      y_steps--;
      Serial.println("y step count: " + y_steps);
      delay(250);
    }   
     
    set_position = true;
  Serial.println("Optimal Position Set");
  // LOCK POSITION STEPPER 1
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  // LOCK POSITION STEPPER 2
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW); 
  int prevLDR = avgLDR();
  currentLDR = prevLDR;
  while (currentLDR + 150 >= prevLDR){     
      delay(10000);
      currentLDR = avgLDR();
      Serial.println(currentLDR) 
  }
  set_position=false;

  }
  
}