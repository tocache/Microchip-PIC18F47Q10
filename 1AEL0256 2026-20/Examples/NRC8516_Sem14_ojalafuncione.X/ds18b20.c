#include "ds18b20.h"

static bool parasite_power_mode = false;

void DS18B20_Init(void) {
    DS18B20_PIN_ANSEL = 0; // Desactivar función analógica en RA4
    DS18B20_PIN_ODCON = 0; // Modo CMOS estándar (permite Strong Pull-Up push-pull)
    DS18B20_PIN_TRIS  = 1; // Liberar bus (alta impedancia, pull-up externo mantiene en 1)
    DS18B20_PIN_LAT   = 0; // Latch en 0 para pulsos en bajo 1-Wire
}

bool DS18B20_Reset(void) {
    uint8_t presence;

    DS18B20_PIN_LAT  = 0;
    DS18B20_PIN_TRIS = 0; // Pulso de reset en bajo (mín. 480 us)
    __delay_us(480);

    DS18B20_PIN_TRIS = 1; // Liberar bus y esperar pulso de presencia (15-60 us)
    __delay_us(70);

    presence = DS18B20_PIN_PORT; // 0 = sensor presente, 1 = sin respuesta
    __delay_us(410);             // Completar ventana de 480 us

    return (presence == 0);
}

void DS18B20_WriteBit(uint8_t bit_val) {
    DS18B20_PIN_LAT  = 0;
    DS18B20_PIN_TRIS = 0; // Flanco de bajada inicia el time-slot

    if (bit_val & 0x01) {
        __delay_us(2);        // + sobrecarga de instrucción ? 6-8 us en bajo
        DS18B20_PIN_TRIS = 1; // Liberar bus para escribir '1'
        __delay_us(60);
    } else {
        __delay_us(60);       // Mantener en bajo 60 us para escribir '0'
        DS18B20_PIN_TRIS = 1; // Liberar bus
        __delay_us(2);        // Recuperación rápida (< 10 us antes de Strong Pull-Up)
    }
}

uint8_t DS18B20_ReadBit(void) {
    uint8_t bit_val;

    DS18B20_PIN_LAT  = 0;
    DS18B20_PIN_TRIS = 0; // Pulso de inicio de lectura (> 1 us)
    __delay_us(2);

    DS18B20_PIN_TRIS = 1; // Liberar bus para que el sensor envíe el bit
    __delay_us(8);        // Muestrear antes de los 15 us desde el flanco de bajada

    bit_val = DS18B20_PIN_PORT;
    __delay_us(50);       // Completar time-slot de 60 us

    return bit_val;
}

void DS18B20_WriteByte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        DS18B20_WriteBit(data & 0x01);
        data >>= 1;
    }
}

uint8_t DS18B20_ReadByte(void) {
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) {
        data >>= 1;
        if (DS18B20_ReadBit()) {
            data |= 0x80;
        }
    }
    return data;
}

// Cálculo CRC8 estándar 1-Wire (Polinomio: X^8 + X^5 + X^4 + 1 -> 0x8C reflejado)
uint8_t DS18B20_CRC8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t inbyte = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) {
                crc ^= 0x8C;
            }
            inbyte >>= 1;
        }
    }
    return crc;
}

/* ============================================================================
 * FUNCIONES DE PARASITE POWER (STRONG PULL-UP)
 * ============================================================================ */

void DS18B20_SetParasitePower(bool enable) {
    parasite_power_mode = enable;
}

bool DS18B20_IsParasitePowerEnabled(void) {
    return parasite_power_mode;
}

// Consulta al sensor con el comando 0xB4 si está alimentado por parásito
// y actualiza automáticamente el estado interno.
bool DS18B20_DetectParasitePower(void) {
    if (!DS18B20_Reset()) return false;

    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);
    DS18B20_WriteByte(DS18B20_CMD_READ_POWER);

    // El DS18B20 responde '0' si usa Parasite Power, o '1' si usa VDD externo
    parasite_power_mode = (DS18B20_ReadBit() == 0);
    return true;
}

void DS18B20_StrongPullUpEnable(void) {
    DS18B20_PIN_LAT  = 1; // Forzar nivel alto directo desde el puerto CMOS
    DS18B20_PIN_TRIS = 0; // Configurar RA4 como salida activa
}

