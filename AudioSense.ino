#include <Arduino.h>
#include "User_Setup.h"

// Define pins
#define AUDIO_SENSE_PIN A0
#define CONTROL_IN 5
#define CONTROL_OUT 6
#define IR_LINE 7

// Define settings
#define SIGNAL_THRESHOLD 250  // milliseconds

void setup() {
    Serial.begin(115200);
    pinMode(AUDIO_SENSE_PIN, INPUT);
    pinMode(CONTROL_IN, INPUT);
    pinMode(CONTROL_OUT, OUTPUT);
    pinMode(IR_LINE, INPUT);
    pinMode(13, OUTPUT); // TESTING
}

bool lastSignalState = false;

bool checkSignal() {
    bool SIG_SENSED = false;
    
    // Find out if speaker is off
    bool POWER_OFF = !digitalRead(CONTROL_IN);

    // If speaker is off, float the CONTROL_OUT line.
    // This will turn on the optical input but won't turn on the power LED.
    if (POWER_OFF)
        pinMode(CONTROL_OUT, INPUT);

    // Check for audio signal
    unsigned long startTime = millis();
    while (millis() - startTime < SIGNAL_THRESHOLD) {
        // I use analogRead instead of digitalRead in order to allow detection of the 3.3v audio signal
        // even if the micro controller has a 5v logic level.
        if (analogRead(AUDIO_SENSE_PIN) != 0) {
            // A signal was sensed for longer than the threshold setting
            SIG_SENSED = true;
        }
    }

    if (POWER_OFF){
        // Reconnect CONTROL LINE
        pinMode(CONTROL_OUT, OUTPUT);
    }

    // DEBUGGING
    if (SIG_SENSED != lastSignalState) {
        Serial.print("Audio signal: ");
        Serial.println(SIG_SENSED);
        digitalWrite(13, SIG_SENSED);
    }

    lastSignalState = SIG_SENSED;

    return SIG_SENSED;
}

bool lastPowerState = false;

bool isPoweredOn() {
    bool value = digitalRead(CONTROL_IN) == HIGH;

    if (lastPowerState != value) {
        Serial.print("Power: ");
        Serial.println(value ? "ON" : "OFF");
    }

    lastPowerState = value;
    return value;
}

void loop() {

    // 1. Check CONTROL LINE IN from sub woofer, if HIGH speakers are already on, abort.
    isPoweredOn();

    checkSignal();

    digitalWrite(CONTROL_OUT, digitalRead(CONTROL_IN));

    delay(1000);
}
