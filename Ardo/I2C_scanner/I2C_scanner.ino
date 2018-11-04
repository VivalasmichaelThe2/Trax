 // --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>

int pin_Out_S0 = 2;
int pin_Out_S1 = 3;
int pin_Out_S2 = 4;
int pin_Out_S3 = 5;
int antenaNum = 8;

void setup()
{
  Wire.begin();
  Serial.println("\nI2C Scanner");
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_Out_S3, OUTPUT);
  Serial.begin(115200);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}


void loop()
{
  for (int i = 0; i < antenaNum; i++) {
  Serial.println("-------------------------------------");
  Serial.print("checking mux channel number ");
  Serial.println(i);
  switchMux(i);
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 36 ; address < 37; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("%%%%%%%%%%I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !%%%%%%%%%%%%%%%%");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("*************No I2C devices found***************\n");
  else
    Serial.println("done\n");

  delay(500);           // wait 5 seconds for next scan
  }
}



void switchMux(int i) {
  switch (i) {
    case 0: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 1: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 2: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 3: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 4: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 5: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 6: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 7: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, LOW );
        break;
      }
    case 8: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 9: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 10: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 11: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, LOW );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 12: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 13: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, LOW);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 14: {
        digitalWrite(pin_Out_S0, LOW );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
    case 15: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }
  }
}
