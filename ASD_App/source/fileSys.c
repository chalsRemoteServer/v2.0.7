/*
 * fileSys.c
 *
 *  Created on: Apr. 16, 2026
 *      Author: desarrollo_1
 */
#include "platform_time.h"
#include "fileSys.h"
#include "system.h"
#include "TFT.h"
#include <string.h>  // para strlen
#include <stdio.h>   // para sprintf (alternativa a itoa)
#include "ft900.h"

// Definiciones de variables globales (únicas en todo el proyecto)
int sd_ready = 0;
FATFS fs;

/* variables del globales ********/
FRESULT res;
FIL f;
UINT towrite, written;
UINT read;
char* testdata;
char buffer[128];
/**************************/


static void print1(const char *c){
#ifdef DEBUG_SYSTEM
	    printf("%s",c);
#endif
}//fin print1---------------------------------------------


void gpio_functions_FileSystem(void){
	gpio_function(GPIO_SD_CLK, pad_sd_clk); gpio_pull(GPIO_SD_CLK, pad_pull_none);
	gpio_function(GPIO_SD_CMD, pad_sd_cmd); gpio_pull(GPIO_SD_CMD, pad_pull_pullup);
	gpio_function(GPIO_SD_DAT3, pad_sd_data3); gpio_pull(GPIO_SD_DAT3, pad_pull_pullup);
	gpio_function(GPIO_SD_DAT2, pad_sd_data2); gpio_pull(GPIO_SD_DAT2, pad_pull_pullup);
	gpio_function(GPIO_SD_DAT1, pad_sd_data1); gpio_pull(GPIO_SD_DAT1, pad_pull_pullup);
	gpio_function(GPIO_SD_DAT0, pad_sd_data0); gpio_pull(GPIO_SD_DAT0, pad_pull_pullup);
	gpio_function(GPIO_SD_CD, pad_sd_cd); gpio_pull(GPIO_SD_CD, pad_pull_pullup);
	gpio_function(GPIO_SD_WP, pad_sd_wp); gpio_pull(GPIO_SD_WP, pad_pull_pullup);

	gpio_idrive(GPIO_SD_CMD, pad_drive_16mA);
	gpio_idrive(GPIO_SD_CLK, pad_drive_16mA);
	gpio_idrive(GPIO_SD_DAT0, pad_drive_16mA);
	gpio_idrive(GPIO_SD_DAT1, pad_drive_16mA);
	gpio_idrive(GPIO_SD_DAT2, pad_drive_16mA);
	gpio_idrive(GPIO_SD_DAT3, pad_drive_16mA);

}//fin gpio function fileSystem-----------------------------------------------------------

void wait_for_inserted_card(void){	 /* Check to see if a card is inserted */
#ifdef DEBUG_SYSTEM
	    print1("Please Insert SD Card\r\n");
#endif
	    while (sdhost_card_detect() != SDHOST_CARD_INSERTED);
#ifdef DEBUG_SYSTEM
	    printf("SD Card Inserted\r\n");
#endif
}//-----------------------------------------------


void montar_memoria_SD(void){/* Initialise FatFS */
	if (FR_OK != f_mount(&fs, "", 0)){
#ifdef DEBUG_SYSTEM
	    	print1("Unable to mount File System\r\n");
#endif
	   }
#ifdef DEBUG_SYSTEM
	    print1("Mounted\r\n");
#endif
}//fin memoria montada---------------------




