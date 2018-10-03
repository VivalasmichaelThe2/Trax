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
int antenaNum = 8;

//int pin_Out_S0 = 3;
//int pin_Out_S1 = 4;
//int pin_Out_S2 = 5;
//int pin_Out_S3 = 6;

void setup(void)
{
  // Serial.begin(9600);
  Serial.begin(115200);
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_Out_S3, OUTPUT);
  uint32_t versiondata;
  for (int i = 0; i < antenaNum; i++) {
    // switchMux(i);
    //        digitalWrite(pin_Out_S0, HIGH && (i & B00000001));
    //    digitalWrite(pin_Out_S1, HIGH && (i & B00000010));
    //    digitalWrite(pin_Out_S2, HIGH && (i & B00000100));
    //    digitalWrite(pin_Out_S3, HIGH && (i & B00001000));
    //  nfc.begin();
  }
  //nfc.begin();
  for (int i = 0; i < antenaNum; i++) {
    Serial.print("setup start of antena ");
    Serial.println(i);
    switchMux(i);
    delay(2500);
    nfc.begin();


    // Serial.println("switch mux to postion");



    // Serial.println("Begin NFC");
    versiondata = nfc.getFirmwareVersion();
    // Serial.println("Get Version");
    if (! versiondata)
    {
      Serial.println("Didn't find PN53x board");
    }
    else
    {
      Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
      nfc.setPassiveActivationRetries(0x12);
      // nfc.setPassiveActivationRetries(0x03);
      //Serial.println(" nfc.setPassiveActivationRetries(0x03);");
      nfc.SAMConfig();
      //Serial.println("  nfc.SAMConfig();");
      delay(100);
    }
    Serial.println("---------------------------------------------");
  }
}
void loop() {
  boolean success;

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength;
  for (int i = 0 ; i < antenaNum; i++) {
    switchMux(i);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
      Serial.print("---------Scan a NFC tag #");
      Serial.print(i);
      Serial.println(" -------");
      Serial.println("Found a card!");
      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t c = 0; c < uidLength; c++)
      {
        Serial.print(" 0x"); Serial.print(uid[c], HEX);
      }
      Serial.println("");
      delay(300);
    }
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
    case 16: {
        digitalWrite(pin_Out_S0, HIGH );
        digitalWrite(pin_Out_S1, HIGH);
        digitalWrite(pin_Out_S2, HIGH );
        digitalWrite(pin_Out_S3, HIGH );
        break;
      }

  }
}
