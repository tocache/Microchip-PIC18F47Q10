/* 
 * Archivo: i2c_lcd.h
 * Controlador: PIC18F47Q10 (Fosc = 4 MHz)
 * Descripción: Librería para LCD 16x2 / 20x4 vía I2C con PCF8574
 * Autor: Kalun Lau con ayuda de Gemini IA
 * Fecha: 17/11/2026
 * Versión: 1.00
 */

#ifndef I2C_LCD_H
#define	I2C_LCD_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define _XTAL_FREQ 32000000UL

// Dirección I2C por defecto del módulo PCF8574 (puede ser 0x27 o 0x3F según los jumpers A0, A1, A2)
#define LCD_I2C_ADDR 0x27

// Definición de pines del PCF8574 conectados al LCD
#define LCD_RS     0x01 // P0
#define LCD_RW     0x02 // P1
#define LCD_EN     0x04 // P2
#define LCD_BL     0x08 // P3
#define LCD_D4     0x10 // P4
#define LCD_D5     0x20 // P5
#define LCD_D6     0x40 // P6
#define LCD_D7     0x80 // P7

// Prototipos de funciones
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_PrintChar(char c);
void LCD_PrintString(const char *str);
//void LCD_PrintInt(int val);
void LCD_PrintInt(int val, uint8_t digits);
void LCD_CreateChar(uint8_t location, uint8_t *charmap);
void LCD_Backlight(uint8_t state);
void LCD_InitBigDigits(void);
void LCD_PrintBigChar3x2(uint8_t row, uint8_t col, char digit);

#endif	/* I2C_LCD_H */