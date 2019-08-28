#ifndef _T_SELF__H_
#define _T_SELF__H_

#include <Arduino.h>
#include <Servo.h>

#define TS_REP_COUNT (3)
#define TS_REP_DELAY (40)

extern Servo tarsLUp, tarsRUp, tarsLTurn, tarsRTurn;

extern unsigned char tsInitial[4];
extern unsigned char tsAngle[9][2];
extern unsigned char tsMode[9];
extern int tsInterval[9];

void tTsInit();
bool tTsSleep();
bool tTsStandby();
bool tTsSetInitial(int * angle);
bool tTsDebug(int lval, int rval, int mode, int itv = 0);

#endif
