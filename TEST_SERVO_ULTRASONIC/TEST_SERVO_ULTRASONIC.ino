#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#define RIGHT 125 //90
#define LEFT 625 //-90
#define CENTER 375 //0

#define TRIGGER_PIN PB4 // PIN 12 trigger on sensor
#define ECHO_PIN PB5     // PIN 13 echo on sensor

#define SERVO PB2 // PIN 10 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupServo();
  setupUltrasonic();
  Serial.println("done setup");
  proximityRead(RIGHT);
  // Serial.println("here");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(proximityRead(LEFT));
  delay(500);

  Serial.println(proximityRead(CENTER));
  delay(500);

  Serial.println(proximityRead(RIGHT));
  delay(500);

  Serial.println(proximityRead(LEFT));
  delay(500);
}

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