/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 20 de febrero de 2026, 06:01 PM
 */

#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

void configuro(void){
    //conf modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;
    //inicializacion del LCD
    LCD_INIT();
}

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Semana 5-2 20260");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("Hot Summer UPC");
    while(1){
        
    }
}
