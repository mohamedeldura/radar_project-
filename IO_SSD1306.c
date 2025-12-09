/*
 * IO_SSD1306.c
 *
 *  Created on: Jan 3, 2024
 *      Author: logiba
 */

#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation
#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

#define I2C_BUFFER_LENGTH 128
/*
 */

/*
 * OLED_ClassV1.cpp
 *
 *  Created on: Nov 22, 2018
 *      Author: ASUS
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// In order include HAL definitions we include main.h file.
#include "main.h"
#include "IO_SSD1306.h"
#include "splash.h"

#ifndef __STM32F4xx_HAL_I2C_H
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	return HAL_ERROR;
}
#endif
void OLED_Construct(OLED *oled, OledChipEN chipModel, uint8_t addr, uint16_t width, uint16_t height) {
	// Graphics
	oled->_width = oled->WIDTH = width;
	oled->_height = oled->HEIGHT = height;
	oled->rotation = 0;
	oled->cursor_y = oled->cursor_x = 0;
	oled->textsize_x = oled->textsize_y = 1;
	oled->textcolor = 0x1;
	oled->textbgcolor = 0x0;
	oled->wrap = true;
	oled->_cp437 = true;
	oled->gfxFont = NULL;
	oled->chip = chipModel;
	if (oled->chip >= CHIP_Generic)
		oled->chip = CHIP_SSD1306;
	// Device
	oled->i2caddr = addr ? addr : 0x78;
	oled->vccstate = SSD1306_SWITCHCAPVCC;
	oled->page_start = 0;

}
uint16_t width(OLED *oled) {
	return oled->_width;
}
uint16_t height(OLED *oled) {
	return oled->_height;
}

// LOW-LEVEL UTILS ---------------------------------------------------------

// Issue single byte out SPI, either soft or hardware as appropriate.
// SPI transaction/selection must be performed in calling function.
/*!
 @brief  Write a single byte to the SPI port.

 @param  d
 Data byte to be written.

 @return void
 @note   See HAVE_PORTREG which defines if the method uses a port or bit-bang
 method
 */
inline void SPIwrite(OLED *oled, uint8_t d) {
	// See Adafruit SSD1306 library
}

void ssd1306_sendByte(OLED *oled, uint8_t initByte, uint8_t c) {
	uint8_t buff[2] = { initByte, c };
	HAL_I2C_Master_Transmit(oled->hi2c, oled->i2caddr, buff, 2, 10);
}

void ssd1306_sendBytes(OLED *oled, uint8_t initByte, const uint8_t *c, uint8_t n) {
	if (oled->hi2c) {
		uint8_t i2c_buffer[I2C_BUFFER_LENGTH + 1];
		i2c_buffer[0] = initByte;
		while (n > I2C_BUFFER_LENGTH) {
			memcpy(i2c_buffer + 1, c, I2C_BUFFER_LENGTH);
			HAL_I2C_Master_Transmit(oled->hi2c, oled->i2caddr, i2c_buffer,
			I2C_BUFFER_LENGTH + 1, 10);
			n -= I2C_BUFFER_LENGTH;
			c += I2C_BUFFER_LENGTH;
		}
		if (n) {
			memcpy(i2c_buffer + 1, c, n);
			HAL_I2C_Master_Transmit(oled->hi2c, oled->i2caddr, i2c_buffer, n + 1, 10);
		}
	}
}

// ALLOCATE & INIT DISPLAY -------------------------------------------------

/*!
 @brief  Allocate RAM for image buffer, initialize peripherals and pins.
 @param  vcs
 VCC selection. Pass SSD1306_SWITCHCAPVCC to generate the display
 voltage (step up) from the 3.3V source, or SSD1306_EXTERNALVCC
 otherwise. Most situations with Adafruit SSD1306 breakouts will
 want SSD1306_SWITCHCAPVCC.
 @param  addr
 I2C address of corresponding SSD1306 display (or pass 0 to use
 default of 0x3C for 128x32 display, 0x3D for all others).
 SPI displays (hardware or software) do not use addresses, but
 this argument is still required (pass 0 or any value really,
 it will simply be ignored). Default if unspecified is 0.
 @return true on successful allocation/init, false otherwise.
 Well-behaved code should check the return value before
 proceeding.
 @note   MUST call this function before any drawing or updates!
 */
