/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 13 de febrero de 2026, 05:50 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

unsigned int cuenta = 0;

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;
    //conf de las E/S
    //conf del PPS
    //conf de las INTs
    //conf de datos iniciales
    LCD_INIT();
}

void main(void) {
    configuro();
    POS_CURSOR(1,1);
    ESCRIBE_MENSAJE2("Hola veranon");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("Microbios 2026");
    __delay_ms(3000);
    BORRAR_LCD();
    while(1){
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Cuenta:");
        LCD_ESCRIBE_VAR_INT(cuenta, 5);
        __delay_ms(50);
        cuenta = cuenta + 1;
    }
}
