#include <Arduino.h>
#include "User_Setup.h"

///*
// * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
// * An IR detector/demodulator must be connected to the input RECV_PIN.
// * Version 0.1 July, 2009
// * Copyright 2009 Ken Shirriff
// * http://arcfn.com
// */
//
//#include <IRremote.h>
//
//int RECV_PIN = 11;
//
//IRrecv irrecv(RECV_PIN);
//
//decode_results results;
//
//void setup()
//{
//  Serial.begin(9600);
//  // In case the interrupt driver crashes on setup, give a clue
//  // to the user what's going on.
//  Serial.println("Enabling IRin");
//  irrecv.enableIRIn(); // Start the receiver
//  Serial.println("Enabled IRin");
//}
//
//void loop() {
//  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
//    irrecv.resume(); // Receive the next value
//  }
//  delay(100);
//}
//
///*
// * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
// * An IR LED must be connected to Arduino PWM pin 3.
// * Version 0.1 July, 2009
// * Copyright 2009 Ken Shirriff
// * http://arcfn.com
// */
//
//
//#include <IRremote.h>
//
//IRsend irsend;


// pulse parameters in usec
#define NEC_HDR_MARK  9000
#define NEC_HDR_SPACE 4500
#define NEC_BIT_MARK  560
#define NEC_ONE_SPACE 1600
#define NEC_ZERO_SPACE  560
#define NEC_RPT_SPACE 2250

#define TOPBIT 0x80000000

const int OutputPin = 13;

void setup()
{
    pinMode(OutputPin, OUTPUT);
    digitalWrite(OutputPin, HIGH);
}

void loop() {
    sendNEC(0x5D0532CD, 32);
    delay(5000);
}

void sendNEC(unsigned long data, int nbits) {
    mark(NEC_HDR_MARK);
    space(NEC_HDR_SPACE);

    for (int i = 0; i < nbits; i++) {
        if (data & TOPBIT) {
            mark(NEC_BIT_MARK);
            space(NEC_ONE_SPACE);
        }
        else {
            mark(NEC_BIT_MARK);
            space(NEC_ZERO_SPACE);
        }
        data <<= 1;
    }
    mark(NEC_BIT_MARK);
    space(0);
}

void mark(int time) {
    digitalWrite(OutputPin, LOW);
    delayMicroseconds(time);
}

void space(int time) {
    digitalWrite(OutputPin, HIGH);
    delayMicroseconds(time);
}