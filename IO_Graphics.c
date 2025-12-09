/*
 * IO_Graphics.c
 *
 *  Created on: Jan 9, 2024
 *      Author: logiba
 */

/*
 This is the core graphics library for all our displays, providing a common
 set of graphics primitives (points, lines, circles, etc.).  It needs to be
 paired with a hardware-specific library for each display device we carry
 (to handle the lower-level functions).

 Adafruit invests time and resources providing this open source code, please
 support Adafruit & open-source hardware by purchasing products from Adafruit!

 Copyright (c) 2013 Adafruit Industries.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "IO_SSD1306.h"
static const unsigned char font[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x5B,
		0x4F, 0x5B, 0x3E, 0x3E, 0x6B, 0x4F, 0x6B, 0x3E, 0x1C, 0x3E, 0x7C, 0x3E,
		0x1C, 0x18, 0x3C, 0x7E, 0x3C, 0x18, 0x1C, 0x57, 0x7D, 0x57, 0x1C, 0x1C,
		0x5E, 0x7F, 0x5E, 0x1C, 0x00, 0x18, 0x3C, 0x18, 0x00, 0xFF, 0xE7, 0xC3,
		0xE7, 0xFF, 0x00, 0x18, 0x24, 0x18, 0x00, 0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
		0x30, 0x48, 0x3A, 0x06, 0x0E, 0x26, 0x29, 0x79, 0x29, 0x26, 0x40, 0x7F,
		0x05, 0x05, 0x07, 0x40, 0x7F, 0x05, 0x25, 0x3F, 0x5A, 0x3C, 0xE7, 0x3C,
		0x5A, 0x7F, 0x3E, 0x1C, 0x1C, 0x08, 0x08, 0x1C, 0x1C, 0x3E, 0x7F, 0x14,
		0x22, 0x7F, 0x22, 0x14, 0x5F, 0x5F, 0x00, 0x5F, 0x5F, 0x06, 0x09, 0x7F,
		0x01, 0x7F, 0x00, 0x66, 0x89, 0x95, 0x6A, 0x60, 0x60, 0x60, 0x60, 0x60,
		0x94, 0xA2, 0xFF, 0xA2, 0x94, 0x08, 0x04, 0x7E, 0x04, 0x08, 0x10, 0x20,
		0x7E, 0x20, 0x10, 0x08, 0x08, 0x2A, 0x1C, 0x08, 0x08, 0x1C, 0x2A, 0x08,
		0x08, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 0x30,
		0x38, 0x3E, 0x38, 0x30, 0x06, 0x0E, 0x3E, 0x0E, 0x06, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00,
		0x14, 0x7F, 0x14, 0x7F, 0x14, 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x23, 0x13,
		0x08, 0x64, 0x62, 0x36, 0x49, 0x56, 0x20, 0x50, 0x00, 0x08, 0x07, 0x03,
		0x00, 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, 0x2A,
		0x1C, 0x7F, 0x1C, 0x2A, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x80, 0x70,
		0x30, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x60, 0x60, 0x00,
		0x20, 0x10, 0x08, 0x04, 0x02, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42,
		0x7F, 0x40, 0x00, 0x72, 0x49, 0x49, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D,
		0x33, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3C,
		0x4A, 0x49, 0x49, 0x31, 0x41, 0x21, 0x11, 0x09, 0x07, 0x36, 0x49, 0x49,
		0x49, 0x36, 0x46, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x00,
		0x00, 0x40, 0x34, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14,
		0x14, 0x14, 0x14, 0x00, 0x41, 0x22, 0x14, 0x08, 0x02, 0x01, 0x59, 0x09,
		0x06, 0x3E, 0x41, 0x5D, 0x59, 0x4E, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x7F,
		0x49, 0x49, 0x49, 0x36, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x7F, 0x41, 0x41,
		0x41, 0x3E, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x7F, 0x09, 0x09, 0x09, 0x01,
		0x3E, 0x41, 0x41, 0x51, 0x73, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41,
		0x7F, 0x41, 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22,
		0x41, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x7F,
		0x04, 0x08, 0x10, 0x7F, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x09, 0x09,
		0x09, 0x06, 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46,
		0x26, 0x49, 0x49, 0x49, 0x32, 0x03, 0x01, 0x7F, 0x01, 0x03, 0x3F, 0x40,
		0x40, 0x40, 0x3F, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x3F, 0x40, 0x38, 0x40,
		0x3F, 0x63, 0x14, 0x08, 0x14, 0x63, 0x03, 0x04, 0x78, 0x04, 0x03, 0x61,
		0x59, 0x49, 0x4D, 0x43, 0x00, 0x7F, 0x41, 0x41, 0x41, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x00, 0x41, 0x41, 0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04,
		0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x03, 0x07, 0x08, 0x00, 0x20, 0x54,
		0x54, 0x78, 0x40, 0x7F, 0x28, 0x44, 0x44, 0x38, 0x38, 0x44, 0x44, 0x44,
		0x28, 0x38, 0x44, 0x44, 0x28, 0x7F, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00,
		0x08, 0x7E, 0x09, 0x02, 0x18, 0xA4, 0xA4, 0x9C, 0x78, 0x7F, 0x08, 0x04,
		0x04, 0x78, 0x00, 0x44, 0x7D, 0x40, 0x00, 0x20, 0x40, 0x40, 0x3D, 0x00,
		0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, 0x7C, 0x04,
		0x78, 0x04, 0x78, 0x7C, 0x08, 0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44,
		0x38, 0xFC, 0x18, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x18, 0xFC, 0x7C,
		0x08, 0x04, 0x04, 0x08, 0x48, 0x54, 0x54, 0x54, 0x24, 0x04, 0x04, 0x3F,
		0x44, 0x24, 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x1C, 0x20, 0x40, 0x20, 0x1C,
		0x3C, 0x40, 0x30, 0x40, 0x3C, 0x44, 0x28, 0x10, 0x28, 0x44, 0x4C, 0x90,
		0x90, 0x90, 0x7C, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x08, 0x36, 0x41,
		0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 0x02,
		0x01, 0x02, 0x04, 0x02, 0x3C, 0x26, 0x23, 0x26, 0x3C, 0x1E, 0xA1, 0xA1,
		0x61, 0x12, 0x3A, 0x40, 0x40, 0x20, 0x7A, 0x38, 0x54, 0x54, 0x55, 0x59,
		0x21, 0x55, 0x55, 0x79, 0x41, 0x22, 0x54, 0x54, 0x78,
		0x42, // a-umlaut
		0x21, 0x55, 0x54, 0x78, 0x40, 0x20, 0x54, 0x55, 0x79, 0x40, 0x0C, 0x1E,
		0x52, 0x72, 0x12, 0x39, 0x55, 0x55, 0x55, 0x59, 0x39, 0x54, 0x54, 0x54,
		0x59, 0x39, 0x55, 0x54, 0x54, 0x58, 0x00, 0x00, 0x45, 0x7C, 0x41, 0x00,
		0x02, 0x45, 0x7D, 0x42, 0x00, 0x01, 0x45, 0x7C, 0x40, 0x7D, 0x12, 0x11,
		0x12,
		0x7D, // A-umlaut
		0xF0, 0x28, 0x25, 0x28, 0xF0, 0x7C, 0x54, 0x55, 0x45, 0x00, 0x20, 0x54,
		0x54, 0x7C, 0x54, 0x7C, 0x0A, 0x09, 0x7F, 0x49, 0x32, 0x49, 0x49, 0x49,
		0x32, 0x3A, 0x44, 0x44, 0x44,
		0x3A, // o-umlaut
		0x32, 0x4A, 0x48, 0x48, 0x30, 0x3A, 0x41, 0x41, 0x21, 0x7A, 0x3A, 0x42,
		0x40, 0x20, 0x78, 0x00, 0x9D, 0xA0, 0xA0, 0x7D, 0x3D, 0x42, 0x42, 0x42,
		0x3D, // O-umlaut
		0x3D, 0x40, 0x40, 0x40, 0x3D, 0x3C, 0x24, 0xFF, 0x24, 0x24, 0x48, 0x7E,
		0x49, 0x43, 0x66, 0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 0xFF, 0x09, 0x29, 0xF6,
		0x20, 0xC0, 0x88, 0x7E, 0x09, 0x03, 0x20, 0x54, 0x54, 0x79, 0x41, 0x00,
		0x00, 0x44, 0x7D, 0x41, 0x30, 0x48, 0x48, 0x4A, 0x32, 0x38, 0x40, 0x40,
		0x22, 0x7A, 0x00, 0x7A, 0x0A, 0x0A, 0x72, 0x7D, 0x0D, 0x19, 0x31, 0x7D,
		0x26, 0x29, 0x29, 0x2F, 0x28, 0x26, 0x29, 0x29, 0x29, 0x26, 0x30, 0x48,
		0x4D, 0x40, 0x20, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x38, 0x2F, 0x10, 0xC8, 0xAC, 0xBA, 0x2F, 0x10, 0x28, 0x34, 0xFA, 0x00,
		0x00, 0x7B, 0x00, 0x00, 0x08, 0x14, 0x2A, 0x14, 0x22, 0x22, 0x14, 0x2A,
		0x14, 0x08, 0x55, 0x00, 0x55, 0x00,
		0x55, // #176 (25% block) missing in old
			  // code
		0xAA, 0x55, 0xAA, 0x55,
		0xAA,             // 50% block
		0xFF, 0x55, 0xFF, 0x55,
		0xFF,             // 75% block
		0x00, 0x00, 0x00, 0xFF, 0x00, 0x10, 0x10, 0x10, 0xFF, 0x00, 0x14, 0x14,
		0x14, 0xFF, 0x00, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x10, 0x10, 0xF0, 0x10,
		0xF0, 0x14, 0x14, 0x14, 0xFC, 0x00, 0x14, 0x14, 0xF7, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0xFF, 0x14, 0x14, 0xF4, 0x04, 0xFC, 0x14, 0x14, 0x17,
		0x10, 0x1F, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0x1F, 0x00,
		0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10,
		0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x00, 0x00, 0x00, 0xFF,
		0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x00,
		0x00, 0x00, 0xFF, 0x14, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x1F,
		0x10, 0x17, 0x00, 0x00, 0xFC, 0x04, 0xF4, 0x14, 0x14, 0x17, 0x10, 0x17,
		0x14, 0x14, 0xF4, 0x04, 0xF4, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14,
		0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, 0x14, 0x17,
		0x14, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0xF4, 0x14, 0x10,
		0x10, 0xF0, 0x10, 0xF0, 0x00, 0x00, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00,
		0x1F, 0x14, 0x00, 0x00, 0x00, 0xFC, 0x14, 0x00, 0x00, 0xF0, 0x10, 0xF0,
		0x10, 0x10, 0xFF, 0x10, 0xFF, 0x14, 0x14, 0x14, 0xFF, 0x14, 0x10, 0x10,
		0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x38, 0x44, 0x44,
		0x38, 0x44, 0xFC, 0x4A, 0x4A, 0x4A,
		0x34, // sharp-s or beta
		0x7E, 0x02, 0x02, 0x06, 0x06, 0x02, 0x7E, 0x02, 0x7E, 0x02, 0x63, 0x55,
		0x49, 0x41, 0x63, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x40, 0x7E, 0x20, 0x1E,
		0x20, 0x06, 0x02, 0x7E, 0x02, 0x02, 0x99, 0xA5, 0xE7, 0xA5, 0x99, 0x1C,
		0x2A, 0x49, 0x2A, 0x1C, 0x4C, 0x72, 0x01, 0x72, 0x4C, 0x30, 0x4A, 0x4D,
		0x4D, 0x30, 0x30, 0x48, 0x78, 0x48, 0x30, 0xBC, 0x62, 0x5A, 0x46, 0x3D,
		0x3E, 0x49, 0x49, 0x49, 0x00, 0x7E, 0x01, 0x01, 0x01, 0x7E, 0x2A, 0x2A,
		0x2A, 0x2A, 0x2A, 0x44, 0x44, 0x5F, 0x44, 0x44, 0x40, 0x51, 0x4A, 0x44,
		0x40, 0x40, 0x44, 0x4A, 0x51, 0x40, 0x00, 0x00, 0xFF, 0x01, 0x03, 0xE0,
		0x80, 0xFF, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x6B, 0x08, 0x36, 0x12, 0x36,
		0x24, 0x36, 0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00,
		0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x40, 0xFF, 0x01, 0x01, 0x00, 0x1F,
		0x01, 0x01, 0x1E, 0x00, 0x19, 0x1D, 0x17, 0x12, 0x00, 0x3C, 0x3C, 0x3C,
		0x3C, 0x00, 0x00, 0x00, 0x00, 0x00 // #255 NBSP
		};

// allow clean compilation with [-Wunused-const-variable=] and [-Wall]
static inline void avoid_unused_const_variable_compiler_warning(void) {
	(void) font;
}



/**************************************************************************/
/*!
 @brief    Fill a rectangle completely with one color. Update in subclasses if
 desired!
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    color 16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillRect(OLED *oled, int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	for (int16_t i = x; i < x + w; i++) {
		drawFastVLine(oled, i, y, h, color);
	}
}

/**************************************************************************/
/*!
 @brief    Fill the screen completely with one color. Update in subclasses if
 desired!
 @param    color 16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillScreen(OLED *oled, uint16_t color) {
	if (color)
		memset(oled->buffer, 0xFF, oled->bufLen);
	else
		memset(oled->buffer, 0, oled->bufLen);
}

/**************************************************************************/
/*!
 @brief    Draw a line
 @param    x0  Start point x coordinate
 @param    y0  Start point y coordinate
 @param    x1  End point x coordinate
 @param    y1  End point y coordinate
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawLine(OLED *oled, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		uint16_t color) {
	if (x0 != x1 && y0 != y1) {
		int16_t steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep) {
			_swap_int16_t(x0, y0);
			_swap_int16_t(x1, y1);
		}

		if (x0 > x1) {
			_swap_int16_t(x0, x1);
			_swap_int16_t(y0, y1);
		}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if (y0 < y1)
			ystep = 1;
		else
			ystep = -1;
		for (; x0 <= x1; x0++) {
			if (steep)
				drawPixel(oled, y0, x0, color);
			else
				drawPixel(oled, x0, y0, color);
			err -= dy;
			if (err < 0) {
				y0 += ystep;
				err += dx;
			}
		}
		return;
	}
	if (x0 == x1) {
		if (y0 > y1)
			_swap_int16_t(y0, y1);
		drawFastVLine(oled, x0, y0, y1 - y0 + 1, color);
		return;
	}
	// y0 = y1
	if (x0 > x1)
		_swap_int16_t(x0, x1);
	drawFastHLine(oled, x0, y0, x1 - x0 + 1, color);
}

/**************************************************************************/
/*!
 @brief    Draw a circle outline
 @param    x0   Center-point x coordinate
 @param    y0   Center-point y coordinate
 @param    r   Radius of circle
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawCircle(OLED *oled, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
#if defined(ESP8266)
  yield();
#endif
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(oled, x0, y0 + r, color);
	drawPixel(oled, x0, y0 - r, color);
	drawPixel(oled, x0 + r, y0, color);
	drawPixel(oled, x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(oled, x0 + x, y0 + y, color);
		drawPixel(oled, x0 - x, y0 + y, color);
		drawPixel(oled, x0 + x, y0 - y, color);
		drawPixel(oled, x0 - x, y0 - y, color);
		drawPixel(oled, x0 + y, y0 + x, color);
		drawPixel(oled, x0 - y, y0 + x, color);
		drawPixel(oled, x0 + y, y0 - x, color);
		drawPixel(oled, x0 - y, y0 - x, color);
	}
}

/**************************************************************************/
/*!
 @brief    Quarter-circle drawer, used to do circles and roundrects
 @param    x0   Center-point x coordinate
 @param    y0   Center-point y coordinate
 @param    r   Radius of circle
 @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
 the circle we're doing
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawCircleHelper(OLED *oled, int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(oled, x0 + x, y0 + y, color);
			drawPixel(oled, x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(oled, x0 + x, y0 - y, color);
			drawPixel(oled, x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(oled, x0 - y, y0 + x, color);
			drawPixel(oled, x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(oled, x0 - y, y0 - x, color);
			drawPixel(oled, x0 - x, y0 - y, color);
		}
	}
}

/**************************************************************************/
/*!
 @brief  Quarter-circle drawer with fill, used for circles and roundrects
 @param  x0       Center-point x coordinate
 @param  y0       Center-point y coordinate
 @param  r        Radius of circle
 @param  corners  Mask bits indicating which quarters we're doing
 @param  delta    Offset from center-point, used for round-rects
 @param  color    16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircleHelper(OLED *oled, int16_t x0, int16_t y0, int16_t r,
		uint8_t corners, int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	int16_t px = x;
	int16_t py = y;

	delta++; // Avoid some +1's in the loop

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		// These checks avoid double-drawing certain lines, important
		// for the SSD1306 library which has an INVERT drawing mode.
		if (x < (y + 1)) {
			if (corners & 1)
				drawFastVLine(oled, x0 + x, y0 - y, 2 * y + delta, color);
			if (corners & 2)
				drawFastVLine(oled, x0 - x, y0 - y, 2 * y + delta, color);
		}
		if (y != py) {
			if (corners & 1)
				drawFastVLine(oled, x0 + py, y0 - px, 2 * px + delta, color);
			if (corners & 2)
				drawFastVLine(oled, x0 - py, y0 - px, 2 * px + delta, color);
			py = y;
		}
		px = x;
	}
}
// Function to implement scan-line polygon filling
void fillPolygon(OLED* oled, uint16_t *x, uint16_t *y, uint16_t edges, uint16_t color) {
	uint16_t i, j, temp;
	uint16_t ymin = oled->_height, ymax = 0;

    // Find the minimum and maximum x-coordinates of the polygon
    for (i = 0; i < edges; i++)
    {
        if (y[i] < ymin)
            ymin = y[i];
        if (y[i] > ymax)
            ymax = y[i];
    }

    // Scan each scan-line within the polygon's vertical extent
	uint16_t *interPoints = calloc(edges,sizeof(uint16_t));
    for (i = ymin; i <= ymax; i++) {
        // Initialize an array to store the intersection points
    	uint16_t count = 0;

        for (j = 0; j < edges; j++) {
            int next = (j + 1) % edges;

            // Check if the current edge intersects with the scan line
            if ((y[j] > i && y[next] <= i) || (y[next] > i && y[j] <= i)) {
                interPoints[count++] = x[j] + (i - y[j]) * (x[next] - x[j]) / (y[next] - y[j]);
            }
        }

        // Sort the intersection points in ascending order
        for (j = 0; j < count - 1; j++) {
            for (int k = 0; k < count - j - 1; k++) {
                if (interPoints[k] > interPoints[k + 1]) {
                    temp = interPoints[k];
                    interPoints[k] = interPoints[k + 1];
                    interPoints[k + 1] = temp;
                }
            }
        }

        // Fill the pixels between pairs of intersection points
        for (j = 0; j < count; j += 2)
        {
            drawFastHLine(oled, interPoints[j], i, interPoints[j+1] - interPoints[j] + 1, color);
        }
    }
    free(interPoints);
}

void fillStar(OLED* oled, uint16_t posX, uint16_t posY, uint16_t rotation, uint16_t radius, uint16_t color)
{
	// Define trigonometric variables
	float c36 = cos(M_PI*36.0/180.0);
	float s36 = sin(M_PI*36.0/180.0);
	float c18 = cos(M_PI*18.0/180.0);
	float s18 = sin(M_PI*18.0/180.0);
	float cr  = cos(M_PI*rotation/180.0);
	float sr  = sin(M_PI*rotation/180.0);
	float fx[10], fy[10], tmp;
	uint16_t x[10], y[10], ii;
	fx[0] = radius/c36*s18; fy[0] = 0.0f;
	fx[1] = radius*c36; fy[1] = radius*s36;
	fx[2] = radius/c36*s18*s18; fy[2] = radius/c36*s18*c18;
	fx[3] = -radius*s18; fy[3] = radius*c18;
	fx[4] = -radius*s18; fy[4] = radius/c36*s18*s36;
	fx[5] = -radius; fy[5] = 0.0f;
	fx[6] = fx[4]; fy[6] = -fy[4];
	fx[7] = fx[3]; fy[7] = -fy[3];
	fx[8] = fx[2]; fy[8] = -fy[2];
	fx[9] = fx[1]; fy[9] = -fy[1];
	if((rotation % 360) != 0)
	{
		for(ii=0;ii<10;ii++)
		{
			tmp = cr*fx[ii] - sr*fy[ii];
			fy[ii] = cr*fy[ii] + sr*fx[ii];
			fx[ii] = tmp;
		}
	}
	for(ii=0;ii<10;ii++)
	{
		x[ii] = fx[ii] + posX;
		y[ii] = fy[ii] + posY;
	}
	fillPolygon(oled, x, y, 10, color);
}

void fillCrescent(OLED* oled, uint16_t posX, uint16_t posY, uint16_t rotation, uint16_t radius, uint16_t color)
{

	fillCircle(oled, posX, posY, radius, color);
	if(rotation)
	{
		fillCircle(oled, posX + (radius>>2)*cos(M_PI*rotation/180.0),
				posY + (radius>>2)*sin(M_PI*rotation/180.0), radius*0.8, !color);
	}else
		fillCircle(oled, posX + (radius>>2), posY, radius*0.8, !color);
}

/**************************************************************************/
/*!
 @brief    Draw a circle with filled color
 @param    x0   Center-point x coordinate
 @param    y0   Center-point y coordinate
 @param    r   Radius of circle
 @param    color 16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircle(OLED *oled, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawFastVLine(oled, x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(oled, x0, y0, r, 3, 0, color);
}

/**************************************************************************/
/*!
 @brief   Draw a rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawRect(OLED *oled, int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	drawFastHLine(oled, x, y, w, color);
	drawFastHLine(oled, x, y + h - 1, w, color);
	drawFastVLine(oled, x, y, h, color);
	drawFastVLine(oled, x + w - 1, y, h, color);
}

/**************************************************************************/
/*!
 @brief   Draw a rounded rectangle with no fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawRoundRect(OLED *oled, int16_t x, int16_t y, int16_t w, int16_t h,
		int16_t r, uint16_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if (r > max_radius)
		r = max_radius;
	// smarter version
	drawFastHLine(oled, x + r, y, w - 2 * r, color);         // Top
	drawFastHLine(oled, x + r, y + h - 1, w - 2 * r, color); // Bottom
	drawFastVLine(oled, x, y + r, h - 2 * r, color);         // Left
	drawFastVLine(oled, x + w - 1, y + r, h - 2 * r, color); // Right
	// draw four corners
	drawCircleHelper(oled, x + r, y + r, r, 1, color);
	drawCircleHelper(oled, x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(oled, x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(oled, x + r, y + h - r - 1, r, 8, color);
}

/**************************************************************************/
/*!
 @brief   Draw a rounded rectangle with fill color
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    w   Width in pixels
 @param    h   Height in pixels
 @param    r   Radius of corner rounding
 @param    color 16-bit 5-6-5 Color to draw/fill with
 */
