#include "tSelf.h"

Servo tarsLUp, tarsRUp, tarsLTurn, tarsRTurn;

unsigned char tsInitial[4] = {100, 75, 55, 70};
unsigned char tsAngle[9][2] = {{15, 160}, {15, 110}, {140, 40}, {40, 85}, {120, 60}, {60, 55}, {100, 75}};
unsigned char tsMode[9] = {0, 1, 0, 1, 0, 1, 0, -1, -1}; // 0: up, 1: turn
int tsInterval[9] = {200, 1200, 800, 800, 200, 800, 2000, -1, -1};

void tTsInit() {
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
}
bool tTsSleep() {
    return false;
}

bool tTsStandby() {
    return false;
}

bool tTsSetInitial() {
    return false;
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
