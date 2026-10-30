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
#define celsius 0
#define farenheit 1

unsigned char i_acento[]={0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
unsigned char o_acento[]={0x02,0x04,0x00,0x0E,0x11,0x11,0x0E,0x00};
unsigned char simb_admir[]={0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x00};
unsigned char escala = celsius;

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
    //conf ADC y FVR
    ADCON0 = 0x94;
    ADPCH = 0x00;
    ADREF = 0x03;
    FVRCON = 0x81;
    //inicializacion del LCD
    LCD_INIT();
    GENERACARACTER(i_acento,0);
    GENERACARACTER(simb_admir,1);
    GENERACARACTER(o_acento,2);
    //conf interrupciones
    INTCONbits.INT0EDG = 0;
    PIE0bits.INT0IE = 1;
    INTCONbits.GIE = 1;
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Term");
        ENVIA_CHAR(2);
        ESCRIBE_MENSAJE2("metro");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Sens: ");
        ADCON0bits.GO_nDONE = 1;
        while(ADCON0bits.GO_nDONE == 1);
        if (escala == celsius){
            LCD_ESCRIBE_VAR_INT(((ADRESH << 8) + ADRESL)/10, 2);
            ENVIA_CHAR(0xDF);
            ENVIA_CHAR('C');
        }
        else{
            float papanatas = (((ADRESH << 8) + ADRESL)/10 * 1.8) + 32;
            unsigned int previo = papanatas;
            LCD_ESCRIBE_VAR_INT(previo, 2);
            ENVIA_CHAR(0xDF);
            ENVIA_CHAR('F');
        }
        //__delay_ms(1000);
    }
}

void __interrupt() INT0_ISR(void){
    PIR0bits.INT0IF = 0;
    if(escala == celsius){
        escala = farenheit;
    }
    else{
        escala = celsius;
    }
}