/**************************************************************************/
void fillRoundRect(OLED *oled, int16_t x, int16_t y, int16_t w, int16_t h,
		int16_t r, uint16_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if (r > max_radius)
		r = max_radius;
	// smarter version
	fillRect(oled, x + r, y, w - 2 * r, h, color);
	// draw four corners
	fillCircleHelper(oled, x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(oled, x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/**************************************************************************/
/*!
 @brief   Draw a triangle with no fill color
 @param    x0  Vertex #0 x coordinate
 @param    y0  Vertex #0 y coordinate
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawTriangle(OLED *oled, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {
	drawLine(oled, x0, y0, x1, y1, color);
	drawLine(oled, x1, y1, x2, y2, color);
	drawLine(oled, x2, y2, x0, y0, color);
}

/**************************************************************************/
/*!
 @brief     Draw a triangle with color-fill
 @param    x0  Vertex #0 x coordinate
 @param    y0  Vertex #0 y coordinate
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    color 16-bit 5-6-5 Color to fill/draw with
 */
/**************************************************************************/
void fillTriangle(OLED *oled, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1);
		_swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(oled, a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			_swap_int16_t(a, b);
		drawFastHLine(oled, a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t) dx12 * (y - y1);
	sb = (int32_t) dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			_swap_int16_t(a, b);
		drawFastHLine(oled, a, y, b - a + 1, color);
	}
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/**************************************************************************/
/*!
 @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position,
 using the specified foreground color (unset bits are transparent).
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    bitmap  byte array with monochrome bitmap
 @param    w   Width of bitmap in pixels
 @param    h   Height of bitmap in pixels
 @param    color 16-bit 5-6-5 Color to draw with
 */
/**************************************************************************/
void drawTransparentBitmap(OLED *oled, int16_t x, int16_t y,
		const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t b = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				b <<= 1;
			else
				b = bitmap[j * byteWidth + i / 8];
			if (b & 0x80)
				drawPixel(oled, x + i, y, color);
		}
	}
}

/**************************************************************************/
/*!
 @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position,
 using the specified foreground (for set bits) and background (unset bits)
 colors.
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    bitmap  byte array with monochrome bitmap
 @param    w   Width of bitmap in pixels
 @param    h   Height of bitmap in pixels
 @param    color 16-bit 5-6-5 Color to draw pixels with
 @param    bg 16-bit 5-6-5 Color to draw background with
 */
/**************************************************************************/
void drawBitmap(OLED *oled, int16_t x, int16_t y, const uint8_t *bitmap,
		int16_t w, int16_t h, uint16_t color, uint16_t bg) {

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t b = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				b <<= 1;
			else
				b = bitmap[j * byteWidth + i / 8];
			drawPixel(oled, x + i, y, (b & 0x80) ? color : bg);
		}
	}
}

