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
void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Summer 2026-0");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("DAC = 2.5V");    
    while(1){
        
    }
}
