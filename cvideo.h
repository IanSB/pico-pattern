//
// Title:	        Pico-mposite Video Output
// Author:	        Dean Belfield
// Created:	        26/01/2021
// Last Updated:	01/03/2022
//
// Modinfo:
// 31/01/2022:      Tweaks to reflect code changes
// 02/02/2022:      Added initialise_cvideo
// 04/02/2022:      Added set_border
// 05/02/2022:      Added support for colour composite board
// 20/02/2022:      Bitmap is now dynamically allocated
// 01/03/2022:      Tweaked sync parameters for colour version

#pragma once

#include "config.h"

#define piofreq_0 1.5f       // Clock frequency of state machine for PIO handling sync (24mhz)
#define piofreq_1_192 9.34f
#define piofreq_1_256 7.00f     // Clock frequency of state machine for PIO handling pixel data at various resolutions
#define piofreq_1_320 3.0f   // 24Mhz


#define piofreq_1_320_7MHZ 5.8737f
#define piofreq_1_320_7MHZN 5.8201f

#define piofreq_1_342 5.222f

#define piofreq_1_480 2.0f 

#define piofreq_1_640 1.5f


//#define piofreq_0 5.25f
//#define piofreq_1_320 5.6f
//#define piofreq_1_640 2.80f


#define sm_sync 0               // State machine number in the PIO for the sync data
#define sm_data 1               // State machine number in the PIO for the pixel data

#if opt_colour == 0
    #define colour_base 0x10    // Start colour; for monochrome version this relates to black level voltage
    #define colour_max  0x0f    // Last available colour
    #define HSLO        0x0001
    #define HSHI        0x000d
    #define VSLO        HSLO
    #define VSHI        HSHI
	#define BORD        0x8000
    #define gpio_base   0
    #define gpio_count  5
	#define gpio_data_count gpio_count

 // For debugging you may use these values to output hsync and vsync on the same pins as the colour mode
 // as well as the normal monochrome composite sync/video
 	// #define HSLO        0x4201
    // #define HSHI        0x400d
    // #define VSLO        0x4101
    // #define VSHI        0x400d
    // #define BORD        0x8000
    // #define gpio_count  10
#elif opt_colour == 1
    #define colour_base 0x00
    #define colour_max  0xFF
    #define HSLO        0x4200
    #define HSHI        0x4000
    #define VSLO        0x4100
    #define VSHI        0x4000
    #define BORD        0x8000
    #define gpio_base   0
    #define gpio_count  10
	#define gpio_data_count gpio_count
#elif opt_colour == 2
	#define colour_base 0x00
    #define colour_max  0x7
    #define HSLO        0x4000
    #define HSHI        0x4008
    #define VSLO        HSLO
    #define VSHI        HSHI
	// BORD must contain the sync level information as it is written by the sync PIO and not the data PIO
    #define BORD        0x8008
    #define gpio_base   16
    #define gpio_count  4
	// Setting gpio_data_count to less than gpio_count stops the data PIO writing to the bits only
	// used for sync. Therefore the sync doesn't need to be included in the data bitmap
	#define gpio_data_count 3
#elif opt_colour == 3
	#define colour_base 0x00
    #define colour_max  0xffff
    #define HSLO        0x420000
    #define HSHI        0x440000
    #define VSLO        0x410000
    #define VSHI        0x440000
	// BORD must contain the sync level information as it is written by the sync PIO and not the data PIO
    #define BORD        0x840000
    #define gpio_base   0
    #define gpio_count  19
	// Setting gpio_data_count to less than gpio_count stops the data PIO writing to the bits only
	// used for sync. Therefore the sync doesn't need to be included in the data bitmap
	#define gpio_data_count 16
#endif

unsigned short * bitmap;

int width;
int height;

int initialise_cvideo(void);
int set_mode(int mode);

void cvideo_configure_pio_dma(PIO pio, uint sm, uint dma_channel, uint transfer_size, size_t buffer_size,  irq_handler_t handler);

void cvideo_pio_handler(void);
void cvideo_dma_handler(void);

void wait_vblank(void);
void set_border(unsigned short colour);