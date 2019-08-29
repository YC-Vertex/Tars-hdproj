#include "tSelf.h"
#include "tAirConditioner.h"
#include "tLight.h"
#include "tPower.h"

#define RX_BUF_SIZE (32)
#define RX1_BUF_SIZE (256)

#define DEBUG_MODE (true)

char rxBuf[RX_BUF_SIZE] = {'\0'};
int rxPtr = 0;
char nucRxBuf[RX1_BUF_SIZE] = {'\0'};
int nucRxPtr = 0;

// void SerialInput();
// void CmdDecode();
void Serial1Input();
void Cmd1Decode();

void RxCleanUp(char * buf, int index, int &size);
bool ExtractVal(char * buf, int findex, int rindex, int &target);
bool ExtractSct(char * buf, int findex, int rindex, int nums, int * target);

void DebugOutput(char * dvc, char * cmd, char * sts);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    tTsInit(true);
}

void loop() {
    Serial1Input();

    // color
    if (curTemp == 0) {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;
        ChangePalettePeriodically();
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
    } else if (curTemp == -1) {
        for (int i = 0; i < LED_CNT; ++i) {
            tsLeds[i].r = curColor[0];
            tsLeds[i].g = curColor[1];
            tsLeds[i].b = curColor[2];
        }
    } else {
        for (int i = 0; i < LED_CNT; ++i) {
            tsLeds[i].r = tsTemplate[curTemp][0];
            tsLeds[i].g = tsTemplate[curTemp][1];
            tsLeds[i].b = tsTemplate[curTemp][2];
        }
    }
}

void Serial1Input() {
    bool decodeFlag = false;
    while (Serial1.available()) {
        nucRxBuf[nucRxPtr++] = Serial1.read();
        if (nucRxBuf[nucRxPtr-1] == '<')
            decodeFlag = true;
    }
    if (decodeFlag) {
        Cmd1Decode();
    }
}

