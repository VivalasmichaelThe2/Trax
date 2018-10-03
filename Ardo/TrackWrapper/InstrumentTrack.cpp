#include "InstrumentTrack.h"

InstrumentTrack::InstrumentTrack(int butPins[8])
{
  ledPin = LED_PIN;
  /// Buttons
  for (int i = 0; i < ButtonNumer; i++) {
    buttonPins[i] = butPins[i];
    pinMode(buttonPins[i], INPUT);
  }
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

/// Has To Be called in setup on the arduino
void InstrumentTrack::Begin()
{

}

/// Has To be called every Update on the arduino
void InstrumentTrack::Run() {
  UpdateButtons();
  if (haveLedsBeenChanged) {
    SetLeds();
  }
}

void InstrumentTrack::SetRole(int pos, TraxRole role) {
  if (pos >= 0 && pos <= 8) {
    if (positionRoles[pos] != role) {
      positionRoles[pos] = role;
      haveLedsBeenChanged = true;
    }
  }
}

void InstrumentTrack::SetLeds() {
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

void InstrumentTrack::UpdateButtons() {
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

String InstrumentTrack::GetTrackStatus()
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


//Empty, , Note_D, Note_E,
//                Note_F, Note_G, Note_A,
//                Note_B, Instrument_1
/// Make a diffrent color for each role
CRGB InstrumentTrack::GetColorByRole(TraxRole role) {
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

String InstrumentTrack::GetUIDString(TraxRole role) {
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



InstrumentTrack::~InstrumentTrack()
{
  //dtor
}