/**************************************************************************/
/*!
 @brief      Draw RAM-resident XBitMap Files (*.xbm), exported from GIMP.
 Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
 C Array can be directly used with this function.
 There is no RAM-resident version of this function; if generating bitmaps
 in RAM, use the format defined by drawBitmap() and call that instead.
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    bitmap  byte array with monochrome bitmap
 @param    w   Width of bitmap in pixels
 @param    h   Height of bitmap in pixels
 @param    color 16-bit 5-6-5 Color to draw pixels with
 */
/**************************************************************************/
void drawXBitmap(OLED *oled, int16_t x, int16_t y, const uint8_t bitmap[],
		int16_t w, int16_t h, uint16_t color) {

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t b = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				b >>= 1;
			else
				b = bitmap[j * byteWidth + i / 8];
			// Nearly identical to drawBitmap(), only the bit order
			// is reversed here (left-to-right = LSB to MSB):
			if (b & 0x01)
				drawPixel(oled, x + i, y, color);
		}
	}
}

/**************************************************************************/
/*!
 @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
 pos. Specifically for 8-bit display devices such as IS31FL3731; no color
 reduction/expansion is performed.
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    bitmap  byte array with grayscale bitmap
 @param    w   Width of bitmap in pixels
 @param    h   Height of bitmap in pixels
 */
