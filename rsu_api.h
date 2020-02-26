/*
 * rsu_api.h
 *
 *  Created on: Aug 7, 2019
 *      Author: emekcan
 */

#ifndef RSU_API_H_
#define RSU_API_H_

#include<stdint.h>

typedef enum {rsu_error, rsu_no_error} rsu_error_code;

typedef enum {rsu_error_detection_on,rsu_error_detection_off} rsu_error_detection;

typedef enum {rsu_gray_coding_on,rsu_gray_coding_off} rsu_gray_coding;

typedef enum{rsu_pwr_off_flash, rsu_pwr_off_eeprom, rsu_pwr_on_only_nvram} rsu_pwr_down_conf;

typedef enum{rsu_enc_on, rsu_enc_off}rus_enc_conf;

typedef enum{RSU_INST_COUNTER,RSU_INST_DATA,RSU_INST_ARRAY} rsu_instances;

typedef struct blob{
	uint32_t checkpoint_interval;
	rsu_pwr_down_conf pwr_down_mode;
	rsu_gray_coding gray_code;
	rsu_error_detection error_detect;
	rus_enc_conf encryption;
	uint8_t  bit_size;
	uint32_t offset;
	uint8_t frame_size;
	void *p_encapsulated_info;
	uint32_t usage_rate ;
}rsu_counter_info_t;

typedef struct rsu_data_i{
	uint32_t checkpoint_interval;
	rsu_pwr_down_conf pwr_down_mode;
	rsu_error_detection error_detect;
	rus_enc_conf encryption;
	uint8_t  bit_size;
	uint8_t frame_size;
	void *p_encapsulated_info;
	uint32_t usage_rate ;
}rsu_data_info_t;

/*typedef struct rsu_array_i{
	uint32_t checkpoint_interval;
	rsu_pwr_down_conf pwr_down_mode;
	rsu_error_detection error_detect;
	rus_enc_conf encryption;
	uint8_t bit_size;
	uint8_t  array_size;
	void *p_encapsulated_info;
}rsu_array_info_t;*/

typedef rsu_error_code (*callback)(rsu_counter_info_t*);
typedef rsu_error_code (*callback_update)(rsu_counter_info_t*,uint32_t);
typedef uint32_t (*callback_get_value_counter)(rsu_counter_info_t*);

typedef uint32_t (*callback_get_value)(void*);
typedef uint32_t (*callback_get_lifetime)(rsu_instances,void*);


typedef rsu_error_code (*callback_write_data)(rsu_data_info_t*, void* );
typedef rsu_error_code (*callback_read)(rsu_data_info_t*,void* );

//typedef rsu_error_code (*callback_write_array)(rsu_array_info_t*, void*);
//typedef rsu_error_code (*callback_read_array)(rsu_array_info_t*, void*);

typedef struct rsu_counter_t {
	rsu_counter_info_t counter_info;
	callback counter_inc;
	callback counter_dec;
	callback_update counter_update;
	callback_get_value_counter counter_get_value;
	callback_get_lifetime get_life_time;
	uint32_t usage_rate;
}rsu_counter_t;

typedef struct rsu_data_t {
	rsu_data_info_t data_info;
	callback_write_data data_write;
	callback_read data_read;
	callback_get_lifetime get_life_time;
	uint32_t usage_rate;
}rsu_data_t;

/*typedef struct rsu_array_t {
	rsu_array_info_t data_info;
	callback_write_array data_burst_write;
	callback_read_array data_burst_read;
	callback_get_value get_life_time;

}rsu_array_t;*/


void* rsu_init(rsu_instances inst,void *p_data_inf);
void rsu_deinitialize(void* p_counter, void* p_data_info);


#endif /* RSU_API_H_ */
