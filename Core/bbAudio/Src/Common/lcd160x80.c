/*
 * lcd160x80.c
 *
 *  Created on: Jan 21, 2021
 *      Author: fil
 */

#include "main.h"
#include "lcd160x80.h"
#include <stdio.h>

uint16_t			current_brightness,current_highlight_line;
VideoTextFrame		VideoText[6];

void setHighLightLine(uint8_t i)
{
	if ( i > 1 )
	{
		if (( VideoText[0].flag & LCD_HI_SIDE_LEFT_FLAG) == LCD_HI_SIDE_LEFT_FLAG)
		{
			ST7735_FillRectangle(VideoText[i].xpos0, VideoText[i].ypos, ST7735_WIDTH/2, ST7735_GetFontHeigth(*VideoText[i].font), ST7735_BLACK);
			ST7735_WriteString(VideoText[i].xpos0, VideoText[i].ypos,VideoText[i].line0,*VideoText[i].font,VideoText[i].fore_color0,ST7735_HILIGHT);
		}
		if (( VideoText[0].flag & LCD_HI_SIDE_RIGHT_FLAG) == LCD_HI_SIDE_RIGHT_FLAG)
		{
			ST7735_FillRectangle(VideoText[i].xpos1, VideoText[i].ypos, (ST7735_WIDTH-VideoText[i].xpos1)-2, ST7735_GetFontHeigth(*VideoText[i].font), ST7735_BLACK);
			ST7735_WriteString(VideoText[i].xpos1, VideoText[i].ypos,VideoText[i].line1,*VideoText[i].font,VideoText[i].fore_color1,ST7735_HILIGHT);
		}
		current_highlight_line = i;
	}
}

void refreshLine(uint8_t i)
{
	if ( i > 1 )
	{
		if (( VideoText[i].flag & LCD_REFRESH_LEFT_FLAG) == LCD_REFRESH_LEFT_FLAG)
		{
			ST7735_FillRectangle(VideoText[i].xpos0, VideoText[i].ypos, ST7735_WIDTH/2, ST7735_GetFontHeigth(*VideoText[i].font), ST7735_BLACK);
			ST7735_WriteString(VideoText[i].xpos0, VideoText[i].ypos,VideoText[i].line0,*VideoText[i].font,VideoText[i].fore_color0,VideoText[i].bkg_color0);
			VideoText[i].flag &= ~LCD_REFRESH_LEFT_FLAG;
		}
		if (( VideoText[i].flag & LCD_REFRESH_RIGHT_FLAG) == LCD_REFRESH_RIGHT_FLAG)
		{
			ST7735_FillRectangle(VideoText[i].xpos1, VideoText[i].ypos, (ST7735_WIDTH/2)-1, ST7735_GetFontHeigth(*VideoText[i].font), ST7735_BLACK);
			ST7735_WriteString(VideoText[i].xpos1, VideoText[i].ypos,VideoText[i].line1,*VideoText[i].font,VideoText[i].fore_color1,VideoText[i].bkg_color1);
			VideoText[i].flag &= ~LCD_REFRESH_RIGHT_FLAG;
		}
	}
}


void UpdateAllVideoText(void)
{
uint32_t	i;
	i = 0;
	ST7735_WriteString(VideoText[i].xpos0, VideoText[i].ypos,VideoText[i].line0,*VideoText[i].font,VideoText[i].fore_color0,VideoText[i].bkg_color0);
	for(i=1;i<LCD_LINES;i++)
	{
		ST7735_WriteString(VideoText[i].xpos0, VideoText[i].ypos,VideoText[i].line0,*VideoText[i].font,VideoText[i].fore_color0,VideoText[i].bkg_color0);
		ST7735_WriteString(VideoText[i].xpos1, VideoText[i].ypos,VideoText[i].line1,*VideoText[i].font,VideoText[i].fore_color1,VideoText[i].bkg_color1);
	}
}