/**************************************************************************/
void drawGrayscaleBitmap(OLED *oled, int16_t x, int16_t y, uint8_t *bitmap,
		int16_t w, int16_t h) {
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			drawPixel(oled, x + i, y, bitmap[j * w + i]);
		}
	}
}

/**************************************************************************/
/*!
 @brief   Draw a RAM-resident 8-bit image (grayscale) with a 1-bit mask
 (set bits = opaque, unset bits = clear) at the specified (x,y) position.
 BOTH buffers (grayscale and mask) must be RAM-residentt, no mix-and-match
 Specifically for 8-bit display devices such as IS31FL3731; no color
 reduction/expansion is performed.
 @param    x   Top left corner x coordinate
 @param    y   Top left corner y coordinate
 @param    bitmap  byte array with grayscale bitmap
 @param    mask  byte array with mask bitmap
 @param    w   Width of bitmap in pixels
 @param    h   Height of bitmap in pixels
 */
/**************************************************************************/
void drawTransparentGrayscaleBitmap(OLED *oled, int16_t x, int16_t y,
		uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h) {
	int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
	uint8_t b = 0;
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				b <<= 1;
			else
				b = mask[j * bw + i / 8];
			if (b & 0x80) {
				drawPixel(oled, x + i, y, bitmap[j * w + i]);
			}
		}
	}
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character

