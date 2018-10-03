#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

int pin_Out_S0 = 3;
int pin_Out_S1 = 5;
int pin_Out_S2 = 6;
int pin_Out_S3 = 9;
int pin_In_Mux1 = A4;
int pin_In_Mux2 = A5;
int Mux1_State[16] = {0};

void setup() {

  Serial.begin(9600);
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_Out_S3, OUTPUT);
  //pinMode(pin_In_Mux1, INPUT);
  nfc.begin();
  /*digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (0 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    Serial.println(HIGH && (1 & B00000001));
    Serial.println(HIGH && (2 & B00000010));
    Serial.println(HIGH && (8 & B00000100));
    Serial.println(HIGH && (16 & B00001000));*/
    /*Serial.print(HIGH && (i & B00000001));
    Serial.print(",");
    Serial.print(HIGH && (i & B00000010));
    Serial.print(",");
    Serial.print(HIGH && (i & B00000100));
    Serial.print(",");
    Serial.println(HIGH && (i & B00001000));
    uint32_t versiondata = 0;*/
    Serial.print("Number: ");
    Serial.println(0);
    digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (0 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (8 & B00001000));
    uint32_t versiondata = 0;
    versiondata = nfc.getFirmwareVersion();
    if (! versiondata)
    {
      Serial.println("-----Didn't find PN53x board------");
      while (versiondata);
    }
    else {
      Serial.print("******Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc.setPassiveActivationRetries(0x03);
      nfc.SAMConfig();
      delay(300);
    }
/*
    Serial.print("Number: ");
    Serial.println("1");
    digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (0 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (0 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    versiondata = 0;
    versiondata = nfc.getFirmwareVersion();
    if (! versiondata)
    {
      Serial.println("-----Didn't find PN53x board------");
      while (versiondata);
    }
    else {
      Serial.print("******Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc.setPassiveActivationRetries(0x03);
      nfc.SAMConfig();
      delay(300);
    }

    Serial.print("Number: ");
    Serial.println("2");
    digitalWrite(pin_Out_S0, HIGH && (0 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (4 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    versiondata = 0;
    versiondata = nfc.getFirmwareVersion();
    if (! versiondata)
    {
      Serial.println("-----Didn't find PN53x board------");
      delay(200);
    }
    else {
      Serial.print("******Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc.setPassiveActivationRetries(0x03);
      nfc.SAMConfig();
      delay(300);
    }

    Serial.print("Number: ");
    Serial.println("3");
    digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (4 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    versiondata = 0;
    versiondata = nfc.getFirmwareVersion();
    if (! versiondata)
    {
      Serial.println("-----Didn't find PN53x board------");
      while (versiondata);
    }
    else {
      Serial.print("******Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc.setPassiveActivationRetries(0x03);
      nfc.SAMConfig();
      delay(300);
    }*/
  
}

void loop() {

    Serial.println("Number: 0");
    digitalWrite(pin_Out_S0, HIGH && (0 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (0 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    delay(100);
    boolean success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
    uint8_t uidLength;
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      Serial.println("Found a card!");
      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t c = 0; c < uidLength; c++)
      {
        Serial.print(" 0x"); Serial.print(uid[c], HEX);
      }
      Serial.println("");
    }

    Serial.println("Number: 1");
    digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (0 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    delay(100);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      Serial.println("Found a card!");
      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t c = 0; c < uidLength; c++)
      {
        Serial.print(" 0x"); Serial.print(uid[c], HEX);
      }
      Serial.println("");
    }

    Serial.println("Number: 2");
    digitalWrite(pin_Out_S0, HIGH && (0 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (4 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    delay(100);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      Serial.println("Found a card!");
      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t c = 0; c < uidLength; c++)
      {
        Serial.print(" 0x"); Serial.print(uid[c], HEX);
      }
      Serial.println("");
    }

    Serial.println("Number: 3");
    digitalWrite(pin_Out_S0, HIGH && (1 & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (2 & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (4 & B00000100));
    digitalWrite(pin_Out_S3, HIGH && (0 & B00001000));
    delay(100);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      Serial.println("Found a card!");
      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t c = 0; c < uidLength; c++)
      {
        Serial.print(" 0x"); Serial.print(uid[c], HEX);
      }
      Serial.println("");
    }
}

