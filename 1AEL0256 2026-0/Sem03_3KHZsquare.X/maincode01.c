/*
 * File:   maincode01.c
 * Author: klnla
 *
 * Created on 6 de febrero de 2026, 07:36 PM
 */

#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //configuramos el modulo de oscilador
    OSCCON1 = 0x60;     //HFINTOSC seleccionado, POSTS 1:1
    OSCFRQ = 0x02;      //HFINTOSC a 4MHz
    OSCEN = 0x40;       //HFINTOSC enabled
    //configuramos las E/S
    TRISDbits.TRISD0 = 0;       //RD0 como salida
    ANSELDbits.ANSELD0 = 0;     //RD0 como digital
    //configuramos el Timer0
    T0CON0 = 0x80;      //Timer0 enabled, 8bit, POSTS 1:1
    T0CON1 = 0x62;      //fuente HFINTOSC, PRESC 1:4, SYNC on
    TMR0H = 166;        //valor de ref de comparacion 166
    PIR0bits.TMR0IF = 0; //bajamos inicialmente la bandera del Timer0
}

void main(void) {
    configuro();
    while(1){
        while(PIR0bits.TMR0IF == 0);
        PIR0bits.TMR0IF = 0;
        LATDbits.LATD0 = !LATDbits.LATD0;
    }
}
