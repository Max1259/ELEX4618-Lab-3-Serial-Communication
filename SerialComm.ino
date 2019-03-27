///////////////////////////////////////////////////////////////////////////////////////////
// MSP432 Energia Communication System for ELEX4618 & ELEX4699
// Prepared by Craig Hennessey
// Last Edited: Feb 7, 2018
///////////////////////////////////////////////////////////////////////////////////////////
//include "Serial.h"
#include <driverlib.h> 
#include <Servo.h>

// Constants for the ELEX4618 communication protocol TYPE field
enum {DIGITAL = 0, ANALOG, SERVO};

#define RGBLED_RED_PIN 39
#define RGBLED_GRN_PIN 38
#define RGBLED_BLU_PIN 37

// The MSP432 has 16 10-Bit ADC channels. The A_PIN array provides an easy way to map the 
// protocol CHANNEL integer to the A? pin
#define ANALOG_PINS 16
int A_PIN[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

//Constants for button pins
#define PUSH1 32
#define PUSH2 33

#define JOY_Y A9
#define JOY_X A15

// Constants for the servo pins 
#define SERVO_PORT0 19
#define SERVO_PORT1 4
#define SERVO_PORT2 5
#define SERVO_PORT3 6

Servo myservo[4];

int type;
int channel;
int value;
int position = 0;
unsigned long time_initial = 0;
const long interval = 1000;

#define BAUD_RATE 115200

void setup()
{
  // initialize serial port
  Serial.begin(BAUD_RATE);

  // initialize digital IO to Input 
  for (int digital_index = 1; digital_index <= 40; digital_index++)
  {
    pinMode(digital_index, INPUT_PULLUP);
  }

  // initialize MSP432 pushbuttons to Input (not on Boosterpack)
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);

  // initialize MSP432 LED to ON (not on Boosterpack). Turn off RGB LED
  pinMode(RED_LED, OUTPUT);
  pinMode(RGBLED_RED_PIN, OUTPUT);
  pinMode(RGBLED_GRN_PIN, OUTPUT);
  pinMode(RGBLED_BLU_PIN, OUTPUT);
  digitalWrite(RGBLED_RED_PIN, LOW);
  digitalWrite(RGBLED_GRN_PIN, LOW);
  digitalWrite(RGBLED_BLU_PIN, LOW);

  // initialize servo pins to output
  pinMode(SERVO_PORT0, OUTPUT);
  pinMode(SERVO_PORT1, OUTPUT);
  pinMode(SERVO_PORT2, OUTPUT);
  pinMode(SERVO_PORT3, OUTPUT);

  // Attach servo pins to the servo objects
  myservo[0].attach(SERVO_PORT0);
  myservo[1].attach(SERVO_PORT1);
  myservo[2].attach(SERVO_PORT2);
  myservo[3].attach(SERVO_PORT3);

  Serial.print("\n////////////////////////////////////////////////////////////////////////////////////");
  Serial.print("\n// ELEX 4618 IO Communication for MSP433 V2.0 Student");
  Serial.print("\n// By: Max Horie, A01010912, 01/17/19");
  Serial.print("\n// MSP432: Digital In/Out 1-40 on 4 headers");
  Serial.print("\n// MSP432: Digital In 41 & 42 are PUSH1 and PUSH2");
  Serial.print("\n// MSP432: Analog in A0 to A15 (0-15)");
  Serial.print("\n// MSP432: Analog out not supported");
  Serial.print("\n// MSP432: Servo 19,4,5,6 header (0-3)");
  Serial.print("\n// BoosterPack (432): Accelerometer (A 11,13,14), Joystick (A 9,15), Buttons (D 32,33), LED (37,38,39)");
  Serial.print("\n// Protocol: DIRECTION (G/S) TYPE (0=D, 1=A, 2=S) CHANNEL VALUE");
  Serial.print("\n// Example: G 0 0, S 2 1 100");
  Serial.print("\n////////////////////////////////////////////////////////////////////////////////////\n");
}

void loop()
{ 
  /*
  //code to blink the LED without delay
  unsigned long time_current = millis();
  
  if (time_current - time_initial >= interval) {
      
      time_initial = time_current;

      if (digitalRead(RED_LED) == LOW) {
        
        digitalWrite(RED_LED, HIGH);
        
      } else {
        
        digitalWrite(RED_LED, LOW);
      }
  }*/
  
  // While there is data in the serial port buffer, continue to process
  while (Serial.available() > 0)
  {
    // Read the first character
    char ch = Serial.read();

    // If it's a COMMAND character (first character in ELEX4618 protocol) then move to next step
    if (ch == 'G' || ch == 'g' || ch == 'S' || ch == 's')
    {      
      // Read the space delimited next value as an integer (TYPE from protocol)
      type = Serial.parseInt();
      // Read the space delimited next value as an integer (CHANNEL from protocol)
      channel = Serial.parseInt();

      // If a SET command then read the space delimited next value as an integer (VALUE from protocol)
      if (ch == 'S' || ch == 's')
      {      
        value = Serial.parseInt();
      }
      

      if ((ch == 'G' || ch == 'g') && type == DIGITAL) {
          value = digitalRead(channel);
          acknowledge(type, channel, value);
        }
      } 
      if ((ch == 'S' || ch == 's') && type == DIGITAL) {
          digitalWrite(channel, value);
          //acknowledge(type, channel, value);
      }

    if ((ch == 'G' || ch == 'g') && type == ANALOG) {
        value = analogRead(A_PIN[channel]);  
        acknowledge(type, channel, value); 
    }

    if ((ch == 'G' || ch == 'g') && type == SERVO) {
        value = myservo[channel].read();
        acknowledge(type, channel, value);
    }

      if ((ch == 'S' || ch == 's') && type == SERVO) {
            myservo[channel].write(value);
            acknowledge(type, channel, value);
      }    


    }
  }

  void acknowledge(int t, int c, int val) {
          // Format and send response
      Serial.print ("A ");
      Serial.print (type);
      Serial.print (" ");
      Serial.print (channel);
      Serial.print (" ");
      Serial.print (value);
      Serial.print ("\n");
  }
  
