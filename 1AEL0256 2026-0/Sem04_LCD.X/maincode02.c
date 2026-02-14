/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 13 de febrero de 2026, 05:50 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

unsigned int cuenta = 0;

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;
    //conf de las E/S
    TRISAbits.TRISA0 = 1;   //RA0 entrada
    ANSELAbits.ANSELA0 = 1; //RA0 analogico
    TRISCbits.TRISC5 = 0;   //RC5 salida
    ANSELCbits.ANSELC5 = 0; //RC5 digital
    //conf del PPS
    //conf de las INTs
    //conf el ADC
    ADCON0bits.ADFM = 1;    //just a la derecha
    ADCON0bits.ADCS = 1;
    ADPCH = 0x00;           //canal seleccionado RA0
    ADCON0bits.ADON = 1;    //encender el ADC
    //conf PWM
    T2PR = 62;
    RC5PPS = 0x05;          //RC5 asignado a CCP1
    CCP1CON = 0x9C;         //CCP1 en PWM just izquierda
    CCPR1H = 31;
    CCPR1L = 0;
    T2CLKCON = 0x01;
    T2CON = 0xC0;
    //conf de datos iniciales
    LCD_INIT();
}

unsigned int tomamuestra_ADC(void){
    ADCON0bits.GO_nDONE = 1;        //toma muestra
    while(ADCON0bits.GO_nDONE == 1);    //espera a que termine
    return ((ADRESH << 8) + ADRESL);
}

void main(void) {
    configuro();
    POS_CURSOR(1,1);
    ESCRIBE_MENSAJE2("Hola veranon");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("Microbios 2026");
    __delay_ms(3000);
    BORRAR_LCD();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("ADC RA0:");
        LCD_ESCRIBE_VAR_INT(tomamuestra_ADC(), 5);
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("DC:");
        LCD_ESCRIBE_VAR_INT(tomamuestra_ADC()/10.333, 3);
        ESCRIBE_MENSAJE2(" %");
        CCPR1H = (tomamuestra_ADC() / 16.5);
        __delay_ms(100);
    }
}
