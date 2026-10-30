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
unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
unsigned char heart_1[]={0x00,0x0A,0x15,0x11,0x11,0x0A,0x04,0x00};
unsigned char heart_2[]={0x00,0x00,0x0A,0x0E,0x0E,0x04,0x00,0x00};
unsigned char heart_3[]={0x00,0x00,0x00,0x04,0x04,0x00,0x00,0x00};


void configuro(void){
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    ANSELB = 0xFC;
    WPUB = 0x03;
    LCD_INIT();
    GENERACARACTER(admiracion,0);
    GENERACARACTER(i_acento,1);
    GENERACARACTER(heart_1,2);
    GENERACARACTER(heart_2,3);
    GENERACARACTER(heart_3,4);
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ENVIA_CHAR(0);      //caracter personalizado 0
        ESCRIBE_MENSAJE2("Hola mundo!");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("UPC Ingenier");
        ENVIA_CHAR(1);
        ENVIA_CHAR('a');
        for(unsigned char x_var=0;x_var<3;x_var++){
            POS_CURSOR(1,15);
            ENVIA_CHAR(x_var+2);
            __delay_ms(200);
        }
        POS_CURSOR(1,15);
        ENVIA_CHAR(3);
        __delay_ms(200);
    }
}
