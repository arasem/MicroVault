/*
 * low_level_noinit_atmega328.h
 *
 *  Created on: Sep 16, 2019
 *      Author: emekcan
 */

#ifndef LOW_LEVEL_NOINIT_ATMEGA328_H_
#define LOW_LEVEL_NOINIT_ATMEGA328_H_
#include "conf.h"
#if NVRAM_EN
typedef struct noinit__info{
	uint8_t address;
	uint8_t size;
	uint8_t page;
	uint8_t last_frame_used;
}noinit_info_t;

#define NO_INIT_SIZE 1024

uint8_t noinit_space[NO_INIT_SIZE] __attribute__((section(".noinit")))  ;


uint8_t current_noinit_address = 0;

noinit_info_t* noinit_alloc(noinit_info_t *p_info, uint32_t bitsize){
	p_info->size = bitsize / 8;

	if(current_noinit_address + p_info->size < NO_INIT_SIZE){
		p_info->address = current_eeprom_address;
		current_eeprom_address = current_eeprom_address + p_info->size;
		p_info->page = 0;
	}else{
		//TODO:error return
	}
	return p_info;
}


noinit_info_t* noinit_write(void *p_store, noinit_info_t *p_info ){
	uint8_t *p_array;
	p_array = (uint8_t*)p_store;

	for(uint8_t counter=0;counter<p_info->size;counter++){
		noinit_space[p_info->address + counter] = p_array[counter];
	}
	delay(1);//TODO:check correct timing

	return p_info;
}

noinit_info_t* noinit_read(void *p_store , noinit_info_t *p_info){
	uint8_t *p_array;
	p_array = (uint8_t*)p_store;


	for(uint8_t counter=0;counter<p_info->size;counter++){
		p_array[counter] = noinit_space[p_info->address + counter];
	}

 	return p_info;
}

#endif
#endif /* LOW_LEVEL_NOINIT_ATMEGA328_H_ */
