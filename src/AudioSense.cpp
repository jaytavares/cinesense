#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\Arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\include\stdlib.h"
#include "..\include\Pins.h"
#include "..\include\IRDirect.h"

// Uncomment to enable serial debugging
//#define DEBUG

// Define settings
// Automatic speaker power trigger times
#define ON_TRIGGER_TIME 2000  // milliseconds; Signal must be present for this long before speakers are powered on
#define OFF_TRIGGER_TIME 1000 * 60 * 60 // seconds; Signal must be absent for this long before speakers are powered off

// Audio signal must be LOW for this amount of time to be considered absent
#define SIGNAL_THRESHOLD 100  // milliseconds
#define __AVR_ATtiny85__ 1


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
        if (analogRead(AUDIO_SENSE_PIN) > 5) {
            // A signal was sensed for longer than the threshold setting
            SIG_SENSED = true;
        }
        delay(25);
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
unsigned long stateTimer = 0;
int lc=0;
    
void blinkLED() {
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, LOW);  // Blink Lights  LOW = LED ON, HIGH = LED OFF
        delay(100);
        digitalWrite(CONTROL_OUT, HIGH);                   
        delay(100);
}

void loop() {
    
    stateTimer = millis() - stateStartTime;

    if (ledOn()) {
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, HIGH);

    } else
        pinMode(CONTROL_OUT, INPUT);



    bool poweredOn = powerOn();
    /* if (!poweredOn){
        pinMode(CONTROL_OUT, OUTPUT);
        digitalWrite(CONTROL_OUT, LOW);  // Blink Lights  LOW = LED ON, HIGH = LED OFF

        //sendNEC(0x5D0532CD);  // Power On.        
        sendNEC(0x5DD232CD);  // Power On.     
        blinkLED();
        blinkLED();
        
        //sendNEC(0x5D0531CE);  // Assert On
        //sendNEC(0x5D05BA45); // auto wake toggle.



        stateStartTime = millis();
    } 
*/

/*
    if (poweredOn && stateStartTime == 0) {
#ifdef DEBUG
        Serial.print("Power: ");
        Serial.println(poweredOn ? "ON" : "OFF");
#endif
        stateStartTime = millis();
    } 
*/


 //   lastPowerState = poweredOn;


    bool signalPresent = checkSignal();
 /*   if (lastSignalState != signalPresent) {
#ifdef DEBUG
        Serial.print("Audio signal: ");
        Serial.println(signalPresent);
#endif
        stateStartTime = millis();
    }
    lastSignalState = signalPresent;
*/
    delay(1000);  

    

    // Auto power on
//    if (!poweredOn){
//#ifdef DEBUG
//        Serial.print("ON Delay: ");
//        Serial.println(stateTimer);
//#endif
//        if (stateTimer >= ON_TRIGGER_TIME){
//            sendNEC(0x5D0532CD);  // Power On.
//            stateStartTime = millis();
//        }
//    }

   if(signalPresent) {
        if(!poweredOn) {
            //if (stateTimer >= ON_TRIGGER_TIME){
                // sendNEC(0x5D0532CD);  // Power On. Cinemate II
                sendNEC(0x5DD232CD);  // Power On.     Cinemate Original
                stateStartTime = millis();
                blinkLED();                
                blinkLED();

            //}

        }
   }
    else {
        if(lc%10==0) {
          blinkLED();
          lc=0;
        }
    }

    // loop counter
    lc++;
 

    // Auto power off
    if (!signalPresent && poweredOn){
#ifdef DEBUG
        Serial.print("OFF Delay: ");
        Serial.println(stateTimer);
#endif
        if (stateTimer >= (unsigned long) OFF_TRIGGER_TIME){
            sendNEC(0x5D0532CD);
            stateStartTime = millis();
        } 
    }  
    
}

