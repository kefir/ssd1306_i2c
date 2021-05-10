#include "ssd1306_i2c.h"

#include <string.h>

static ssd13_driver_i2c_t* drv = NULL;

static uint8_t page_update_flags[SSD13_PAGE_COUNT];

static void default_init();
static void address_set(uint8_t x, uint8_t page);

void ssd13_init(ssd13_driver_i2c_t* driver)
{
    memset(page_update_flags, 0, SSD13_PAGE_COUNT);

    if (driver) {
        if (driver->screen_buff) {
            drv = driver;
            default_init();
            ssd13_display_clear();
        }
    }
}

void ssd13_cmd(uint8_t cmd, uint8_t* data, uint32_t len)
{
    if (drv) {
        uint8_t dataOut[len + 1];
        dataOut[0] = cmd | 0x40;

        memcpy(&dataOut[1], data, len);
        drv->write(dataOut, (len + 1));
    }
}

static void default_init()
{
    uint8_t cmd[] = {
        SSD13_CONTROL_BYTE_CMD_STREAM,
        SSD13_CMD_SET_CHARGE_PUMP, 0x14,
        SSD13_CMD_SET_SEGMENT_REMAP,
        SSD13_CMD_SET_COM_SCAN_MODE,
        SSD13_CMD_DISPLAY_ON
    };

    drv->write(cmd, sizeof(cmd));
}

void ssd13_display_clear()
{
    memset(page_update_flags, 1, SSD13_PAGE_COUNT);
    memset(drv->screen_buff, 0xAA, SSD13_BUFFER_SIZE);
    ssd13_display_update();
}

void ssd13_display_update()
{

    for (uint8_t i = 0; i < SSD13_PAGE_COUNT; i++) {

        if (page_update_flags[i]) {
            page_update_flags[i] = 0;
            address_set(0, i);
            uint8_t cmd = SSD13_CONTROL_BYTE_DATA_STREAM;
            drv->write(&cmd, 1);
            drv->write(&drv->screen_buff[i * SSD13_PAGE_SIZE], SSD13_PAGE_SIZE);
        }
    }
}

static void address_set(uint8_t x, uint8_t page)
{
    uint8_t cmd[5] = { 0 };
    cmd[0] = SSD13_CMD_SET_PAGE_ADDRESS | (page > (SSD13_PAGE_COUNT - 1) ? (SSD13_PAGE_COUNT - 1) : page);

    uint8_t lsb, msb;

    msb = (((x & 0xF0) >> 4) | 0x10);
    lsb = (x & 0x0F);

    cmd[1] = SSD13_CONTROL_BYTE_CMD_STREAM;
    cmd[2] = lsb;
    cmd[3] = msb;

    drv->write(cmd, 4);
}