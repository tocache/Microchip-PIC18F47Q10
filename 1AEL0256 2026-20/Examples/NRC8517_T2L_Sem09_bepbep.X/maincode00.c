/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 22 de octubre de 2026, 01:20 PM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf el modulo de osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;
    ANSELD = 0x00;
    ANSELB = 0xFC;
    WPUB = 0x03;
}

void main(void) {
    configuro();
    while(1){
        LATD = 0xFF;
        __delay_ms(300);
        LATD = 0x00;
        __delay_ms(300);
    }
}
