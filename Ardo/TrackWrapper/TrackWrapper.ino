#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include "InstrumentTrack.h"
#include <Thread.h>
#include <ThreadController.h>

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

/// Start Instrument Track
int buttonPins[8] = {0,1,2,3,4,5,6,7};

InstrumentTrack track(buttonPins);

int pin_Out_S0 = 3;
int pin_Out_S1 = 5;
int pin_Out_S2 = 6;
int pin_Out_S3 = 9;
int antenaNum = 8;

void setup(void)
{
  // Serial.begin(9600);
  Serial.begin(115200);

  
}
void loop() {
  
}

