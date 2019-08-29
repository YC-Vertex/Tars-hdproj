#include "tSelf.h"

Servo tarsLUp, tarsRUp, tarsLTurn, tarsRTurn;

unsigned char tsInitial[4] = {100, 75, 55, 70};
unsigned char tsfAngle[9][2] = {{15, 160}, {15, 110}, {140, 40}, {40, 85}, {120, 60}, {60, 55}, {100, 75}};
unsigned char tsfMode[9] = {0, 1, 0, 1, 0, 1, 0, 3, 3}; // 0: up, 1: turn
int tsfInterval[9] = {200, 1200, 800, 800, 200, 800, 2000, -1, -1};
unsigned char tsbAngle[9][2] = {{15, 160}, {15, 110}, {140, 40}, {40, 85}, {120, 60}, {60, 55}, {100, 75}};
unsigned char tsbMode[9] = {0, 1, 0, 1, 0, 1, 0, 3, 3}; // 0: up, 1: turn
int tsbInterval[9] = {200, 1200, 800, 800, 200, 800, 2000, -1, -1};

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
        EEPROM.get(eeAddr, tsfAngle);
        eeAddr += sizeof(tsfAngle);
        EEPROM.get(eeAddr, tsfMode);
        eeAddr += sizeof(tsfMode);
        EEPROM.get(eeAddr, tsfInterval);
        eeAddr += sizeof(tsfInterval);
        EEPROM.get(eeAddr, tsbAngle);
        eeAddr += sizeof(tsbAngle);
        EEPROM.get(eeAddr, tsbMode);
        eeAddr += sizeof(tsbMode);
        EEPROM.get(eeAddr, tsbInterval);
        delay(100);
    }
}

bool tTsSave() {
    int eeAddr = 0;
    EEPROM.put(eeAddr, tsInitial);
    eeAddr += sizeof(tsInitial);
    EEPROM.put(eeAddr, tsfAngle);
    eeAddr += sizeof(tsfAngle);
    EEPROM.put(eeAddr, tsfMode);
    eeAddr += sizeof(tsfMode);
    EEPROM.put(eeAddr, tsfInterval);
    eeAddr += sizeof(tsfInterval);
    EEPROM.put(eeAddr, tsbAngle);
    eeAddr += sizeof(tsbAngle);
    EEPROM.put(eeAddr, tsbMode);
    eeAddr += sizeof(tsbMode);
    EEPROM.put(eeAddr, tsbInterval);
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

void tTsSetForwardSingle(int index, int* val){
    tsfAngle[index][0] = val[0];
    tsfAngle[index][1] = val[1];
    tsfMode[index] = val[2];
    tsfInterval[index] = val[3];
}

void tTsSetBackwardSingle(int index, int* val){
    tsbAngle[index][0] = val[0];
    tsbAngle[index][1] = val[1];
    tsbMode[index] = val[2];
    tsbInterval[index] = val[3];
}

void tTsMoveForward(){
    for(int i = 0; i < 9; i++){
        if(tsfMode[i]){
            tarsLTurn.write(tsfAngle[i][0]);
            tarsRTurn.write(tsfAngle[i][1]);
        }else{
            tarsLUp.write(tsfAngle[i][0]);
            tarsRUp.write(tsfAngle[i][1]);
        }
    }
}

void tTsMoveBackward(){
    for(int i = 0; i < 9; i++){
        if(tsbMode[i]){
            tarsLTurn.write(tsbAngle[i][0]);
            tarsRTurn.write(tsbAngle[i][1]);
        }else{
            tarsLUp.write(tsbAngle[i][0]);
            tarsRUp.write(tsbAngle[i][1]);
        }
    }
}

void tTsStop(){

}