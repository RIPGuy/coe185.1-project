#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

//this is the turn of the servo for 64 prescaler
#define RIGHT 125 //90 
#define LEFT 625 //-90
#define CENTER 375 //0

#define MAX 30 // for checkoff which is 0.762 meters

void setupUltrasonic();
double measureDistance();
void setupServo();
void moveServo(int pulseWidth);
void ledOn(char led);
void ledOff(char led);
void setupLed();
void proximityReadLed(int direction, double threshold);
double proximityRead(int direction);
void suga();
void center();

#endif