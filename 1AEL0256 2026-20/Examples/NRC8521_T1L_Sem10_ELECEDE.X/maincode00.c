/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on October 28, 2026, 10:13 AM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
unsigned char o_acento[]={0x02,0x04,0x00,0x0E,0x11,0x11,0x0E,0x00};

void configuro(void){
    //conf mod osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf E/S
    ANSELB = 0xFC;  //RB1 y RB0 digitales
    WPUB = 0x03;    //RB1 y RB0 weak pullup enabled
    //inicializar el LCD
    LCD_INIT();
    GENERACARACTER(i_acento,0);
    GENERACARACTER(o_acento,1);
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("UPC Ingenier");
        ENVIA_CHAR(0);
        ENVIA_CHAR('a');
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Mecatr");
        ENVIA_CHAR(1);
        ESCRIBE_MENSAJE2("nica");
        __delay_ms(2000);
    }
}
