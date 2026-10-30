/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on 30 de octubre de 2026, 03:56 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char admiracion[]={0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x00};
unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
unsigned char o_acento[]={0x02,0x04,0x00,0x0E,0x11,0x11,0x0E,0x00};
unsigned char heart_0[]={0x00,0x0A,0x15,0x11,0x11,0x0A,0x04,0x00};
unsigned char heart_1[]={0x00,0x00,0x0A,0x0E,0x0E,0x04,0x00,0x00};
unsigned char heart_2[]={0x00,0x00,0x00,0x04,0x04,0x00,0x00,0x00};
unsigned char heart_3[]={0x00,0x0A,0x1F,0x1B,0x1B,0x0E,0x04,0x00};

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf las E/S
    ANSELB = 0xFC;
    WPUB = 0x03;
    //inicializar el LCD
    LCD_INIT();
    GENERACARACTER(admiracion,0);
    GENERACARACTER(i_acento,1);
    GENERACARACTER(o_acento,2);
    GENERACARACTER(heart_0,3);
    GENERACARACTER(heart_1,4);
    GENERACARACTER(heart_2,5);
    GENERACARACTER(heart_3,6);
    //conf el ADC y el FVR
    ADCON0 = 0x94;
    ADPCH = 0x00;
    ADREF = 0x03;
    FVRCON = 0x81;
}

unsigned int una_muestrita_pe_ADC(void){
    //tomar una muestra
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE == 1);
    return (ADRESH << 8) + ADRESL;
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);        //primera linea
        ESCRIBE_MENSAJE2(" Termotr");
        ENVIA_CHAR(2);
        ENVIA_CHAR('n');
        unsigned int resultado = una_muestrita_pe_ADC()/10;
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Sn. LM35: ");
        LCD_ESCRIBE_VAR_INT(resultado,3);
        LCD_CHAR_GRADO();
        ENVIA_CHAR('C');
        for(unsigned char x_var=0;x_var<4;x_var++){
            POS_CURSOR(2,15);
            ENVIA_CHAR(x_var+3);
            __delay_ms(200);
        }
    }
}
