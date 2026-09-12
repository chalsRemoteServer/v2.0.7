/**
  @file eve.c
 */
/*
 * ============================================================================
 * History
 * =======
 * 2017-03-15 : Created
 *
 * (C) Copyright Bridgetek Pte Ltd
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * http://www.ftdichip.com/FTSourceCodeLicenceTerms.htm ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
* By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */


/* INCLUDES ************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "EVE_config.h"
#include "EVE.h"
#include "HAL.h"
//#include <ft900_usb_hid.h>
#include <ft900_dlog.h>
#include "eve_app.h"
#include "eve.h"
//#include "camera.h"
//#include "tinyprintf.h"
#include "EVE_arduino.h"




/**
 @brief Link to datalogger area defined in crt0.S file.
 @details Must be passed to dlog library functions to initialise and use
        datalogger functions. We use the datalogger area for persistent
        configuration storage.
 */
extern __flash__ uint32_t __dlog_partition[];

/* CONSTANTS ***********************************************************************/

/**
 @brief Page number in datalogger memory in Flash for touchscreen calibration
 values.
 */
#define CONFIG_PAGE_TOUCHSCREEN 0
/**
 @brief Key for identifying if touchscreen calibration values are programmed into
 datalogger memory in the Flash.
 */
#define VALID_KEY_TOUCHSCREEN 0xd72f91a3

/**
 @brief Custom font and bitmap definitions.
 @details These utilise handles 0 to 14.
 */
//@{
#define CUSTOM_BITMAP0 5
#define CUSTOM_BITMAP1 6
#define CUSTOM_BITMAP2 7
//@}

/* GLOBAL VARIABLES ****************************************************************/

/**
 @brief Pointers to custom images in external C file.
 */
//@{
extern const uint32_t img_bridgetek_icon_size;
extern const uint8_t __flash__ *img_bridgetek_icon_data;
/// Address in RAM_G
uint32_t img_bridgetek_icon_address;
uint32_t img_bridgetek_icon_width;
uint32_t img_bridgetek_icon_height;

extern const uint32_t img_bridgetek_logo_size;
extern const uint8_t __flash__ *img_bridgetek_logo_data;
/// Address in RAM_G
uint32_t img_bridgetek_logo_address;
uint32_t img_bridgetek_logo_width;
uint32_t img_bridgetek_logo_height;

uint32_t img_end_address;
//@}

/* LOCAL VARIABLES *****************************************************************/

/**
 @brief Structure to hold touchscreen calibration settings.
 @details This is used to store the touchscreen calibration settings persistently
 in Flash and identify if the calibration needs to be re-performed.
 */
struct touchscreen_calibration {
	uint32_t key; // VALID_KEY_TOUCHSCREEN
	uint32_t transform[6];
};

/* MACROS **************************************************************************/

/* LOCAL FUNCTIONS / INLINES *******************************************************/

/**
 * @brief Functions used to store calibration data in flash.
 */
//@{
int8_t flash_calib_init(void)
{
	int	pgsz;
	int	i;
	int ret;

	ret = dlog_init(__dlog_partition, &pgsz, &i);
	if (ret < 0)
	{
		printf("dlog support required.\r\n");

		// Project settings incorrect. Require dlog support with modified
		// linker script and crt0.S file.
		// See AN_398 for examples.
		return -1;
	}
	return 0;
}

int8_t flash_calib_write(struct touchscreen_calibration *calib)
{
	uint8_t	flashbuf[260] __attribute__((aligned(4)));
	dlog_erase();

	calib->key = VALID_KEY_TOUCHSCREEN;
	memset(flashbuf, 0xff, sizeof(flashbuf));
	memcpy(flashbuf, calib, sizeof(struct touchscreen_calibration));
	if (dlog_prog(CONFIG_PAGE_TOUCHSCREEN, (uint32_t *)flashbuf) < 0)
	{
		// Flash not written.
		return -1;
	}

	return 0;
}

int8_t flash_calib_read(struct touchscreen_calibration *calib)
{
	uint8_t	flashbuf[260] __attribute__((aligned(4)));
	memset(flashbuf, 0x00, sizeof(flashbuf));
	if (dlog_read(CONFIG_PAGE_TOUCHSCREEN, (uint32_t *)flashbuf) < 0)
	{
		return -1;
	}

	if (((struct touchscreen_calibration *)flashbuf)->key == VALID_KEY_TOUCHSCREEN)
	{
		memcpy(calib, flashbuf, sizeof(struct touchscreen_calibration));
		return 0;
	}
	return -2;
}
//@}

