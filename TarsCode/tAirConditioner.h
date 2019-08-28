#ifndef _T_AIR_CONDITIONER__H_
#define _T_AIR_CONDITIONER__H_

#include <Arduino.h>
#include <IRremote.h>

#define AC_DEFAULT_TEMP (26)
#define AC_REP_COUNT (3)
#define AC_REP_DELAY (100)

enum AcMode { TAC_COOL, TAC_HEAT, TAC_FAN, TAC_DEHUMID };
enum AcSpeed { TAC_AUTO, TAC_LOW, TAC_MED, TAC_HIGH };

struct AcStatus {
    int temperature;
    AcMode mode;
    AcSpeed speed;
};

extern IRsend irsend;
extern const bool act[13][212];
extern AcStatus curStats;

bool tAcOn(int temperature = AC_DEFAULT_TEMP);
bool tAcOff();

bool tAcTemperature(int temperature);
bool tAcMode(AcMode mode);
bool tAcSpeed(AcSpeed speed);

#endif
