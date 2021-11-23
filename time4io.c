#include <stdint.h>
#include <pic32mx.h> 
#include "mipslab.h"

int getsw (void) {
    return (PORTD >> 8) & 0xf; // least 4 significant bits, we shiften the bits to begin with to be able to gather the data from switches SW4, SW3, SW2 and SW1

}

int getbtns (void) {
    return (((PORTD >> 5) & 0x7) << 1) | ((PORTF >> 1) & 0x1); //What we are doing here is that we are returning the 4 BTNs that the chipKit has. BTN2-4 can be found in port D, (bits 5,6,7). However, BTN1 is found in port F, (bit 1) which means that we need to take the first bit from port F.
                                                               //Furthermore, as we continue, we shift the bits from port D one step to the right, so we can include btn 1 as we return the bits.

}