// Draw a character
/**************************************************************************/
/*!
 @brief   Draw a single character
 @param    x   Bottom left corner x coordinate
 @param    y   Bottom left corner y coordinate
 @param    c   The 8-bit font-indexed character (likely ascii)
 @param    color 16-bit 5-6-5 Color to draw chraracter with
 @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
 no background)
 @param    size_x  Font magnification level in X-axis, 1 is 'original' size
 @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
 */
/**************************************************************************/
void drawChar(OLED *oled, int16_t x, int16_t y, unsigned char c, uint16_t color,
		uint16_t bg, uint8_t size_x, uint8_t size_y) {

	if (!oled->gfxFont) { // 'Classic' built-in font

		if ((x >= oled->_width) ||              // Clip right
				(y >= oled->_height) ||             // Clip bottom
				((x + 6 * size_x - 1) < 0) || // Clip left
				((y + 8 * size_y - 1) < 0))   // Clip top
			return;

		if (!oled->_cp437 && (c >= 176))
			c++; // Handle 'classic' charset behavior

		for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
			uint8_t line = font[c * 5 + i];
			for (int8_t j = 0; j < 8; j++, line >>= 1) {
				if (line & 1) {
					if (size_x == 1 && size_y == 1)
						drawPixel(oled, x + i, y + j, color);
					else
						fillRect(oled, x + i * size_x, y + j * size_y, size_x,
								size_y, color);
				} else if (bg != color) {
					if (size_x == 1 && size_y == 1)
						drawPixel(oled, x + i, y + j, bg);
					else
						fillRect(oled, x + i * size_x, y + j * size_y, size_x,
								size_y, bg);
				}
			}
		}
		if (bg != color) { // If opaque, draw vertical line for last column
			if (size_x == 1 && size_y == 1)
				drawFastVLine(oled, x + 5, y, 8, bg);
			else
				fillRect(oled, x + 5 * size_x, y, size_x, 8 * size_y, bg);
		}

	} else { // Custom font

		// Character is assumed previously filtered by write() to eliminate
		// newlines, returns, non-printable characters, etc.  Calling
		// drawChar() directly with 'bad' characters of font may cause mayhem!

		c -= (uint8_t) oled->gfxFont->first;
		GFXglyph *glyph = oled->gfxFont->glyph + c;
		uint8_t *bitmap = oled->gfxFont->bitmap;

		uint16_t bo = glyph->bitmapOffset;
		uint8_t xx, yy, bits = 0, bit = 0;

		// Todo: Add character clipping here

		// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
		// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
		// has typically been used with the 'classic' font to overwrite old
		// screen contents with new data.  This ONLY works because the
		// characters are a uniform size; it's not a sensible thing to do with
		// proportionally-spaced fonts with glyphs of varying sizes (and that
		// may overlap).  To replace previously-drawn text when using a custom
		// font, use the getTextBounds() function to determine the smallest
		// rectangle encompassing a string, erase the area with fillRect(),
		// then draw new text.  This WILL unfortunately 'blink' the text, but
		// is unavoidable.  Drawing 'background' pixels will NOT fix this,
		// only creates a new set of problems.  Have an idea to work around
		// this (a canvas object type for MCUs that can afford the RAM and
		// displays supporting setAddrWindow() and pushColors()), but haven't
		// implemented this yet.

		for (yy = 0; yy < glyph->height; yy++) {
			for (xx = 0; xx < glyph->width; xx++) {
				if (!(bit++ & 7))
					bits = bitmap[bo++];
				if (bits & 0x80) {
					if (size_x == 1 && size_y == 1)
						drawPixel(oled, x + glyph->xOffset + xx,
								y + glyph->yOffset + yy, color);
					else
						fillRect(oled,
								x + ((uint16_t) glyph->xOffset + xx) * size_x,
								y + ((uint16_t) glyph->yOffset + yy) * size_y,
								size_x, size_y, color);
				}
				bits <<= 1;
			}
		}
	} // End classic vs custom font
}
void printText(OLED *oled, const char *text) {
	const char *ptr = text;
	while (*ptr)
		write(oled, *(ptr++));
}
/**************************************************************************/
/*!
 @brief  Print one byte/character of data, used to support print()
 @param  c  The 8-bit ascii character to write
 */
