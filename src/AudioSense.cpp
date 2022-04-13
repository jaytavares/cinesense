#include <Arduino.h>
#include "Pins.h"
#include "IRDirect.h"

// Uncomment to enable serial debugging
//#define DEBUG

// Define settings
// Automatic speaker power trigger times
#define ON_TRIGGER_TIME 300  // milliseconds; Signal must be present for this long before speakers are powered on
#define OFF_TRIGGER_TIME 1000 * 10 // seconds; Signal must be absent for this long before speakers are powered off

// Audio signal must be LOW for this amount of time to be considered absent
#define SIGNAL_THRESHOLD 100  // milliseconds

// IR power on/off command
#define POWER_COMMAND 0x5D0532CD

// Helper functions >>
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

// Initial power up
void setup() {
#ifdef DEBUG
    Serial.begin(115200);
#endif
    pinMode(AUDIO_SENSE_PIN, INPUT);
    pinMode(CONTROL_OUT, OUTPUT);
    pinMode(IR_LINE, INPUT);
    pinMode(CONTROL_IN, OUTPUT);

    // Start-Up routine >>>

    // CONTROL_IN must be held low for at least 4 seconds on power up in order to avoid locking out the speakers (see https://github.com/jaytavares/cinesense/issues/1)
    digitalWrite(CONTROL_IN, LOW);
    delay(4000);
    pinMode(CONTROL_IN, INPUT);

    // Send power command for faster initial start up.
    sendNEC(POWER_COMMAND);
}

bool lastPowerState = false;
bool lastSignalState = false;

unsigned long stateStartTime = 0;

// Main loop
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
            sendNEC(POWER_COMMAND);
        }
    }

    // Auto power off
    if (!signalPresent && poweredOn){
#ifdef DEBUG
        Serial.print("OFF Delay: ");
        Serial.println(stateTimer);
#endif
        if (stateTimer >= OFF_TRIGGER_TIME){
            sendNEC(POWER_COMMAND);
        }
    }
}
