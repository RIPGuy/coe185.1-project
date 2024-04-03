#include <util/delay.h>
#include "ultrasonic.h"
#include "motors.h"

const double originalSpeed = 0.3;
const double distanceThreshold = 12;
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
  proximityRead(RIGHT);  // Initial read to adjust sensor state if needed
  t = millis();
}

//PROBLEMA KAY ANG MGA TURN KEMERUT

void loop() {
  
  if (idealDirection == RIGHT) {  //ARGUMENT IS THROWN HERE IF THERE IS A WALL ON THE CENTER
    setMotorSpeed(LEFT, percentSpeed, FORWARD);
    setMotorSpeed(RIGHT, percentSpeed, FORWARD);
    double d = proximityRead(RIGHT);
    // Adjust speed slightly to maintain consistency
    //percentSpeed += 0.0004;
    if (d > 3 * distanceThreshold && d < 60) {  // if after turning there is a distance is between 37-59
      percentSpeed = originalSpeed;
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      setMotorSpeed(LEFT, 0.35, FORWARD);  // Speed adjustments to maneuver around obstacles
      setMotorSpeed(RIGHT, 0.4, FORWARD);
      delay(1000);
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);  // Execute a right turn
      setMotorSpeed(RIGHT, percentSpeed + 0.4, BACKWARD);
      Serial.println("turn right");
      delay(400);
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      idealDirection = CENTER;
      proximityRead(RIGHT);
      firstWall = true;
    }
  } 
  
  else if (idealDirection == LEFT) {
    setMotorSpeed(LEFT, percentSpeed, FORWARD);
    setMotorSpeed(RIGHT, percentSpeed, FORWARD);
    double d = proximityRead(CENTER);
    //percentSpeed += 0.0004;
    if (d < distanceThreshold) {
      percentSpeed = originalSpeed;
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD); // turn left
      setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
      delay(400);
      setMotorSpeed(LEFT, 0, FORWARD);
      setMotorSpeed(RIGHT, 0, FORWARD);
      delay(50);
      idealDirection = CENTER;
      proximityRead(RIGHT);
      secondWall = true;
      }
    } 
    
    // if (idealDirection = CENTER)
    else 
    {   // CENTER
    if ((!firstWall && !secondWall)) {
      if (millis() - t < 3000) {  // the servo looks to right for 3 seconds
        Serial.println(millis() - t);
        followWall(percentSpeed, distanceThreshold, RIGHT);
        //percentSpeed += 0.0001;
        Serial.println("here");
      } else {  //executes this if it is past 3 seconds
        setMotorSpeed(LEFT, 0, FORWARD);
        setMotorSpeed(RIGHT, 0, FORWARD);
        if (proximityRead(CENTER) < distanceThreshold) {  // executes this if there is a wall within the distance threshhold
          percentSpeed = originalSpeed;
          if (!firstWall) {                                     //turn left if there is a wall in the center
            setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD);  // WE NEED TO EVALUATE THIS ONE, IT SHOULD TURN EXACTLY AN ANGLE WHERE IT CAN MOVE PAST THE WALL
            setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
            delay(500);  //THIS MIGHT BE CHANGED DEPENDING ON THE ANGLE TURN
            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            proximityRead(RIGHT);
            idealDirection = RIGHT;  //THIS THROWS THE ARGUMENT TO THE FIRST IF STATEMENT
            firstWall = true;
          }
        }
        moveServo(RIGHT);  //this is executed if there are no walls
        t = millis();
      }
    } else if (firstWall && secondWall) { //
      followWall(percentSpeed, distanceThreshold, RIGHT);
      //percentSpeed += 0.0001;
      Serial.println("here");
    } else { 
      setMotorSpeed(LEFT, percentSpeed, FORWARD);
      setMotorSpeed(RIGHT, percentSpeed, FORWARD);
      //percentSpeed += 0.0001;
      if (proximityRead(CENTER) < distanceThreshold) {
        percentSpeed = originalSpeed;
        setMotorSpeed(LEFT, 0, FORWARD);
        setMotorSpeed(RIGHT, 0, FORWARD);
        delay(50);
        setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);  // evaluate this turn right, og nganong mo turn right siya diari
        setMotorSpeed(RIGHT, percentSpeed + 0.4 , BACKWARD);
        Serial.println("turn right");
        delay(425);
        setMotorSpeed(LEFT, 0, FORWARD);
        setMotorSpeed(RIGHT, 0, FORWARD);
        delay(50);
        idealDirection = LEFT;
      }
      // setMotorSpeed(RIGHT, 0.4, FORWARD);
      // setMotorSpeed(LEFT, 0.4, FORWARD);
      // delay(100);
      // setMotorSpeed(RIGHT, 0.3, BACKWARD);
      // setMotorSpeed(LEFT, 0.3, FORWARD);
      // delay(425);
      // setMotorSpeed(LEFT, 0, FORWARD);
      // setMotorSpeed(RIGHT, 0, FORWARD);
      // delay(50);
      //idealDirection = CENTER;
    }
  }
}
//followWall(percentSpeed, distanceThreshold, RIGHT); //0.3, 10, RIGHT servo
//moveServo(RIGHT);
//motorTest();
