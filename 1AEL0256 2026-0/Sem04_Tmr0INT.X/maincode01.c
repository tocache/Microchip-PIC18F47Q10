/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 12 de febrero de 2026, 07:36 PM
 */

#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

unsigned char diuty = 0;

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISAbits.TRISA0 = 0;
    ANSELAbits.ANSELA0 = 0;
    TRISBbits.TRISB6 = 1;
    ANSELBbits.ANSELB6 = 0;
    WPUBbits.WPUB6 = 1;
    //conf del PPS
    INT0PPS = 0x0E;
    //conf del Timer0
    T0CON0 = 0x90;
    T0CON1 = 0xC2;
    //conf las INTS
    PIE0bits.TMR0IE = 1;
    PIR0bits.TMR0IF = 0;
    PIE0bits.INT0IE = 1;
    PIR0bits.INT0IF = 0;
    INTCONbits.INT0EDG = 0; //int0 falling edge
    IPR0bits.INT0IP = 0;    //int0 a low priority
    INTCONbits.IPEN = 1;    //priority inabled
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

void main(void) {
    configuro();
    while(1){
        
    }
}

void __interrupt(high_priority) TMR0_ISR(void){
    PIR0bits.TMR0IF = 0;    //bajamos bandera TMR0IF
    if(PORTAbits.RA0 == 1){
        LATAbits.LATA0 = 0;
        if(diuty == 0){
            TMR0H = 0xFF;
            TMR0L = 0xA8;
        }
        else{
            TMR0H = 0xFF;
            TMR0L = 0xDB;
        }
    }
    else{
        LATAbits.LATA0 = 1;
        if(diuty == 0){
            TMR0H = 0xFF;
            TMR0L = 0xDB;
        }
        else{
            TMR0H = 0xFF;
            TMR0L = 0xA8;
        }
    }
}

void __interrupt(low_priority) INT0_ISR(void){
    PIR0bits.INT0IF = 0;
    if(diuty == 0){
        diuty = 1;
    }
    else{
        diuty = 0;
    }
}

