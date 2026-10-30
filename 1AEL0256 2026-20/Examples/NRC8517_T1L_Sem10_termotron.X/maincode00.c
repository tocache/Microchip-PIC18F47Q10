/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 29 de octubre de 2026, 07:58 AM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf modulo oscilador
    OSCCON1 = 0x60; //NOSC=HFINTOSC,NDIV=1:1
    OSCFRQ = 0x02;  //HFINTOSC a 4MHz
    OSCEN = 0x40;   //HFINTOSC enabled
    //conf las E/S
    /*RA0 ya es entrada analogica*/
    /*RD lo configura la librería LCD*/
    ANSELB = 0xFC;  //RB(0:1) digitales
    WPUB = 0x03;    //RB(1:0) weak pullup enabled
    //inicializacion del LCD
    LCD_INIT();
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Hola mundo!");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("UPC Ingenieria");
        __delay_ms(1000);
    }
}
