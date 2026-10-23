/*
 * File:   mancode00.c
 * Author: Alumnos
 *
 * Created on 23 de octubre de 2026, 05:32 PM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL
#define ESCRIBIR_FOQUITOS LATD

unsigned char AUTASO[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void configuro(){
    //conf el modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;       //RD como salida
    ANSELD = 0x00;      //RD como digital
    ANSELB = 0xFC;      //RB1 y RB0 como digital
    WPUB = 0x03;         //RB1 y RB0 con weakpullup
}

void TEMPORIZADO(){
    if(PORTBbits.RB0 == 0){
        __delay_ms(60);
    }
    else{
        __delay_ms(300);      
  }
}

void main(void) {
    configuro();
    while(1){
        for(unsigned char z_var=0;z_var<8;z_var++){
            ESCRIBIR_FOQUITOS = AUTASO[z_var];
            TEMPORIZADO();
        }
        for(unsigned char z_var=6;z_var>0;z_var--){
            ESCRIBIR_FOQUITOS = AUTASO[z_var];
            TEMPORIZADO();
        }
    }
}

