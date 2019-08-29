#include "tSelf.h"

Servo tarsLUp, tarsRUp, tarsLTurn, tarsRTurn;

unsigned char tsInitial[4] = {100, 75, 55, 70};
unsigned char tsAngle[9][2] = {{15, 160}, {15, 110}, {140, 40}, {40, 85}, {120, 60}, {60, 55}, {100, 75}};
unsigned char tsMode[9] = {0, 1, 0, 1, 0, 1, 0, 3, 3}; // 0: up, 1: turn
int tsInterval[9] = {200, 1200, 800, 800, 200, 800, 2000, -1, -1};

void tTsInit(bool isInitFromEeprom) {
    tarsLUp.attach(9);
    tarsRUp.attach(10);
    tarsLTurn.attach(11);
    tarsRTurn.attach(12);

    for (int i = 0; i < TS_REP_COUNT; ++i) {
        tarsLUp.write(tsInitial[0]);   // + : down
        tarsRUp.write(tsInitial[1]);   // - : down
        tarsLTurn.write(tsInitial[2]); // - : front
        tarsRTurn.write(tsInitial[3]); // + : front
        delay(TS_REP_DELAY);
    }

    if (isInitFromEeprom) {
        int eeAddr = 0;
        EEPROM.get(eeAddr, tsInitial);
        eeAddr += sizeof(tsInitial);
        EEPROM.get(eeAddr, tsAngle);
        eeAddr += sizeof(tsAngle);
        EEPROM.get(eeAddr, tsMode);
        eeAddr += sizeof(tsMode);
        EEPROM.get(eeAddr, tsInterval);
        delay(100);
    }
}

bool tTsSave() {
    int eeAddr = 0;
    EEPROM.put(eeAddr, tsInitial);
    eeAddr += sizeof(tsInitial);
    EEPROM.put(eeAddr, tsAngle);
    eeAddr += sizeof(tsAngle);
    EEPROM.put(eeAddr, tsMode);
    eeAddr += sizeof(tsMode);
    EEPROM.put(eeAddr, tsInterval);
    delay(100);
    return true;
}

bool tTsSleep() {
    return false;
}

bool tTsStandby() {
    return false;
}

bool tTsSetInitial(int * angle) {
    for (int i = 0; i < 4; ++i) {
        tsInitial[i] = angle[i];
    }
}

bool tTsDebug(int lval, int rval, int mode, int itv) {
    if (mode == 0) {
        // 0: up
        tarsLUp.write(lval);
        tarsRUp.write(rval);
        delay(itv);
    } else if (mode == 1) {
        tarsLTurn.write(lval);
        tarsRTurn.write(rval);
        delay(itv);
    } else {
        return false;
    }
    return true;
}

void angleScale(int &val) {
    if (val <= 4)
        val = 4;
    if (val >= 176)
        val = 176;
    if (val <= -20 || val >= 200)
        val = 90;
}
