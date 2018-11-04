#include <FastLED.h>

#define DIS_ARR     10

#define LED_PIN     6
#define NUM_LEDS    20
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 16

//////////// distance sensor //////////
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;
////////////////////// testing light ////////////
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


void setup() {
  Serial.print("start ");
    delay(100); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(  BRIGHTNESS );
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
  
}


void loop() {
   leds[0] = CRGB::Red; 
   leds[1] = CRGB::Blue;
   leds[2] = CRGB::Green;
   leds[3] = CRGB::Red; 
   leds[4] = CRGB::Yellow; 
   leds[5] = CRGB::Blue;
   leds[6] = CRGB::Red;
   leds[7] = CRGB::Red; 
   leds[8] = CRGB::Red; 
   leds[9] = CRGB::Blue;
   leds[10] = CRGB::Green;
   leds[11] = CRGB::Red; 
   leds[12] = CRGB::Yellow; 
   leds[13] = CRGB::Blue;
   leds[14] = CRGB::Green;
   leds[15] = CRGB::Red; 
        FastLED.show(); 
        delay(3000); 
    ////////////distance sensor /////////////////
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
///////////////////////////////////////////////////////////////////////
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;

      ChangePalettePeriodically();
      
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
      
      FillLEDsFromPaletteColors( startIndex);
      
      FastLED.show();
      FastLED.delay(1000 / UPDATES_PER_SECOND);
    
    /*if (distance<50){
      for (int j=0;j<DIS_ARR;j++)
            leds[j] = CRGB(0,255,0);
          FastLED.show();
          FastLED.delay(2000 / UPDATES_PER_SECOND);
    }
    else if (distance>50 && distance <100)
    {
      for (int j=10;j<20;j++)
            leds[j] = CRGB(0,255,0);
          FastLED.show();
          FastLED.delay(2000 / UPDATES_PER_SECOND);
    }
    else if (distance>100&& distance <150)
    {
      for (int j=20;j<30;j++)
            leds[j] = CRGB(0,0,255);
          FastLED.show();
          FastLED.delay(2000 / UPDATES_PER_SECOND);
    }
    else if (distance>150&& distance <200)
    {
      for (int j=30;j<40;j++)
            leds[j] = CRGB(255,0,0);
          FastLED.show();
          FastLED.delay(2000 / UPDATES_PER_SECOND);
    }
    else if (distance>200)
    {
      for (int j=40;j<50;j++)
            leds[j] = CRGB(255,255,0);
          FastLED.show();
          FastLED.delay(2000 / UPDATES_PER_SECOND);
    }*/
    /*delay(1000);
    for (int j=0;j<50;j++)
            leds[j] = CRGB(0,0,0);*/
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
