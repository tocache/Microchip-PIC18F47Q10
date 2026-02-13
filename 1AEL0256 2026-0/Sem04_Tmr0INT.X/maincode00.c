/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 12 de febrero de 2026, 07:36 PM
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
    TRISAbits.TRISA0 = 0;
    ANSELAbits.ANSELA0 = 0;
    //conf del Timer0
    T0CON0 = 0x90;
    T0CON1 = 0xC2;
    //conf las INTS
    PIE0bits.TMR0IE = 1;
    PIR0bits.TMR0IF = 0;
    INTCONbits.GIE = 1;
}

void main(void) {
    configuro();
    while(1){
        
    }
}

void __interrupt() TMR0_ISR(void){
    PIR0bits.TMR0IF = 0;    //bajamos bandera TMR0IF
    if(PORTAbits.RA0 == 1){
        LATAbits.LATA0 = 0;
        //TMR0 = 65448;           //esto no funciona
        TMR0H = 0xFF;
        TMR0L = 0xA8;
    }
    else{
        LATAbits.LATA0 = 1;
        //TMR0 = 65499;           //esto no funciona
        TMR0H = 0xFF;
        TMR0L = 0xDB;
    }
}