/**************************************************************************/
size_t write(OLED *oled, uint8_t c) {
	if (!oled->gfxFont) {
		// 'Classic' built-in font
		if (c == '\n') {              // Newline?
			oled->cursor_x = 0;               // Reset x to zero,
			oled->cursor_y += oled->textsize_y * 8; // advance y one line
		} else if (c != '\r') {       // Ignore carriage returns
			if (oled->wrap
					&& ((oled->cursor_x + oled->textsize_x * 6) > oled->_width)) { // Off right?
				oled->cursor_x = 0;                          // Reset x to zero,
				oled->cursor_y += oled->textsize_y * 8; // advance y one line
			}
			drawChar(oled, oled->cursor_x, oled->cursor_y, c, oled->textcolor,
					oled->textbgcolor, oled->textsize_x, oled->textsize_y);
			oled->cursor_x += oled->textsize_x * 6; // Advance x one char
		}
	} else { // Custom font

		if (c == '\n') {
			oled->cursor_x = 0;
			oled->cursor_y += (int16_t) oled->textsize_y
					* oled->gfxFont->yAdvance;
		} else if (c != '\r') {
			uint8_t first = (uint8_t)oled->gfxFont->first;
			if ((c >= first)
					&& (c <= (uint8_t)oled->gfxFont->last)) {
				GFXglyph *glyph = oled->gfxFont->glyph + c - first;
				if ((glyph->width > 0) && (glyph->height > 0)) { // Is there an associated bitmap?
					if (oled->wrap &&
							((oled->cursor_x + oled->textsize_x * ((int16_t)glyph->xOffset + glyph->width)) > oled->_width))
					{
						oled->cursor_x = 0;
						oled->cursor_y += (int16_t) oled->textsize_y * oled->gfxFont->yAdvance;
					}
					drawChar(oled, oled->cursor_x, oled->cursor_y, c,
							oled->textcolor, oled->textbgcolor,
							oled->textsize_x, oled->textsize_y);
				}
				oled->cursor_x += glyph->xAdvance * (int16_t) oled->textsize_x;
			}
		}
	}
	return 1;
}

