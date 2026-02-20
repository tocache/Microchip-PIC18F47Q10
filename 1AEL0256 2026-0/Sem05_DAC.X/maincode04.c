/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 19 de febrero de 2026, 05:59 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

unsigned char cuad_0[]={0x80,0x14,0x15,0x19,0x01,0x01,0x01,0x02};
unsigned char cuad_1[]={0x0E,0x14,0x04,0x14,0x0C,0x04,0x14,0x0C};
unsigned char cuad_2[]={0x03,0x02,0x03,0x03,0x02,0x05,0x0F,0x10};
unsigned char cuad_3[]={0x08,0x18,0x08,0x08,0x12,0x05,0x05,0x02};

void configuro(void){
    //modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;       //HFINTOSC a 32MHz
    //conf las E/S
    TRISAbits.TRISA0 = 1;   //RA0 entrada
    ANSELAbits.ANSELA0 = 1; //RA0 analogico
    //conf del ADC
    ADCON0bits.ADFM = 1;    //just a la derecha
    ADCON0bits.ADCS = 1;
    ADPCH = 0x00;           //canal seleccionado RA0
    ADCON0bits.ADON = 1;    //encender el ADC    
    //conf del DAC
    DAC1CON0 = 0x90;        //DAC enabled, DAC1OUT2 enabled
    DAC1CON1 = 0x10;     //DAC a 2.5V
    //inicializacion de librerias
    LCD_INIT();
    GENERACARACTER(cuad_0, 0);
    GENERACARACTER(cuad_1, 1);
    GENERACARACTER(cuad_2, 2);
    GENERACARACTER(cuad_3, 3);
}

unsigned int tomamuestra_ADC(void){
    ADCON0bits.GO_nDONE = 1;        //toma muestra
    while(ADCON0bits.GO_nDONE == 1);    //espera a que termine
    return ((ADRESH << 8) + ADRESL);
}

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Summer 2026-0");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("DAC = ADC");
    POS_CURSOR(1,14);
    ENVIA_CHAR(0);
    ENVIA_CHAR(1);
    POS_CURSOR(2,14);  
    ENVIA_CHAR(2);
    ENVIA_CHAR(3);    
    while(1){
        DAC1CON1 = tomamuestra_ADC() >> 5;
        __delay_ms(200);
    }
}
