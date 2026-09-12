/*
 * diag_dlog.c
 *
 *  Created on: 3 abr. 2026
 *      Author: oscar
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ft900.h>
#include <ft900_dlog.h>
#include "diag_dlog.h"
#include <ft900_memctl.h>

/* La partición dlog debe venir del linker/runtime */
extern __flash__ uint32_t __dlog_partition[];

int diag_dlog_init_only(void)
{
    int ret;
    int pgsz = -1;
    int pages = -1;

    printf("=== DLOG INIT TEST ===\r\n");
    printf("__dlog_partition = %p\r\n", __dlog_partition);

    ret = dlog_init(__dlog_partition, &pgsz, &pages);

    printf("dlog_init ret   = %d\r\n", ret);
    printf("dlog page size  = %d\r\n", pgsz);
    printf("dlog pages      = %d\r\n", pages);

    if (ret < 0)
    {
        printf("DLOG INIT FAIL\r\n");
        return -1;
    }

    if (pgsz <= 0 || pages <= 0)
    {
        printf("DLOG INIT INVALID GEOMETRY\r\n");
        return -2;
    }

    printf("DLOG INIT OK\r\n");
    return 0;
}


void diag_read_flash_raw_3f000(void)
{
    uint8_t buf[32];
    uint8_t i;

    memset(buf, 0x00, sizeof(buf));

    /* Lee 32 bytes desde el inicio de la región reservada dlog */
    memcpy_flash2dat(buf, 0x3F000, sizeof(buf));

    printf("RAW FLASH @ 0x3F000:\r\n");
    for (i = 0; i < sizeof(buf); i++)
    {
        printf("%02X ", buf[i]);

        if (((i + 1u) % 16u) == 0u)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}
