/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 30 de octubre de 2026, 09:55 AM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char admiracion[]={0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x00};

void configuro(void){
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    ANSELB = 0xFC;
    WPUB = 0x03;
    LCD_INIT();
    GENERACARACTER(admiracion,0);
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ENVIA_CHAR(0);      //caracter personalizado 0
        ESCRIBE_MENSAJE2("Hola mundo!");
    }
}