bool OLED_Init(OLED *oled, uint8_t vcs, I2C_HandleTypeDef *hi2c, SPI_HandleTypeDef *hspi) {
	// Set i2c device
	oled->hi2c = hi2c;
	oled->hspi = hspi;

	oled->bufLen = oled->WIDTH * ((oled->HEIGHT + 7) / 8);
	oled->buffer = (uint8_t*) malloc(oled->bufLen);
	clearDisplay(oled);

#ifndef SSD1306_SPLASH
	if (oled->HEIGHT > 32) {
		drawBitmap(oled, (oled->WIDTH - splash1_width) / 2, (oled->HEIGHT - splash1_height) / 2, splash1_data,
		splash1_width, splash1_height, SSD1306_WHITE, SSD1306_BLACK);
	} else {
		drawBitmap(oled, (oled->WIDTH - splash2_width) / 2, (oled->HEIGHT - splash2_height) / 2, splash2_data,
		splash2_width, splash2_height, SSD1306_WHITE, SSD1306_BLACK);
	}
#endif

	if (oled->chip == CHIP_SSD1306) {
		oled->vccstate = vcs;
		ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_DISPLAYOFF,
				SSD1306_SETDISPLAYCLOCKDIV, 0x80,
				SSD1306_SETMULTIPLEX }, 4);
		ssd1306_sendByte(oled, 0, oled->HEIGHT - 1);

		ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_SETDISPLAYOFFSET, 0,
				SSD1306_SETSTARTLINE | 0x0, SSD1306_CHARGEPUMP }, 4);
		ssd1306_sendByte(oled, 0, (oled->vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

		ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_MEMORYMODE, 0x00, SSD1306_SEGREMAP | 0x1, SSD1306_COMSCANDEC }, 4);
		ssd1306_sendByte(oled, 0, (oled->vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

		uint8_t comPins = 0x02;
		oled->contrast = 0x0F;
		if ((oled->WIDTH == 128) && (oled->HEIGHT == 32)) {
			comPins = 0x02;
			oled->contrast = 0x8F;
		} else if ((oled->WIDTH == 128) && (oled->HEIGHT == 64)) {
			comPins = 0x12;
			oled->contrast = 0xCF;
		} else if ((oled->WIDTH == 96) && (oled->HEIGHT == 16)) {
			comPins = 0x2;
			oled->contrast = 0xAF;
		}
		ssd1306_sendByte(oled, 0, SSD1306_SETCOMPINS);
		ssd1306_sendByte(oled, 0, comPins);
		ssd1306_sendByte(oled, 0, SSD1306_SETCONTRAST);
		ssd1306_sendByte(oled, 0, oled->contrast);
		ssd1306_sendByte(oled, 0, SSD1306_SETPRECHARGE);
		ssd1306_sendByte(oled, 0, (oled->vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
		ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_SETVCOMDETECT, 0x40,
				SSD1306_DISPLAYALLON_RESUME, SSD1306_NORMALDISPLAY,
				SSD1306_DEACTIVATE_SCROLL, SSD1306_DISPLAYON }, 6);

		invertDisplay(oled, false);
		oled->bInverted = false;
	} else {
		oled->page_start = 2;
		static const uint8_t init[] = {
		SSD1306_DISPLAYOFF,               // 0xAE
				SSD1306_SETDISPLAYCLOCKDIV, 0x80, // 0xD5, 0x80,
				SSD1306_SETMULTIPLEX, 0x3F,       // 0xA8, 0x3F,
				SSD1306_SETDISPLAYOFFSET, 0x00,   // 0xD3, 0x00,
				SSD1306_SETSTARTLINE,             // 0x40
				SSD1306_DCDC, 0x8B,               // DC/DC on
				SSD1306_SEGREMAP + 1,             // 0xA1
				SSD1306_COMSCANDEC,               // 0xC8
				SSD1306_SETCOMPINS, 0x12,         // 0xDA, 0x12,
				SSD1306_SETCONTRAST, 0xFF,        // 0x81, 0xFF
				SSD1306_SETPRECHARGE, 0x1F,       // 0xD9, 0x1F,
				SSD1306_SETVCOMDETECT, 0x40,      // 0xDB, 0x40,
				0x33,                            // Set VPP to 9V
				SSD1306_NORMALDISPLAY,
				SSD1306_MEMORYMODE, 0x10,         // 0x20, 0x00
				SSD1306_DISPLAYALLON_RESUME, };
		ssd1306_sendBytes(oled, 0, init, sizeof(init));
		ssd1306_sendByte(oled, 0, SSD1306_DISPLAYON); // 0xaf
	}
	return true;
}

