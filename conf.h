/*
 * conf.h
 *
 *  Created on: Sep 11, 2019
 *      Author: emekcan
 */

#ifndef CONF_H_
#define CONF_H_

#include <Arduino.h>
// Define the number of pages you want to write to here (limited by flash size)
#define NUMBER_OF_PAGES 8
#define MAX_ADDRESS_IN_PAGE 0x80
#define NVRAM_EN 0
#define FLASH_EN 0
#define EEPROM_EN 1

//TODO:add cpu preprocessor primitive
#define EEPROM_FAILURE_CYCLE 100000
#define FLASH_FAILURE_CYCLE 10000

// The temporary data (data that's read or is about to get written) is stored here
uint8_t ramBuffer[SPM_PAGESIZE];

#if FLASH_EN
// This array allocates the space you'll be able to write to
const uint8_t flashSpace[SPM_PAGESIZE * NUMBER_OF_PAGES] __attribute__ (( aligned(SPM_PAGESIZE) )) PROGMEM = {
  "This some default content stored on page one"
};
const char blankChar = '.';
#endif

#endif /* CONF_H_ */
