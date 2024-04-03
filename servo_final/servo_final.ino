#include <avr/io.h>
#include <util/delay.h>


void setupTimer1() {
    // Set PB2/OC1B (Digital Pin 10 on Arduino) as output
    DDRB |= (1 << PB2);

    // Configure Timer1 for Fast PWM mode with OCR1A as TOP
    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B1); // Set Fast PWM mode 15 and output on OC1B (Digital Pin 10)
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    //TCCR1B = (1 << CS11); // prescaler = 8

    // Set prescaler for Timer1 to 64
    TCCR1B |= (1<<CS11) | (1<<CS10);
    TCCR1B &= ~(1<<CS12);

    //timer1 is only 16 bit which can cater only to 65535 so we cannot set our prescaler to 1 because the top value would be 319999 which is beyond the max range which is
    //why we selected 64 prescaler
    // Set up the period for the timer
    OCR1A = 4999; //(16000000 / (64 * 50)) - 1
    //ICR1 = 4999;
    //ICR1 = 39999; // TOP value for 50 Hz computed by (16000000 / (8 * 50)) - 1
    OCR1B = 375; //center
}


void moveServo(int pulseWidth) {
    OCR1B = pulseWidth; // Set the pulse width on OC1B
}


int main() {
    setupTimer1();

    //the timer frequency is 16mhz/64 prescaler = 250 000
    //the computed tick interval is 1/250 000 = 4 microseconds

    while (1) {
        // -90 degrees
        moveServo(125); //idk why but the value has to be 250 since -90 is 1 ms pulse (1ms/4us = 250a)
        _delay_ms(500); // Wait for 5 ms


        // 0 degrees
        moveServo(375); // idk why but the value has to be 375 since 0 degrees is 1.5 ms pulse (1.5ms/4us = 375)
        _delay_ms(500); // Wait for 5 ms


        // +90 degrees
        moveServo(625); // idk why but the value has to be 500 since 90 degrees is 2 ms pulse (2ms/4Us = 500)
        _delay_ms(500); // Wait for 5 ms

    }
}

