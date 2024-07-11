#ifndef MOTORS_H
#define MOTORS_H

#define FORWARD 1
#define BACKWARD -1
#define MOTOR_TOP_SPEED 180

// Disable both motors because you don't often disable just one
void disableMotors();

// Disable both motors because you don't often enable just one
void enableMotors();

// Set up the PWM on the motors and initialize them to off
void setupMotors();

// Set the direction and speed of one of the motors
void setMotorSpeed(int motor, double speed, int direction);

// This is for the required functionality
void motorTest();

// this function will correct the motor speeds based on distance away from the wall
void followWall(double percentPower, double maintainDistance, int side);

#endif