/**************************************************************************/
/*!
 @brief   Set text 'magnification' size. Each increase in s makes 1 pixel
 that much bigger.
 @param  s_x  Desired text width magnification level in X-axis. 1 is default
 @param  s_y  Desired text width magnification level in Y-axis. 1 is default
 */
/**************************************************************************/
void setTextSize2(OLED *oled, uint8_t s_x, uint8_t s_y) {
	oled->textsize_x = (s_x > 0) ? s_x : 1;
	oled->textsize_y = (s_y > 0) ? s_y : 1;
}
void setTextSize(OLED *oled, uint8_t txtSize) {
	oled->textsize_x = (txtSize > 0) ? txtSize : 1;
	oled->textsize_y = oled->textsize_x;
}
void setTextColor(OLED *oled, uint16_t c) {
	oled->textcolor = c;
}
void setTextBGColor(OLED *oled, uint16_t bg) {
	oled->textbgcolor = bg;
}
void setTextWrap(OLED *oled, bool w) {
	oled->wrap = w;
}
void setCursor(OLED *oled, int16_t x, int16_t y) {
	oled->cursor_x = x;
	oled->cursor_y = y;
}
void cp437(OLED *oled, bool x) {
	oled->_cp437 = x;
}
;
/**************************************************************************/
/*!
 @brief      Set rotation setting for display
 @param  x   0 thru 3 corresponding to 4 cardinal rotations
 */
/**************************************************************************/
void setRotation(OLED *oled, uint8_t x) {
	oled->rotation = (x & 3);
	switch (oled->rotation) {
	case 0:
	case 2:
		oled->_width = oled->WIDTH;
		oled->_height = oled->HEIGHT;
		break;
	case 1:
	case 3:
		oled->_width = oled->HEIGHT;
		oled->_height = oled->WIDTH;
		break;
	}
}

/**************************************************************************/
/*!
 @brief Set the font to display when print()ing, either custom or default
 @param  f  The GFXfont object, if NULL use built in 6x8 font
 */
