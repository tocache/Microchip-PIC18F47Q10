/*
 * File:   mancode00.c
 * Author: Alumnos
 *
 * Created on 23 de octubre de 2026, 05:32 PM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(){
    //conf el modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;       //RD como salida
    ANSELD = 0x00;      //RD como digital
    ANSELB = 0xFC;      //RB1 y RB0 como digital
    WPUB = 0x03;         //RB1 y RB0 con weakpullup
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
