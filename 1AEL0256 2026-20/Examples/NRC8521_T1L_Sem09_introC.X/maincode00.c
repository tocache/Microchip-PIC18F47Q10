/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on October 21, 2026, 11:10 AM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf el modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf los puertos de E/S
    TRISD = 0x00;
    ANSELD = 0x00;
    LATD = 0;
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSELB0 = 0;
    WPUBbits.WPUB0 = 1;

}

void main(void) {
    configuro();
    while(1){
        LATDbits.LATD0 = 1;
        __delay_ms(300);
        LATDbits.LATD0 = 0;
        __delay_ms(300);
    }
}
