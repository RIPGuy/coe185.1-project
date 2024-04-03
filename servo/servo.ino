#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define SERVO_PIN 9

Servo myservo;

void left1(){
  //1 ms pulse
  TCNT1H = 0xC1;
  TCNT1L = 0x80;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void stop1 (){
  //1.25 ms pulse
  TCNT1H = 0xC1;
  TCNT1L = 0x80;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void right1() {
  //1.5 ms pulse
  TCNT1H = 0xA2;
  TCNT1L = 0x40;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void left2() {
  //1.5 ms pulse
  TCNT1H = 0xA2;
  TCNT1L = 0x40;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void stop2 () {
  //1.5 ms pulse
  TCNT1H = 0x92;
  TCNT1L = 0xA0;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void right2 () {
  //1.5 ms pulse
  TCNT1H = 0x83;
  TCNT1L = 0x00;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000; //change
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void twenty (){
  //20msec pulse
  TCNT1H = 0xB1;
  TCNT1L = 0xE0;
  TCCR1A = 0x00;
  TCCR1B = 0x01; //no prescaler
  PORTD = 0b0000000;
  while((TIFR1&(1<<TOV1))==0);
  TCCR1A = 0;
  TCCR1B = 0;
  TIFR1 = (1<<TOV1);
}

void setup(){
  DDRD = 1<<2;
  PORTD &= ~(1<<2);
}

void loop(){
  //1.0ms pulse
  for int ( i = 0; i < 141; i++){
    left1();
    twenty();
  }
}