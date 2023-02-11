///////////////////////////////////////////////////////////////////////////////////////////
// Tiva C TM4C123G Energia Communication System for ELEX4618 & ELEX4699
// Prepared by Craig Hennessey, Modified by Vrishank Prabhu
// Last Edited: February 7, 2023
// code for debounce modified from https://digilent.com/reference/learn/microprocessor/tutorials/debouncing-via-software/start
///////////////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>

// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  RED_LED;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

// Constants for the ELEX4618 communication protocol TYPE field
enum {DIGITAL = 0, ANALOG, SERVO};

#define RGBLED_RED_PIN 39
#define RGBLED_GRN_PIN 38
#define RGBLED_BLU_PIN 37

// protocol CHANNEL integer indexes into array to select to the A? pin
#define ANALOG_PINS 12
int A_PIN [] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};

// Constants for the servo pins 
#define SERVO_PORT0 19
#define SERVO_PORT1 4
#define SERVO_PORT2 5
#define SERVO_PORT3 6

#define BUTTON1 33
#define BUTTON2 32

Servo myservo[4];

int type;
int channel;
int value;
int pos = 0;
int value_prev;

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

  // initialize pushbuttons on uC to Input (not on Boosterpack)
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  // initialize LED on uC to ON (not on Boosterpack). Turn off RGB LED
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
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
  Serial.print("\n// ELEX 4618 IO Communication for TM4C123G V3.0 Student");
  Serial.print("\n// By: STUDENT NAME, DATE");
  Serial.print("\n// TM4C123G: Digital In/Out 1-40 on 4x 10 pin headers");
  Serial.print("\n// TM4C123G: Digital In 41 & 42 are PUSH1 and PUSH2 (TM4C123G)");
  Serial.print("\n// TM4C123G: Analog in A0 to A15 (0-15)");
  Serial.print("\n// TM4C123G: Analog out not supported");
  Serial.print("\n// TM4C123G: Servo 19,4,5,6 header (0-3)");
  Serial.print("\n// BoosterPack: Joystick (Analog 9,15), Accelerometer (Analog 11,13,14)");
  Serial.print("\n// BoosterPack: Buttons (Digital 32,33), LED (Digital 37,38,39)");
  Serial.print("\n// Protocol: DIRECTION (G/S) TYPE (0=D, 1=A, 2=S) CHANNEL VALUE");
  Serial.print("\n// Example: G 0 0, S 2 1 100");
  Serial.print("\n////////////////////////////////////////////////////////////////////////////////////\n");
}

void loop()
{
// check to see if it's time to blink the LED; that is, if the 
// difference between the current time and last time you blinked 
// the LED is bigger than the interval at which you want to 
// blink the LED.

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
    {
      previousMillis = currentMillis;

      if (ledState == LOW)
      {
        ledState == HIGH;
      }
      else
      {
        ledState == LOW;
      }
      digitalWrite(RED_LED, ledState);
    }
  
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

      /////////////////////////////////////////
      // TODO: Get / Set Digital
      /////////////////////////////////////////
      if (ch == 'G' || ch == 'g') {
        if (type == 0) {
          if (channel == 1) {
            value = digitalRead(BUTTON1);
          }
          else if (channel == 2) {
            value = digitalRead(BUTTON2);           
          }
        }
      }
      // IF GET -> DO A DIGITAL READ -> and return the VALUE
      // IF SET -> DO A DIGITAL WRITE
      
      /////////////////////////////////////////
      // TODO: Get / Set Analog
      /////////////////////////////////////////

      // IF GET DO AN ANALOG READ and return the VALUE
      if (ch == 'G' || ch == 'g') {
        if ((type == 1) && (channel == 11)) {
          value = analogRead(A11);
        }
        else if ((type == 1) && (channel == 4)) {
          value = analogRead(A4);
        }
      }
      
      //value_prev = 0;
      /////////////////////////////////////////
      // TODO: Get / Set Servo
      /////////////////////////////////////////
      // IF GET RETURN THE LAST SERVO VALUE SENT
      if (ch == 'G' || ch == 'g') {
        if (type == 2) {
          value = myservo[0].read(); // CRAIG LIED
          //value = value_prev;
          //value = analogRead(channel);
        }
      }
      // IF SET SEND TO SERVO OBJECT
      else if (ch == 'S' || ch == 's') {
        if (type == 2) {
          myservo[0].write(value);
          //value_prev = value;
        }
      }

      // Format and send response
      Serial.print ("A ");
      Serial.print (type);
      Serial.print (" ");
      Serial.print (channel);
      Serial.print (" ");
      Serial.print (value);
      Serial.print ("\n");
    }
  }
  
} 
