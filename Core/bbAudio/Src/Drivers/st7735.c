/*
 * st7735.c
 *
 *  Created on: Sep 9, 2019
 *      Author: fil
 */

#include "stm32h7xx_hal.h"
#include "st7735.h"
#include "main.h"

static void ST7735_Select() {
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
}

void ST7735_Unselect() {
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ST7735_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART };
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + ST7735_YSTART;
    data[3] = y1 + ST7735_YSTART;
    ST7735_WriteData(data, sizeof(data));

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

static void ST7735_WriteSingleData(uint8_t cmd) {
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

void ST7735_Init(void)
{
	HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
	HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	ST7735_WriteCommand(ST7735_SWRESET);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	ST7735_WriteCommand(ST7735_SLPOUT);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_FRMCTR1);
	ST7735_WriteSingleData(0x01);
	ST7735_WriteSingleData(0x2c);
	ST7735_WriteSingleData(0x2d);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_FRMCTR2);
	ST7735_WriteSingleData(0x01);
	ST7735_WriteSingleData(0x2c);
	ST7735_WriteSingleData(0x2d);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_FRMCTR3);
	ST7735_WriteSingleData(0x01);
	ST7735_WriteSingleData(0x2c);
	ST7735_WriteSingleData(0x2d);
	ST7735_WriteSingleData(0x01);
	ST7735_WriteSingleData(0x2c);
	ST7735_WriteSingleData(0x2d);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_INVCTR);
	ST7735_WriteSingleData(0x07);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_PWCTR1);
	ST7735_WriteSingleData(0xA2);
	ST7735_WriteSingleData(0x02);
	ST7735_WriteSingleData(0x84);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_PWCTR2);
	ST7735_WriteSingleData(0xC5);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_PWCTR3);
	ST7735_WriteSingleData(0x0A);
	ST7735_WriteSingleData(0x00);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_PWCTR4);
	ST7735_WriteSingleData(0x8A);
	ST7735_WriteSingleData(0x2A);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_PWCTR5);
	ST7735_WriteSingleData(0x8A);
	ST7735_WriteSingleData(0xEE);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_VMCTR1);
	ST7735_WriteSingleData(0x0E);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_INVOFF);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_MADCTL);
	ST7735_WriteSingleData(ST7735_ROTATION);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_COLMOD);
	ST7735_WriteSingleData(0x05);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_CASET);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0x4F);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_RASET);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0);
	ST7735_WriteSingleData(0x9F);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_INVON);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_GMCTRP1);
	ST7735_WriteSingleData(0x02);
	ST7735_WriteSingleData(0x1c);
	ST7735_WriteSingleData(0x07);
	ST7735_WriteSingleData(0x12);
	ST7735_WriteSingleData(0x37);
	ST7735_WriteSingleData(0x32);
	ST7735_WriteSingleData(0x29);
	ST7735_WriteSingleData(0x2d);
	ST7735_WriteSingleData(0x29);
	ST7735_WriteSingleData(0x25);
	ST7735_WriteSingleData(0x2B);
	ST7735_WriteSingleData(0x39);
	ST7735_WriteSingleData(0x00);
	ST7735_WriteSingleData(0x01);
	ST7735_WriteSingleData(0x03);
	ST7735_WriteSingleData(0x10);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_GMCTRP1);
	ST7735_WriteSingleData(0x03);
	ST7735_WriteSingleData(0x1d);
	ST7735_WriteSingleData(0x07);
	ST7735_WriteSingleData(0x06);
	ST7735_WriteSingleData(0x2e);
	ST7735_WriteSingleData(0x2c);
	ST7735_WriteSingleData(0x29);
	ST7735_WriteSingleData(0x2d);
	ST7735_WriteSingleData(0x2e);
	ST7735_WriteSingleData(0x2e);
	ST7735_WriteSingleData(0x37);
	ST7735_WriteSingleData(0x3f);
	ST7735_WriteSingleData(0x00);
	ST7735_WriteSingleData(0x00);
	ST7735_WriteSingleData(0x02);
	ST7735_WriteSingleData(0x10);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_NORON);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    ST7735_WriteCommand(ST7735_DISPON);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	ST7735_WriteCommand(ST7735_NORON);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	ST7735_WriteCommand(ST7735_DISPON);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_Select();

    ST7735_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ST7735_WriteData(data, sizeof(data));

    ST7735_Unselect();
}

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ST7735_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}

void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    ST7735_Select();

    while(*str) {
        if(x + font.width >= ST7735_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ST7735_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ST7735_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    ST7735_Unselect();
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            HAL_SPI_Transmit(&ST7735_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
        }
    }
    ST7735_Unselect();
}

void ST7735_FillScreen(uint16_t color) {
    ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}


void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) return;
    if((y + h - 1) >= ST7735_HEIGHT) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);
    ST7735_WriteData((uint8_t*)data, sizeof(uint16_t)*w*h);
    ST7735_Unselect();
}

void ST7735_InvertColors(bool invert) {
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

uint8_t ST7735_GetFontHeigth(FontDef font)
{
	return font.height;
}

uint8_t ST7735_GetFontWidth(FontDef font)
{
	return font.width;
}
