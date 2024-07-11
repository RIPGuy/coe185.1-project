#include <util/delay.h>
#include "ultrasonic.h"
#include "motors.h"

const double originalSpeed = 0.3;
const double distanceThreshold = 10;
double percentSpeed = originalSpeed;
int idealDirection = CENTER;
unsigned long t = 0;
bool firstWall = false;
bool secondWall = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Setup");
  setupUltrasonic();
  setupMotors();
  setupServo();
  setupLed();
  Serial.println("Finished Setup");
  // proximityRead(RIGHT);  // Initial read to adjust sensor state if needed
  t = millis();
}


void loop() {
  double d = proximityRead(RIGHT);
  if (millis() - t < 1000) {  // the servo looks to right for 3 seconds
    followWall(percentSpeed, distanceThreshold, RIGHT);
    if (d > 3 * distanceThreshold && d < 60) { //change started here
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      //forward
      setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
      setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);
      delay(300);
      setMotorSpeed(RIGHT, 0, BACKWARD);
      setMotorSpeed(LEFT, 0, BACKWARD);
      delay(500);
      // turn Right
      setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);
      setMotorSpeed(RIGHT, percentSpeed + 0.4, BACKWARD);
      delay(300);
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(500);
      //forward
      setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
      setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);
      delay(400);
      setMotorSpeed(RIGHT, 0, BACKWARD);
      setMotorSpeed(LEFT, 0, BACKWARD);
      delay(500);
    } //change ended here
  } else {
    // SLLA();
    setMotorSpeed(LEFT, 0, FORWARD);
    setMotorSpeed(RIGHT, 0, FORWARD);
    if (proximityRead(CENTER) < distanceThreshold) {  // executes this if there is a wall within the distance threshhold
      percentSpeed = originalSpeed;
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD);  // WE NEED TO EVALUATE THIS ONE, IT SHOULD TURN EXACTLY AN ANGLE WHERE IT CAN MOVE PAST THE WALL
      setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
      delay(300);  //THIS MIGHT BE CHANGED DEPENDING ON THE ANGLE TURN
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      proximityRead(RIGHT);
    }
    moveServo(RIGHT);  //this is executed if there are no walls
    t = millis();
  }
}
