/*
 * IO_SSD1306.h
 *
 *  Created on: Jan 3, 2024
 *      Author: Ibrahim Onaran
 *  Dec 10, 2025 Add SH110X support
 *     
 */

#ifndef SSD1306_IO_SSD1306_H_
#define SSD1306_IO_SSD1306_H_


//Includes
#include <stdint.h>
#include <stdbool.h>
/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SH110X_SETPAGEADDR 0xB0			 ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
    #define SSD1306_DCDC 0xAD            ///< See datasheet SH110X															   
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
    #define SSD1306_SETPAGEADDR                                                     \
      0xB0 ///< Specify page address to load display RAM data to page address
           ///< register SH110X
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet
#define SSD1306_SETDISPSTARTLINE                                                \
  0xDC ///< Specify Column address to determine the initial display line or
       ///< COM0. SH110X


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

#define SSD1306_EXTERNALVCC 0x01  		 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 		  ///< Gen. display voltage from 3.3V

#ifndef __STM32F4xx_HAL_I2C_H
typedef struct {
	int __DUMMY__;
}I2C_HandleTypeDef;
#endif
#ifndef STM32F4xx_HAL_SPI_H
typedef struct{
	int __DUMMY__;
}SPI_HandleTypeDef;
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

/// Font data stored PER GLYPH
typedef struct {
  uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
  uint8_t width;         ///< Bitmap dimensions in pixels
  uint8_t height;        ///< Bitmap dimensions in pixels
  uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
  int8_t xOffset;        ///< X dist from cursor pos to UL corner
  int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
  uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
  GFXglyph *glyph;  ///< Glyph array
  uint16_t first;   ///< ASCII extents (first char)
  uint16_t last;    ///< ASCII extents (last char)
  uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;
enum ENOledChip
{
	CHIP_SSD1306,
	CHIP_SH_1106,
	CHIP_SH_1107,
	CHIP_Generic
};
typedef enum ENOledChip OledChipEN;
struct OLEDS
{
	// Graphics
	int16_t WIDTH;        ///< This is the 'raw' display width - never changes
	int16_t HEIGHT;       ///< This is the 'raw' display height - never changes
	int16_t _width;       ///< Display width as modified by current rotation
	int16_t _height;      ///< Display height as modified by current rotation
	int16_t cursor_x;     ///< x location to start print()ing text
	int16_t cursor_y;     ///< y location to start print()ing text
	uint16_t textcolor;   ///< 16-bit background color for print()
	uint16_t textbgcolor; ///< 16-bit text color for print()
	uint8_t textsize_x;   ///< Desired magnification in X-axis of text to print()
	uint8_t textsize_y;   ///< Desired magnification in Y-axis of text to print()
	uint8_t rotation;     ///< Display rotation (0 thru 3)
	bool wrap;            ///< If set, 'wrap' text at right edge of display
	bool _cp437;          ///< If set, use correct CP437 charset (default is off)
	GFXfont *gfxFont;     ///< Pointer to special font


	OledChipEN chip;	///< The chip model that the display uses.
	bool bInverted;
	uint8_t* buffer;	//kept public because of big size, check for alternative
	uint32_t bufLen; 	//length of the buffer

	// Device related variables
	I2C_HandleTypeDef *hi2c;
	SPI_HandleTypeDef *hspi;
	uint8_t i2caddr; // = 0x78;
	uint8_t contrast;
	uint8_t vccstate;
	  /*! some displays are 'inset' in memory, so we have to skip some memory to
	   * display */
	uint8_t page_start;
};
typedef struct OLEDS OLED;

// Constructs the OLED object, use OLED_Init to initialize it
void OLED_Construct(OLED* oled, OledChipEN chipModel, uint8_t addr, uint16_t width, uint16_t height);
// Initialize the OLED, if SPI is used make hi2c NULL.
bool OLED_Init(OLED *oled, uint8_t vcs, I2C_HandleTypeDef *hi2c, SPI_HandleTypeDef* hspi);
// De-initialize the device
void OLED_DeInit(OLED *oled);


// The functions that affects the device itself

// Displays the screen by sending buffer data to the device using i2c interface if enabled.
void display(OLED* oled);
// Sets the contrast of the device
void setContrast(OLED* oled, uint8_t contrast);
// Invert the display
void invertDisplay(OLED* oled, bool isInverse);
// Toggles the display
void toggleDisplay(OLED* oled);


// The functions those modifies the screen buffer

// Clears display
void clearDisplay(OLED* oled);	// Clears display buffer
// Fills screen with specified color
void fillScreen(OLED* oled,uint16_t color);
// Draws a pixel on a specified location
void drawPixel(OLED* oled, int16_t x, int16_t y, uint16_t color);
// Draws line fast
void drawFastVLine(OLED* oled,int x, int y, int h, uint16_t color);
void drawFastHLine(OLED* oled,int x, int y, int w, uint16_t color);


// Get the rotated with and height
uint16_t width(OLED* oled);
uint16_t height(OLED* oled);
void setRotation(OLED* oled,uint8_t x);

// Text functions
void setTextSize2(OLED* oled,uint8_t s_x, uint8_t s_y);
void setTextSize(OLED* oled,uint8_t txtSize);
void setTextColor(OLED* oled, uint16_t c);
void setTextBGColor(OLED* oled, uint16_t bg);
void setTextWrap(OLED* oled, bool w);
void cp437(OLED* oled, bool x);
size_t write(OLED* oled,uint8_t c);
void printText(OLED* oled,const char* text);
void setCursor(OLED* oled, int16_t x, int16_t y);
void setFont(OLED *oled, const GFXfont *f);

// Scrolling
void startscrolldiagright(OLED* oled, uint8_t start, uint8_t stop);
void startscrolldiagleft(OLED* oled, uint8_t start, uint8_t stop);
void startscrollleft(OLED* oled, uint8_t start, uint8_t stop);
void startscrollright(OLED* oled, uint8_t start, uint8_t stop);
void stopscroll(OLED* oled);

// Bitmaps
void drawTransparentBitmap(OLED* oled,int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void drawBitmap(OLED* oled,int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void drawXBitmap(OLED* oled,int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);

// Line, Rectangle, Triangle and Circles
void drawLine(OLED* oled,int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawRect(OLED* oled,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawRoundRect(OLED* oled,int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void drawTriangle(OLED* oled,int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void drawCircle(OLED* oled,int16_t x0, int16_t y0, int16_t r, uint16_t color);

void fillRect(OLED* oled,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRoundRect(OLED* oled,int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void fillTriangle(OLED* oled,int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillCircle(OLED* oled,int16_t x0, int16_t y0, int16_t r, uint16_t color);

void fillPolygon(OLED* oled, uint16_t *x, uint16_t *y, uint16_t edges, uint16_t color);
void fillStar(OLED* oled, uint16_t posX, uint16_t posY, uint16_t rotation, uint16_t radius, uint16_t color);
void fillCrescent(OLED* oled, uint16_t posX, uint16_t posY, uint16_t rotation, uint16_t radius, uint16_t color);

#endif /* SSD1306_IO_SSD1306_H_ */
