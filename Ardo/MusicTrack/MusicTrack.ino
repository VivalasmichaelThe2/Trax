#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <Thread.h>
#include <ThreadController.h>
#include <FastLED.h>

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

///////////////// LEDS  //////////////////
#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

#define ButtonNumer 8

/// Start Instrument Track
int buttonPins[8] = {0, 1, 2, 3, 4, 5, 6, 7};

//InstrumentTrack track(buttonPins);

int pin_Out_S0 = 3;
int pin_Out_S1 = 5;
int pin_Out_S2 = 6;
int pin_Out_S3 = 9;
PN532_I2C pn532_i2c0(Wire);
PN532 nfc0(pn532_i2c0);
PN532_I2C pn532_i2c1(Wire);
PN532 nfc1(pn532_i2c1);
PN532_I2C pn532_i2c2(Wire);
PN532 nfc2(pn532_i2c2);
PN532_I2C pn532_i2c3(Wire);
PN532 nfc3(pn532_i2c3);
PN532_I2C pn532_i2c4(Wire);
PN532 nfc4(pn532_i2c4);
PN532_I2C pn532_i2c5(Wire);
PN532 nfc5(pn532_i2c5);
PN532_I2C pn532_i2c6(Wire);
PN532 nfc6(pn532_i2c6);

//PN532_I2C pn532_i2c7(Wire);
//PN532 nfc7(pn532_i2c7);
//PN532_I2C pn532_i2c8(Wire);
//PN532 nfc8(pn532_i2c8);
//PN532_I2C pn532_i2c9(Wire);
//PN532 nfc9(pn532_i2c9);
//PN532_I2C pn532_i2c10(Wire);
//PN532 nfc10(pn532_i2c10);

enum TraxRole { Empty, Note_C, Note_D, Note_E,
                Note_F, Note_G, Note_A,
                Note_B, Instrument_1 };

PN532 *nfc_arr[] = {&nfc0, &nfc1, &nfc2, &nfc3, &nfc4, &nfc5, &nfc6};
uint8_t UIDList[9][8] = {{ 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }};
uint8_t uidLength[9];
TraxRole positionRoles[9] = {Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty};;

int ledPin;
CRGB leds[NUM_LEDS];

int positionLeds[8] = {1, 2, 3, 4, 5, 6, 7, 8};
int buttonLeds[8] = {9, 10, 11, 12, 13, 14, 15, 16};
bool buttonState[8];

bool haveLedsBeenChanged = false;
int antenaNum = 7;

void setup(void)
{
  /// Init Serial
  Serial.begin(9600);
  //Serial.begin(115200);
  /// Init Mux
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_Out_S3, OUTPUT);
  /// Init LEDs
  ledPin = LED_PIN;
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  /// Init Buttons
  for (int i = 0; i < ButtonNumer; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  ///Init NRF
  InitNRF();
}

void loop() {
  Serial.println("Loop");
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength;
  for (int i = 0 ; i < antenaNum; i++) {
    Serial.print("i = ");
    Serial.println(i);
    switchMux(i);
    delay(100);
    success = nfc_arr[i]->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      PrintNfcTag( uid, uidLength, i);
      SetRole(uid, uidLength, i);
    }
  }
  UpdateButtons();
  SetLeds() ;
  SendTrackStatus();
}

/// Need to build this with BT technology
void SendTrackStatus() {
  String stat = GetTrackStatus();

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
      trackStatus += "t";
    } else {
      trackStatus += "f";
    }
  }
  trackStatus += "\n";
  trackStatus += "P:";
  for (int i = 0; i < 9; i++) {
    trackStatus += GetUIDString(positionRoles[i]);
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


void InitNRF() {
  uint32_t versiondata;
  for (int i = 0; i < antenaNum; i++) {
    Serial.print("setup start of antena ");
    Serial.println(i);
    switchMux(i);
    delay(1000);
    nfc_arr[i]->begin();

    versiondata = nfc_arr[i]->getFirmwareVersion();
    Serial.println(versiondata);
    if (! versiondata)
    {
      Serial.println("Didn't find PN53x board");
    }
    else
    {
      Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc_arr[i]->setPassiveActivationRetries(0x12);
      nfc_arr[i]->SAMConfig();
      delay(1000);
    }
    Serial.println("---------------------------------------------");
  }

}

void PrintNfcTag(uint8_t uid[], uint8_t uidLength, int i) {
  Serial.print("---------Scan a NFC tag #");
  Serial.print(i);
  Serial.println(" -------");
  Serial.println("Found a card!");
  Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
  Serial.print("UID Value: ");
  //SetRole(int pos, TraxRole role);
  for (uint8_t c = 0; c < uidLength; c++)
  {
    Serial.print(" 0x"); Serial.print(uid[c], HEX);
  }
  Serial.println("");
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


