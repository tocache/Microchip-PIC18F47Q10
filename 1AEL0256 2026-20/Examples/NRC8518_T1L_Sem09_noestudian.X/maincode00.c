/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 23 de octubre de 2026, 11:14 AM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;   //RD como salidas
    ANSELD = 0x00;  //RD como digitales
    ANSELB = 0xFC;  //RB1 y RB0 como digitales
    WPUB = 0x03;    //RB1 y RB0 con pullup enabled
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
