#include <xc.h>
#include "cabecera.h"
#include "LCD.h"
#define _XTAL_FREQ 32000000UL

//variables globales para el reloj
unsigned char   horas=21, minutos=15,
                segundos=0, centesimas=0;

void configuro(void){
    //configuracion del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;      //HFINTOSC a 32MHz
    OSCEN = 0x40;
    //configuraciuon de las E/S
    //configuracion del TMR1
    TMR1CLK = 0x01;           //TMR1 source FOSC/4
    T1CON = 0x33;           //TMR1 ON, PSC 1:8, RD16=1
    //configuracion del CCP1 en modo comparacion para TMR1
    CCP1CON = 0x8B;          //CCP1 ON, compare mode rst al TMR1
    CCPR1H = 0x27;
    CCPR1L = 0x10;          //valor de comparacion 10000
    //configuracion de las INTs
    PIE6bits.CCP1IE = 1;        //CCP1IE enabled
    PIR6bits.CCP1IF = 0;        //bajamos la bandera CCP1IF
    INTCONbits.PEIE = 1;        //peripherals interrupts enabled
    INTCONbits.GIE = 1;        //global interrupts enabled
    //inicializacion de librerias
    LCD_INIT();
}

void visualizacion_hora(void){
    POS_CURSOR(1,2);
    ESCRIBE_MENSAJE("Hora actual:",12);
    POS_CURSOR(2,2);
    ENVIA_CHAR((horas/10) + 0x30);
    ENVIA_CHAR((horas%10) + 0x30);
    ENVIA_CHAR(':');
    ENVIA_CHAR((minutos/10) + 0x30);
    ENVIA_CHAR((minutos%10) + 0x30);
    ENVIA_CHAR(':');
    ENVIA_CHAR((segundos/10) + 0x30);
    ENVIA_CHAR((segundos%10) + 0x30);
    ENVIA_CHAR(':');
    ENVIA_CHAR((centesimas/10) + 0x30);
    ENVIA_CHAR((centesimas%10) + 0x30);    
}

void main(void) {
    configuro();
    while(1){
        visualizacion_hora();
    }
}

void __interrupt() CCP1_ISR(void){
    PIR6bits.CCP1IF = 0;
    if(centesimas == 99){
        centesimas = 0;
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
    else{
        centesimas++;
    }
}