/* 
 * Archivo: i2c_lcd.c
 */

#include "i2c_lcd.h"

static uint8_t backlight_state = LCD_BL;

// Inicializa el módulo MSSP en modo I2C Master (Fosc = 4MHz, I2C = 100kHz)
static void I2C_Master_Init(void) {
    // Configurar pines SCL (RC3) y SDA (RC4) como entradas (requerido por MSSP)
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    SSP1CON1 = 0x28; // Habilita MSSP en modo I2C Master, reloj = FOSC / (4 * (SSP1ADD + 1))
    SSP1CON2 = 0x00;
    SSP1STAT = 0x80; // Slew rate desactivado para 100kHz
    
    // Para 4 MHz y 100 kHz: SSP1ADD = (4000000 / (4 * 100000)) - 1 = 9
    SSP1ADD = 9; 
}

static void I2C_Master_Start(void) {
    SSP1CON2bits.SEN = 1;
    while (SSP1CON2bits.SEN);
}

static void I2C_Master_Stop(void) {
    SSP1CON2bits.PEN = 1;
    while (SSP1CON2bits.PEN);
}

static void I2C_Master_Write(uint8_t data) {
    SSP1BUF = data;
    while (SSP1STATbits.BF);
    while (SSP1CON2bits.ACKSTAT);
}

// Envía un byte al expansor PCF8574 por I2C
static void LCD_WriteI2C(uint8_t data) {
    I2C_Master_Start();
    I2C_Master_Write(LCD_I2C_ADDR << 1); // Dirección con bit de escritura (0)
    I2C_Master_Write(data | backlight_state);
    I2C_Master_Stop();
}

// Envía un pulso de habilitación (Enable) al LCD
static void LCD_PulseEnable(uint8_t data) {
    LCD_WriteI2C(data | LCD_EN);
    __delay_us(1);
    LCD_WriteI2C(data & ~LCD_EN);
    __delay_us(50);
}

// Envía un nibble (4 bits) al LCD
static void LCD_WriteNibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode;
    LCD_WriteI2C(data);
    LCD_PulseEnable(data);
}

// Envía un comando completo (8 bits divididos en dos nibbles)
void LCD_SendCommand(uint8_t cmd) {
    LCD_WriteNibble(cmd & 0xF0, 0);          // Parte alta
    LCD_WriteNibble((cmd << 4) & 0xF0, 0);    // Parte baja
    if (cmd < 4) {
        __delay_ms(2); // Los comandos de limpiar pantalla y home requieren más tiempo
    } else {
        __delay_us(50);
    }
}

// Envía un dato (carácter) al LCD
void LCD_SendData(uint8_t data) {
    LCD_WriteNibble(data & 0xF0, LCD_RS);          // Parte alta con RS=1
    LCD_WriteNibble((data << 4) & 0xF0, LCD_RS);    // Parte baja con RS=1
    __delay_us(50);
}

// Inicialización del LCD en modo de 4 bits
void LCD_Init(void) {
    I2C_Master_Init();
    __delay_ms(50); // Esperar a que el LCD se energice
    
    // Secuencia de inicialización según datasheet del HD44780 en modo 4 bits
    LCD_WriteNibble(0x30, 0);
    __delay_ms(5);
    LCD_WriteNibble(0x30, 0);
    __delay_us(150);
    LCD_WriteNibble(0x30, 0);
    __delay_us(50);
    
    LCD_WriteNibble(0x20, 0); // Configurar interfaz a 4 bits
    __delay_us(50);
    
    // Configuración general del display
    LCD_SendCommand(0x28); // Function Set: 2 líneas, matriz de 5x8, modo 4 bits
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF, Blink OFF
    LCD_SendCommand(0x06); // Entry Mode Set: Incrementa cursor, sin desplazamiento
    LCD_Clear();
}

// Limpiar pantalla
void LCD_Clear(void) {
    LCD_SendCommand(0x01);
    __delay_ms(2);
}

// Mover cursor a una posición específica (Fila: 0 o 1, Columna: 0 a 15)
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54}; // Soporta pantallas de hasta 4x20
    if (row > 3) row = 3;
    LCD_SendCommand(0x80 | (row_offsets[row] + col));
}

// Escribir un carácter individual
void LCD_PrintChar(char c) {
    LCD_SendData((uint8_t)c);
}

// Escribir una cadena de caracteres
void LCD_PrintString(const char *str) {
    while (*str) {
        LCD_PrintChar(*str++);
    }
}

// Escribir un número entero (variables)
void LCD_PrintInt(int val) {
    char buffer[16];
    sprintf(buffer, "%d", val);
    LCD_PrintString(buffer);
}

// Crear un carácter personalizado (CGRAM) en una localidad de 0 a 7
void LCD_CreateChar(uint8_t location, uint8_t *charmap) {
    location &= 0x07; // Solo hay 8 localidades (0-7)
    LCD_SendCommand(0x40 | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        LCD_SendData(charmap[i]);
    }
}

// Controlar la luz de fondo (Backlight)
void LCD_Backlight(uint8_t state) {
    if (state) {
        backlight_state = LCD_BL;
    } else {
        backlight_state = 0x00;
    }
    LCD_WriteI2C(0x00); // Actualiza estado de salida del PCF8574
}