void demo_SDCard(void){
static unsigned char estado61;
static int delay1,read2;
static char *ptr;
static int bytes_to_send;  // Nuevo: contador de bytes pendientes



	switch(estado61){
	case 1:res = dir("");estado61++;break;/* List the root directory */
	case 2:printf("\r\n\r\n");estado61++;break;
	case 3:delay1=millis();estado61++;break;
	case 4:if(millis()>(delay1+1000))estado61++;break;
	case 5:res = f_stat(EXAMPLE_FILE, NULL);/* Check to see if the example file is there */
		   if (FR_OK == res){
	    	    print1(EXAMPLE_FILE " already exists. Deleting\r\n");
	            if (FR_OK != f_unlink(EXAMPLE_FILE)){//deleting file
	        	print1("Problem deleting " EXAMPLE_FILE "\r\n");}
	            else{estado61++;}}
		   else{estado61++;}break;
	case 6:print1("Opening " EXAMPLE_FILE " for writing\r\n");/* Write some data to the SD Card */
	       res = f_open(&f, EXAMPLE_FILE, FA_WRITE | FA_CREATE_NEW);
	       if (FR_OK != res){
	    	 print1("Problem creating " EXAMPLE_FILE "\r\n");}
	       else{estado61++;}break;
	case 7: testdata = (char *) LOREM_IPSUM;
    		towrite = strlen(testdata);
			written = 0;estado61++;break;
	case 8:if(towrite>0){
               f_write(&f, testdata, towrite, &written);
               towrite -= written;
               testdata += written;
               print1("Wrote ");print1(itoa(written));
               print1(" bytes\r\n");}
			else{estado61++;}break;
	case 9:print1("Closing " EXAMPLE_FILE "\r\n");estado61++;break;
	case 10:if(FR_OK != f_close(&f)){estado61++;
	    	  print1("Error closing " EXAMPLE_FILE "\r\n");}
	       else{estado61+=2;}break;
	case 11:break;//loop del estado anterior error closing
	case 12:print1("Opening " EXAMPLE_FILE " for reading\r\n\r\n");
		   estado61++;break;
	case 13:if (FR_OK != f_open(&f, EXAMPLE_FILE, FA_READ)){
		        print1("\r\n" "Could not open " EXAMPLE_FILE " for reading\r\n");
		        estado61=8;}
			else{estado61++;}break;
	case 14:f_read(&f, buffer, 128, &read);
	        ptr=buffer;bytes_to_send=read;
	        estado61++;break;
	case 15:if(bytes_to_send>0){// Verificar si UART está listo para enviar
		       if(UART0->LSR_ICR_XON2 & MASK_UART_LSR_THRE) {// Enviar un solo carácter
		    	   UART0->RHR_THR_DLL = *ptr;
		    	   ptr++;
		    	   bytes_to_send--;}}
	        else{estado61=20;}
			break;
	case 16:
	case 20:if(read<128){estado61++;}else{estado61=14;}break;
	case 21:print1("\r\n" "Closing " EXAMPLE_FILE "\r\n");estado61++;break;
	case 22:delay1=millis();estado61++;break;
	case 23:if(millis()>(delay1+10)){estado61++;}break;
	case 24:f_close(&f);print1("\r\n\r\n");estado61++;break;
	case 25:delay1=millis();estado61++;break;
	case 26:if(millis()>(delay1+20)){estado61++;}break;
	case 27:estado61=12;break;
	default: estado61=1;break;}//fin switch---------
}//fin  demo_SDCard----------------------------------




/* Based on: http://elm-chan.org/fsw/ff/en/readdir.html */
/** List the directory
 *  @param path The directory to list
 *  @return The status of the operation */
