/*
 * rsu_developer_version.c
 *
 *  Created on: Oct 2, 2019
 *      Author: emekcan
 */

#include "rsu_developer_version.h"
#include "rsu_api.h"
#include "conf.h"

//TODO:MEMORY check, return error
rsu_dev_storage_t rsu_dev_declare_counter(uint32_t usage_rate_per_day, uint32_t reliable_month, uint32_t nvram_backup_period, uint32_t size){
	rsu_counter_t* p_counter_api;
	rsu_counter_info_t counter_inf0;
	counter_inf0.bit_size = size*8;
	counter_inf0.checkpoint_interval = nvram_backup_period;
	counter_inf0.encryption = rsu_enc_off;
	counter_inf0.error_detect = rsu_error_detection_off;
	counter_inf0.gray_code = rsu_gray_coding_on;
	counter_inf0.offset = 0;
	if(EEPROM_EN){
		counter_inf0.pwr_down_mode = rsu_pwr_off_eeprom;
	}else if(FLASH_EN){
		counter_inf0.pwr_down_mode = rsu_pwr_off_eeprom;
	}else{
		counter_inf0.pwr_down_mode = rsu_pwr_on_only_nvram;
	}
	uint32_t wear_off_rate =   (reliable_month * 30 * usage_rate_per_day)  / EEPROM_FAILURE_CYCLE;
	if(nvram_backup_period){
		counter_inf0.frame_size = wear_off_rate / nvram_backup_period;
	}else{
		counter_inf0.frame_size = wear_off_rate;
	}

	p_counter_api = (rsu_counter_t*)rsu_init(RSU_INST_COUNTER,&counter_inf0);

	return p_counter_api;

}

rsu_dev_storage_t rsu_dev_counter_increment(rsu_dev_storage_t storage){
	rsu_counter_t* p_counter;
	p_counter = (rsu_counter_t*)storage;
	p_counter->counter_inc(&p_counter->counter_info);

	return p_counter;

}
rsu_dev_storage_t rsu_dev_counter_decrement(rsu_dev_storage_t storage){
	rsu_counter_t* p_counter;
	p_counter = (rsu_counter_t*)storage;
	p_counter->counter_dec(&p_counter->counter_info);

	return p_counter;
}
rsu_dev_storage_t rsu_dev_counter_update_value(rsu_dev_storage_t storage, uint32_t value){
	rsu_counter_t* p_counter;
	p_counter = (rsu_counter_t*)storage;

	p_counter->counter_update(&p_counter->counter_info,value);

	return p_counter;

}
uint32_t rsu_dev_counter_get_value(rsu_dev_storage_t storage){
	rsu_counter_t* p_counter;
	uint32_t value;
	p_counter = (rsu_counter_t*)storage;

	value = p_counter->counter_get_value(&p_counter->counter_info);

	return value;

}

rsu_dev_storage_t rsu_dev_declare_data(uint32_t usage_rate_per_day, uint32_t reliable_month, uint32_t nvram_backup_period, uint32_t size){

	rsu_data_t* p_rsu_data;
	rsu_data_info_t data_inf;

	data_inf.bit_size =  size*8;
	data_inf.encryption = rsu_enc_off;
	data_inf.checkpoint_interval = nvram_backup_period;
	data_inf.error_detect = rsu_error_detection_off;
	if(EEPROM_EN){
		data_inf.pwr_down_mode = rsu_pwr_off_eeprom;
	}else if(FLASH_EN){
		data_inf.pwr_down_mode = rsu_pwr_off_flash;

	}
	uint32_t wear_off_rate =   (reliable_month * 30 * usage_rate_per_day)  / EEPROM_FAILURE_CYCLE;

	if(nvram_backup_period){
		data_inf.frame_size = wear_off_rate / nvram_backup_period;
		}else{
			data_inf.frame_size = wear_off_rate;
		}
	p_rsu_data = (rsu_data_t*)rsu_init(RSU_INST_DATA,&data_inf);


	return p_rsu_data;

}

rsu_dev_storage_t rsu_dev_data_write(rsu_dev_storage_t storage, void *p_data_w, uint32_t size){
	rsu_data_t* p_data;
	p_data = (rsu_data_t*)storage;

	p_data->data_write(&p_data->data_info,p_data_w);

	return p_data;
}

rsu_dev_storage_t rsu_dev_data_read(rsu_dev_storage_t storage, void *p_data_r, uint32_t size){
	rsu_data_t* p_data;
	p_data = (rsu_data_t*)storage;

	p_data->data_read(&p_data->data_info,p_data_r);

	return p_data;
}

void rsu_dev_remove_data(rsu_dev_storage_t* storage){
	//TODO: not enough to free the memory
	free(storage);
}
