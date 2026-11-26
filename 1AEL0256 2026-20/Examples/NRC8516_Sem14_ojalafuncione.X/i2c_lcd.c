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
    ANSELCbits.ANSELC3 = 0;
    ANSELCbits.ANSELC4 = 0;
    SSP1CLKPPS = 0x13;
    SSP1DATPPS = 0x14;
    RC3PPS = 0x0F;
    RC4PPS = 0x10;
    SSP1CON1 = 0x28; // Habilita MSSP en modo I2C Master, reloj = FOSC / (4 * (SSP1ADD + 1))
    SSP1CON2 = 0x00;
    SSP1STAT = 0x80; // Slew rate desactivado para 100kHz
    
    // Para 4 MHz y 100 kHz: SSP1ADD = (4000000 / (4 * 100000)) - 1 = 9
    //SSP1ADD = 9; //para trabajar a 4MHz
    SSP1ADD = 79; //para trabajar a 32MHz
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
    PIR3bits.SSP1IF = 0;
    SSP1BUF = data;
    while (!PIR3bits.SSP1IF); // Esperar a que termine los 9 pulsos (8 bits + ACK)
    PIR3bits.SSP1IF = 0;
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
/*void LCD_PrintInt(int val) {
    char buffer[16];
    sprintf(buffer, "%d", val);
    LCD_PrintString(buffer);
}*/

// Escribir un número entero con cantidad de dígitos dinámica
void LCD_PrintInt(int val, uint8_t digits) {
    char buffer[16];
    
    // Validar un rango seguro para los dígitos (por ejemplo, entre 1 y 10)
    if (digits == 0) digits = 1;
    if (digits > 10) digits = 10;
    
    // "%0*d" usa la variable 'digits' para definir el ancho y rellena con ceros a la izquierda
    sprintf(buffer, "%0*d", digits, val);
    
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

/* 
 * Patrones Gráficos Optimizados (3x2 bloques, gruesos y sin huecos intermedios)
 * Se utilizan los índices del 1 al 7 de la CGRAM (el 0 se omite por compatibilidad con Proteus).
 */
static const uint8_t big_digit_patterns_solid[8][8] = {
    // Índice 0: No usado (evita errores en simulación)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // Índice 1: Barra Superior Completa
    {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // Índice 2: Barra Inferior Completa
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F},
    
    // Índice 3: Línea Vertical Completa
    {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07},
    
    // Índice 4: Línea Vertical chaflan arriba izquierda
    {0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},
    
    // Índice 5: Bloque Central / Barra Horizontal Media (Sella el espacio central de 3, 6, 8, 9)
    {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F},
    
    // Índice 6: Conector Izquierdo con Barra Media
    {0x1C, 0x1C, 0x1C, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C},
    
    // Índice 7: Conector Derecho con Barra Media (Ideal para el 3, 6 y 9)
    {0x07, 0x07, 0x07, 0x1F, 0x1F, 0x07, 0x07, 0x07}
};

// --- Mapeo de Dígitos Optimizado (3 columnas x 2 filas) ---
// 0x20 representa un espacio en blanco
static const uint8_t big_digits_map_solid[10][6] = {
    // Sup Izq, Sup Cen, Sup Der, Inf Izq, Inf Cen, Inf Der
    {3,     1,     3,     3,     2,     3}, // 0
    {1,     3,     0,     2,     3,     2}, // 1
    {5,     5,     3,     3,     2,     2}, // 2
    {1,     1,     7,     2,     2,     7}, // 3 (Mejorado para cerrar el 3)
    {4,     2,     3,     0x20,  0x20,  3}, // 4
    {4,     1,     1,     2,     2,     3}, // 5
    {4,     1,     1,     6,     2,     7}, // 6 (Cierra perfectamente el bucle del 6)
    {1,     1,     3,     0x20,  0x20,  3}, // 7
    {4,     1,     3,     6,     2,     7}, // 8
    {4,     1,     7,     2,     2,     3}  // 9 (Cierra la parte superior del 9)
};

// Inicializar CGRAM omitiendo el índice 0
void LCD_InitBigDigits(void) {
    for (uint8_t i = 1; i <= 7; i++) {
        uint8_t temp[8];
        for (uint8_t j = 0; j < 8; j++) {
            temp[j] = big_digit_patterns_solid[i][j];
        }
        LCD_CreateChar(i, temp);
    }
}

// Función para imprimir el dígito grande con trazos sólidos y unidos
void LCD_PrintBigChar3x2(uint8_t row, uint8_t col, char digit) {
    if (digit < '0' || digit > '9') return;
    uint8_t num = digit - '0';
    
    // Fila Superior (3 caracteres consecutivos)
    LCD_SetCursor(row, col);
    for(uint8_t i = 0; i < 3; i++) {
        LCD_SendData(big_digits_map_solid[num][i]);
    }
    
    // Fila Inferior (3 caracteres consecutivos)
    LCD_SetCursor(row + 1, col);
    for(uint8_t i = 3; i < 6; i++) {
        LCD_SendData(big_digits_map_solid[num][i]);
    }
}