#include "..\include\Pins.h"
#include <Arduino.h>

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
    // delay(1000); // removed jk. in code.
}
