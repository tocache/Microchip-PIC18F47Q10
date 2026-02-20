/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 19 de febrero de 2026, 05:59 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

void configuro(void){
    //modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;       //HFINTOSC a 32MHz
    //conf las E/S
    //TRISBbits.TRISB7 = 0;   //RB7 como salida
    //ANSELBbits.ANSELB7 = 1; //RB7 como analogica
    //conf del DAC
    DAC1CON0 = 0x90;        //DAC enabled, DAC1OUT2 enabled
    DAC1CON1 = 0x10;     //DAC a 2.5V
    //inicializacion de librerias
    LCD_INIT();
}

unsigned char seno[]={16, 19, 22, 25, 27, 29, 30, 31, 31, 31, 30, 29, 27, 25, 22, 19, 16, 13, 10, 7, 5, 3, 2, 1, 0, 0, 0, 1, 2, 3, 5, 7, 10, 13};

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Summer 2026-0");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("DAC = sine");    
    while(1){
        for(unsigned char x_var=0;x_var<32;x_var++){
            DAC1CON1 = seno[x_var];
            __delay_us(30000);
        }
    }
}
