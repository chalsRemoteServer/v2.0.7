/*
 * ui_flash_storage.h
 *
 *  Created on: 11 mar. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

uint8_t ui_flash_read_bytes(uint32_t addr, void* dst, uint32_t len);
uint8_t ui_flash_write_bytes(uint32_t addr, const void* src, uint32_t len);
uint8_t ui_flash_erase_sector(uint32_t addr);
