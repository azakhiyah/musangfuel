#pragma once

// Menggunakan konfigurasi custom dari project ini
#define USER_SETUP_LOADED

// Driver TFT
#define ILI9488_DRIVER

// Resolusi
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

// ESP32 VSPI pin configuration
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS    5
#define TFT_DC   26
#define TFT_RST  25

// SPI frequency
#define SPI_FREQUENCY  27000000

// Font
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF