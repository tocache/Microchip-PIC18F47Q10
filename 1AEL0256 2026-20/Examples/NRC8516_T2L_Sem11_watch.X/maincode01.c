/*
 * File:   maincode00.c
 * Author: SURFACE
 *
 * Created on 2 de noviembre de 2026, 06:07 PM
 */
#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000UL

//declaracion de variables globales
unsigned char horas=18, minutos=47, segundos=00;

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf los puertos
    ANSELB = 0xF8;
    WPUB = 0x07;
    //conf el Timer1 para que funcione en RTC
    TMR1CLK = 0x06;     //fuente SOSC 32.768KHz
    T1CON = 0x07;       //TMR1 on, async, 1:1presc
    //conf de interrupciones
    PIE0bits.INT1IE = 1;    //INT1 enabled
    PIE0bits.INT2IE = 1;    //INT2 enabled
    IPR0bits.INT1IP = 0;    //INT1 low priority
    IPR0bits.INT2IP = 0;    //INT2 low priority
    INTCONbits.INT1EDG = 0; //INT1 falling edge
    INTCONbits.INT2EDG = 0; //INT2 falling edge
    PIE4bits.TMR1IE = 1;    //interrupt del TMR1 enabled
    INTCONbits.IPEN = 1;    //interrupt priority enabled
    INTCONbits.GIEL = 1;    //global low priority enabled
    INTCONbits.GIEH = 1;    //global high priority enabledProgram 131,072 (0x20000) bytes
    //inicializar el LCD
    LCD_INIT();
}

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Reloj UPC 202620");
    while(1){
        //aca vas a visualizar la hora actual
        POS_CURSOR(2,0);
        LCD_ESCRIBE_VAR_CHAR(horas,2);
        ENVIA_CHAR(':');
        LCD_ESCRIBE_VAR_CHAR(minutos,2);
        ENVIA_CHAR(':');
        LCD_ESCRIBE_VAR_CHAR(segundos,2);
    }
}

void __interrupt(high_priority) TMR1_ISR(void){
    PIR4bits.TMR1IF = 0;    //bajamos el flag TMR1IF;
    TMR1H = 0x80;
    TMR1L = 0x00;       //precarga de cuenta de 32768 a TMR1
    if(segundos == 59){
        segundos = 0;
        if(minutos == 59){
            minutos = 0;
            if(horas == 23){
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
        PIR0bits.INT1IF = 0;
        if(horas == 23){
            horas = 0;
        }
        else{
            horas++;
        }
    }
    else if(PIR0bits.INT2IF == 1){
        PIR0bits.INT2IF = 0;
        if(minutos == 59){
            minutos = 0;
        }
        else{
            minutos++;
        }
    }
}