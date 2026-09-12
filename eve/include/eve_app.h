/**
 @file eve.h
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

#ifndef INCLUDES_EVE_H_
#define INCLUDES_EVE_H_

/* For MikroC const qualifier will place variables in Flash
 * not just make them constant.
 */
#if defined(__GNUC__)
#define DESCRIPTOR_QUALIFIER const
#elif defined(__MIKROC_PRO_FOR_FT90x__)
#define DESCRIPTOR_QUALIFIER data
#endif

/**
 @brief Resolution of image.
 */
//@{
#define CAMERA_USE_QVGA 1
#define CAMERA_USE_VGA 2
#define CAMERA_RESOLUTION CAMERA_USE_VGA
//@}

/**
 @brief Draw overlay.
 @details Show a simple overlay containing several lines on top of the
 	 JPEG image received from the camera.
 */
//@{
#define EVE_USE_OVERLAY
//@}

/**
 @brief Display frame info.
 @details Shows the current frame size (640x480, 320x240 etc), the
 	 size in bytes of the JPEG frame received from the camera and the
 	 total number of frames displayed as debug information on the EVE
 	 display.
 */
//@{
#define EVE_SHOW_FRAME_INFO
//@}

/**
 @brief Zoom QVGA to VGA size
 @@details Use the EVE CMD_SCALE coprocessor function to expand the size of
 	 a QVGA image received from the camera to VGA size.
 */
//@{
#undef EVE_ZOOM_QVGA
//@}

/**
 @brief Perform EVE touchscreen calibration.
 @details This may not be needed.
 */
//@{
#undef EVE_DO_CALIBRATION
//@}

void eve_setup(void);
void eve_start();
void eve_wait();
//int8_t eve_loop(uint8_t *buffer, int32_t length);
void eve_close(void);

void eve_tick(void);


#endif /* INCLUDES_EVE_H_ */