FRESULT dir(char* path){
    FRESULT res;
    FILINFO info;
    DIR dir;
    char * fname;
    uint8_t day, month, hour, min;
    uint16_t year;
    uint32_t totalsize = 0, totalfiles = 0;
#if _USE_LFN
    static char longfn[_MAX_LFN+1];

    info.lfname = longfn;
    info.lfsize = sizeof(longfn);
#endif

    res = f_opendir(&dir, path);
    if (FR_OK == res){
        /* Display a header */
        printf("ls(path = \"%s\"):\r\n",path);
        printf("DD/MM/YYYY HH:MM               Size Filename\r\n");

        for(;;){
            res = f_readdir(&dir, &info);
            /* Break if we have finished or an error has occured */
            if ((FR_OK != res) || (info.fname[0] == 0))
                break;
            /* Assign fname with the proper filename */
#if _USE_LFN
            if (*(info.lfname)){
               fname = info.lfname;}
            else
#else
            {fname = info.fname;}
#endif
            /* Work out the date and time the file was last written to */
            day   = FDATE_DAY(info.fdate);
            month = FDATE_MONTH(info.fdate);
            year  = FDATE_YEAR(info.fdate);

            hour  = FTIME_HOUR(info.ftime);
            min   = FTIME_MIN(info.ftime);

            /* Count the number of files in this folder */
            if (!(info.fattrib & AM_DIR))
                totalfiles++;
            /* Count the total size of all files in this folder */
            totalsize += info.fsize;
            /* Display the item */
            printf("%02d/%02d/%04d %02d:%02d   %s %10ld %s\r\n",
                       day, month, year, hour, min,
                       (info.fattrib & AM_DIR) ? "<DIR>" : "     ",
                       info.fsize,
                       fname);
           }//fin for
        if (FR_OK == res){
            /* Display the summary if things were ok */
            printf("  %10ld File(s)     %10ld bytes\r\n",
                       totalfiles, totalsize);}
        f_closedir(&dir);}
return res;
}//-------------------------------------------------


/* FatFS Functions ******************/

/** Initialise a drive
 *  @param pdrv Physical Drive number
 *  @return Disk Status */
DSTATUS disk_initialize(BYTE pdrv){
DSTATUS stat = 0;

	if(SDHOST_OK != sdhost_card_init()){
		stat = STA_NOINIT;}
    else{sd_ready = 1;}
return stat;
}//----------------------------------

/** Disk Status
 *  @param pdrv Physical Drive number
 *  @return Disk Status */
DSTATUS disk_status(BYTE pdrv){
DSTATUS stat = 0;

	if (0 == sd_ready){
		stat |= STA_NOINIT;}

    if (sdhost_card_detect() == SDHOST_CARD_REMOVED){
        stat |= STA_NODISK;}
return stat;
}//------------------------------------------

/** Read sector(s) from disk
 *  @param pdrv Physical Drive number
 *  @param buff Data buffer to store into
 *  @param sector The logical sector address
 *  @param count The number of sectors to read
 *  @return Disk Status */
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count){
DRESULT res = RES_OK;

	if (SDHOST_OK !=
        sdhost_transfer_data(SDHOST_READ, (void*)buff, SDHOST_BLK_SIZE * count, sector)){
		res = RES_ERROR;}
return res;
}//------------------------------------


/** Write sector(s) to the disk
 *  @param pdrv Physical Drive number
 *  @param buff Data buffer to write to the disk
 *  @param sector The logical sector address
 *  @param count The number of sectors to write
 *  @return Disk Status */
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
DRESULT res = RES_OK;

	if (SDHOST_OK !=
        sdhost_transfer_data(SDHOST_WRITE, (void*)buff, SDHOST_BLK_SIZE * count, sector)){
		res = RES_ERROR;}
return res;
}//--------------------------------------------



/** Disk IO Control
 *  @param pdrv Physical Drive Number
 *  @param cmd Control Code
 *  @param buff Buffer to send/receive control data
 *  @return Disk Status */
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff){
	DRESULT res = RES_OK;
    /* Not Supported */
	return res;
}//-----------------------------------------------


#if _FS_READONLY == 0
/** Get the current time
 *  @return The time in the following format:
 *          bit[31:25] = Year from 1980 (0..127),
 *          bit[24:21] = Month (1..12),
 *          bit[20:16] = Day of the Month (1..31),
 *          bit[15:11] = Hour (0..23),
 *          bit[10:5]  = Minute (0..59),
 *          bit[4..0]  = Second / 2 (0..29) */
DWORD get_fattime(void){
    return 0; /* Invalid timestamp */
}//------------------------------------------
#endif