/*!
 @brief  Set/clear/invert a single pixel. This is also invoked by the
 Adafruit_GFX library in generating many higher-level graphics
 primitives.
 @param  oled
 The display handle.
 @param  x
 Column of display -- 0 at left to (screen width - 1) at right.
 @param  y
 Row of display -- 0 at top to (screen height -1) at bottom.
 @param  color
 Pixel color, one of: SSD1306_BLACK, SSD1306_WHITE or
 SSD1306_INVERSE.
 @return None (void).
 @note   Changes buffer contents only, no immediate effect on display.
 Follow up with a call to display(), or with other graphics
 commands as needed by one's own application.
 */
void drawPixel(OLED *oled, int16_t x, int16_t y, uint16_t color) {
	if ((x >= 0) && (x < oled->_width) && (y >= 0) && (y < oled->_height)) {
		// Pixel is in-bounds. Rotate coordinates if needed.
		switch (oled->rotation) {
		case 1:
			ssd1306_swap(x, y);
			x = oled->WIDTH - x - 1;
			break;
		case 2:
			x = oled->WIDTH - x - 1;
			y = oled->HEIGHT - y - 1;
			break;
		case 3:
			ssd1306_swap(x, y);
			y = oled->HEIGHT - y - 1;
			break;
		}
		switch (color) {
		case SSD1306_WHITE:
			oled->buffer[x + (y / 8) * oled->WIDTH] |= (1 << (y & 7));
			break;
		case SSD1306_BLACK:
			oled->buffer[x + (y / 8) * oled->WIDTH] &= ~(1 << (y & 7));
			break;
		case SSD1306_INVERSE:
			oled->buffer[x + (y / 8) * oled->WIDTH] ^= (1 << (y & 7));
			break;
		}
	}
}

// Clears display buffer
void clearDisplay(OLED *oled) {
	memset(oled->buffer, 0, oled->bufLen);
}

void drawFastHLineInternal(OLED *oled, int16_t x, int16_t y, int16_t w, uint16_t color) {

	if ((y >= 0) && (y < oled->HEIGHT)) { // Y coord in bounds?
		if (x < 0) {                  // Clip left
			w += x;
			x = 0;
		}
		if ((x + w) > oled->WIDTH) { // Clip right
			w = (oled->WIDTH - x);
		}
		if (w > 0) { // Proceed only if width is positive
			uint8_t *pBuf = &oled->buffer[(y / 8) * oled->WIDTH + x], mask = 1 << (y & 7);
			switch (color) {
			case SSD1306_WHITE:
				while (w--) {
					*pBuf++ |= mask;
				}
				break;
			case SSD1306_BLACK:
				mask = ~mask;
				while (w--) {
					*pBuf++ &= mask;
				}
				break;
			case SSD1306_INVERSE:
				while (w--) {
					*pBuf++ ^= mask;
				}
				break;
			}
		}
	}
}

