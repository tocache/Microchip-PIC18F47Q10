/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 22 de octubre de 2026, 01:20 PM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

unsigned char deco_7s[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
unsigned char primos[]={2,3,5,7,11,13,17,19,23,29,31};
unsigned char decena, unidad;

void configuro(void){
    //conf el modulo de osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;
    ANSELD = 0x00;
    TRISC = 0x00;
    ANSELC = 0x00;
}

void main(void) {
    configuro();
    while(1){
        for(unsigned char x_var=0;x_var<11;x_var++){
            decena = primos[x_var] / 10;
            unidad = primos[x_var] % 10;
            LATC = deco_7s[decena];
            LATD = deco_7s[unidad];
            __delay_ms(1000);
        }
    }
}