void Cmd1Decode() {
    // message flags & ptr
    bool isExist = false;
    bool isStart = false;
    bool isReady = false;
    // message header
    char dvc[3] = "..";
    char cmd[4] = "...";
    int nsct = -1;
    // message content
    int nsmc = -1;
    int smcpos[10] = {-1};

    // check if contains a whole message
    for (int i = 0; i < nucRxPtr; ++i) {
        if (nucRxBuf[i] == '$') {
            isStart = true;
            RxCleanUp(nucRxBuf, i, nucRxPtr);
            i = 0;
        } else if (nucRxBuf[i] == '<') {
            if (isStart) {
                isExist = true;
                break;
            } else {
                RxCleanUp(nucRxBuf, i + 1, nucRxPtr);
                i = -1;
            }
        }
    }
    if (!isExist) {
        return;
    } else {
        isStart = isReady = false;
    }
    
    // decode message
    for (;;) {
        for (int i = 0; i < nucRxPtr; ++i) {
            if (nucRxBuf[i] == '$') {
                RxCleanUp(nucRxBuf, i, nucRxPtr);
                i = 0;
                isStart = true;
                // reinit
                strcpy(dvc, "..");
                strcpy(cmd, "...");
                nsct = -1;
                nsmc = 0;
                for (int j = 0; j < 10; ++j)
                    smcpos[j] = -1;
            } else if (nucRxBuf[i] == '<') {
                isReady = isStart;
                isStart = false;
                if (isReady && nsct == nsmc && i == smcpos[nsct] + 1) {
                    break;
                } else {
                    RxCleanUp(nucRxBuf, i + 1, nucRxPtr);
                    i = -1;
                }
            }

            if (isStart) {
                if (i == 1) {
                    dvc[0] = nucRxBuf[i];
                    dvc[1] = nucRxBuf[i+1];
                    dvc[2] = '\0';
                } else if (i == 3) {
                    cmd[0] = nucRxBuf[i];
                    cmd[1] = nucRxBuf[i+1];
                    cmd[2] = nucRxBuf[i+2];
                    cmd[3] = '\0';
                } else if (i == 6) {
                    if (nucRxBuf[i] < '0' || nucRxBuf[i] > '9') {
                        RxCleanUp(nucRxBuf, i + 1, nucRxPtr);
                        i = -1;
                        isStart = false;
                    } else {
                        nsct = nucRxBuf[i] - '0';
                    }
                } else if (i == 7) {
                    if (nucRxBuf[i] != ':') {
                        RxCleanUp(nucRxBuf, i + 1, nucRxPtr);
                        i = -1;
                        isStart = false;
                    } else {
                        smcpos[0] = i;
                    }
                }
                else if (nucRxBuf[i] == ';') {
                    smcpos[++nsmc] = i;
                }
            }

            if (i >= nucRxPtr - 1)
                return;
        }

        if (isReady) {
            DebugOutput(dvc, cmd, "Ready");
            // tars
            if (strcmp(dvc, "TS") == 0) {
                if (strcmp(cmd, "SLP") == 0 && nsct == 0) {
                    tTsSleep();
                }
                else if (strcmp(cmd, "SDB") == 0 && nsct == 0) {
                    tTsStandby();
                }
                else if (strcmp(cmd, "INT") == 0 && nsct == 4) {
                    int val[4];
                    bool sucFlag = true;
                    for (int i = 0; i < 4; ++i) {
                        sucFlag *= ExtractVal(nucRxBuf, smcpos[i], smcpos[i+1], val[i]);
                    }
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tTsSetInitial(val);
                    }
                }
                else if (strcmp(cmd, "FWD") == 0) {
                    for (int i = 0; i < nsct; ++i) {
                        int val[4];
                        bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 4, val);
                        if (!sucFlag) {
                            DebugOutput(dvc, cmd, "Error");
                        } else {
                            tTsSetForwardSingle(i, val);
                        }
                    }
                }
                else if (strcmp(cmd, "BWD") == 0) {
                    for (int i = 0; i < nsct; ++i) {
                        int val[4];
                        bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 4, val);
                        if (!sucFlag) {
                            DebugOutput(dvc, cmd, "Error");
                        } else {
                            tTsSetBackwardSingle(i, val);
                        }
                    }
                }
                else if (strcmp(cmd, "FWS") == 0 && nsct == 1) {
                    int val[5];
                    bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 5, val);
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tTsSetForwardSingle(val[0], &val[1]);
                    }
                }
                else if (strcmp(cmd, "BWS") == 0 && nsct == 1) {
                    int val[5];
                    bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 5, val);
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tTsSetForwardSingle(val[0], &val[1]);
                    }
                }
                else if (strcmp(cmd, "MVF") == 0 && nsct == 0) {
                    tTsMoveForward();
                }
                else if (strcmp(cmd, "MVB") == 0 && nsct == 0) {
                    tTsMoveBackward();
                }
                else if (strcmp(cmd, "STP") == 0 && nsct == 0) {
                    tTsStop();
                }
                else if (strcmp(cmd, "DBG") == 0 && nsct == 1) {
                    int val[4];
                    bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 4, val);
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tTsDebug(val[0], val[1], val[2], val[3]);
                    }
                }
                else if (strcmp(cmd, "SAV") == 0 && nsct == 0) {
                    tTsSave();
                }
            }

            // air conditioner
            else if (strcmp(dvc, "AC") == 0) {
                if (strcmp(cmd, "ONN") == 0) {
                    if (nsct == 0) 
                        tAcOn();
                    else if (nsct == 1) {
                        int val = 0;
                        bool sucFlag = ExtractVal(nucRxBuf, smcpos[0], smcpos[1], val);
                        if (!sucFlag) {
                            DebugOutput(dvc, cmd, "Error");
                        } else {
                            tAcOn(val);
                        }
                    }
                }
                else if (strcmp(cmd, "OFF") == 0 && nsct == 0) {
                    tAcOff();
                }

            }

            // light            
            else if (strcmp(dvc, "LT") == 0) {
                if (strcmp(cmd, "ONN") == 0 && nsct == 0) {
                    tLtOn();
                }
                else if (strcmp(cmd, "OFF") == 0 && nsct == 0) {
                    tLtOff();
                }
                else if (strcmp(cmd, "TPL") == 0 && nsct == 1) {
                    int val = 0;
                    bool sucFlag = ExtractVal(nucRxBuf, smcpos[0], smcpos[1], val);
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tLtSetTemplate(val);
                    }
                }
                else if (strcmp(cmd, "CLR") == 0 && nsct == 1) {
                    int val[3] = {0};
                    bool sucFlag = ExtractSct(nucRxBuf, smcpos[0], smcpos[1], 3, val);
                    if (!sucFlag) {
                        DebugOutput(dvc, cmd, "Error");
                    } else {
                        tLtSetColor(val);
                    }
                }
            }

            // power
            else if (strcmp(dvc, "PW") == 0) {
                if (strcmp(cmd, "ONN") == 0 && nsct == 0) {
                    tLtOn();
                }
                else if (strcmp(cmd, "OFF") == 0 && nsct == 0) {
                    tLtOff();
                }
            }

            isReady = false;
            RxCleanUp(nucRxBuf, smcpos[nsct] + 2, nucRxPtr);
        }

        if (nucRxPtr <= 0) {
            return;
        }
    }
}

void RxCleanUp(char * buf, int index, int &size) {
    for (int i = index; i < size; ++i)
        buf[i-index] = buf[i];
    for (int i = size - index; i < size; ++i)
        buf[i] = '\0';
    size -= index;
}

bool ExtractVal(char * buf, int findex, int rindex, int &target) {
    int val = 0;
    for (int i = findex + 1; i < rindex; ++i) {
        if (buf[i] < '0' || buf[i] > '9')
            return false;
        val *= 10;
        val += buf[i] - '0';
    }
    target = val;
    return true;
}

bool ExtractSct(char * buf, int findex, int rindex, int nums, int * target) {
    int val[nums] = {0};
    int compos[nums+1] = {-1};
    int ncom = 0;

    compos[0] = findex;
    for (int i = findex + 1; i < rindex; ++i) {
        if (buf[i] == ',')
            compos[++ncom] = i;
    }
    if (ncom + 1 != nums)
        return false;

    for (int i = 0; i < nums; ++i) {
        if (!ExtractVal(buf, compos[i], compos[i+1], val[i]))
            return false;
    }

    for (int i = 0; i < nums; ++i) {
        target[i] = val[i];
    }
    return true;
}

void DebugOutput(char * dvc, char * cmd, char * sts) {
    if (DEBUG_MODE) {
        Serial.print("> decive = ");
        Serial.print(dvc);
        Serial.print(", command = ");
        Serial.print(cmd);
        Serial.print(": ");
        Serial.println(sts);
    }
}
