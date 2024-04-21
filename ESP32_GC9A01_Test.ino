/*  This demo operates by setting directly the display memory.
    Therefore is really slow. Better use TFT_eSPI with virtual memory mapping 
    and fast transfer.
*/

#include <SPI.h>
#include "GC9A01.h"

#define RES 33
#define DC  27
#define CS  5

/*
	scl 	    18
	sda/miso 	19
	dc 	      27
	cs 	      5
	rst 	    33
 
  *** my alterantive configuration is using overload (miso not connected):
  SPI.begin(18,-1,19,5);  //parameters: sclk,miso,mosi,ss => SPI.begin() does not operate!

	ESP32 Default SPI Pins:
    Many ESP32 boards come with default SPI pins pre-assigned. The pin mapping for most boards is as follows:
    SPI	  	MOSI	| MISO		| SCLK		| CS
 *  VSPI	GPIO 23	| GPIO 19	| GPIO 18	| GPIO 5
    HSPI	GPIO 13	| GPIO 12	| GPIO 14	| GPIO 15

  #warning Info: Here is to find pin declaration for ESP32 with GC9A01.V1
  #define TFT_MOSI 19 //SDA In some display driver board, it might be written as "SDA" and so on.
  #define TFT_SCLK 18
  #define TFT_CS   5  // Chip select control pin
  #define TFT_DC   27  // Data Command control pin
  #define TFT_RST  33  // Reset pin (could connect to Arduino RESET pin)
  #define TFT_BL   -1  // LED back-light 

** C:\Users\jschw\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPI\src\SPI.cpp
  if(sck == -1 && miso == -1 && mosi == -1 && ss == -1) {
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
        _sck = (_spi_num == FSPI) ? SCK : -1;
        _miso = (_spi_num == FSPI) ? MISO : -1;
        _mosi = (_spi_num == FSPI) ? MOSI : -1;
        _ss = (_spi_num == FSPI) ? SS : -1;
#elif CONFIG_IDF_TARGET_ESP32C3
        _sck = SCK;
        _miso = MISO;
        _mosi = MOSI;
        _ss = SS;
#else
        _sck = (_spi_num == VSPI) ? SCK : 14;
        _miso = (_spi_num == VSPI) ? MISO : 12;
        _mosi = (_spi_num == VSPI) ? MOSI : 13;
        _ss = (_spi_num == VSPI) ? SS : 15;
#endif
    } else {
        _sck = sck;
        _miso = miso;
        _mosi = mosi;
        _ss = ss;
    }
*/

void GC9A01_set_reset(uint8_t val) {
    digitalWrite(RES, val);
}

void GC9A01_set_data_command(uint8_t val) {
    digitalWrite(DC, val);
}

void GC9A01_set_chip_select(uint8_t val) {
    digitalWrite(CS, val);
}

void GC9A01_spi_tx(uint8_t *data, size_t len) {
    while (len--) {
        SPI.transfer(*data);
        data++;
    }
}

void GC9A01_delay(uint16_t ms) {
    delay(ms);
}

void setup() {
    
    pinMode(RES, OUTPUT);
    pinMode(DC, OUTPUT);
    pinMode(CS, OUTPUT);
    SPI.begin(18,-1,19,5);  //parameters: sclk,miso,mosi,ss => SPI.begin() does not function.

    GC9A01_init();
    struct GC9A01_frame frame = {{0,0},{239,239}};
    GC9A01_set_frame(frame);
    
}

void loop() {

    uint8_t color[3];

    // Triangle
    color[0] = 0xFF;
    color[1] = 0xFF;
    for (int x = 0; x < 240; x++) {
        for (int y = 0; y < 240; y++) {
            if (x < y) {
                color[2] = 0xFF;
            } else {
                color[2] = 0x00;
            }
            if (x == 0 && y == 0) {
                GC9A01_write(color, sizeof(color));
            } else {
                GC9A01_write_continue(color, sizeof(color));
            }
        }
    }

    delay(1000);

    // Rainbow
    float frequency = 0.026;
    for (int x = 0; x < 240; x++) {
        color[0] = sin(frequency*x + 0) * 127 + 128;
        color[1] = sin(frequency*x + 2) * 127 + 128;
        color[2] = sin(frequency*x + 4) * 127 + 128;
        for (int y = 0; y < 240; y++) {
            if (x == 0 && y == 0) {
                GC9A01_write(color, sizeof(color));
            } else {
                GC9A01_write_continue(color, sizeof(color));
            }
        }
    }

    delay(1000);

    // Checkerboard
    for (int x = 0; x < 240; x++) {
        for (int y = 0; y < 240; y++) {
            if ((x / 10) % 2 ==  (y / 10) % 2) {
                color[0] = 0xFF;
                color[1] = 0xFF;
                color[2] = 0xFF;
            } else {
                color[0] = 0x00;
                color[1] = 0x00;
                color[2] = 0x00;
            }
            if (x == 0 && y == 0) {
                GC9A01_write(color, sizeof(color));
            } else {
                GC9A01_write_continue(color, sizeof(color));
            }
        }
    }

    delay(1000);

    // Swiss flag
    color[0] = 0xFF;
    for (int x = 0; x < 240; x++) {
        for (int y = 0; y < 240; y++) 
        {
            if ((x >= 1*48 && x < 4*48 && y >= 2*48 && y < 3*48) ||
                (x >= 2*48 && x < 3*48 && y >= 1*48 && y < 4*48)) 
            {
                color[1] = 0xFF;
                color[2] = 0xFF;
            } 
            else {
                color[1] = 0x00;
                color[2] = 0x00;
            }
            if (x == 0 && y == 0) {
                GC9A01_write(color, sizeof(color));
            } else {
                GC9A01_write_continue(color, sizeof(color));
            }
        }
    }

    delay(1000);

}
