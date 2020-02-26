/*
 * rsu_developer_version.h
 *
 *  Created on: Oct 2, 2019
 *      Author: emekcan
 */

#ifndef RSU_DEVELOPER_VERSION_H_
#define RSU_DEVELOPER_VERSION_H_
typedef enum {rsu_dev_error, rsu_dev_no_error} rsu_dev_error_code;
#include <stdint.h>

typedef rsu_dev_error_code (*func_rsu)(void);
typedef void* rsu_dev_storage_t;

rsu_dev_storage_t rsu_dev_declare_counter(uint32_t usage_rate_per_day, uint32_t reliable_month, uint32_t nvram_backup_period, uint32_t size);
rsu_dev_storage_t rsu_dev_counter_increment(rsu_dev_storage_t storage);
rsu_dev_storage_t rsu_dev_counter_decrement(rsu_dev_storage_t storage);
rsu_dev_storage_t rsu_dev_counter_update_value(rsu_dev_storage_t storage, uint32_t value);
uint32_t rsu_dev_counter_get_value(rsu_dev_storage_t storage);
void rsu_dev_remove_counter(rsu_dev_storage_t storage);


rsu_dev_storage_t rsu_dev_declare_data(uint32_t usage_rate_per_day, uint32_t reliable_month, uint32_t nvram_backup_period, uint32_t size);
rsu_dev_storage_t rsu_dev_data_write(rsu_dev_storage_t storage, void *p_data_w, uint32_t size);
rsu_dev_storage_t rsu_dev_data_read(rsu_dev_storage_t storage, void *p_data_r, uint32_t size);
void rsu_dev_remove_data(rsu_dev_storage_t* storage);


#endif /* RSU_DEVELOPER_VERSION_H_ */
