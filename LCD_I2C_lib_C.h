#ifndef LCD_I2C_lib_C_H_
#define LCD_I2C_lib_C_H_

#include <stdbool.h>
#include <stdint.h>
//#include "twi.h"


/* Defines */

// Devices
#define LCD         0x00
#define BACKLIGTH   0x01

#define REG_LCD             0x08
#define REG_LCD_LINHA_1     0x80
#define REG_LCD_LINHA_2     0xC0

// Device I2C Adrress
#define LCD_ADDRESS     (0x7c>>1)
#define RGB_ADDRESS     (0xc4>>1)

// color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_BL_RED         0x04        // pwm2
#define REG_BL_GREEN       0x03        // pwm1
#define REG_BL_BLUE        0x02        // pwm0

#define REG_BL_MODE1       0x00
#define REG_BL_MODE2       0x01
#define REG_BL_OUTPUT      0x08

#define BL_INIT_COMAND     0x00
#define BL_PWM_COMAND      0xFF
#define BL_BLINK_COMAND    0x20

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00



/*  Structs */

typedef enum{
    _E_WHITE = 0,
    _E_RED,
    _E_GREEN,
    _E_BLUE,
//  COLOR_MAX   4
} eColors;
#define COLOR_MAX   4

typedef enum{
	_E_LINHA_1 = 0,
	_E_LINHA_2,
	} eLinha;

typedef enum{
	_E_COLUNA_1 = 0,
	_E_COLUNA_2,
	_E_COLUNA_3,
	_E_COLUNA_4,
	_E_COLUNA_5,
	_E_COLUNA_6,
	_E_COLUNA_7,
	_E_COLUNA_8,
	_E_COLUNA_9,
	_E_COLUNA_10,
	_E_COLUNA_11,
	_E_COLUNA_12,
	_E_COLUNA_13,
	_E_COLUNA_14,
	_E_COLUNA_15,
	_E_COLUNA_16,
	} eColuna;


/* Prototipos */
void LCD_I2C_SetRGB(uint8_t p_u8Red, uint8_t p_u8Green, uint8_t p_u8Blue);

void LCD_I2C_SetCollor(eColors p_eColor);

void LCD_I2C_WriteChar(uint8_t p_u8Char);

void LCD_I2C_WriteString(const char *p_pu8Char, eLinha p_eLinha);

void LCD_I2C_Init(void);

void LCD_I2C_Clear(void);

void LCD_I2C_ReturnToHome(void);

void LCD_I2C_SetCursor(eLinha p_eLinha, eColuna p_eColuna);

void LCD_I2C_DisplayOff(void);

void LCD_I2C_DisplayOn(void);

void LCD_I2C_CursosOff(void);

void LCD_I2C_CursosOn(void);

void LCD_I2C_BlinkOff(void);

void LCD_I2C_BlinkOn(void);

void LCD_I2C_ScrollDisplayLeft(void);

void LCD_I2C_ScrollDisplayRight(void);

void LCD_I2C_RightToLeft(void);

void LCD_I2C_LeftToRight(void);

void LCD_I2C_AutoScrollOff(void);

void LCD_I2C_AutoScrollOn(void);

void LCD_I2C_CreateChar(uint8_t p_u8Address, uint8_t p_u8CharMap[]);

void LCD_I2C_BlinkLedOn(void);

void LCD_I2C_BlinkLedOff(void);

#endif // LCD_I2C_lib_C_H_