#include "tAirConditioner.h"

char rxBuf[128];
int rxPtr = 0;
char rx1Buf[128];
int rx1Ptr = 0;

// void SerialInput();
// void CmdDecode();
// void Serial1Input();
// void Cmd1Decode();

void setup() {
    Serial.begin(115200);
    // Serial1.begin(115200);
}

void loop() {
    // Serial1Input();
    Serial.println("on");
    tAcOn();
    delay(5000);
    Serial.println("24");
    tAcTemperature(24);
    delay(5000);
    Serial.println("27");
    tAcTemperature(27);
    delay(5000);
    Serial.println("off");
    tAcOff();
    delay(5000);
}

/*
void Serial1Input() {
    while (Serial1.available()) {
        rxBuf[rxPtr++] = Serial1.read();
    }
    Cmd1Decode();
}

void Cmd1Decode() {
    bool isStart = false;
    int curpos = 0;
    char device[2];
    char command[3];
    int cmdcount = 0;
    
    for (int i = 0; i < rxPtr; ++i) {
        if (rxBuf[i] == '$') {
            curpos = 0;
            isStart = true;
        } else if (rxBuf[i] == '<' && isStart) {
            isStart = false;
        }
        if (isStart) {
            if (curpos == 1) {
                device[0] = rxBuf[i];
                device[1] = rxBuf[i+1];
            } else if (curpos == 3) {
                command[0] = rxBuf[i];
                command[1] = rxBuf[i+1];
                command[2] = rxBuf[i+2];
            }
            ++curpos;
        }
    }
}
*/
