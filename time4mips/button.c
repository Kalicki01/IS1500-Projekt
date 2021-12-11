#include <stdint.h>
#include <pic32mx.h>
#include "header.h"

int getsw (void) {
    return (PORTD >> 8) & 0xf; // least 4 significant bits, we shiften the bits to begin with to be able to gather the data from switches SW4, SW3, SW2 and SW1
}

int getbtns (void) {
    return ((((PORTD  >> 5) & 0x7) << 1) | ((PORTF >> 1) & 0x1)); // We take the 3 least significant bits from PORT D, to gather the data from buttons BTN4, BTN3 and BTN2. We then bit shift 1 step to the left, so we can add the corresponding bit from button 1 (from PORT F)
}

