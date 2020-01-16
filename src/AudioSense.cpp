#include <Arduino.h>
#include "Pins.h"
#include "IRDirect.h"

// Uncomment to enable serial debugging
//#define DEBUG

// Define settings
// Automatic speaker power trigger times
#define ON_TRIGGER_TIME 300  // milliseconds; Signal must be present for this long before speakers are powered on

// Audio signal must be LOW for this amount of time to be considered absent
#define SIGNAL_THRESHOLD 100  // milliseconds

void setup() {
#ifdef DEBUG
    Serial.begin(115200);
#endif
    pinMode(AUDIO_SENSE_PIN, INPUT);
    pinMode(CONTROL_IN, INPUT);
    pinMode(CONTROL_OUT, OUTPUT);
    pinMode(IR_LINE, INPUT);
}

bool powerOn() {
    return analogRead(CONTROL_IN) > 50;
}

bool ledOn() {
    return analogRead(CONTROL_IN) > 700;
}

bool checkSignal() {
    bool SIG_SENSED = false;

    // Find out if speaker is off
    bool POWER_OFF = !powerOn();

    // If speaker is off, float the CONTROL_OUT line.
    // This will turn on the optical input but won't turn on the power LED.
    if (POWER_OFF)
        pinMode(CONTROL_OUT, INPUT);

    // Check for audio signal
    unsigned long startTime = millis();
    while (millis() - startTime < SIGNAL_THRESHOLD) {
        if (analogRead(AUDIO_SENSE_PIN) > 100) {
            // A signal was sensed for longer than the threshold setting
            SIG_SENSED = true;
        }
    }

    if (POWER_OFF){
        // Reconnect CONTROL LINE
        pinMode(CONTROL_OUT, OUTPUT);
    }

    return SIG_SENSED;
}

bool lastPowerState = false;
bool lastSignalState = false;

unsigned long stateStartTime = 0;

void loop() {

    if (ledOn()) {
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, HIGH);
    } else
        pinMode(CONTROL_OUT, INPUT);

    bool poweredOn = powerOn();
    if (!poweredOn){
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, LOW);
    }

    if (lastPowerState != poweredOn) {
#ifdef DEBUG
        Serial.print("Power: ");
        Serial.println(poweredOn ? "ON" : "OFF");
#endif
        stateStartTime = millis();
    }
    lastPowerState = poweredOn;

    bool signalPresent = checkSignal();
    if (lastSignalState != signalPresent) {
#ifdef DEBUG
        Serial.print("Audio signal: ");
        Serial.println(signalPresent);
#endif
        stateStartTime = millis();
    }
    lastSignalState = signalPresent;

    unsigned long stateTimer = millis() - stateStartTime;

    // Auto power on
    if (signalPresent && !poweredOn){
#ifdef DEBUG
        Serial.print("ON Delay: ");
        Serial.println(stateTimer);
#endif
        if (stateTimer >= ON_TRIGGER_TIME){
            sendNEC(0x5D0532CD);
        }
    }
}
