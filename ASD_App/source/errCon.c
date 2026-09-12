/*
 * errCon.c
 *
 *  Created on: Jul 22, 2026
 *      Author: chals
 */

#include "errCon.h"
#include "system.h"

status_code_e ERORES;
status_bar_s StatusBar={
		.fault_count=0,
		.code=STATUS_OK
};