/**************************************************************************/
void setFont(OLED *oled, const GFXfont *f) {
	if (f) {          // Font struct pointer passed in?
		// And no current font struct?
		// Switching from classic to new font behavior.
		// Move cursor pos down 6 pixels so it's on baseline.
		if (oled->gfxFont)
			oled->cursor_y +=  f->yAdvance - oled->gfxFont->yAdvance;
		else
			oled->cursor_y +=  f->yAdvance - 6;
	}
	else if (oled->gfxFont)
	{
		// NULL passed.  Current font struct defined?
		// Switching from new to classic font behavior.
		// Move cursor pos up 6 pixels so it's at top-left of char.
		oled->cursor_y -= oled->gfxFont->yAdvance - 6;
	}
	oled->gfxFont = (GFXfont*) f;
}

/**************************************************************************/
/*!
 @brief  Helper to determine size of a character with current font/size.
 Broke this out as it's used by both the PROGMEM- and RAM-resident
 getTextBounds() functions.
 @param  c     The ASCII character in question
 @param  x     Pointer to x location of character. Value is modified by
 this function to advance to next character.
 @param  y     Pointer to y location of character. Value is modified by
 this function to advance to next character.
 @param  minx  Pointer to minimum X coordinate, passed in to AND returned
 by this function -- this is used to incrementally build a
 bounding rectangle for a string.
 @param  miny  Pointer to minimum Y coord, passed in AND returned.
 @param  maxx  Pointer to maximum X coord, passed in AND returned.
 @param  maxy  Pointer to maximum Y coord, passed in AND returned.
 */
/**************************************************************************/
void charBounds(OLED *oled, unsigned char c, int16_t *x, int16_t *y,
		int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {

	if (oled->gfxFont) {

		if (c == '\n') { // Newline?
			*x = 0;        // Reset x to zero, advance y by one line
			*y += oled->textsize_y * oled->gfxFont->yAdvance;
		} else if (c != '\r') { // Not a carriage return; is normal char
			if ((c >= oled->gfxFont->first) && (c <= oled->gfxFont->last)) { // Char present in this font?
				GFXglyph *glyph = oled->gfxFont->glyph + c - (uint8_t)oled->gfxFont->first;
				if (oled->wrap
						&& ((*x + (((int16_t) glyph->xOffset + glyph->width) * oled->textsize_x))
								> oled->_width)) {
					*x = 0; // Reset x to zero, advance y by one line
					*y += oled->textsize_y * oled->gfxFont->yAdvance;
				}
				int16_t tsx = (int16_t) oled->textsize_x, tsy =
						(int16_t) oled->textsize_y, x1 = *x + glyph->xOffset * tsx, y1 = *y
						+ glyph->yOffset * tsy, x2 = x1 + glyph->width * tsx - 1, y2 = y1 + glyph->height * tsy
						- 1;
				if (x1 < *minx)
					*minx = x1;
				if (y1 < *miny)
					*miny = y1;
				if (x2 > *maxx)
					*maxx = x2;
				if (y2 > *maxy)
					*maxy = y2;
				*x += glyph->xAdvance * tsx;
			}
		}

	} else { // Default font

		if (c == '\n') {        // Newline?
			*x = 0;               // Reset x to zero,
			*y += oled->textsize_y * 8; // advance y one line
			// min/max x/y unchanged -- that waits for next 'normal' character
		} else if (c != '\r') { // Normal char; ignore carriage returns
			if (oled->wrap && ((*x + oled->textsize_x * 6) > oled->_width)) { // Off right?
				*x = 0;                                      // Reset x to zero,
				*y += oled->textsize_y * 8;                // advance y one line
			}
			int x2 = *x + oled->textsize_x * 6 - 1, // Lower-right pixel of char
			y2 = *y + oled->textsize_y * 8 - 1;
			if (x2 > *maxx)
				*maxx = x2; // Track max x, y
			if (y2 > *maxy)
				*maxy = y2;
			if (*x < *minx)
				*minx = *x; // Track min x, y
			if (*y < *miny)
				*miny = *y;
			*x += oled->textsize_x * 6; // Advance x one char
		}
	}
}

/**************************************************************************/
/*!
 @brief  Helper to determine size of a string with current font/size.
 Pass string and a cursor position, returns UL corner and W,H.
 @param  str  The ASCII string to measure
 @param  x    The current cursor X
 @param  y    The current cursor Y
 @param  x1   The boundary X coordinate, returned by function
 @param  y1   The boundary Y coordinate, returned by function
 @param  w    The boundary width, returned by function
 @param  h    The boundary height, returned by function
 */
/**************************************************************************/
void getTextBounds(OLED *oled, const char *str, int16_t x, int16_t y,
		int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {

	uint8_t c; // Current character
	int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
	// Bound rect is intentionally initialized inverted, so 1st char sets it

	*x1 = x; // Initial position is value passed in
	*y1 = y;
	*w = *h = 0; // Initial size is zero

	while ((c = *str++)) {
		// charBounds() modifies x/y to advance for each character,
		// and min/max x/y are updated to incrementally build bounding rect.
		charBounds(oled, c, &x, &y, &minx, &miny, &maxx, &maxy);
	}

	if (maxx >= minx) {     // If legit string bounds were found...
		*x1 = minx;           // Update x1 to least X coord,
		*w = maxx - minx + 1; // And w to bound rect width
	}
	if (maxy >= miny) { // Same for height
		*y1 = miny;
		*h = maxy - miny + 1;
	}
}

