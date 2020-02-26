/*
 * low_level_eeprom_atmega328.h
 *
 *  Created on: Sep 16, 2019
 *      Author: emekcan
 */

#ifndef LOW_LEVEL_EEPROM_ATMEGA328_H_
#define LOW_LEVEL_EEPROM_ATMEGA328_H_
#include <Arduino.h>
#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/io.h>

typedef struct eeprom__info{
	uint8_t address;
	uint8_t size;
	uint8_t page;
	uint8_t frame_size;
	uint8_t current_frame;
	uint32_t usage_rate ;

}eeprom_info_t;

#define MAX_ADDRESS_IN_EEPROM 1024

static uint8_t current_eeprom_address = 6;//normally 0, for test purposes

eeprom_info_t* eeprom_alloc(eeprom_info_t *p_info, uint32_t bitsize, uint8_t frame_size){
	p_info->size = (bitsize / 8);
	p_info->frame_size = frame_size;
	p_info->current_frame = 0;
	if(current_eeprom_address + (frame_size * p_info->size) < MAX_ADDRESS_IN_EEPROM){
		p_info->address = current_eeprom_address;
		current_eeprom_address = current_eeprom_address + (p_info->size*frame_size);
		p_info->page = 0;
		p_info->usage_rate = 0;
	}else{
		//TODO:error return
	}
	return p_info;
}


eeprom_info_t* eeprom_write(void *p_store, eeprom_info_t *p_info ){
	uint8_t next_frame;
	uint8_t *p_array;
	p_array = (uint8_t*)p_store;
	for(uint8_t counter=0;counter<p_info->size;counter++){
		eeprom_write_byte((uint8_t*)((p_info->address + p_info->current_frame*p_info->size) + counter),p_array[counter]);
	}
	p_info->usage_rate++;
	delay(1);//TODO:check correct timing
	next_frame = (p_info->current_frame + 1) % p_info->frame_size;
	(p_info->current_frame) = next_frame;
	return p_info;
}

eeprom_info_t* eeprom_read(void *p_store , eeprom_info_t *p_info){
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
		p_array[counter] = eeprom_read_byte((uint8_t*)((p_info->address+ frame_to_read*p_info->size) + counter));
	}

 	return p_info;
}


#endif /* LOW_LEVEL_EEPROM_ATMEGA328_H_ */