void eve_calibrate()
{
	struct touchscreen_calibration calib;

	flash_calib_init();

	if (flash_calib_read(&calib) != 0)
	{
		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();
		EVE_CLEAR_COLOR_RGB(0, 0, 0);
		EVE_CLEAR(1,1,1);
		EVE_COLOR_RGB(255, 255, 255);
		EVE_CMD_TEXT(EVE_DISP_WIDTH/2, EVE_DISP_HEIGHT/2,
				27, EVE_OPT_CENTERX|EVE_OPT_CENTERY,"Please tap on a dot");
		EVE_CMD_CALIBRATE(0);
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		calib.transform[0] = HAL_MemRead32(REG_TOUCH_TRANSFORM_A);
		calib.transform[1] = HAL_MemRead32(REG_TOUCH_TRANSFORM_B);
		calib.transform[2] = HAL_MemRead32(REG_TOUCH_TRANSFORM_C);
		calib.transform[3] = HAL_MemRead32(REG_TOUCH_TRANSFORM_D);
		calib.transform[4] = HAL_MemRead32(REG_TOUCH_TRANSFORM_E);
		calib.transform[5] = HAL_MemRead32(REG_TOUCH_TRANSFORM_F);
		flash_calib_write(&calib);
	}
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_A, calib.transform[0]);
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_B, calib.transform[1]);
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_C, calib.transform[2]);
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_D, calib.transform[3]);
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_E, calib.transform[4]);
	HAL_MemWrite32(REG_TOUCH_TRANSFORM_F, calib.transform[5]);
}

/* FUNCTIONS ***********************************************************************/

