/*
 * File:   maincode00.c
 * Author: Kalun Lau
 * Programa para hacer pruebas con el LCD y
 * el LM35, configurando el ADC y el FVR.
 * Created on 29 de octubre de 2026, 12:13 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char admiracion[]={0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x00};
unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};

void configuro(void){
    //conf modulo de osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf E/S
    ANSELB = 0xFC;
    WPUB = 0x03;
    //inicialicaion del LCD
    LCD_INIT();
    GENERACARACTER(admiracion,0);
    GENERACARACTER(i_acento,1);
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ENVIA_CHAR(0);
        ESCRIBE_MENSAJE2("Hola mundo!");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Ingenier");
        ENVIA_CHAR(1);
        ENVIA_CHAR('a');
        __delay_ms(1000);
    }
}
