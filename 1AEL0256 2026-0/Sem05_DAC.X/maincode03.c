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
    while(1){
        DAC1CON1 = tomamuestra_ADC() >> 5;
        __delay_ms(200);
    }
}
