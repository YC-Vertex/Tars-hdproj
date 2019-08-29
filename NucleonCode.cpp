void setup() {
    Serial1.begin(115200);
    Particle.function("send", sendString);
    Particle.function("tssts", sendTsStatus);
    Particle.function("tsint", sendTsInitial);
    Particle.function("tsdbg", sendTsDebug);
    Particle.function("acsts", sendAcStatus);
    Particle.function("actpr", sendAcTemperature);
    Particle.function("ltsts", sendLtStatus);
    Particle.function("pwsts", sendPwStatus);
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

int sendPwStatus(String command) {
    Serial1.write("$PW");
    for (int i = 0; i < strlen(command); ++i) {
        command[i] = command[i] + ('A' - 'a');
    }
    Serial1.write(command);
    Serial1.write("0:<");
    return 0;
}