void InitVideo(void)
{
uint32_t	i,pot,ypos;
	current_brightness = 10000;
	ST7735_Init();
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	ST7735_FillScreen(ST7735_DARKBLUE);
	ST7735_FillRectangle(1, 1, ST7735_WIDTH-2, ST7735_HEIGHT-2, ST7735_BLACK);

	sprintf(VideoText[0].line0, "VCO Bb201");
	VideoText[0].flag = LCD_FULL_FLAG | LCD_CENTER_TEXT_FLAG;
	VideoText[0].font = &Font_11x18;
	VideoText[0].xpos0 = (ST7735_WIDTH - (ST7735_GetFontWidth(*VideoText[0].font)*strlen(VideoText[0].line0)))/2;
	VideoText[0].ypos = 5;
	VideoText[0].fore_color0 = ST7735_WHITE;
	VideoText[0].bkg_color0 = ST7735_BLACK;
	ypos = VideoText[0].ypos + ST7735_GetFontHeigth(*VideoText[0].font) + VMARGIN;

	pot = 1;
	for(i=2;i<LCD_LINES;i++)
	{
		VideoText[0].flag = LCD_HI_SIDE_RIGHT_FLAG;
		VideoText[i].font = &Font_7x10;
		VideoText[i].xpos0 = 2;
		VideoText[i].xpos1 = 98;
		VideoText[i].ypos = ypos;
		VideoText[i].fore_color0 = ST7735_BLUE;
		VideoText[i].bkg_color0  = ST7735_BLACK;
		VideoText[i].fore_color1 = ST7735_BLUE;
		VideoText[i].bkg_color1  = ST7735_BLACK;
		sprintf(VideoText[i].line0, "W%d DeTune",(int )pot);
		sprintf(VideoText[i].line1, "Vol");
		ypos += (ST7735_GetFontHeigth(*VideoText[i].font) + VMARGIN);
		pot++;
	}
}

void LcdInit(void)
{
	InitVideo();
	UpdateAllVideoText();
    setHighLightLine(3);
}


void writeFloatingText(uint16_t x, uint16_t y, uint16_t line , int16_t var)
{
char	varchar[4];
	if ( var > 0 )
		sprintf(varchar, "+%d",var);
	else
		sprintf(varchar, "%d",var);
	ST7735_FillRectangle(x, y, ST7735_GetFontWidth(*VideoText[line].font)*4, ST7735_GetFontHeigth(*VideoText[line].font), ST7735_BLACK);
	ST7735_WriteString(x, y,varchar,*VideoText[line].font,VideoText[line].fore_color0,VideoText[line].bkg_color0);
}

uint16_t	graph_counter = 0 , graph_color=0 , graph_up=1;
void DoGraphicProcess(void)
{
uint16_t	pot=3;
double		norm_percent,detune_delta;
int16_t		detune_var;
	graph_counter++;
	if ( graph_counter > 25 )
	{
		norm_percent = control_buf.ain[1] - 2048;
		detune_delta = (norm_percent / 2048)*100;
		detune_var = (int16_t )detune_delta;
		if ( detune_var < -99 )
			detune_var = -99;
		writeFloatingText(ST7735_GetFontWidth(*VideoText[pot].font)*(strlen(VideoText[pot].line0)+1),VideoText[pot].ypos,pot,detune_var);

		norm_percent = (double )control_buf.ain[0];
		detune_delta = (norm_percent / 4096)*100;
		detune_var = (int16_t )detune_delta;
		writeFloatingText(VideoText[pot].xpos1+(ST7735_GetFontWidth(*VideoText[pot].font)*4),VideoText[pot].ypos,pot,detune_var);
		graph_counter = 0;
		ST7735_FillRectangle(8, 8, 8, 8, graph_color);
		if ( graph_up == 1 )
		{
			graph_color += 0x0020;
			if ( graph_color > 0x07E0 )
			{
				graph_color = 0x07E0;
				graph_up = 0;
				sprintf(VideoText[pot].line1, "Ph");
				setHighLightLine(pot);
			}
		}
		else
		{
			graph_color -= 0x0020;
			if ( graph_color <= 0x020 )
			{
				graph_color = 0x020;
				graph_up = 1;
				sprintf(VideoText[pot].line1, "Vol");
				setHighLightLine(pot);
			}
		}
	}
}
