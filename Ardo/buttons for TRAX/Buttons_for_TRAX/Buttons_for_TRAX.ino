/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = A0;
const int buttonPin2 = A1;
const int buttonPin3 = A2;
const int buttonPin4 = A3;
const int buttonPin5 = 12;
const int buttonPin6 = 11;
const int buttonPin7 = 10;

const int ledPin =  13;

// variables will change:
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;

void setup() {
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  buttonState6 = digitalRead(buttonPin6);
  buttonState7 = digitalRead(buttonPin7);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState1 == HIGH)
    Serial.println("1 - High");
  else
    Serial.println("1 - lOW");

  if (buttonState2 == HIGH)
    Serial.println("2 - High");
  else
    Serial.println("2 - lOW");

  if (buttonState3 == HIGH)
    Serial.println("3 - High");
  else
    Serial.println("3 - lOW");

  if (buttonState4 == HIGH)
    Serial.println("4 - High");
  else
    Serial.println("4 - lOW");

  if (buttonState5 == HIGH)
    Serial.println("5 - High");
  else
    Serial.println("5 - lOW");

  if (buttonState6 == HIGH)
    Serial.println("6 - High");
  else
    Serial.println("6 - lOW");

  if (buttonState7 == HIGH)
    Serial.println("7 - High");
  else
    Serial.println("7 - lOW");

  Serial.println("----------------------");
  delay(1000);
}
