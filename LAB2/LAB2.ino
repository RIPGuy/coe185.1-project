#include <Arduino.h>
//#include <avr/io.h>

// Definitions for Port B & D registors
#define PINB_Reg (*((volatile uint8_t *)0x23))
#define DDRB_Reg (*((volatile uint8_t *)0x24))
#define PORTB_Reg (*((volatile uint8_t *)0x25))
#define PIND_Reg (*((volatile uint8_t *)0x29))
#define DDRD_Reg (*((volatile uint8_t *)0x2a))
#define PORTD_Reg (*((volatile uint8_t *)0x2b))

#define BUTTON 5
#define ICP1 8  //Connect your button to the appropriate input capture pin (ICP) on your microcontroller.

uint16_t regValue = TCNT1;
int buttonEndTime = 0;
int buttonStartTime = 0;
int pressed = 1;
bool buttonState = true;
int buttonPressCount = 0;
bool bounceDetected = false;  // Flag to track if a bounce has been detected


void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");

  //Set pinmode for Button as input
  DDRD_Reg &= ~(1 << BUTTON);
  //Enable pullup
  PORTD_Reg |= (1 << BUTTON);  //set output to enable pullup resistor

  DDRB_Reg &= ~(1 << ICP1);
  PORTB_Reg |= (1 << ICP1);


  //Init counter1
  TCCR1A = 0;  //Normal mode 0xffff top, rolls over

  // TCCR1B |= (1 << CS12) | (1 << CS11);  //for checkOFF1 x1
  // TCCR1B = (1 << CS12) | (1 << CS10); //checkOFF3 & checkOFF4 x1024
  TCCR1B = (1 << CS11) | (1 << CS10);  //checkOFF5 x64

  TCCR1C = 0;
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;
  //Make sure interrupts are disabled
  TIMSK1 = 0;
  TIFR1 = 0;

  Serial.println("Finished setup!");
}

void checkOFF1() {
  if (regValue != TCNT1) {
    if (TCNT1 - regValue > 1 && (TCNT1 != 0) && (regValue != 0)) {
      Serial.println("Warning button has deboucned");
    }
    Serial.print("TCNT1: ");
    Serial.println(TCNT1);
    regValue = TCNT1;
  }
  delay(100);
}


// volatile bool overflowOccurred = false;

// void checkOFF2() {
//   if (regValue != TCNT1) {
//     if (TCNT1 - regValue > 1 && TCNT1 != 0 && regValue != 0) {

//       // Mark start time or just reset the timer if that's your intention
//       buttonStartTime = millis(); // Use millis() for a simple timing reference
//       Serial.println(millis());
//       // Wait for a condition or period here (example purpose, adjust accordingly)
//       // delay(100); // This delay is just an example, ideally use non-blocking methods

//       // Check for overflow and adjust behavior accordingly
//       // this shit is not executed
//       if (TIFR1 & (1 << TOV1)) {
//         overflowOccurred = true;
//         TIFR1 |= (1 << TOV1);

//         buttonEndTime = millis();
//         Serial.print("Bounce time on press: ");
//         Serial.println(buttonEndTime - buttonStartTime);
//       }

//       if (overflowOccurred) {
//         Serial.println("Overflow occurred during the operation.");
//         overflowOccurred = false; // Reset the flag for the next operation
//       }
//     }
//     Serial.print("TCNT1: ");
//     Serial.println(TCNT1);
//     regValue = TCNT1; // Update the last known value
//   }
//   // Consider removing delay here if timing is critical
//   delay(100);
// }

void checkOFF3() {
  //TCCR1B |= (1 << ICES1);  // set to 1 to configure Timer1 to trigger an input capture event on falling edge
  //checks when button pressed using digitalRead
  if (digitalRead(ICP1) == HIGH) {
    if (buttonState) {  // is executed since true and pressed
      buttonState = false;
      TCNT1 = 0;               // clear timer counter
      ICR1 = 0;                // clear input capture register
      buttonStartTime = ICR1;  // get time since initial press
      delay(100);              // delay a bit to debounce
      regValue = ICR1;         // store value for potential debounce
      bounceDetected = false;  // Assume no bounce detected initially

      // Logic to detect bounce here - adjust according to actual bounce detection mechanism
      if (regValue != 0) {  // if there is a debounce, indicate a bounce was detected
        buttonEndTime = ICR1;
        Serial.print("Bounce time on press: ");
        Serial.println(buttonEndTime - buttonStartTime);
        bounceDetected = true;
      }
    }
  } else {  // is executed once released
    if (!buttonState) {
      buttonState = true;

      if (!bounceDetected) {  // Only increment and print if no bounce was detected
        buttonPressCount++;
        Serial.print("Button Presses: ");
        Serial.println(buttonPressCount);
      }

      // Reset the bounceDetected flag here to prepare for the next button interaction
      // bounceDetected = false;
    }
  }
}

void checkOFF4() {
  // TCCR1B &= ~(1 << ICES1);  // set to 1 to configure Timer1 to trigger an input capture event on rising edge
  //checks when button pressed using digitalRead
  if (digitalRead(ICP1) == HIGH) {
    if (buttonState) {
      buttonState = false;
      //TCNT1 = 0;
      //ICR1 = 0;
      //buttonStartTime = ICR1;
      //delay(10);
      //regValue = ICR1;
      //if (regValue != 0){
      //The value in variable represents the time elapsed since the button press
      //buttonEndTime = ICR1;
      // Serial.print("Bounce time on press: ");
      //Subtract current and previous delay
      // Serial.println(buttonEndTime-buttonStartTime);
    }
  } else {
    if (!buttonState) {  //on release
      buttonState = true;

      TCNT1 = 0;
      ICR1 = 0;
      buttonStartTime = ICR1;  //get initial time on release
      delay(10);
      regValue = ICR1;      //store debounce on release
      if (regValue != 0) {  //check if there is a debounce
        //The value in variable represents the time elapsed since the button press
        buttonEndTime = ICR1;
        Serial.print("Bounce time on release: ");
        //Subtract current and previous delay
        Serial.println(buttonEndTime - buttonStartTime);
      } else {
        buttonPressCount++;
        Serial.print("Button Presses: ");
        Serial.println(buttonPressCount);
      }
    }
  }
}

void checkOFF5(){
  TCCR1B &= ~(1 << ICES1);
  //checks if button pressed
  if (digitalRead(ICP1) == HIGH){
    if (buttonState){
      buttonState = false;
      TCNT1 = 0;
      ICR1 = 0;
      buttonStartTime = millis();
    }
  } else {
    if(!buttonState){
      buttonState = true;
      delay(10);
      buttonEndTime = millis();

      uint32_t bounceDuration = ICR1;  
      delay(10);
      uint16_t newbounceDuration = ICR1;

      if (newbounceDuration != bounceDuration) { // Bounce occurred, print the duration of the bounce
        Serial.println("Bounce detected");
      } else { // No bounce occurred, print the number of button presses
        buttonPressCount++;
        Serial.print("Button Press Count: ");
        Serial.println(buttonPressCount);
      }


      // The duration of the button while being pressed
      Serial.print("Press Duration: ");
      Serial.println(buttonEndTime - buttonStartTime);  
    }
  }
}


void loop() {
//  checkOFF1();
  // checkOFF2();
  // checkOFF3();
  // checkOFF4();
  checkOFF5();
}
