/*
 * low_level_flash_atmega328.h
 *
 *  Created on: Sep 10, 2019
 *      Author: emekcan
 */

#ifndef LOW_LEVEL_FLASH_ATMEGA328_H_
#define LOW_LEVEL_FLASH_ATMEGA328_H_
#include <avr/boot.h>
#include <Arduino.h>
#include "optiboot.h"
#include "conf.h"
typedef struct flash_info{
	uint8_t address;
	uint8_t size;
	uint8_t page;
	uint8_t frame_size;
	uint8_t current_frame;
	uint32_t usage_rate ;

}flash_info_t;
#if FLASH_EN



typedef struct flash_free{
	uint8_t address;
	uint8_t page;
}flash_free_info_t;

typedef struct page_info{
	uint8_t current_address ;
	uint32_t usage_rate ;

}page_info_t;

static page_info_t page_addressing[NUMBER_OF_PAGES];
//uint8_t current_page = 1;
//uint8_t current_address = 0;
static flash_free_info_t info;

flash_free_info_t* flash_find_free_space(flash_info_t *p_info){
	for(uint8_t counter=0;counter<NUMBER_OF_PAGES;counter++){
		if((page_addressing[counter].current_address + (p_info->size*p_info->frame_size)) < MAX_ADDRESS_IN_PAGE){
			info.address = page_addressing[counter].current_address;
			page_addressing[counter].current_address = page_addressing[counter].current_address + (p_info->size*p_info->frame_size);
			info.page = counter + 1;
			return &info;
		}
	}
	//TODO:return error
	return &info;

}

flash_info_t* flash_alloc(flash_info_t *p_info, uint32_t bitsize, uint32_t frame_size){
	flash_free_info_t *flash_info;
	p_info->size = bitsize / 8;
	p_info->frame_size = frame_size;
	p_info->current_frame = 0;
	flash_info = flash_find_free_space(p_info);
	p_info->page = flash_info->page;
	p_info->address =  flash_info->address;
	p_info->usage_rate = 0;
//	page_addressing[flash_info->page - 1].current_address = page_addressing[flash_info->page - 1].current_address + (p_info->size*frame_size);
/*	if((current_address + p_info->size) < MAX_ADDRESS_IN_PAGE){
		p_info->address = current_address;
		p_info->page = current_page;
		current_address = current_address + p_info->size;

	}else{
		if(current_page == 8){
		//TODO:error return
		}else{
			current_page++;
			p_info->address = current_address;
			p_info->page = current_page;
			current_address = current_address + p_info->size;
		}
	}*/
	return p_info;
}


flash_info_t* flash_write(void *p_store, flash_info_t *p_info ){
	uint8_t next_frame;


	for(uint8_t counter=0;counter<SPM_PAGESIZE;counter++){
		ramBuffer[counter] = pgm_read_byte(&flashSpace[SPM_PAGESIZE*(p_info->page - 1 ) + counter]);
	}
	memcpy(ramBuffer + p_info->address+ p_info->current_frame*p_info->size, p_store,p_info->size);
	optiboot_writePage(flashSpace, ramBuffer, p_info->page);
	next_frame = (p_info->current_frame + 1) % p_info->frame_size;
	(p_info->current_frame) = next_frame;
	page_addressing[p_info->page].usage_rate++;
	p_info->usage_rate = page_addressing[p_info->page].usage_rate;
	delay(1);//TODO:check correct timing

	return p_info;
}

flash_info_t* flash_read(void *p_store , flash_info_t *p_info){
	uint8_t *p_array;
	p_array = (uint8_t*)p_store;

	//adjust frame
	uint8_t frame_to_read = 0;
	if(p_info->current_frame > 0){
		frame_to_read = (p_info->current_frame - 1);
	}else{
		frame_to_read = (p_info->frame_size - 1);
	}

	for(uint8_t counter=0;counter<p_info->size;counter++){
		p_array[counter] = pgm_read_byte(&flashSpace[p_info->address+ frame_to_read*p_info->size + SPM_PAGESIZE*(p_info->page - 1) + counter]);
	}

 	return p_info;
}

#endif
#endif /* LOW_LEVEL_FLASH_ATMEGA328_H_ */
