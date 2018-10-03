#ifndef INSTRUMENTTRACK_H
#define INSTRUMENTTRACK_H
#include "Arduino.h"
#include <FastLED.h>


///////////////// LEDS  //////////////////
#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

#define ButtonNumer 8
enum TraxRole { Empty, Note_C, Note_D, Note_E,
                Note_F, Note_G, Note_A,
                Note_B, Instrument_1   };


class InstrumentTrack
{
  public:
    InstrumentTrack(int buttonPins[8]);
    /// Has To be called every Update on the arduino
    void Run();
    /// Has To Be called in setup on the arduino
    void Begin();
    ///Updates all button states
    void UpdateButtons();
///
    String GetTrackStatus();
    ///
    String GetUIDString(TraxRole role);

    ///
    void SetRole(int pos, TraxRole role);
    //
    void SetLeds();
    CRGB GetColorByRole(TraxRole role);

    virtual ~InstrumentTrack();

  private:

    uint8_t UIDList[9][8] = {{ 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }};
    uint8_t uidLength[9];
    TraxRole positionRoles[9];

    int ledPin;
    CRGB leds[NUM_LEDS];

    int positionLeds[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int buttonLeds[8] = {9, 10, 11, 12, 13, 14, 15, 16};

    int buttonPins[8];
    bool buttonState[8];

    bool haveLedsBeenChanged = false;

};



#endif // INSTRUMENTTRACK_H
