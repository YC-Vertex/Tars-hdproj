#ifndef _T_AIR_CONDITIONER__H_
#define _T_AIR_CONDITIONER__H_

#include <IRremote.h>

#define DEFAULT_TEMP (26)
extern const bool act[13][209];
IRsend irsend;

enum AcMode { TAC_COOL, TAC_HEAT, TAC_FAN, TAC_DEHUMID };
enum AcSpeed { TAC_AUTO, TAC_LOW, TAC_MED, TAC_HIGH };

struct AcStatus {
    int temperature;
    AcMode mode;
    AcSpeed speed;
} curStatus;

bool tAcOn(int temperature = DEFAULT_TEMP);
bool tAcOff();

bool tAcTemperature(int temperature);
bool tAcMode(AcMode mode);
bool tAcSpeed(AcSpeed speed);

#endif
