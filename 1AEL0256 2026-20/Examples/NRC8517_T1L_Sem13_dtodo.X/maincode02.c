/*
 * File:   maincode01.c
 * Author: Alumnos
 *
 * Created on 19 de noviembre de 2026, 09:10 AM
 */
#include <xc.h>
#include "cabecera.h"
#include "i2c_lcd.h"
#define _XTAL_FREQ 4000000UL

const uint8_t seno_5bits[] = {
    16, 19, 21, 24, 26, 28, 30, 31,
    31, 31, 30, 28, 26, 24, 21, 19,
    16, 12, 10,  7,  5,  3,  1,  0,
     0,  0,  1,  3,  5,  7, 10, 12
};

const uint8_t triangular_5bits[32] = {
     0,  2,  4,  6,  8, 10, 12, 14,
    16, 18, 20, 22, 24, 26, 28, 30,
    31, 29, 27, 25, 23, 21, 19, 17,
    15, 13, 11,  9,  7,  5,  3,  2
};

const uint8_t sierra_5bits[32] = {
     0,  1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31
};

void configuro(void){
    //conf mod osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //inicializar i2c_lcd
    LCD_Init();
    //conf el adc
    FVRCON = 0x00;
    ADCON0 = 0x94;
    ADREF = 0x00;
    ADPCH = 0x00;
    //conf del dac
    DAC1CON0 = 0xA0;
    //conf de las e-s
    ANSELBbits.ANSELB0 = 0;
    WPUBbits.WPUB0 = 1;
    ANSELBbits.ANSELB1 = 0;
    WPUBbits.WPUB1 = 1;    
}

unsigned int ADC_GetSample(void){
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    return (ADRESH << 8) + ADRESL;
}

void periodo(void){
    for(unsigned int z_var=0;z_var<ADC_GetSample();z_var++){
        __delay_us(1);
    }
}

void main(void) {
    configuro();
    LCD_SetCursor(0,0);     //primero es y luego es x
    LCD_PrintString("Hola upcino 2026");
    LCD_SetCursor(1,0);     //primero es y luego es x
    LCD_PrintString("Kalun Lau");
    __delay_ms(2000);
    LCD_Clear();
    LCD_SetCursor(0,0);     //primero es y luego es x
    LCD_PrintString("Generador");
    LCD_SetCursor(1,2);     //primero es y luego es x
    LCD_PrintString("de Se");
    LCD_PrintChar(0xEE);
    LCD_PrintString("ales");
    while(1){
        for(unsigned char y_var=0;y_var<32;y_var++){
            switch(PORTB & 0x03){
                case 1:
                    DAC1CON1 = triangular_5bits[y_var];
                    break;
                case 2:
                    DAC1CON1 = sierra_5bits[y_var];
                    break;
                case 3:
                    DAC1CON1 = seno_5bits[y_var];
                    break;
            }            
            periodo();
        }
    }
}
