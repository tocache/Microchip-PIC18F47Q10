#ifndef DS18B20_H
#define DS18B20_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000UL
#endif

// Definición del pin RA4 (Bus 1-Wire)
#define DS18B20_PIN_PORT    PORTAbits.RA4
#define DS18B20_PIN_LAT     LATAbits.LATA4
#define DS18B20_PIN_TRIS    TRISAbits.TRISA4
#define DS18B20_PIN_ANSEL   ANSELAbits.ANSELA4
#define DS18B20_PIN_ODCON   ODCONAbits.ODCA4

// Comandos ROM (64 bits)
#define DS18B20_CMD_READ_ROM        0x33
#define DS18B20_CMD_MATCH_ROM       0x55
#define DS18B20_CMD_SKIP_ROM        0xCC

// Comandos de función del DS18B20
#define DS18B20_CMD_CONVERT_T       0x44
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE
#define DS18B20_CMD_READ_POWER      0xB4

// Inicialización y bajo nivel 1-Wire
void DS18B20_Init(void);
bool DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t bit_val);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
uint8_t DS18B20_CRC8(const uint8_t *data, uint8_t len);

// Gestión de Parasite Power (Strong Pull-Up en RA4)
void DS18B20_SetParasitePower(bool enable);
bool DS18B20_IsParasitePowerEnabled(void);
bool DS18B20_DetectParasitePower(void);
void DS18B20_StrongPullUpEnable(void);
void DS18B20_StrongPullUpDisable(void);

// Lectura de Número de Serie (ROM de 64 bits)
bool DS18B20_ReadROM(uint8_t rom_code[8]);
void DS18B20_FormatSerialHex(const uint8_t rom_code[8], char hex_str[13]);

// Lectura de Temperatura
bool DS18B20_ReadTempRaw(int16_t *raw_temp);
bool DS18B20_ReadTempRawByROM(const uint8_t rom_code[8], int16_t *raw_temp);
bool DS18B20_ReadTempCelsius(float *temp_c);

#endif