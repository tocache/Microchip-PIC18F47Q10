/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 22 de octubre de 2026, 01:20 PM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

unsigned char AUTO_FANT1[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char AUTO_FANT2[]={0x07,0x0E,0x1C,0x38,0x70,0xE0};
unsigned char AUTO_FANT3[]={0x1F,0x3E,0x7C,0xF8};
unsigned char AUTO_FANT4[]={0xC3,0x66,0x3C};
unsigned char entrada;

void configuro(void){
    //conf el modulo de osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;
    ANSELD = 0x00;
    ANSELB = 0xFC;
    WPUB = 0x03;
}

void main(void) {
    configuro();
    while(1){
        entrada = PORTB & 0x03;
          switch(entrada){
            case 0:
                for(unsigned char z_var=0;z_var<8;z_var++){
                    LATD = AUTO_FANT1[z_var];
                    __delay_ms(300);
                }
                for(unsigned char z_var=0;z_var<6;z_var++){
                    LATD = AUTO_FANT1[6-z_var];
                    __delay_ms(300);
                }
                break;
            case 1:
                for(unsigned char z_var=0;z_var<6;z_var++){
                    LATD = AUTO_FANT2[z_var];
                    __delay_ms(300);
                }
                for(unsigned char z_var=0;z_var<4;z_var++){
                    LATD = AUTO_FANT2[4-z_var];
                    __delay_ms(300);
                }
                break;
            case 2:
                for(unsigned char z_var=0;z_var<4;z_var++){
                    LATD = AUTO_FANT3[z_var];
                    __delay_ms(300);
                }
                for(unsigned char z_var=0;z_var<2;z_var++){
                    LATD = AUTO_FANT3[2-z_var];
                    __delay_ms(300);
                }
                break;
            case 3:
                for(unsigned char z_var=0;z_var<3;z_var++){
                    LATD = AUTO_FANT4[z_var];
                    __delay_ms(300);
                }
                for(unsigned char z_var=0;z_var<1;z_var++){
                    LATD = AUTO_FANT4[1-z_var];
                    __delay_ms(300);
                }
                break;
          }
    }
}
