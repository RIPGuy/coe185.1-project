#include <avr/io.h>
#include <util/delay.h>

void setupTimer1() {
    // Configure Timer1 for Fast PWM mode with ICR1 as TOP
    TCCR1A = (1 << WGM11) | (1 << COM1B1); // Use COM1B1 to output on OC1B (Digital Pin 10)
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11); // WGM13:12 = 14 (Fast PWM), Prescaler = 8

    ICR1 = 39999; // TOP value for 50 Hz

    // Set PB2/OC1B (Digital Pin 10 on Arduino) as output
    DDRB |= (1 << PB2);
}

void moveServo(int pulseWidth) {
    OCR1B = pulseWidth; // Set the pulse width on OC1B
}

int main() {
    setupTimer1();

    while (1) {
        // -90 degrees
        moveServo(2000);
        _delay_ms(500); // Wait for 1 second

        // 0 degrees
        moveServo(3000);
        _delay_ms(500); // Wait for 1 second

        // +90 degrees
        moveServo(4000);
        _delay_ms(500); // Wait for 1 second

        // Return to -90 degrees
        moveServo(2000);
        _delay_ms(500); // Wait for 1 second
    }
}
