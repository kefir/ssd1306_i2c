/**
 * @file ssd1306_i2c.h
 * @author Sergey Tkachenko (sergey.iray@yandex.ru)
 * @brief 
 * @version 0.1
 * @date 2021-05-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

// #define SSD13_ADDRESS 0x7A
#define SSD13_ADDRESS 0x3C

#define NT75x_ONE 1
#define NT75x_ZERO 0

#define SSD13_WIDTH 128
#define SSD13_HEIGHT 64

#define SSD13_BUFFER_SIZE (SSD13_WIDTH * SSD13_HEIGHT) / 8

#define SSD13_PAGE_COUNT 8
#define SSD13_PAGE_SIZE SSD13_BUFFER_SIZE / SSD13_PAGE_COUNT

#define SSD13_I2C_ADDRESS 0x3C

// Control byte
#define SSD13_CONTROL_BYTE_CMD_SINGLE 0x80
#define SSD13_CONTROL_BYTE_CMD_STREAM 0x00
#define SSD13_CONTROL_BYTE_DATA_STREAM 0x40

// Fundamental commands (pg.28)
#define SSD13_CMD_SET_CONTRAST 0x81 // follow with 0x7F
#define SSD13_CMD_DISPLAY_RAM 0xA4
#define SSD13_CMD_DISPLAY_ALLON 0xA5
#define SSD13_CMD_DISPLAY_NORMAL 0xA6
#define SSD13_CMD_DISPLAY_INVERTED 0xA7
#define SSD13_CMD_DISPLAY_OFF 0xAE
#define SSD13_CMD_DISPLAY_ON 0xAF

// Addressing Command Table (pg.30)
#define SSD13_CMD_SET_MEMORY_ADDR_MODE 0x20 // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define SSD13_CMD_SET_COLUMN_RANGE 0x21 // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define SSD13_CMD_SET_PAGE_RANGE 0x22 // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
#define SSD13_CMD_SET_PAGE_ADDRESS 0xB0 // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define SSD13_CMD_SET_DISPLAY_START_LINE 0x40
#define SSD13_CMD_SET_SEGMENT_REMAP 0xA1
#define SSD13_CMD_SET_MUX_RATIO 0xA8 // follow with 0x3F = 64 MUX
#define SSD13_CMD_SET_COM_SCAN_MODE 0xC8
#define SSD13_CMD_SET_DISPLAY_OFFSET 0xD3 // follow with 0x00
#define SSD13_CMD_SET_COM_PIN_MAP 0xDA // follow with 0x12
#define SSD13_CMD_NOP 0xE3 // NOP

// Timing and Driving Scheme (pg.32)
#define SSD13_CMD_SET_DISPLAY_CLK_DIV 0xD5 // follow with 0x80
#define SSD13_CMD_SET_PRECHARGE 0xD9 // follow with 0xF1
#define SSD13_CMD_SET_VCOMH_DESELCT 0xDB // follow with 0x30

// Charge Pump (pg.62)
#define SSD13_CMD_SET_CHARGE_PUMP 0x8D // follow with 0x14

/**
 * @brief I2C driver structure.
 * User is to provide implementations of read and write functions 
 * 
 */
typedef struct
{
    void (*write)(uint8_t* data, uint32_t len); /**< I2C write function prototype*/
    void (*read)(uint8_t* data, uint32_t len); /**< I2C read function prototype*/
    uint8_t* screen_buff; /**< Pointer to screen buffer*/
} ssd13_driver_i2c_t;

/**
 * @brief Initializes the library with used-provided driver.
 * Applayind default display settings.
 * 
 * @param driver User-provided implementation.
 */
void ssd13_init(ssd13_driver_i2c_t* driver);

void ssd13_cmd(uint8_t cmd, uint8_t* data, uint32_t len);

void ssd13_display_clear();
void ssd13_display_update();