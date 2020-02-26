/*
 * rsu_api.c
 *
 *  Created on: Aug 7, 2019
 *      Author: emekcan
 */

#include "rsu_api.h"
#include <stdlib.h>
#include <stdio.h>
#include "low_level_flash_atmega328.h"
#include "utilities.h"
#include "low_level_eeprom_atmega328.h"
#include "low_level_noinit_atmega328.h"
#include "conf.h"
uint32_t counter_func_get_value(rsu_counter_info_t *p_counter);
rsu_error_code counter_func_count_up(rsu_counter_info_t *p_counter);
rsu_error_code counter_func_count_down(rsu_counter_info_t *p_counter);
rsu_error_code counter_func_update(rsu_counter_info_t *p_counter, uint32_t value);
rsu_error_code data_write(rsu_data_info_t *p_data,void *p_data_w);
rsu_error_code data_read(rsu_data_info_t *p_data,void* p_storage);
/*rsu_error_code data_burst_write(rsu_array_info_t *p_data);
rsu_error_code data_burst_read(rsu_array_info_t *p_data,void* p_storage);*/
uint32_t inst_get_life_time(rsu_instances instance, void* p_inst);

//typedef struct data_encapsulated_info{
//	uint32_t address;
//	uint32_t page ;
//	uint32_t usage_rate;
//}rsu_data_enc_info_t;


void* rsu_init(rsu_instances inst,void *p_data_inf ){
	flash_info_t *enc_info;
	enc_info = malloc(sizeof(flash_info_t));
	switch(inst){
		case RSU_INST_COUNTER:{
			uint32_t initial_value = 0;
			rsu_counter_t *p_counter;
			p_counter = malloc(sizeof(rsu_counter_t));
			memcpy(&p_counter->counter_info,p_data_inf,sizeof(rsu_counter_info_t));
			p_counter->counter_dec = counter_func_count_down;
			p_counter->counter_inc = counter_func_count_up;
			p_counter->counter_update = counter_func_update;
			p_counter->counter_get_value = counter_func_get_value;
			p_counter->get_life_time = inst_get_life_time;
			p_counter->counter_info.p_encapsulated_info = enc_info;
			//((flash_info_t *)p_counter->counter_info.p_encapsulated_info)->usage_rate = 0;
			initial_value = initial_value + p_counter->counter_info.offset;

			if(p_counter->counter_info.pwr_down_mode == rsu_pwr_off_flash ){
#if FLASH_EN
				flash_alloc(p_counter->counter_info.p_encapsulated_info,p_counter->counter_info.bit_size,p_counter->counter_info.frame_size);
				flash_write(&initial_value,p_counter->counter_info.p_encapsulated_info);
#endif
			}else if(p_counter->counter_info.pwr_down_mode == rsu_pwr_off_eeprom){
#if EEPROM_EN
				eeprom_alloc(p_counter->counter_info.p_encapsulated_info,p_counter->counter_info.bit_size,p_counter->counter_info.frame_size);
				eeprom_write(&initial_value,p_counter->counter_info.p_encapsulated_info);
#endif
			}else if(p_counter->counter_info.pwr_down_mode == rsu_pwr_on_only_nvram){
				#if NVRAM_EN
				noinit_alloc(p_counter->counter_info.p_encapsulated_info,p_counter->counter_info.bit_size);
				noinit_write(&initial_value,p_counter->counter_info.p_encapsulated_info);
				#endif
			}else{
				//error return
			}
			//
			return p_counter;
		}
		case RSU_INST_DATA:
		{
			rsu_data_t *p_data;
			p_data = malloc(sizeof(rsu_data_t));
			memcpy(&p_data->data_info,p_data_inf,sizeof(rsu_data_info_t));
			p_data->data_write = data_write;
			p_data->data_read = data_read;
			p_data->get_life_time = inst_get_life_time;
			p_data->data_info.p_encapsulated_info = enc_info;
			//((flash_info_t *)p_data->data_info.p_encapsulated_info)->usage_rate = 0;

			//flash_alloc(p_data->data_info.p_encapsulated_info,((rsu_data_info_t*)p_data_inf)->bit_size);
			//flash_alloc(p_data->data_info.p_encapsulated_info,p_data->data_info.bit_size);
			if(p_data->data_info.pwr_down_mode == rsu_pwr_off_flash ){
#if FLASH_EN
				flash_alloc(p_data->data_info.p_encapsulated_info,p_data->data_info.bit_size,p_data->data_info.frame_size);
#endif
			}else if(p_data->data_info.pwr_down_mode == rsu_pwr_off_eeprom){
#if EEPROM_EN
				eeprom_alloc(p_data->data_info.p_encapsulated_info,p_data->data_info.bit_size,p_data->data_info.frame_size);
#endif
			}else if(p_data->data_info.pwr_down_mode == rsu_pwr_on_only_nvram){
				#if NVRAM_EN
				noinit_alloc(p_data->data_info.p_encapsulated_info,p_data->data_info.bit_size);
				#endif
			}else{
				//error return

			}
			return p_data;
		}
		/*case RSU_INST_ARRAY:
		{
			rsu_array_t *p_array;
			p_array = malloc(sizeof(rsu_array_t));
			p_array->data_burst_write = data_burst_write;
			p_array->data_burst_read = data_burst_read;
			p_array->get_life_time = inst_get_life_time;
			p_array->data_info.p_encapsulated_info = enc_info;
			flash_alloc(p_array->data_info.p_encapsulated_info,0);
			return p_array;
		}*/
		default:
			break;

	}

	return rsu_error;
}

void rsu_deinitialize(void* p_counter, void* p_data_info){
	free(p_counter);
	free(p_data_info);


}


