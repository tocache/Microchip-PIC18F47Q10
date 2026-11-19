#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    TRISEbits.TRISE0 = 0;
    ANSELEbits.ANSELE0 = 0;
}

void main(void) {
    configuro();
    while(1){
        LATEbits.LATE0 = ~PORTEbits.RE0;
        __delay_ms(500);
    }
}
