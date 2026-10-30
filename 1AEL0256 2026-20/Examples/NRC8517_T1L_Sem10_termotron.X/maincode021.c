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

unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
unsigned char simb_admir[]={0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x00};


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
    GENERACARACTER(i_acento,0);
    GENERACARACTER(simb_admir,1);
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ENVIA_CHAR(1);
        ESCRIBE_MENSAJE2("Hola mundo!");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("UPC Ingenier");
        ENVIA_CHAR(0);
        ENVIA_CHAR('a');
        __delay_ms(1000);
    }
}
