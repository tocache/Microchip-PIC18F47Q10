/*
 * File:   maincode00.c
 * Author: SURFACE
 *
 * Created on 2 de noviembre de 2026, 02:11 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

unsigned char horas=15,minutos=15,segundos=00;

void configuro(void){
    //conf el modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //para el oscilador secundario
    OSCCON3bits.SOSCPWR = 1;
    OSCENbits.SOSCEN = 1;
    //conf de las E/S
    ANSELB = 0xF8;  //RB(2:0) con entradas digitales
    WPUB = 0x07;    //RB(2:0) con weak pullup
    ANSELCbits.ANSELC0 = 0; //RC0 entrada digital! (solo Proteus)
    //conf Timer1
    TMR1CLK = 0x00;     //conf solo Proteus
    //TMR1CLK = 0x06;
    T1CON = 0x07;
    TMR1H = 0x80;
    TMR1L = 0x00;   //carga de cuenta inicial 32768
    //conf interrupciones
    PIE0bits.INT0IE = 1;    //INT0 enabled
    PIE0bits.INT1IE = 1;    //INT1 enabled
    PIE0bits.INT2IE = 1;    //INT2 enabled
    INTCONbits.INT0EDG = 0; //INT0 falling edge
    INTCONbits.INT1EDG = 0; //INT1 falling edge
    INTCONbits.INT2EDG = 0; //INT2 falling edge
    IPR0bits.INT0IP = 0;    //INT0 low priority
    IPR0bits.INT1IP = 0;    //INT1 low priority
    IPR0bits.INT2IP = 0;    //INT2 low priority
    PIE4bits.TMR1IE = 1;    //interrupt for TMR1 enabled
    PIR4bits.TMR1IF = 0;    //bajamos bandera de TMR1
    INTCONbits.IPEN = 1;    //habilitando las prioridades
    INTCONbits.GIEL = 1;    //global low priority enabled
    INTCONbits.GIEH = 1;    //global high priority enabled
    //inicializacion del LCD
    LCD_INIT();
}

void main(void) {
    configuro();
    while(1){
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Reloj UPC 2026-2");
        POS_CURSOR(2,0);
        LCD_ESCRIBE_VAR_CHAR(horas,2);
        ENVIA_CHAR(':');
        LCD_ESCRIBE_VAR_CHAR(minutos,2);
        ENVIA_CHAR(':');
        LCD_ESCRIBE_VAR_CHAR(segundos,2);
    }
}

void __interrupt(high_priority) Timer1_ISR(void){
    PIR4bits.TMR1IF = 0;        //bajamos bandera TMR1IF
    TMR1H = 0x80;
    TMR1L = 0x00;   //carga de cuenta inicial 32768
    if(segundos==59){
        segundos = 0;
        if(minutos==59){
            minutos = 0;
            if(horas==23){
                horas = 0;
            }
            else{
                horas++;
            }
        }
        else{
            minutos++;
        }
    }
    else{
        segundos++;
    }
}

void __interrupt(low_priority) INTs_ISR(void){
    if(PIR0bits.INT1IF == 1){
        PIR0bits.INT1IF = 0;    //bajar bandera INT1IF
        if(horas==23){
            horas = 0;
        }
        else{
            horas++;
        }        
    }
    else if(PIR0bits.INT2IF == 1){
        PIR0bits.INT2IF = 0;    //bajar bandera INT2IF
        if(minutos==59){
            minutos = 0;
        }
        else{
            minutos++;
        }        
        
    }
    else if(PIR0bits.INT0IF ==1){
        PIR0bits.INT0IF = 0;    //bajar bandera INT0IF
    }
}
