#include <Arduino.h>
#include "motors.h"
#include "ultrasonic.h"


#define LEFT_CONTROL PD2 //pin 2
#define LEFT_PWM PD5 //pin 5

#define RIGHT_CONTROL PD4 //pin 4
#define RIGHT_PWM PD6 //pin 6

void disableMotors(){ //reached and corrected
  //Left
  DDRD &= ~(1 << LEFT_CONTROL); //PD2 control direction of motor A
  DDRD &= ~(1 << LEFT_PWM);   //PD5 output PWM signal to control speed of motor A

  //Right
  DDRD &= ~(1 << RIGHT_CONTROL); //PD4 control direction of motor B
  DDRD &= ~(1 << RIGHT_PWM); //PD6 output PWM signal to control speed of motor B
}

void enableMotors() { //reached and corrected
  //Left
  DDRD |= (1<<LEFT_CONTROL); //PD2 control direction of motor A
  DDRD |= (1<<LEFT_PWM); //PD5 output PWM signal to control speed of motor A

  //Right
  DDRD |= (1<<RIGHT_CONTROL); //PD4 control direction of motor B
  DDRD |= (1<<RIGHT_PWM); //PD6 output PWM signal to control speed of motor B
}

void setupMotors() { //revise
    // Enable motors
    enableMotors();
    // set up PWM
// Configure Timer0 for Fast PWM mode
    TCCR0A = (1 << WGM01) | (1 << WGM00);
    
    // Clear OC0A and OC0B on compare match, set at BOTTOM (non-inverting mode)
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);

    // Set prescaler to 64 (assuming a 16 MHz clock, this gives a PWM frequency of about 976.56 Hz)
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    // Initialize PWM values to 0 (motors off)
    OCR0A = 0; // PWM for right motor (PD6)
    OCR0B = 0; // PWM for left motor (PD5)
}

void setMotorSpeed(int motor, double speed, int direction) { // reached and corrected
if (motor == LEFT) {
        OCR0B = speed * MOTOR_TOP_SPEED; // Set PWM duty cycle for left motor
        if (direction == FORWARD) {
            PORTD |= (1 << PD2); 
        } else {
            PORTD &= ~(1 << PD2); 
        }
    } else if (motor == RIGHT) {
        OCR0A = speed * MOTOR_TOP_SPEED; // Set PWM duty cycle for right motor
        if (direction == FORWARD) {
          PORTD &= ~(1 << PD4); 
        } else {
          PORTD |= (1 << PD4); 
        }
    }
}

void followWall(double percentPower, double maintainDistance, int side) { //problem occurs here
    // //HAS TO BE IN POSITION BEFORE IT CAN READ                    
    moveServo(side);
    delay(100);

    measureDistance();
    delay(60);
    Serial.println("hi");

    double distance = measureDistance(); 
    Serial.println(distance);
    
    //double distance = proximityRead(CENTER);
    if (distance > 60) { // For our purposes, we can filter out really long distances when following walls
        distance = maintainDistance;
    }
    if (side == LEFT) {
        setMotorSpeed(RIGHT, percentPower * (distance / maintainDistance), FORWARD);
        setMotorSpeed(LEFT, percentPower * ( maintainDistance / distance), FORWARD);
    } else if (side == RIGHT) {
        setMotorSpeed(LEFT, percentPower * (distance / maintainDistance), FORWARD);
        setMotorSpeed(RIGHT, percentPower * (maintainDistance / distance), FORWARD);
        //Serial.println("here");
    }
    delay(60);
}

void motorTest() {
  Serial.println("Begin Robot Motion");
  // Forward
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, FORWARD);
  delay(500);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  //Backward
  setMotorSpeed(RIGHT, 0.4, BACKWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(500);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  //Small Left
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(350);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Small Right
  setMotorSpeed(RIGHT, 0.4, BACKWARD);
  setMotorSpeed(LEFT, 0.4, FORWARD);
  delay(425);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Large Left
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(700);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Large Right
  setMotorSpeed(RIGHT, 0.4, BACKWARD); 
  setMotorSpeed(LEFT, 0.4, FORWARD);
  delay(700);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);

  Serial.println("End Robot Motion");
}