/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 22 de octubre de 2026, 09:20 AM
 */
#include <xc.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL
#define FOQUITOS LATD

unsigned char AUTO_FANTASTICO[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char AUTO_FANTASTICO2[]={0x81,0x42,0x24,0x18};

void configuro(void){
    //conf mod osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    TRISD = 0x00;   //RD salidas
    ANSELD = 0x00;  //RD digitales
    ANSELB = 0xFC;  //RB(2:1) digitales
    WPUB = 0x03;    //RB(1:0) weakpullup enabled
}

void main(void) {
    configuro();
    while(1){
      if(PORTBbits.RB1 == 0){
        for(unsigned z_var=0;z_var<4;z_var++){
          LATD = AUTO_FANTASTICO2[z_var];
          if(PORTBbits.RB0 == 0){
            __delay_ms(80);
          }
          else{
            __delay_ms(300);
          }
        }
        for(unsigned z_var=0;z_var<2;z_var++){
          LATD = AUTO_FANTASTICO2[2-z_var];
          if(PORTBbits.RB0 == 0){
            __delay_ms(80);
          }
          else{
            __delay_ms(300);
          }
        }  
      }
      else{
        for(unsigned z_var=0;z_var<8;z_var++){
          LATD = AUTO_FANTASTICO[z_var];
          if(PORTBbits.RB0 == 0){
            __delay_ms(80);
          }
          else{
            __delay_ms(300);
          }
        }
        for(unsigned z_var=0;z_var<6;z_var++){
          LATD = AUTO_FANTASTICO[6-z_var];
          if(PORTBbits.RB0 == 0){
            __delay_ms(80);
          }
          else{
            __delay_ms(300);
          }
        }  
      }
    }
}