void drawFastVLineInternal(OLED *oled, int16_t x, int16_t __y, int16_t __h, uint16_t color) {

	if ((x >= 0) && (x < oled->WIDTH)) { // X coord in bounds?
		if (__y < 0) {               // Clip top
			__h += __y;
			__y = 0;
		}
		if ((__y + __h) > oled->HEIGHT) { // Clip bottom
			__h = (oled->HEIGHT - __y);
		}
		if (__h > 0) { // Proceed only if height is now positive
			// this display doesn't need ints for coordinates,
			// use local byte registers for faster juggling
			uint8_t y = __y, h = __h;
			uint8_t *pBuf = &oled->buffer[(y / 8) * oled->WIDTH + x];

			// do the first partial byte, if necessary - this requires some masking
			uint8_t mod = (y & 7);
			if (mod) {
				// mask off the high n bits we want to set
				mod = 8 - mod;
				// note - lookup table results in a nearly 10% performance
				// improvement in fill* functions
				// uint8_t mask = ~(0xFF >> mod);
				static const uint8_t premask[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
				uint8_t mask = premask[mod];
				// adjust the mask if we're not going to reach the end of this byte
				if (h < mod)
					mask &= (0XFF >> (mod - h));

				switch (color) {
				case SSD1306_WHITE:
					*pBuf |= mask;
					break;
				case SSD1306_BLACK:
					*pBuf &= ~mask;
					break;
				case SSD1306_INVERSE:
					*pBuf ^= mask;
					break;
				}
				pBuf += oled->WIDTH;
			}

			if (h >= mod) { // More to go?
				h -= mod;
				// Write solid bytes while we can - effectively 8 rows at a time
				if (h >= 8) {
					if (color == SSD1306_INVERSE) {
						// separate copy of the code so we don't impact performance of
						// black/white write version with an extra comparison per loop
						do {
							*pBuf ^= 0xFF; // Invert byte
							pBuf += oled->WIDTH; // Advance pointer 8 rows
							h -= 8;        // Subtract 8 rows from height
						} while (h >= 8);
					} else {
						// store a local value to work with
						uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
						do {
							*pBuf = val;   // Set byte
							pBuf += oled->WIDTH; // Advance pointer 8 rows
							h -= 8;        // Subtract 8 rows from height
						} while (h >= 8);
					}
				}

				if (h) { // Do the final partial byte, if necessary
					mod = h & 7;
					// this time we want to mask the low bits of the byte,
					// vs the high bits we did above
					// uint8_t mask = (1 << mod) - 1;
					// note - lookup table results in a nearly 10% performance
					// improvement in fill* functions
					static const uint8_t postmask[8] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
					uint8_t mask = postmask[mod];
					switch (color) {
					case SSD1306_WHITE:
						*pBuf |= mask;
						break;
					case SSD1306_BLACK:
						*pBuf &= ~mask;
						break;
					case SSD1306_INVERSE:
						*pBuf ^= mask;
						break;
					}
				}
			}
		} // endif positive height
	}   // endif x in bounds
}

void drawFastHLine(OLED *oled, int x, int y, int w, uint16_t color) {
	bool bSwap = false;
	switch (oled->rotation) {
	case 1:
		// 90 degree rotation, swap x & y for rotation, then invert x
		bSwap = true;
		ssd1306_swap(x, y);
		x = oled->WIDTH - x - 1;
		break;
	case 2:
		// 180 degree rotation, invert x and y, then shift y around for height.
		x = oled->WIDTH - x - 1;
		y = oled->HEIGHT - y - 1;
		x -= (w - 1);
		break;
	case 3:
		// 270 degree rotation, swap x & y for rotation,
		// then invert y and adjust y for w (not to become h)
		bSwap = true;
		ssd1306_swap(x, y);
		y = oled->HEIGHT - y - 1;
		y -= (w - 1);
		break;
	}

	if (bSwap)
		drawFastVLineInternal(oled, x, y, w, color);
	else
		drawFastHLineInternal(oled, x, y, w, color);
}

void drawFastVLine(OLED *oled, int x, int y, int h, uint16_t color) {
	bool bSwap = false;
	switch (oled->rotation) {
	case 1:
		// 90 degree rotation, swap x & y for rotation,
		// then invert x and adjust x for h (now to become w)
		bSwap = true;
		ssd1306_swap(x, y);
		x = oled->WIDTH - x - 1;
		x -= (h - 1);
		break;
	case 2:
		// 180 degree rotation, invert x and y, then shift y around for height.
		x = oled->WIDTH - x - 1;
		y = oled->HEIGHT - y - 1;
		y -= (h - 1);
		break;
	case 3:
		// 270 degree rotation, swap x & y for rotation, then invert y
		bSwap = true;
		ssd1306_swap(x, y);
		y = oled->HEIGHT - y - 1;
		break;
	}

	if (bSwap)
		drawFastHLineInternal(oled, x, y, h, color);
	else
		drawFastVLineInternal(oled, x, y, h, color);
}

/*!
 @brief  Return color of a single pixel in display buffer.
 @param  x
 Column of display -- 0 at left to (screen width - 1) at right.
 @param  y
 Row of display -- 0 at top to (screen height -1) at bottom.
 @return true if pixel is set (usually SSD1306_WHITE, unless display invert
 mode is enabled), false if clear (SSD1306_BLACK).
 @note   Reads from buffer contents; may not reflect current contents of
 screen if display() has not been called.
 */
uint16_t getPixel(OLED *oled, int16_t x, int16_t y) {
	if ((x >= 0) && (x < oled->_width) && (y >= 0) && (y < oled->_height)) {
		// Pixel is in-bounds. Rotate coordinates if needed.
		switch (oled->rotation) {
		case 1:
			ssd1306_swap(x, y);
			x = oled->WIDTH - x - 1;
			break;
		case 2:
			x = oled->WIDTH - x - 1;
			y = oled->HEIGHT - y - 1;
			break;
		case 3:
			ssd1306_swap(x, y);
			y = oled->HEIGHT - y - 1;
			break;
		}
		return (((oled->buffer[x + (y / 8) * oled->WIDTH] >> (y & 7)) & 0x01) ^ oled->bInverted) ? SSD1306_BLACK : SSD1306_WHITE;
	}
	return SSD1306_BLACK; // Pixel out of bounds
}

// REFRESH DISPLAY ---------------------------------------------------------

void display(OLED *oled) //I2C_HandleTypeDef *hi2c1
{
	uint8_t *ptr = oled->buffer;
	if (oled->chip == CHIP_SSD1306) {
		uint16_t count = oled->bufLen;
		ssd1306_sendBytes(oled, 0, (uint8_t[] ) {
				SSD1306_PAGEADDR, 0,        // Page start address
						0xFF,           // Page end (not really, but works here)
						SSD1306_COLUMNADDR, 0, oled->WIDTH - 1 } // Column start address
				, 6);

		do {
			if (count > oled->WIDTH) {
				ssd1306_sendBytes(oled, 0x40, ptr, oled->WIDTH);
				ptr += oled->WIDTH;
				count -= oled->WIDTH;
			} else {
				ssd1306_sendBytes(oled, 0x40, ptr, count);
				count = 0;
			}
		} while (count);
	} else {
		const uint8_t pages = ((oled->HEIGHT + 7) / 8);
		for (uint8_t page = 0; page < pages; page++) {
			ssd1306_sendBytes(oled, 0, (uint8_t[] ) { (uint8_t) (SH110X_SETPAGEADDR + page), (uint8_t) (0x10 + ((oled->page_start) >> 4)),
							(uint8_t) ((oled->page_start) & 0xF) }, 3);
			ssd1306_sendBytes(oled, 0x40, ptr, oled->WIDTH);
			ptr += oled->WIDTH;
		}
	}
}
// SCROLLING FUNCTIONS -----------------------------------------------------

/*!
 @brief  Activate a right-handed scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrollright(0x00, 0x0F)
void startscrollright(OLED *oled, uint8_t start, uint8_t stop) {
	static const uint8_t scrollList1a[] = { SSD1306_RIGHT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList1a, sizeof(scrollList1a));
	ssd1306_sendByte(oled, 0, start);
	ssd1306_sendByte(oled, 0, 0X00);
	ssd1306_sendByte(oled, 0, stop);
	static const uint8_t scrollList1b[] = { 0X00, 0XFF,
	SSD1306_ACTIVATE_SCROLL };
	ssd1306_sendBytes(oled, 0, scrollList1b, sizeof(scrollList1b));
}

/*!
 @brief  Activate a left-handed scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrollleft(0x00, 0x0F)
void startscrollleft(OLED *oled, uint8_t start, uint8_t stop) {
	static const uint8_t scrollList1a[] = { SSD1306_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList1a, sizeof(scrollList1a));
	ssd1306_sendByte(oled, 0, start);
	ssd1306_sendByte(oled, 0, 0X00);
	ssd1306_sendByte(oled, 0, stop);
	static const uint8_t scrollList1b[] = { 0X00, 0XFF,
	SSD1306_ACTIVATE_SCROLL };
	ssd1306_sendBytes(oled, 0, scrollList1b, sizeof(scrollList1b));
}

/*!
 @brief  Activate a diagonal scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// display.startscrolldiagright(0x00, 0x0F)
void startscrolldiagright(OLED *oled, uint8_t start, uint8_t stop) {
	static const uint8_t scrollList3a[] = {
	SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList3a, sizeof(scrollList3a));
	ssd1306_sendByte(oled, 0, oled->HEIGHT);
	static const uint8_t scrollList3b[] = {
	SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList3b, sizeof(scrollList3b));
	ssd1306_sendByte(oled, 0, start);
	ssd1306_sendByte(oled, 0, 0X00);
	ssd1306_sendByte(oled, 0, stop);
	static const uint8_t scrollList3c[] = { 0X01, SSD1306_ACTIVATE_SCROLL };
	ssd1306_sendBytes(oled, 0, scrollList3c, sizeof(scrollList3c));

}

/*!
 @brief  Activate alternate diagonal scroll for all or part of the display.
 @param  start
 First row.
 @param  stop
 Last row.
 @return None (void).
 */
// To scroll the whole display, run: display.startscrolldiagleft(0x00, 0x0F)
void startscrolldiagleft(OLED *oled, uint8_t start, uint8_t stop) {

	static const uint8_t scrollList4a[] = {
	SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList4a, sizeof(scrollList4a));
	ssd1306_sendByte(oled, 0, oled->HEIGHT);
	static const uint8_t scrollList4b[] = {
	SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0X00 };
	ssd1306_sendBytes(oled, 0, scrollList4b, sizeof(scrollList4b));
	ssd1306_sendByte(oled, 0, start);
	ssd1306_sendByte(oled, 0, 0X00);
	ssd1306_sendByte(oled, 0, stop);
	static const uint8_t scrollList4c[] = { 0X01, SSD1306_ACTIVATE_SCROLL };
	ssd1306_sendBytes(oled, 0, scrollList4c, sizeof(scrollList4c));

}

/*!
 @brief  Cease a previously-begun scrolling action.
 @return None (void).
 */
void stopscroll(OLED *oled) {

	ssd1306_sendByte(oled, 0, SSD1306_DEACTIVATE_SCROLL);

}

// OTHER HARDWARE SETTINGS -------------------------------------------------
void setContrast(OLED *oled, uint8_t contrast) {
	oled->contrast = contrast;
	ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_SETCONTRAST, contrast }, 2);
}

/*!
 @brief  Dim the display.
 @param  dim
 true to enable lower brightness mode, false for full brightness.
 @return None (void).
 @note   This has an immediate effect on the display, no need to call the
 display() function -- buffer contents are not changed.
 */
void dim(OLED *oled, bool dim) {
	// the range of contrast to too small to be really useful
	// it is useful to dim the display
	ssd1306_sendBytes(oled, 0, (uint8_t[] ) { SSD1306_SETCONTRAST, dim ? 0 : oled->contrast }, 2);
}

void invertDisplay(OLED *oled, bool isInverse) {
	oled->bInverted = isInverse;
	ssd1306_sendByte(oled, 0, isInverse ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}
void toggleDisplay(OLED *oled) {
	oled->bInverted = !oled->bInverted;
	ssd1306_sendByte(oled, 0, oled->bInverted ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void drawFullscreenData(OLED *oled, bool *newFrame) {
	memcpy(oled->buffer, newFrame, oled->bufLen);
	display(oled); //hi2cI	//replaced the below!
}

