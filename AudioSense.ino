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

/*
 * IR STUFF -- Move to separate file
 */

// pulse parameters in usec
#define NEC_HDR_MARK  9000
#define NEC_HDR_SPACE 4500
#define NEC_BIT_MARK  560
#define NEC_ONE_SPACE 1600
#define NEC_ZERO_SPACE  560

#define TOPBIT 0x80000000

void mark(int time) {
    digitalWrite(IR_LINE, LOW);
    delayMicroseconds(time);
}

void space(int time) {
    digitalWrite(IR_LINE, HIGH);
    delayMicroseconds(time);
}

void sendNEC(unsigned long data) {
    pinMode(IR_LINE, OUTPUT);
    digitalWrite(IR_LINE, HIGH);
    delay(100);

    mark(NEC_HDR_MARK);
    space(NEC_HDR_SPACE);

    for (int i = 0; i < 32; i++) {
        if (data & TOPBIT) {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        }
        else {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        data <<= (uint8_t) 1;
    }
    mark(NEC_BIT_MARK);
    space(0);

    pinMode(IR_LINE, INPUT);
}

/*
 * END IR STUFF
 */

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


void loop() {

    // 1. Check CONTROL LINE IN from sub woofer, if HIGH speakers are already on, abort.
    isPoweredOn();

    if (checkSignal() && !isPoweredOn())
        sendNEC(0x5D0532CD);

    // Match LED to system state
    // TODO: Handle CONTROL_IN floating state (used to flash LED when remote is used and when speakers are muted)
    digitalWrite(CONTROL_OUT, digitalRead(CONTROL_IN));

    delay(100);
}
