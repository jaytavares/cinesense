#include <Arduino.h>
#include "User_Setup.h"

// Define pins
#define AUDIO_SENSE_PIN 4
#define CONTROL_IN 5
#define CONTROL_OUT 6
#define IR_LINE

// Define settings
#define AUDIO_THRESHOLD 250  // milliseconds

void setup() {
    pinMode(AUDIO_SENSE_PIN, INPUT);
    pinMode(CONTROL_IN, INPUT);
    pinMode(CONTROL_OUT, LOW);
    pinMode(IR_LINE, INPUT);
    pinMode(13, OUTPUT); // TESTING
}

// TESTING
bool LED_ON = false;

void checkSignal() {
    bool SIG_SENSED = false;

//    1. Check CONTROL LINE IN from sub woofer, if HIGH abort
    if (digitalRead(CONTROL_IN) == LOW) {
        // 2. Float the CONTROL LINE OUT (set to input) or set to 700mv if that doesn’t work
        pinMode(CONTROL_OUT, INPUT);

        unsigned long startTime = millis();
        while (millis() - startTime < AUDIO_THRESHOLD) {
            // 3. Check for signal on audio signal line
            if (digitalRead(AUDIO_SENSE_PIN) == HIGH) {
                SIG_SENSED = true;
            }
        }
        // 4. Reconnect CONTROL LINE
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, LOW);
    }
}

void loop() {
    checkSignal();

    if (LED_ON) // TESTING
        digitalWrite(13, HIGH);
    else
        digitalWrite(13, LOW);

    delay(1000);
}
