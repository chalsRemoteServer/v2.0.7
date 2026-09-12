/*
 * fileSys.h
 *
 *  Created on: Apr. 16, 2026
 *      Author: desarrollo_1
 */

#ifndef ASD_APP_INCLUDE_FILESYS_H_
#define ASD_APP_INCLUDE_FILESYS_H_

#include "ft900.h"
#include "ff.h"
#include "diskio.h"
#include "assert.h"

#define GPIO_SD_CLK  (19)
#define GPIO_SD_CMD  (20)
#define GPIO_SD_DAT3 (21)
#define GPIO_SD_DAT2 (22)
#define GPIO_SD_DAT1 (23)
#define GPIO_SD_DAT0 (24)
#define GPIO_SD_CD   (25)
#define GPIO_SD_WP   (26)

#define EXAMPLE_FILE "LOREM.TXT"

#define FDATE_DAY(fdate) ((fdate)&(0x1F))
#define FDATE_MONTH(fdate) (((fdate)>>5)&(0x0F))
#define FDATE_YEAR(fdate) ( (((fdate)>>9)&(0x7F)) + 1980 )

#define FTIME_HOUR(ftime) (((ftime)>>11)&(0x1F))
#define FTIME_MIN(ftime) (((ftime)>>5)&(0x3F))
FRESULT dir(char* path);
DSTATUS disk_initialize(BYTE pdrv);
DSTATUS disk_status(BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);
DWORD get_fattime(void);
void myputc(void* p, char c);

extern int sd_ready;
extern FATFS fs;

static const char* LOREM_IPSUM =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi dictum mi "
    "eget malesuada auctor. Cras tellus ligula, feugiat ac ante eu, tincidunt "
    "consectetur mauris. Phasellus in mollis enim, dapibus venenatis est. Sed "
    "urna tellus, varius a dui sed, scelerisque commodo lectus. In pretium "
    "lobortis tortor, semper ultricies odio viverra a. Ut sit amet aliquam "
    "lectus. Phasellus non risus a nisl semper vehicula a vitae lorem. Fusce "
    "suscipit, purus nec facilisis lacinia, lacus massa aliquet augue, in "
    "feugiat neque nibh a lacus. Curabitur pharetra viverra massa quis "
    "efficitur. \r\n"
    "\r\n"
    "Mauris posuere nisl vel aliquam finibus. Aenean ac fringilla justo. Nulla "
    "eu sollicitudin erat. Duis in ligula at quam pretium hendrerit. Fusce quis "
    "egestas metus. In hac habitasse platea dictumst. Fusce tincidunt enim at "
    "tempus ullamcorper. Aenean pellentesque condimentum sapien vel porta. In "
    "sollicitudin tempor pulvinar. Pellentesque aliquet justo lacus, scelerisque "
    "feugiat augue commodo viverra. \r\n"
    "\r\n"
    "Etiam pulvinar quam a pulvinar aliquam. Cras rutrum quis tortor ut ultrices. "
    "Curabitur sit amet odio eros. Mauris auctor erat non risus interdum, at "
    "venenatis urna interdum. Nam eget auctor risus, auctor fringilla leo. "
    "Quisque sit amet ligula mattis, gravida tortor quis, ullamcorper odio. "
    "Nullam semper mauris at leo aliquam, quis mollis tortor iaculis. Mauris ut "
    "tempor elit, sed sodales magna. Donec non eros tortor. Donec lorem justo, "
    "vestibulum vitae sagittis ac, bibendum vitae velit. Integer ante mi, tempus "
    "sodales consectetur vel, porta ac libero. Maecenas dapibus orci at rhoncus "
    "bibendum. Nulla elementum lectus massa, non varius lorem scelerisque sit "
    "amet. \r\n";

void gpio_functions_FileSystem(void);
void wait_for_inserted_card(void);
void montar_memoria_SD(void);
//void print1(const char *c);
void demo_SDCard(void);








#endif /* ASD_APP_INCLUDE_FILESYS_H_ */
