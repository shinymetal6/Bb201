/*
 * lcd160x80.h
 *
 *  Created on: Jan 21, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_LCD160X80_H_
#define BBAUDIO_INC_LCD160X80_H_

#include "main.h"
#include "fonts.h"
#include "st7735.h"

extern	uint16_t	current_brightness;

#define	LCD_LINES	6
#define VMARGIN 	2

#define	LCD_FULL_FLAG			(1 << 7)
#define	LCD_CENTER_TEXT_FLAG	(1 << 6)
#define	LCD_HI_SIDE_LEFT_FLAG	(1 << 5)
#define	LCD_HI_SIDE_RIGHT_FLAG	(1 << 4)
#define	LCD_REFRESH_LEFT_FLAG	(1 << 3)
#define	LCD_REFRESH_RIGHT_FLAG	(1 << 2)
#define	LCD_NO_FLAG				0

typedef struct _VideoTextFrame {
    uint8_t flag;	// if set it's a full line, uses only xpos0 and line0. An half line uses both xpos0:xpos1, and line0:line1
    uint8_t xpos0;
    uint8_t xpos1;
	uint8_t	ypos;
	FontDef	*font;
    char line0[12],line1[12];
    uint16_t fore_color0,fore_color1;
	uint16_t bkg_color0,bkg_color1;
} VideoTextFrame;

extern	void LcdInit(void);
extern	void UpdateAllVideoText(void);
extern	void DoGraphicProcess(void);

#endif /* BBAUDIO_INC_LCD160X80_H_ */
