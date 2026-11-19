/*
 * File:   maincode00.c
 * Author: Alumnos
 *
 * Created on November 19, 2026, 11:59 AM
 */
#include <xc.h>
#include <string.h>
#include "cabecera.h"
#include "i2c_lcd.h"
#define _XTAL_FREQ 4000000UL

unsigned char fig1[]={"  __   ____  _  _  ____  ____  __ _     __   _  _  ____ "};
unsigned char fig2[]={" / _\\ (  _ \\/ )( \\(  _ \\(  __)(  ( \\   /  \\ / )( \\(  __)"};
unsigned char fig3[]={"/    \\ ) __/) \\/ ( )   / ) _) /    /  (  O )) \\/ ( ) _) "};
unsigned char fig4[]={"\\_/\\_/(__)  \\____/(__\\_)(____)\\_)__)   \\__\\)\\____/(____)"};
unsigned char fig5[]={" ____  ____    _  _   __     ____  __                   "};
unsigned char fig6[]={"/ ___)(  __)  / )( \\ / _\\   (  __)(  )                  "};
unsigned char fig7[]={"\\___ \\ ) _)   \\ \\/ //    \\   ) _) / (_/\\                "};
unsigned char fig8[]={"(____/(____)   \\__/ \\_/\\_/  (____)\\____/                "};
unsigned char fig9[]={" ____  __  ____  _  _  ____   __                        "};
unsigned char fig10[]={"(_  _)(  )(  __)( \\/ )(  _ \\ /  \\                       "};
unsigned char fig11[]={"  )(   )(  ) _) / \\/ \\ ) __/(  O )                      "};
unsigned char fig12[]={" (__) (__)(____)\\_)(_/(__)   \\__/                       "};

void configuro(void){
    //conf del mod de osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf EUSART2
    TX2STAbits.BRGH = 1;
    SP2BRG = 25;
    TX2STAbits.TXEN = 1;
    RC2STAbits.SPEN = 1;
    RX2PPS = 0x19;
    RD0PPS = 0x0B;
    ANSELDbits.ANSELD1 = 0;
    TRISDbits.TRISD0 = 0;
    ANSELDbits.ANSELD0 = 0;
    //inicializacion del I2C-LCD
    LCD_Init();
    //conf ADC
    ADCON0 = 0x94;
    ADPCH = 0x00;
    ADREF = 0x00;
    FVRCON = 0x00;
}

void Serial2_SendByte(unsigned char dato){
    TX2REG = dato;
    while(!TX2STAbits.TRMT);
}

void Serial2_SendString(unsigned char *cadena){
    for(unsigned char x_var=0;x_var<strlen(cadena);x_var++){
        Serial2_SendByte(cadena[x_var]);
    }
}

void Serial2_NewLine(void){
    Serial2_SendByte(0x0A);
    Serial2_SendByte(0x0D);
}

unsigned int ADC_GetSample(unsigned char canal){
    ADPCH = canal;
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    return (ADRESH <<8) + ADRESL;
}

void mensajon(void){
        Serial2_SendString(fig1);
        Serial2_NewLine();
        Serial2_SendString(fig2);
        Serial2_NewLine();
        Serial2_SendString(fig3);
        Serial2_NewLine();
        Serial2_SendString(fig4);
        Serial2_NewLine();
        Serial2_SendString(fig5);
        Serial2_NewLine();
        Serial2_SendString(fig6);
        Serial2_NewLine();
        Serial2_SendString(fig7);
        Serial2_NewLine();
        Serial2_SendString(fig8);
        Serial2_NewLine();
        Serial2_SendString(fig9);
        Serial2_NewLine();
        Serial2_SendString(fig10);
        Serial2_NewLine();
        Serial2_SendString(fig11);
        Serial2_NewLine();
        Serial2_SendString(fig12);
        Serial2_NewLine();    
}

void main(void) {
    configuro();
    LCD_SetCursor(0,0);
    LCD_PrintString("Ing.Meca-Ele-Bio");
    LCD_SetCursor(1,0);
    LCD_PrintString("Hecho por Kalun");
    Serial2_SendString("Hola mundo!");
    Serial2_NewLine();
    Serial2_SendString("Programa de prueba hecho por Kalun");
    Serial2_NewLine();
    Serial2_SendString("Tipeeen, no tomen foto");
    Serial2_NewLine();
    mensajon();
    __delay_ms(3000);
    LCD_Clear();
    while(1){
        LCD_SetCursor(0,0);
        LCD_PrintString("ADC CH0:");
        LCD_PrintInt(ADC_GetSample(0),4);
    }
}
