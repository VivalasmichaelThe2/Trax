
#include <SoftwareSerial.h>// import the serial library
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <Thread.h>
#include <ThreadController.h>
#include <FastLED.h>

SoftwareSerial btSerial(1, 0); // RX, TX
//int ledpin = 13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer
String data; // the data given from Computer

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

///////////////// LEDS  //////////////////
#define LED_PIN     6
#define NUM_LEDS    8
#define BRIGHTNESS  128
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define ButtonNumer 8

/// Start Instrument Track
int buttonPins[8] = {A0, 7, 8, 9, 10, 11, 12, 13};

int pin_Out_S0 = 2;
int pin_Out_S1 = 3;
int pin_Out_S2 = 4;
int pin_Out_S3 = 5;

PN532_I2C pn532_i2c0(Wire);
PN532 nfc0(pn532_i2c0);

enum TraxRole { Empty, Note_C, Note_D, Note_E,
                Note_F, Note_G, Note_A,
                Note_B, Instrument_1
              };

uint8_t UIDList[9][8] = {{ 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }};
uint8_t uidLength[9];
TraxRole positionRoles[9] = {Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty};;

//int ledPin;
//CRGB leds[NUM_LEDS];

int positionLeds[8] = {1, 2, 3, 4, 5, 6, 7, 8};
int buttonLeds[8] = {9, 10, 11, 12, 13, 14, 15, 16};
bool buttonState[8];
bool haveLedsBeenChanged = false;


int antenaNum = 9;
int current_antenna = 0;
unsigned long delta;

void setup(void)
{
  /// Init Serial
  //Serial.begin(9600);
  btSerial.begin(9600);
  //Serial.begin(115200);
  /// Init Mux
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_Out_S3, OUTPUT);
  
  /// Init LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  /// Init Buttons
  for (int i = 0; i < ButtonNumer; i++) {
   // pinMode(buttonPins[i], INPUT);
  }

  for (int i = 0; i < 3; i++) {
    leds[i] = CRGB::Red;
  }



  ///Init NRF
  InitNRF();
}



void InitNRF() {
 // btSerial.println("3");
  uint32_t versiondata;
  for (int i = 0; i < antenaNum; i++) {
    btSerial.print("setup antena #");
    btSerial.println(i);
    switchMux(i);
    delay(100);
    nfc0.begin();

    versiondata = nfc0.getFirmwareVersion();
    if (! versiondata)
    {
      btSerial.println("Didn't find PN53x board");
    }
    else
    {
      btSerial.print("Found chip PN5");

      // btSerial.print((versiondata >> 24) & 0xFF, HEX);
      //  btSerial.print("Firmware ver. ");
      // btSerial.print((versiondata >> 16) & 0xFF, DEC);
      //  btSerial.print(".");
      //  btSerial.println((versiondata >> 8) & 0xFF, DEC);
      nfc0.setPassiveActivationRetries(0x12);
      nfc0.SAMConfig();
      delay(100);
    }
    btSerial.println("---------------------------------------------");
  }
  //btSerial.println("1");
}


void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength;
  if (current_antenna + 1 == antenaNum) {
    // btSerial.println("Loop");
    current_antenna = 0;
    btSerial.println(SendTrackStatus());
    // btSerial.print("Time to round = ");
    // btSerial.println(millis() - delta);
    // delta = millis();
  }
  else {
    current_antenna++;
  }

  switchMux(current_antenna);
  btSerial.print("i = ");
  btSerial.println(current_antenna);

  success = nfc0.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {

    PrintNfcTag( uid, uidLength);
    SetRole(uid, uidLength, current_antenna);
  }

  // UpdateButtons();
  // SetLeds() ;






  /* boolean success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
    uint8_t uidLength;
    // for (int i = 0 ; i < antenaNum; i++) {
    btSerial.print("i = ");
    btSerial.println(current_antenna);
    switchMux(current_antenna);
    //delay(1000);
    delay(200);
    success = nfc0.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
     btSerial.print("---------Scan a NFC tag #");
     btSerial.print(current_antenna);
     btSerial.println(" -------");
     btSerial.println("Found a card!");
     btSerial.print("UID Length: ");
     btSerial.print(uidLength, DEC);
     btSerial.println(" bytes");
     btSerial.print("UID Value: ");
     for (uint8_t c = 0; c < uidLength; c++)
     {
       btSerial.print(" 0x");
       btSerial.print(uid[c], HEX);
     }
     btSerial.println("");
    }*/


}

