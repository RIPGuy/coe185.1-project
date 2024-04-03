#include <avr/io.h>
#include <util/delay.h>

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1
#define FORWARD 1
#define BACKWARD -1

#define MOTOR_SPEED 255

void setupMotors() {
    // Set PD2 and PD4 as outputs for motor direction control
    DDRD |= (1 << PD2) | (1 << PD4);

    // Set PD5 (OC0B) and PD6 (OC0A) as outputs for PWM speed control
    DDRD |= (1 << PD5) | (1 << PD6);

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

void setMotor(uint8_t motor, double speed, int direction) {
    if (motor == MOTOR_LEFT) {
        OCR0B = speed * MOTOR_SPEED; // Set PWM duty cycle for left motor
        if (direction == FORWARD) {
            PORTD |= (1 << PD2); 

        } else {

            PORTD &= ~(1 << PD2); 
        }
    } else if (motor == MOTOR_RIGHT) {
        OCR0A = speed * MOTOR_SPEED; // Set PWM duty cycle for right motor
        if (direction == FORWARD) {

          PORTD &= ~(1 << PD4); 
        } else {
          PORTD |= (1 << PD4); 

        }
    }
}

void setup() {
  Serial.begin(9600);
    setupMotors(); // Call the motor setup function
     Serial.print("done");
}

void loop() {
        // Move forward at half speed for 2 seconds
    // // Move backward at half speed for 2 seconds

  //BACKWARD
  // setMotor(MOTOR_RIGHT, 0.4, BACKWARD);
  // setMotor(MOTOR_LEFT, 0.4, BACKWARD);
  // delay(350);
  // setMotor(MOTOR_RIGHT, 0, BACKWARD);
  // setMotor(MOTOR_LEFT, 0, BACKWARD);
  // delay(500);

  Serial.print("moving");
  //SMALL LEFT
  setMotor(MOTOR_RIGHT, 0.4, FORWARD);
  // setMotor(MOTOR_LEFT, 0.4, BACKWARD);
  // delay(350);
  // setMotor(MOTOR_RIGHT, 0, BACKWARD);
  // setMotor(MOTOR_LEFT, 0, BACKWARD);
  delay(500);

    // // Stop both motors for 2 seconds
    // setMotor(MOTOR_LEFT, 0, FORWARD);  // Stop left motor
    // setMotor(MOTOR_RIGHT, 0, FORWARD); // Stop right motor
    // _delay_ms(2000);                   // Wait for 2000 milliseconds (2 seconds)
  
}

