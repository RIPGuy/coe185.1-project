#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include <Arduino.h>

#define TRIGGER_PIN PB4 // PIN 12 trigger on sensor
#define ECHO_PIN PB5     // PIN 13 echo on sensor

#define RED_LED PB1 // PIN 5 
#define GREEN_LED PB0 // PIN 6

#define SERVO PB2 // PIN 10 

void setupUltrasonic() { //reached and corrected
    // Set trigger pin as output
    DDRB |= (1 << TRIGGER_PIN);
    // Clear Trigger
    PORTB &= ~(1 << TRIGGER_PIN);
    // Set echo pin as input
    DDRB &= ~(1 << ECHO_PIN);
}

double measureDistance() { // reached and corrected
    double duration, distance;
    // Trigger the sensor
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
    
    // Wait for echo start
    while(!(PINB & (1 << ECHO_PIN)));

    TCNT1 = 0; // timer
    // Wait for echo end
    while(PINB & (1 << ECHO_PIN)); // Wait while the pin is high
    duration = TCNT1; // The current value of TCNT1 is stored in ICR1

    // distance = duration * 0.02700787; // this is measured in cm; the formula should match the prescaler of the servo w/c is 64

    distance = duration * 0.02700787; // this is measured in inches; the formula should match the prescaler of the servo w/c is 64
    
    return distance;
}


void setupServo() { // reached and corrected
    DDRB |= (1 << SERVO);
    
    // Configure Timer1 for Fast PWM mode with OCR1A as TOP
    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B1); // Set Fast PWM mode 15 and output on OC1B (Digital Pin 10)
    TCCR1B = (1 << WGM13) | (1 << WGM12);
    
    // Set prescaler for Timer1 to 64
    TCCR1B |= (1<<CS11) | (1<<CS10);
    TCCR1B &= ~(1<<CS12);
    OCR1B = CENTER;
    OCR1A = 4999; //(16000000 / (64 * 50)) - 1
}

void moveServo(int pulseWidth) { //reached and corrected
    OCR1B = pulseWidth; // Set the pulse width on OC1RB
}

void ledOn(char led) { //reached
  if (led == 'r') {
    PORTB |= (1 << RED_LED);
  } else if (led == 'g')  {
    PORTB |= (1 << GREEN_LED);
  }
}

void ledOff(char led) { //reached
  if (led == 'r') {
    PORTB &= ~(1 << RED_LED);
  } else if (led == 'g') {
    PORTB &= ~(1 << GREEN_LED);
  }
}

//without the Led functionality
double proximityRead(int direction) {
  moveServo(direction);
  // Wait for servo to be in position before sending out a pulse
  delay(500);
  // Not sure why, but the Ultrasonic sensor is giving delayed values 
  // So I have to "flush out" the value here so that I get an accurate read
  measureDistance();
  delay(60);
  return measureDistance();
}

void setupLed(){
  //Set led as output
  DDRD |= (1 << RED_LED) | (1 << GREEN_LED);
  ledOn('r');
  delay(500);
  ledOff('r');
  delay(500);

  ledOn('g');
  delay(500);
  ledOff('g');
  delay(500);

  ledOn('r');
  delay(500);
  ledOff('r');
  delay(500);

  ledOn('g');
  delay(500);
  ledOff('g');
  delay(500);
}

// for checkOff with led Functionality
void proximityReadLed(int direction, double threshold){
  moveServo(direction);
  delay(500);

  measureDistance();
  delay(60); //standard based on datasheet

  double distance  = measureDistance();
  
  if (distance < threshold){
    switch (direction){
      case LEFT:
        ledOn('g');
        ledOff('r');
        break;
      case RIGHT:
        ledOn('r');
        ledOff('g');
        break;
      case CENTER:
        ledOn('g');
        ledOn('r');
        break;   
    }
  } else {
      ledOff('r');
      ledOff('g'); 
  }

  return distance;
}