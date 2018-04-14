#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <stddef.h>
#include <util/delay.h>
#include "LCD_I2C_lib_c.h"
#include "twi.h"

#define LCD_I2C_SetColorWhite()         LCD_I2C_SetRGB(255, 255, 255)
#define FunctionToCommand(function)     (LCD_FUNCTIONSET    | function)
#define ControlToCommand(control)       (LCD_DISPLAYCONTROL | control)
#define ModeToCommand(mode)             (LCD_ENTRYMODESET   | mode)
#define AddressToCommand(Address)       (LCD_SETCGRAMADDR   | Address)

#define R       0x00
#define G       0x01
#define B       0x02
#define RGB     0x03

const uint8_t m_u8ColorDefine[COLOR_MAX][RGB] = 
{
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

static void _LCD_I2C_SendComand(uint8_t p_u8Value, uint8_t p_u8Device, uint8_t p_u8Reg);

static uint8_t m_u8DisplayControl   = 0;
static uint8_t m_u8DisplayFunction  = 0;
static uint8_t m_u8DisplayMode      = 0;

void LCD_I2C_SetCollor(eColors p_eColor)
{
    if(p_eColor >  COLOR_MAX)
    {
        return;
    }

    LCD_I2C_SetRGB(m_u8ColorDefine[p_eColor][R], m_u8ColorDefine[p_eColor][G], m_u8ColorDefine[p_eColor][B]);
}

static void _LCD_I2C_SendComand(uint8_t p_u8Value, uint8_t p_u8Device, uint8_t p_u8Reg)
{
    uint8_t u8TxBuffer[2] = {p_u8Reg, p_u8Value};
    uint8_t u8WaitTx = true;
    uint8_t u8SendStop = true;
    uint8_t u8Adress = 0xFF;

    switch(p_u8Device)
    {
        case LCD:
            u8Adress = LCD_ADDRESS;
            break;
        case BACKLIGTH:
            u8Adress = RGB_ADDRESS;
            break;
        default:
            break;
    }

    twi_writeTo(u8Adress, u8TxBuffer, sizeof(u8TxBuffer), u8WaitTx, u8SendStop);
}

void LCD_I2C_SetRGB(uint8_t p_u8Red, uint8_t p_u8Green, uint8_t p_u8Blue)
{   
    uint8_t m_u8TxBuffer[6] = {REG_BL_RED, p_u8Red, REG_BL_GREEN, p_u8Green, REG_BL_BLUE, p_u8Blue};
    uint8_t m_u8WaitTx = true;
    uint8_t m_u8SendStop = true;
    uint8_t i = 0;

   for(i=0; i<sizeof(m_u8TxBuffer); i+=2)
   {
        twi_writeTo(RGB_ADDRESS, &m_u8TxBuffer[i], 2, m_u8WaitTx, m_u8SendStop);   
   }
}

void LCD_I2C_WriteChar(uint8_t p_u8Char)
{
    uint8_t u8TxBuffer[2] = {0x40, p_u8Char};
    uint8_t u8WaitTx = true;
    uint8_t u8SendStop = true;

    twi_writeTo(LCD_ADDRESS, u8TxBuffer, sizeof(u8TxBuffer), u8WaitTx, u8SendStop);
	_delay_ms(100);
}

void LCD_I2C_WriteString(const char *p_pu8Char, eLinha p_eLinha)
{
	uint8_t u8TxBuffer[16 + 1];
    uint8_t u8WaitTx = true;
    uint8_t u8SendStop = true;
    uint8_t i = 0;
	
    if(p_pu8Char == NULL) return;
	
	switch(p_eLinha)
	{
		case _E_LINHA_1:
			LCD_I2C_SetCursor(_E_LINHA_1, _E_COLUNA_1);
			break;
		case _E_LINHA_2:
			LCD_I2C_SetCursor(_E_LINHA_2, _E_COLUNA_1);
			break;			
	}
	
	u8TxBuffer[0] = 0x40;      
    for(i=1; i<=sizeof(u8TxBuffer); i++) u8TxBuffer[i] = ' ';
	
	for(i=1; *p_pu8Char; i++) //(*p_pu8Char || i <= (16+1))
    {        
		if(i <= sizeof(u8TxBuffer)) u8TxBuffer[i] = *p_pu8Char;
		p_pu8Char++;
    }

    twi_writeTo(LCD_ADDRESS, u8TxBuffer, sizeof(u8TxBuffer), u8WaitTx, u8SendStop);
}

void LCD_I2C_Init(void)
{        
    // I2C init
    twi_init();

    //LCD Init
    m_u8DisplayFunction |= LCD_2LINE;
   
    _delay_ms(50);
    _LCD_I2C_SendComand(FunctionToCommand(m_u8DisplayFunction), LCD, REG_LCD);
    _delay_ms(5);
    _LCD_I2C_SendComand(FunctionToCommand(m_u8DisplayFunction), LCD, REG_LCD);
    _delay_us(150);
    _LCD_I2C_SendComand(FunctionToCommand(m_u8DisplayFunction), LCD, REG_LCD);
    // Comment about
    _LCD_I2C_SendComand(FunctionToCommand(m_u8DisplayFunction), LCD, REG_LCD);

    m_u8DisplayControl |= (LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
    _delay_us(50);

    LCD_I2C_Clear();

    m_u8DisplayMode |= (LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    _LCD_I2C_SendComand(ModeToCommand(m_u8DisplayMode), LCD, REG_LCD);
    _delay_us(50);

    // Backlight Init
    _LCD_I2C_SendComand(BL_INIT_COMAND, BACKLIGTH, REG_BL_MODE1);
    _delay_us(50);
    _LCD_I2C_SendComand(BL_PWM_COMAND, BACKLIGTH, REG_BL_OUTPUT);
    _delay_us(50);
    _LCD_I2C_SendComand(BL_BLINK_COMAND, BACKLIGTH, REG_BL_MODE2);
    _delay_us(50);

    LCD_I2C_SetColorWhite();
}

void LCD_I2C_Clear(void)
{
    _LCD_I2C_SendComand(LCD_CLEARDISPLAY, LCD, REG_LCD);
    _delay_ms(3);
}

void LCD_I2C_ReturnToHome(void)
{
    _LCD_I2C_SendComand(LCD_RETURNHOME, LCD, REG_LCD);
    _delay_ms(2);
}

void LCD_I2C_SetCursor(eLinha p_eLinha, eColuna p_eColuna)
{
    p_eColuna = (p_eLinha == 0 ? p_eColuna|REG_LCD_LINHA_1 : p_eColuna|REG_LCD_LINHA_2);
    _LCD_I2C_SendComand(p_eColuna, LCD, REG_LCD);
	_delay_ms(3);
}

void LCD_I2C_DisplayOff(void)
{
    m_u8DisplayControl &= ~LCD_DISPLAYON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_DisplayOn(void)
{
    m_u8DisplayControl |= LCD_DISPLAYON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_CursosOff(void)
{
    m_u8DisplayControl  &= ~LCD_CURSORON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_CursosOn(void)
{
    m_u8DisplayControl |= LCD_CURSORON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_BlinkOff(void)
{
    m_u8DisplayControl  &= ~LCD_BLINKON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_BlinkOn(void)
{
    m_u8DisplayControl |= LCD_BLINKON;
    _LCD_I2C_SendComand(ControlToCommand(m_u8DisplayControl), LCD, REG_LCD);
}

void LCD_I2C_ScrollDisplayLeft(void)
{
    uint8_t u8Command = 0;

    u8Command = (LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    _LCD_I2C_SendComand(u8Command, LCD, REG_LCD);
}

void LCD_I2C_ScrollDisplayRight(void)
{
    uint8_t u8Command = 0;

    u8Command = (LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    _LCD_I2C_SendComand(u8Command, LCD, REG_LCD);
}

void LCD_I2C_RightToLeft(void)
{
    m_u8DisplayMode &= ~LCD_ENTRYLEFT;
    _LCD_I2C_SendComand(ModeToCommand(m_u8DisplayMode), LCD, REG_LCD);
}

void LCD_I2C_LeftToRight(void)
{
    m_u8DisplayMode |= LCD_ENTRYLEFT;
    _LCD_I2C_SendComand(ModeToCommand(m_u8DisplayMode), LCD, REG_LCD);
}

void LCD_I2C_AutoScrollOff(void)
{
    m_u8DisplayMode  &= ~LCD_ENTRYSHIFTINCREMENT;
    _LCD_I2C_SendComand(ModeToCommand(m_u8DisplayMode), LCD, REG_LCD);
}

void LCD_I2C_AutoScrollOn(void)
{
    m_u8DisplayMode |= LCD_ENTRYSHIFTINCREMENT;
    _LCD_I2C_SendComand(ModeToCommand(m_u8DisplayMode), LCD, REG_LCD);
}

void LCD_I2C_CreateChar(uint8_t p_u8Address, uint8_t p_u8CharMap[])
{
    uint8_t m_u8TxBuffer[9];
    p_u8Address &= 0x07;
    _LCD_I2C_SendComand(AddressToCommand(p_u8Address<<3), LCD, REG_LCD);
    
    m_u8TxBuffer[0] = 0x40;
    for(int i=0; i<8; i++)
    {
        m_u8TxBuffer[i+1] = p_u8CharMap[i];
    }
    twi_writeTo(LCD_ADDRESS, m_u8TxBuffer, sizeof(m_u8TxBuffer), true, true);
}

void LCD_I2C_BlinkLedOn(void)
{
    // blink period in seconds = (<reg 7> + 1) / 24
    // on/off ratio = <reg 6> / 256
    _LCD_I2C_SendComand(0x17, BACKLIGTH, 0x07);
    _LCD_I2C_SendComand(0x7f, BACKLIGTH, 0x06);
}

void LCD_I2C_BlinkLedOff(void)
{
    _LCD_I2C_SendComand(0x00, BACKLIGTH, 0x07);  // blink every second
    _LCD_I2C_SendComand(0xFF, BACKLIGTH, 0x06);  // half on, half off
}

