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
    PIE4bits.TMR1IE = 1;    //interrupt del TMR1 enabled
    INTCONbits.PEIE = 1;    //interrupt de perifericos enabled
    INTCONbits.GIE = 1;     //interrupt global enabled
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

void __interrupt() TMR1_ISR(void){
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