void eve_write_from_flash(const uint8_t __flash__ *ImgData, uint32_t length)
{
    uint32_t offset = 0;
	uint8_t ramData[512];
	uint32_t left;

	while (offset < length)
	{
		memcpy_flash2dat(ramData, (uint32_t)ImgData, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToCMD(ramData, left);
		offset += left;
		ImgData += left;
	};
}

void eve_load_images(uint32_t start)
{
	img_bridgetek_icon_address = start;
	uint32_t dummy;

	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_BITMAP_HANDLE(CUSTOM_BITMAP0);
	EVE_CMD_LOADIMAGE(img_bridgetek_icon_address, 0);
	// Send raw JPEG encoded image data to coprocessor. It will be decoded
	// as the data is received.
	eve_write_from_flash(img_bridgetek_icon_data, img_bridgetek_icon_size);
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	// We know the start address in RAM so do not seek this information.
	EVE_LIB_GetProps(&dummy,
			// Obtain the width and height of the expanded JPEG image.
			&img_bridgetek_icon_width, &img_bridgetek_icon_height);

	img_bridgetek_logo_address = img_bridgetek_icon_address +
			(img_bridgetek_icon_width * 2 * img_bridgetek_icon_height);
	img_bridgetek_logo_address += 3;
	img_bridgetek_logo_address &= (~3);

	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_BITMAP_HANDLE(CUSTOM_BITMAP1);
	EVE_CMD_LOADIMAGE(img_bridgetek_logo_address, 0);
	eve_write_from_flash(img_bridgetek_logo_data, img_bridgetek_logo_size);
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	EVE_LIB_GetProps(&dummy,
			&img_bridgetek_logo_width, &img_bridgetek_logo_height);

	img_end_address = img_bridgetek_logo_address +
			(img_bridgetek_logo_width * 2 * img_bridgetek_logo_height);
	img_end_address += 3;
	img_end_address &= (~3);
}

void eve_setup()
{
	 EVE_Init();
     #ifdef EVE_DO_CALIBRATION
	        eve_calibrate();
     #endif // EVE_DO_CALIBRATION

	// Decode JPEG images from flash into RAM_DL on FT8xx.
	// Start at RAM_DL address zero.
	 eve_load_images(0);
}//-fin eve setup------------------------------------------

void eve_start(void)
{
	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CLEAR_COLOR_RGB(0, 0, 0);
	EVE_CLEAR(1,1,1);
	EVE_COLOR_RGB(255, 255, 255);
	EVE_CMD_TEXT(EVE_DISP_WIDTH/2, EVE_DISP_HEIGHT/2,
						27, OPT_CENTERX|OPT_CENTERY,"Waiting for images...");
	EVE_BITMAP_HANDLE(CUSTOM_BITMAP1);
	EVE_BEGIN(BITMAPS);
	EVE_BITMAP_SOURCE(img_bridgetek_logo_address);
	EVE_BITMAP_LAYOUT(RGB565, img_bridgetek_logo_width * 2, img_bridgetek_logo_height);
	EVE_BITMAP_SIZE(NEAREST, BORDER, BORDER,
			img_bridgetek_logo_width, img_bridgetek_logo_height);
	EVE_BITMAP_LAYOUT_H((img_bridgetek_logo_width * 2) >> 10, img_bridgetek_logo_height >> 9);
	EVE_BITMAP_SIZE_H(img_bridgetek_logo_width >> 9, img_bridgetek_logo_height >> 9);
	EVE_VERTEX2II((EVE_DISP_WIDTH/2)-(img_bridgetek_logo_width/2), 0, CUSTOM_BITMAP1, 0);
	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();
}

#ifdef EVE_ZOOM_QVGA
#define IMAGE_SCALE 2
#else
#define IMAGE_SCALE 1
#endif

#if CAMERA_RESOLUTION == CAMERA_USE_QVGA
#define IMAGE_WIDTH CAMERA_FRAME_WIDTH_QVGA
#define IMAGE_HEIGHT CAMERA_FRAME_HEIGHT_QVGA
#elif CAMERA_RESOLUTION == CAMERA_USE_VGA
//#define IMAGE_WIDTH CAMERA_FRAME_WIDTH_VGA
//#define IMAGE_HEIGHT CAMERA_FRAME_HEIGHT_VGA
#endif

//#define IMAGE_DISP_WIDTH (IMAGE_WIDTH * IMAGE_SCALE)
//#define IMAGE_DISP_HEIGHT (IMAGE_HEIGHT * IMAGE_SCALE)

//#define LEFT_X ((EVE_DISP_WIDTH / 2) - (IMAGE_DISP_WIDTH / 2))
#define RIGHT_X ((EVE_DISP_WIDTH / 2) + (IMAGE_DISP_WIDTH / 2))
//#define TOP_Y ((EVE_DISP_HEIGHT / 2) - (IMAGE_DISP_HEIGHT / 2))
//#define BOTTOM_Y ((EVE_DISP_HEIGHT / 2) + (IMAGE_DISP_HEIGHT / 2))

int8_t eve_loop(uint8_t *buffer, int32_t length)
{
	const uint32_t image_start_address = 0;
	static uint8_t eof_pending = 0;
	static uint8_t frame_active = 0;

#ifdef EVE_SHOW_FRAME_INFO
	static uint32_t image_len = 0;
	static uint32_t count = 0;
	static char infoLen[64];
	static char infoCount[64];
	//static char infoFrame[64];
#endif // EVE_SHOW_FRAME_INFO

	uint8_t *pmark;
	uint8_t *pstart = buffer;


	if (!frame_active)
	{
		if ((buffer[0] == 0xff) && (buffer[1] == 0xd8))
		{
			// Start of JPEG marker found.
			image_len = 0;
			frame_active = 1;

			// Start new JPEG decode.
			EVE_LIB_BeginCoProList();
			EVE_CMD_DLSTART();
			EVE_BITMAP_HANDLE(CUSTOM_BITMAP2);
			EVE_CMD_LOADIMAGE(image_start_address, OPT_NODL);
		}
	}

	if (frame_active)
	{
		length = (length + 3) & (~3);

		// Find 0xff 0xd9 for end of image!
		if (eof_pending)
		{
			if (*pstart == 0xd9)
			{
				frame_active = 0;
				length = 1;
			}
			eof_pending = 0;
		}

		pmark = pstart;
		while (pmark)
		{
			pmark = memchr(pmark, 0xff, length - (pmark - pstart));
			if (pmark)
			{
				pmark++;
				if ((pmark - pstart) == length)
				{
					// The last byte of the sample is 0xff so there might be
					// and end of frame marker continued on the next sample.
					eof_pending = 1;
				}
				else if (*pmark == 0xd9)
				{
					frame_active = 0;
					length = pmark - pstart + 1;
				}
			}
		}

		// Write JPEG data to command buffer
		EVE_LIB_WriteDataToCMD(buffer, length);
		image_len += length;

		if (!frame_active)
		{
#ifdef EVE_SHOW_FRAME_INFO
//			sprintf(infoFrame, "Camera Frame: %dx%d", IMAGE_WIDTH, IMAGE_HEIGHT);
			sprintf(infoLen, "Image Size: %lu", image_len);
			sprintf(infoCount, "Frame count: %lu", count++);
#endif // EVE_SHOW_FRAME_INFO

			// Finalise decoding of JPEG data
			EVE_LIB_EndCoProList();

			// New display list to show JPEG image
			EVE_LIB_BeginCoProList();
			EVE_CMD_DLSTART();
			EVE_CLEAR(1, 1, 1);

			EVE_BITMAP_HANDLE(CUSTOM_BITMAP2);
			EVE_BEGIN(BITMAPS);

			EVE_BITMAP_SOURCE(image_start_address);
	//		EVE_BITMAP_LAYOUT(RGB565, IMAGE_WIDTH * 2, IMAGE_HEIGHT);
	//		EVE_BITMAP_LAYOUT_H((IMAGE_WIDTH * 2) >> 10, IMAGE_HEIGHT >> 9);

#if CAMERA_RESOLUTION == CAMERA_USE_QVGA
#ifdef EVE_ZOOM_QVGA
			// Zoom QVGA image to VGA image
			EVE_CMD_LOADIDENTITY();
			EVE_CMD_SCALE(2 << 16, 2 << 16);
			EVE_CMD_SETMATRIX();
#endif // EVE_ZOOM_QVGA
			EVE_BITMAP_SIZE(NEAREST, BORDER, BORDER,
					IMAGE_WIDTH << 1, IMAGE_HEIGHT << 1);
			EVE_BITMAP_SIZE_H(IMAGE_WIDTH >> 8, IMAGE_HEIGHT >> 8);

#elif CAMERA_RESOLUTION == CAMERA_USE_VGA
//			EVE_BITMAP_SIZE(NEAREST, BORDER, BORDER,
//					IMAGE_WIDTH, IMAGE_HEIGHT);
//			EVE_BITMAP_SIZE_H(IMAGE_WIDTH >> 9, IMAGE_HEIGHT >> 9);
#endif
			// Display image centred on screen
//			EVE_VERTEX2II(LEFT_X, TOP_Y, CUSTOM_BITMAP2, 0);
			EVE_END();

#ifdef EVE_ZOOM_QVGA
			// Return scale to 1:1
			EVE_CMD_LOADIDENTITY();
			EVE_CMD_SCALE(1 << 16, 1 << 16);
			EVE_CMD_SETMATRIX();
#endif // EVE_ZOOM_QVGA

#ifdef EVE_USE_OVERLAY
			EVE_BEGIN(LINES);
			EVE_COLOR_RGB(128, 0, 0);
			EVE_LINE_WIDTH(2 * 16);
			// Draw overlay lines
//			EVE_VERTEX2F(LEFT_X * 16, BOTTOM_Y * 16);
//			EVE_VERTEX2F((LEFT_X + (IMAGE_DISP_WIDTH / 3)) * 16, (BOTTOM_Y / 2) * 16);
//			EVE_VERTEX2F((LEFT_X + (IMAGE_DISP_WIDTH / 3)) * 16, (BOTTOM_Y / 2) * 16) ;
//			EVE_VERTEX2F((LEFT_X + (IMAGE_DISP_WIDTH * 2 / 3)) * 16, (BOTTOM_Y / 2) * 16);
//			EVE_VERTEX2F((LEFT_X + (IMAGE_DISP_WIDTH * 2 / 3)) * 16, (BOTTOM_Y / 2) * 16);
//			EVE_VERTEX2F((LEFT_X + (IMAGE_DISP_WIDTH)) * 16, BOTTOM_Y * 16);
#endif // EVE_USE_OVERLAY

#ifdef EVE_SHOW_FRAME_INFO
			EVE_COLOR_RGB(255, 255, 255);
//			EVE_CMD_TEXT(LEFT_X, TOP_Y, 27, 0, infoFrame);
//			EVE_CMD_TEXT(LEFT_X, TOP_Y + 20, 27, 0, infoLen);
//			EVE_CMD_TEXT(LEFT_X, TOP_Y + 40, 27, 0, infoCount);
#endif // EVE_SHOW_FRAME_INFO

			EVE_DISPLAY();
			EVE_CMD_SWAP();
			EVE_LIB_EndCoProList();
		}
	}

	return frame_active;
}

void eve_close(void)
{
}

void eve_tick(void)
{
}