uint32_t counter_func_get_value(rsu_counter_info_t *p_counter){
	uint32_t counter_value;
	void *p_value;
	p_value = &counter_value;
	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_counter->p_encapsulated_info;

	if(p_counter->pwr_down_mode == rsu_pwr_off_flash){
#if FLASH_EN
		flash_read(p_value,p_enc_info);
#endif FLASH_EN
	}else if(p_counter->pwr_down_mode == rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_read(p_value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode == rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_read(p_value,p_enc_info);
		#endif
	}else{
		//TODO:return error
	}


	if(p_counter->gray_code == rsu_gray_coding_on){
		if(p_counter->error_detect == rsu_error_detection_on){

		}else{
			counter_value = gray_decode(counter_value);
		}
	}else if(p_counter->error_detect == rsu_error_detection_on){
		if(p_counter->gray_code == rsu_gray_coding_on){

		}else{

		}
	}


	return counter_value;
}


rsu_error_code counter_func_count_up(rsu_counter_info_t *p_counter){
	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_counter->p_encapsulated_info;
	uint32_t p_counter_value;
	//flash_read(&p_counter_value,p_enc_info);
	if(p_counter->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_read((void*)&p_counter_value,p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_read((void*)&p_counter_value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_read((void*)&p_counter_value,p_enc_info);
		#endif
	}else{
		return rsu_error;
		//TODO:define errors
	}

	if(p_counter->gray_code == rsu_gray_coding_on){
		p_counter_value = gray_decode(p_counter_value);
		p_counter_value = p_counter_value + 1;
		p_counter_value = gray_encode(p_counter_value);

	}else{
		p_counter_value = p_counter_value + 1;
	}

	if(p_counter->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_write((void*)&p_counter_value,p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_write((void*)&p_counter_value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_write((void*)&p_counter_value,p_enc_info);
		#endif
	}else{

		return rsu_error;
		//TODO:define errors
	}



	return rsu_no_error;
}

rsu_error_code counter_func_count_down(rsu_counter_info_t *p_counter){

	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_counter->p_encapsulated_info;
	uint32_t p_counter_value;


	if(p_counter->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_read((void*)&p_counter_value,p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_read((void*)&p_counter_value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_read((void*)&p_counter_value,p_enc_info);
		#endif

	}else{

		return rsu_error;

		//TODO:define errors
	}
	//flash_read(&p_counter_value,p_enc_info);

	if(p_counter->gray_code == rsu_gray_coding_on){
		p_counter_value = gray_decode(p_counter_value);
		p_counter_value = p_counter_value - 1;
		p_counter_value = gray_encode(p_counter_value);

	}else{
		p_counter_value = p_counter_value - 1;
	}

	if(p_counter->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_write(&p_counter_value,p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_write(&p_counter_value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_write(&p_counter_value,p_enc_info);
		#endif
	}else{
		return rsu_error;
		//TODO:define errors
	}

	return rsu_no_error;
}


rsu_error_code counter_func_update(rsu_counter_info_t *p_counter,uint32_t value){
	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_counter->p_encapsulated_info;

	if(p_counter->gray_code == rsu_gray_coding_on){
		value = gray_encode(value);
	}

	if(p_counter->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_write(&value,p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_write(&value,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_counter->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_write(&value,p_enc_info);
		#endif
	}else{

		return rsu_error;
		//TODO:define errors
	}
	//flash_write(&value,p_enc_info);


	return rsu_no_error;
}
//TODO:only flash now
rsu_error_code data_write(rsu_data_info_t *p_data, void *p_data_w){
	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_data->p_encapsulated_info;

	//flash_write(p_data_w,p_enc_info);
	if(p_data->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_write(p_data_w,p_enc_info);
#endif
	}else if(p_data->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_write(p_data_w,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_data->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_write(p_data_w,p_enc_info);
		#endif
	}else{
		return rsu_error;
		//TODO:define errors
	}
	return rsu_no_error;
}

rsu_error_code data_read(rsu_data_info_t *p_data,void* p_storage){
	flash_info_t *p_enc_info;
	p_enc_info = (flash_info_t*)p_data->p_encapsulated_info;

	//flash_read(p_storage,p_enc_info);

	if(p_data->pwr_down_mode ==  rsu_pwr_off_flash){
#if FLASH_EN
		flash_read(p_storage,p_enc_info);
#endif
	}else if(p_data->pwr_down_mode ==  rsu_pwr_off_eeprom){
#if EEPROM_EN
		eeprom_read(p_storage,(eeprom_info_t*)p_enc_info);
#endif
	}else if(p_data->pwr_down_mode ==  rsu_pwr_on_only_nvram){
		#if NVRAM_EN
		noinit_read(p_storage,p_enc_info);
		#endif

	}else{
		return rsu_error;
		//TODO:define errors
	}

	return rsu_no_error;
}

/*rsu_error_code data_burst_write(rsu_array_info_t *p_array){


	return rsu_no_error;
}

rsu_error_code data_burst_read(rsu_array_info_t *p_array,void* p_storage){
	void *p;

	return rsu_no_error;
}
*/
uint32_t inst_get_life_time(rsu_instances instance, void* p_inst){
	void *p_info;
	void *p_enc_info;
	if(instance == RSU_INST_DATA){
		p_info = &((rsu_data_t*)p_inst)->data_info;
		p_enc_info = ((rsu_data_info_t*)p_info)->p_encapsulated_info;
		return ((flash_info_t*)p_enc_info)->usage_rate;
	}else if(instance == RSU_INST_COUNTER){
		p_info = (rsu_counter_t*)p_inst;
		return ((rsu_counter_t*)p_info)->counter_info.usage_rate;
	}

	return rsu_no_error;

}



