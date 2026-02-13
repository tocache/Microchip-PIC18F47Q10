/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 13 de febrero de 2026, 05:50 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf de las E/S
    //conf del PPS
    //conf de las INTs
    //conf de datos iniciales
    LCD_INIT();
}

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Hola verano");
    while(1){
        
    }
}