void DS18B20_StrongPullUpDisable(void) {
    DS18B20_PIN_TRIS = 1; // Volver a alta impedancia (open-drain)
    DS18B20_PIN_LAT  = 0; // Dejar latch listo en 0 para futuras escrituras
}

/* ============================================================================
 * FUNCIONES DE LECTURA DE NÚMERO DE SERIE (ROM DE 64 BITS)
 * ============================================================================ */

// Lee los 8 bytes de la ROM (solo válido con UN sensor en el bus):
// rom_code[0]    = Código de familia (0x28 para DS18B20)
// rom_code[1..6] = Número de serie único de 48 bits (LSB en [1], MSB en [6])
// rom_code[7]    = CRC8 de los primeros 7 bytes
bool DS18B20_ReadROM(uint8_t rom_code[8]) {
    if (!DS18B20_Reset()) return false;

    DS18B20_WriteByte(DS18B20_CMD_READ_ROM);

    for (uint8_t i = 0; i < 8; i++) {
        rom_code[i] = DS18B20_ReadByte();
    }

    // Validar integridad con CRC8
    return (DS18B20_CRC8(rom_code, 7) == rom_code[7]);
}

// Convierte los 48 bits del número de serie (rom_code[6]..rom_code[1])
// en una cadena ASCII hexadecimal de 12 caracteres lista para el LCD.
void DS18B20_FormatSerialHex(const uint8_t rom_code[8], char hex_str[13]) {
    const char hex_chars[] = "0123456789ABCDEF";
    uint8_t pos = 0;

    for (int8_t i = 6; i >= 1; i--) {
        hex_str[pos++] = hex_chars[(rom_code[i] >> 4) & 0x0F];
        hex_str[pos++] = hex_chars[rom_code[i] & 0x0F];
    }
    hex_str[12] = '\0';
}

/* ============================================================================
 * FUNCIONES DE LECTURA DE TEMPERATURA
 * ============================================================================ */

static bool DS18B20_ReadScratchpadTemp(int16_t *raw_temp) {
    uint8_t scratchpad[9];

    DS18B20_WriteByte(DS18B20_CMD_READ_SCRATCHPAD);
    for (uint8_t i = 0; i < 9; i++) {
        scratchpad[i] = DS18B20_ReadByte();
    }

    // Verificar CRC8 del Scratchpad
    if (DS18B20_CRC8(scratchpad, 8) != scratchpad[8]) {
        return false;
    }

    *raw_temp = (int16_t)(((uint16_t)scratchpad[1] << 8) | scratchpad[0]);
    return true;
}

bool DS18B20_ReadTempRaw(int16_t *raw_temp) {
    if (!DS18B20_Reset()) return false;

    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);
    DS18B20_WriteByte(DS18B20_CMD_CONVERT_T);

    // Si está en Parasite Power, activar Strong Pull-Up en < 10 us tras 0x44
    if (parasite_power_mode) {
        DS18B20_StrongPullUpEnable();
    }

    __delay_ms(750); // Tiempo máximo de conversión a 12 bits

    if (parasite_power_mode) {
        DS18B20_StrongPullUpDisable();
    }

    if (!DS18B20_Reset()) return false;
    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);

    return DS18B20_ReadScratchpadTemp(raw_temp);
}

bool DS18B20_ReadTempRawByROM(const uint8_t rom_code[8], int16_t *raw_temp) {
    if (!DS18B20_Reset()) return false;

    DS18B20_WriteByte(DS18B20_CMD_MATCH_ROM);
    for (uint8_t i = 0; i < 8; i++) {
        DS18B20_WriteByte(rom_code[i]);
    }
    DS18B20_WriteByte(DS18B20_CMD_CONVERT_T);

    if (parasite_power_mode) {
        DS18B20_StrongPullUpEnable();
    }

    __delay_ms(750);

    if (parasite_power_mode) {
        DS18B20_StrongPullUpDisable();
    }

    if (!DS18B20_Reset()) return false;
    DS18B20_WriteByte(DS18B20_CMD_MATCH_ROM);
    for (uint8_t i = 0; i < 8; i++) {
        DS18B20_WriteByte(rom_code[i]);
    }

    return DS18B20_ReadScratchpadTemp(raw_temp);
}

bool DS18B20_ReadTempCelsius(float *temp_c) {
    int16_t raw;
    if (!DS18B20_ReadTempRaw(&raw)) return false;
    *temp_c = (float)raw * 0.0625f;
    return true;
}