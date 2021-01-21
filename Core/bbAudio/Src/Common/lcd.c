/*
 * LcdTask.c
 *
 *  Created on: Sep 9, 2019
 *      Author: fil
 */
#include "lcd.h"

#include "main.h"
#include "st7735.h"
#include "fonts.h"

#include <stdio.h>

extern	uint16_t	current_brightness;

#define VMARGIN 1
#define HMARGIN (12*7)-1
uint8_t	horizontal_line_space;
uint8_t	vertical_line_space;
uint8_t	current_highlight_line;

char line1[32],line2[32],line3[32],line4[32],line5[32],line6[32];

#define JOY_THRESHOLD_LOW 8
#define JOY_THRESHOLD_HIGH 120

typedef struct {
    uint8_t xpos;
	uint8_t	ypos;
	uint8_t	font;
    char line[29];
    uint16_t fore_color;
	uint16_t bkg_color;
} Video;

typedef struct {
    uint8_t xpos;
	uint8_t	ypos;
    char line[32];
    uint16_t fore_color;
	uint16_t bkg_color;
} Function;

#define	NUMLINES	6
Function B201Function[NUMLINES]=
{
	{		0,
			0,
			"Not used",
			ST7735_RED,
			ST7735_BLACK,
	},
	{		0,
			1,
			"VCO",
			ST7735_BLUE,
			ST7735_BLACK,
	},
	{		0,
			2,
			"Sine",
			ST7735_BLUE,
			ST7735_BLACK,
	},
	{		0,
			3,
			"External",
			ST7735_BLUE,
			ST7735_BLACK,
	},
	{		0,
			4,
			" 0%",
			ST7735_BLUE,
			ST7735_BLACK,
	},
	{		0,
			5,
			"Disabled",
			ST7735_BLUE,
			ST7735_BLACK,
	},
};


Video VideoMem[NUMLINES]=
{
		{		0,
				0,
				"B201 Init Complete",
				ST7735_RED,
				ST7735_BLACK,
		},
		{		0,
				1,
				"Function : ",
				ST7735_BLUE,
				ST7735_BLACK,
		},
		{		0,
				2,
				"Waveform : ",
				ST7735_BLUE,
				ST7735_BLACK,
		},
		{		0,
				3,
				"Control  : ",
				ST7735_BLUE,
				ST7735_BLACK,
		},
		{		0,
				4,
				"De Tune  : ",
				ST7735_BLUE,
				ST7735_BLACK,
		},
		{		0,
				5,
				"MIDI In  : ",
				ST7735_BLUE,
				ST7735_BLACK,
		},
};

void initVideo(void)
{
int i;
	ST7735_FillScreen(ST7735_BLACK);
	horizontal_line_space = ST7735_GetFontWidth(Font_7x10);
	vertical_line_space = ST7735_GetFontHeigth(Font_7x10) + VMARGIN;
	for (i=0;i<NUMLINES;i++)
	{
		VideoMem[i].ypos *= vertical_line_space;
		B201Function[i].ypos *= vertical_line_space;
		B201Function[i].xpos = horizontal_line_space * 11;
		ST7735_WriteString(VideoMem[i].xpos, VideoMem[i].ypos,VideoMem[i].line,Font_7x10,VideoMem[i].fore_color,VideoMem[i].bkg_color);
		if ( i > 0 )
			ST7735_WriteString(B201Function[i].xpos, B201Function[i].ypos,B201Function[i].line,Font_7x10,B201Function[i].fore_color,B201Function[i].bkg_color);
	}
}

void setHighLightLine(uint8_t i)
{
	if ( i > 1 )
	{
		ST7735_WriteString(B201Function[current_highlight_line].xpos, B201Function[current_highlight_line].ypos,B201Function[current_highlight_line].line,Font_7x10,B201Function[i].fore_color,B201Function[current_highlight_line].bkg_color);
		current_highlight_line = i;
		ST7735_WriteString(B201Function[current_highlight_line].xpos, B201Function[current_highlight_line].ypos,B201Function[current_highlight_line].line,Font_7x10,ST7735_CYAN,B201Function[current_highlight_line].bkg_color);
	}
}

void LcdInit(void)
{
	current_brightness = 10000;
	ST7735_Init();
    initVideo();
    //setHighLightLine(2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
}
/*
 * joy_y = 0 : down
 * joy_y = 127 : up
 * joy_x = 127 : left
 * joy_x = 0 : right
 *
 */
uint16_t	current_brightness;

void LcdSetBrightness(uint16_t brightness)
{
	htim3.Instance->CCR3 = brightness;
}

void LcdProcess(uint32_t control_values )
{
	uint8_t joy_y, joy_x, joy_up, joy_down , i;
	joy_y = ( uint8_t)((control_values >> 8 ) & 0x7f);
	i = current_highlight_line;
	if ( joy_y < JOY_THRESHOLD_LOW )
	{
		i++;
		if ( i > 5 )
			i = 2;
		setHighLightLine(i);
	}
	if ( joy_y > JOY_THRESHOLD_HIGH )
	{
		i--;
		if ( i < 2 )
			i = 5;
		setHighLightLine(i);
	}


	/*
	sprintf(line1,"PL : 0x%02x",( uint8_t)(control_values >> 24));
	sprintf(line2,"PR : 0x%02x",( uint8_t)(control_values >> 16));
	sprintf(line3,"JX : 0x%02x",( uint8_t)((control_values >> 8 ) & 0x7f));
	sprintf(line4,"JY : 0x%02x",( uint8_t)(control_values & 0x7f));
    ST7735_WriteString(0, 0,    line1, Font_7x10, ST7735_BLUE, ST7735_BLACK);
    ST7735_WriteString(0, 2*10, line2, Font_7x10, ST7735_GREEN, ST7735_BLACK);
    if ( control_values & JOY_BTN_PRESSED)
    {
		ST7735_WriteString(0, 4*10, line3, Font_7x10, ST7735_RED, ST7735_BLACK);
		ST7735_WriteString(0, 6*10, line4, Font_7x10, ST7735_RED, ST7735_BLACK);
    }
    else
    {
        ST7735_WriteString(0, 4*10, line3, Font_7x10, ST7735_YELLOW, ST7735_BLACK);
        ST7735_WriteString(0, 6*10, line4, Font_7x10, ST7735_YELLOW, ST7735_BLACK);
    }
    */
}

