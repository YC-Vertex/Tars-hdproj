#include "tLight.h"

const unsigned char tsTemplate[2][3] = {
    {BRIGHTNESS, BRIGHTNESS, BRIGHTNESS}, {20, 20, BRIGHTNESS}
};
int curTemp = 1;
int curColor[3] = {0, 0, 0};

CRGB tsLeds[LED_CNT];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void tLtInit() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(tsLeds, LED_CNT).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

bool tLtOn() {
    return false;
}

bool tLtOff() {
    return false;
}

void tLtSetTemplate(int index) {
    curTemp = index;
}

void tLtSetColor(int * color) {
    curColor[0] = color[0];
    curColor[1] = color[1];
    curColor[2] = color[2];
    curTemp = -1;
}



void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for (int i = 0; i < LED_CNT; i++) {
        tsLeds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if (lastSecond != secondHand) {
        lastSecond = secondHand;
        if (secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if (secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if (secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if (secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if (secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if (secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if (secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if (secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if (secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if (secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if (secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

void SetupTotallyRandomPalette()
{
    for(int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette()
{
    fill_solid(currentPalette, 16, CRGB::Black);
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green  = CHSV(HUE_GREEN, 255, 255);
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
