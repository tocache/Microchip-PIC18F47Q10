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
    //conf ADC y FVR
    FVRCON = 0x81;
    ADPCH = 0x00;
    ADREF = 0x03;
    ADCON0 = 0x94;
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Term");
        ENVIA_CHAR(1);
        ESCRIBE_MENSAJE2("metro UPC");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("LM35: ");
        ADCON0bits.GO_nDONE = 1;
        while(ADCON0bits.GO_nDONE == 1);
        LCD_ESCRIBE_VAR_INT(((ADRESH << 8) + ADRESL) / 10, 3);
        ENVIA_CHAR(0xDF);
        ENVIA_CHAR('C');
        __delay_ms(1000);
    }
}
