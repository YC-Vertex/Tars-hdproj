void setup() {
    Serial1.begin(9600);
    Particle.function("send", sendString);
    Particle.function("tssts", sendTsStatus);       // slp, sdb, mvf, mvb, stp, sav
    Particle.function("tsint", sendTsInitial);
    Particle.function("tsfwd", sendTsForward);
    Particle.function("tsbwd", sendTsBackward);
    Particle.function("tsfws", sendTsForwardSingle);
    Particle.function("tsbws", sendTsBackwardSingle);
    Particle.function("tsdbg", sendTsDebug);
    Particle.function("acsts", sendAcStatus);       // onn, off
    Particle.function("actpr", sendAcTemperature);
    Particle.function("ltsts", sendLtStatus);       // onn, off
    Particle.function("lttpl", sendLtTemplate);
    Particle.function("ltclr", sendLtColor);
    Particle.function("pwsts", sendPwStatus);       // onn, off
}

void loop() {

}

int sendString(String command) {
    Serial1.write(command);
    return 0;
}

int sendTsStatus(String command) {
    Serial1.write("$TS");
    for (int i = 0; i < strlen(command); ++i) {
        command[i] = command[i] + ('A' - 'a');
    }
    Serial1.write(command);
    Serial1.write("0:<");
    return 0;
}

int sendTsInitial(String command) {
    Serial1.write("$TSINT4:");
    Serial1.write(command);
    Serial1.write("<");
    return 0;
}

int sendTsForward(String command) {
    int nsct = 0;
    for (int i = 0; i < strlen(command); ++i) {
        if (command[i] == ';')
            ++nsct;
    }
    Serial1.write("$TSFWD");
    Serial1.write('0' + nsct);
    Serial1.write(':');
    Serial1.write(command);
    Serial1.write('<');
}

int sendTsBackward(String command) {
    int nsct = 0;
    for (int i = 0; i < strlen(command); ++i) {
        if (command[i] == ';')
            ++nsct;
    }
    Serial1.write("$TSBWD");
    Serial1.write('0' + nsct);
    Serial1.write(':');
    Serial1.write(command);
    Serial1.write('<');
}

int sendTsForwardSingle(String command) {
    Serial1.write("$TSFWS1:");
    Serial1.write(command);
    Serial1.write("<");
}

int sendTsBackwardSingle(String command) {
    Serial1.write("$TSBWS1:");
    Serial1.write(command);
    Serial1.write("<");
}

int sendTsDebug(String command) {
    Serial1.write("$TSDBG1:");
    Serial1.write(command);
    Serial1.write("<");
    return 0;
}

int sendAcStatus(String command) {
    Serial1.write("$AC");
    for (int i = 0; i < strlen(command); ++i) {
        command[i] = command[i] + ('A' - 'a');
    }
    Serial1.write(command);
    Serial1.write("0:<");
    return 0;
}

int sendAcTemperature(String command) {
    Serial1.write("$ACTPR1:");
    Serial1.write(command);
    Serial1.write("<");
    return 0;
}

int sendLtStatus(String command) {
    Serial1.write("$LT");
    for (int i = 0; i < strlen(command); ++i) {
        command[i] = command[i] + ('A' - 'a');
    }
    Serial1.write(command);
    Serial1.write("0:<");
    return 0;
}

int sendLtTemplate(String command) {
    Serial1.write("$LTTPL1:");
    Serial1.write(command);
    Serial1.write("<");
    return 0;
}

int sendLtColor(String command) {
    Serial1.write("$LTCLR1:");
    Serial1.write(command);
    Serial1.write("<");
    return 0;
}

int sendPwStatus(String command) {
    Serial1.write("$PW");
    for (int i = 0; i < strlen(command); ++i) {
        command[i] = command[i] + ('A' - 'a');
    }
    Serial1.write(command);
    Serial1.write("0:<");
    return 0;
}