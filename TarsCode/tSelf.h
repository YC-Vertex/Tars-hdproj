#ifndef _T_SELF__H_
#define _T_SELF__H_

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

#define TS_REP_COUNT (3)
#define TS_REP_DELAY (40)

extern Servo tarsLUp, tarsRUp, tarsLTurn, tarsRTurn;

extern unsigned char tsInitial[4];
extern unsigned char tsfAngle[9][2];
extern unsigned char tsfMode[9];
extern int tsfInterval[9];
extern unsigned char tsbAngle[9][2];
extern unsigned char tsbMode[9];
extern int tsbInterval[9];

void tTsInit(bool isInitFromEeprom = false);
bool tTsSave();
bool tTsSleep();
bool tTsStandby();
bool tTsSetInitial(int * angle);
void tTsSetForwardSingle(int index, int* val);
void tTsSetBackwardSingle(int index, int* val);
void tTsMoveForward();
void tTsMoveBackward();
void tTsStop();
bool tTsDebug(int lval, int rval, int mode, int itv = 0);

void angleScale(int &val);

#endif
