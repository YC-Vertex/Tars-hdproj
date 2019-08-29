#ifndef _T_LIGHT__H_
#define _T_LIGHT__H_

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     5
#define LED_CNT     12
#define BRIGHTNESS  180
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 100

extern const unsigned char tsTemplate[2][3];
extern int curTemp;
extern int curColor[3];

extern CRGB tsLeds[LED_CNT];

bool tLtOn();
bool tLtOff();
void tLtSetTemplate(int index);
void tLtSetColor(int * color);

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void FillLEDsFromPaletteColors(uint8_t colorIndex);
void ChangePalettePeriodically();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();

#endif
