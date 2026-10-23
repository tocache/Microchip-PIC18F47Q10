/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 23 de octubre de 2026, 11:14 AM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL
#define FOQUITOS LATD

unsigned char AUTO_FANTASTICO[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;   //RD como salidas
    ANSELD = 0x00;  //RD como digitales
    ANSELB = 0xFC;  //RB1 y RB0 como digitales
    WPUB = 0x03;    //RB1 y RB0 con pullup enabled
}

void main(void) {
    configuro();
    while(1){
        for(unsigned char z_var=0;z_var<8;z_var++){
            FOQUITOS = AUTO_FANTASTICO[z_var];
            if(PORTBbits.RB0 == 0){
                __delay_ms(60);
            }
            else{
                __delay_ms(300);
            }
        }
        for(unsigned char z_var=0;z_var<6;z_var++){
            FOQUITOS = AUTO_FANTASTICO[6-z_var];
            if(PORTBbits.RB0 == 0){
                __delay_ms(60);
            }
            else{
                __delay_ms(300);
            }
        }
    }
}
