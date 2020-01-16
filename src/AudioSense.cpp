#include <Arduino.h>

// Uncomment to enable serial debugging
//#define DEBUG

// Define pins
#define AUDIO_SENSE_PIN 0
#define CONTROL_IN A2
#define CONTROL_OUT 2
#define IR_LINE 3

// Define settings

// Automatic speaker power trigger times
#define ON_TRIGGER_TIME 300  // milliseconds; Signal must be present for this long before speakers are powered on

// Audio signal must be LOW for this amount of time to be considered absent
#define SIGNAL_THRESHOLD 100  // milliseconds

void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    pinMode(13, OUTPUT);
#endif
//    pinMode(AUDIO_SENSE_PIN, INPUT);
    pinMode(CONTROL_IN, INPUT);
    pinMode(CONTROL_OUT, OUTPUT);
//    pinMode(IR_LINE, OUTPUT);
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
#ifdef DEBUG
    Serial.println("Sending IR");
#endif
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
    delay(1000);
}

/*
 * END IR STUFF
 */

bool powerOn() {
    return analogRead(CONTROL_IN) > 100;
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
        if (digitalRead(AUDIO_SENSE_PIN) == HIGH) {
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

bool ledOn() {
    return analogRead(CONTROL_IN) > 700;
}

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

//    if (lastPowerState != poweredOn) {
//#ifdef DEBUG
//        Serial.print("Power: ");
//        Serial.println(poweredOn ? "ON" : "OFF");
//#endif
//        stateStartTime = millis();
//    }
//    lastPowerState = poweredOn;
//
//    bool signalPresent = checkSignal();
//    if (lastSignalState != signalPresent) {
//#ifdef DEBUG
//        Serial.print("Audio signal: ");
//        Serial.println(signalPresent);
//        digitalWrite(13, signalPresent);
//#endif
//        stateStartTime = millis();
//    }
//    lastSignalState = signalPresent;
//
//    unsigned long stateTimer = millis() - stateStartTime;
//
//    // Auto power on
//    if (signalPresent && !poweredOn){
//#ifdef DEBUG
//        Serial.print("ON Delay: ");
//        Serial.println(stateTimer);
//#endif
//        if (stateTimer >= ON_TRIGGER_TIME){
//            sendNEC(0x5D0532CD);
//        }
//    }
}
