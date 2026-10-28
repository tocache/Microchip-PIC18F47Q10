/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on October 21, 2026, 11:10 AM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf el modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf los puertos de E/S
    TRISD = 0x00;
    ANSELD = 0x00;
    LATD = 0;
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSELB0 = 0;
    WPUBbits.WPUB0 = 1;

}

const unsigned char automon[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void main(void) {
    configuro();
    while(1){
        for(unsigned char x_var=0;x_var<8;x_var++){
            LATD = automon[x_var];
            if(PORTBbits.RB0 == 0){
                __delay_ms(50);
            }
            else{
                __delay_ms(300); 
            }
        }
        for(unsigned char x_var=0;x_var<6;x_var++){
            LATD = automon[6-x_var];
            if(PORTBbits.RB0 == 0){
                __delay_ms(50);
            }
            else{
                __delay_ms(300);
            }
        }
    }
}