/// Need to build this with BT technology
String SendTrackStatus() {
  String stat = GetTrackStatus();
  return stat;
}


void SetRole(uint8_t uid[], uint8_t uidLength, int pos) {
  for (uint8_t c = 0; c < uidLength; c++)
  {
    UIDList[pos][c] = uid[c];
  }
  TraxRole role = GetRoleByUID(uid);
  if (pos >= 0 && pos <= 8) {
    if (positionRoles[pos] != role) {
      positionRoles[pos] = role;
      haveLedsBeenChanged = true;
    }
  }
}

void SetLeds() {
  haveLedsBeenChanged = false;
  for (int i = 0; i < ButtonNumer; i++) {

    if (positionRoles[i] != Empty) {
      CRGB ledsColor = GetColorByRole(positionRoles[i]);
      //Make Led in position leds work
      leds[positionLeds[i]] = ledsColor;

      bool done = false;
      int curentPosition = i;
      while (!done) {
        if (buttonState[curentPosition] == true) {
          leds[buttonLeds[curentPosition]] = ledsColor;
          leds[positionLeds[curentPosition]] = ledsColor;
          curentPosition++;
          //Make Led in button slot work if active leds work
        } else {
          done = true;
        }
        if (curentPosition >= ButtonNumer) {
          done = true;
        }
      }
    }
  }
}

void UpdateButtons() {

  for (int i = 0; i < ButtonNumer; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      if (buttonState[i] == false) {
        buttonState[i] = true;
        haveLedsBeenChanged = true;
      }

    } else {
      if (buttonState[i] == true) {
        buttonState[i] = false;
        haveLedsBeenChanged = true;
      }

    }
  }
}

String GetTrackStatus()
{
  String trackStatus = "";
  trackStatus += "B:";
  for (int i = 0; i < 8; i++) {
    if (buttonState[i]) {
      trackStatus += "t,";
    } else {
      trackStatus += "f,";
    }
  }
  trackStatus += "\n";
  trackStatus += "P:";
  for (int i = 0; i < 9; i++) {
    trackStatus += GetUIDString(positionRoles[i]);
    trackStatus += ",";
  }
  trackStatus += "\n";
  return trackStatus;
}

CRGB GetColorByRole(TraxRole role) {
  switch (role) {
    case Note_C: {
        return CRGB::Red;
      }
    case Note_D: {
        return CRGB::Red;
      }
    case Note_E: {
        return CRGB::Red;
      }
    case Note_F: {
        return CRGB::Red;
      }
    case Note_G: {
        return CRGB::Red;
      }
    default: {
        return CRGB::Blue;
      }

  }
}

TraxRole GetRoleByUID(uint8_t uid[]) {
  switch (uid[0]) {
    case 1: {
        return Note_C;
      }
    case 2: {
        return Note_C;
      }
    default: {
        return Note_C;
      }

  }
}


String GetUIDString(TraxRole role) {
  switch (role) {
    case Empty: {
        return "em";
      }
    case Note_C: {
        return "nc";
      }
    case Note_D: {
        return "nd";
      }
    case Note_E: {
        return "ne";
      }
    case Note_F: {
        return "nf";
      }
    case Note_G: {
        return "ng";
      }
    case Note_A: {
        return "na";
      }
    case Note_B: {
        return "nb";
      }
    case Instrument_1: {
        return "i1";
      }

  }
  return "  ";
}


void PrintNfcTag(uint8_t uid[], uint8_t uidLength) {
  btSerial.print("tag #");
  btSerial.print(current_antenna);
  btSerial.println(" -------");
  // btSerial.println("Found a card!");
  // btSerial.print("UID Length: ");
  // btSerial.print(uidLength, DEC);
  // btSerial.println(" bytes");
  btSerial.print("UID Value: ");
  for (uint8_t c = 0; c < uidLength; c++)
  {
    btSerial.print(" 0x");
    btSerial.print(uid[c], HEX);
  }
  btSerial.println("");
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


