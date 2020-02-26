// Do not remove the include below
#include "reliable_storage_unit.h"
#include "optiboot.h"
#include "utilities.h"
//#include "reed_solomon.h"
//#include "conf.h"
#include "RS-FEC.h"

void test_case_0(void);
void test_case_1(void);
void test_case_2(void);
void test_at32u4_eeprom(void);
void test_at32u4_eeprom_counter(void);
void test_developer_lib(void);

//int set_flag __attribute__((section(".noinit"))) ;
//int savedFlag __attribute__((section(".noinit")))  ;

rsu_counter_t* test_counter;
rsu_data_t* test_data;
rsu_data_t* test_data2;
rsu_data_t* test_data3;


uint8_t test[11] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};
uint8_t test_eeprom1=0;
uint8_t test_eeprom2=1;
typedef struct data_encapsulated_info{
	uint8_t address;
	uint8_t size;
	uint8_t page;
	uint8_t frame_size;
	uint8_t current_frame;
	uint32_t usage_rate ;

}rsu_data_enc_info_t;

rsu_data_enc_info_t *p_info;
rsu_data_enc_info_t *p_info2;
rsu_data_enc_info_t *p_info3;
rsu_data_enc_info_t *p_counter_info;

uint32_t read_data;
uint8_t read_data2;
uint8_t read_data3;

uint8_t test_read[11];
uint32_t value;


int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup()
{
	Serial.begin(57600);
	delay(5000);
	Serial.println("test has started");
    //Serial.println("\n[memCheck]");
    //Serial.println(freeRam());
	rsu_data_info_t test_data_inf;
	test_data_inf.checkpoint_interval = 0;
	test_data_inf.error_detect = rsu_error_detection_off;
	test_data_inf.pwr_down_mode = rsu_pwr_off_eeprom;
	test_data_inf.bit_size =  8;
	test_data_inf.encryption = rsu_enc_off;
	test_data_inf.frame_size = 100;
	test_data = (rsu_data_t*)rsu_init(RSU_INST_DATA,&test_data_inf);
	    //Serial.println("\n[memCheck]");
	/*test_data_inf.checkpoint_interval = 0;
	test_data_inf.error_detect = rsu_error_detection_off;
	test_data_inf.pwr_down_mode = rsu_pwr_off_eeprom;
	test_data_inf.bit_size = sizeof(test) * 8;
	test_data_inf.encryption = rsu_enc_off;
	test_data_inf.frame_size = 5;
	test_data = (rsu_data_t*)rsu_init(RSU_INST_DATA,&test_data_inf);*/
    //Serial.println("\n[memCheck]");
    //Serial.println(freeRam());
	rsu_data_info_t test_data_inf2;
	test_data_inf2.checkpoint_interval = 0;
	test_data_inf2.error_detect = rsu_error_detection_off;
	test_data_inf2.pwr_down_mode = rsu_pwr_off_eeprom;
	test_data_inf2.bit_size = 8;
	test_data_inf2.encryption = rsu_enc_off;
	test_data_inf2.frame_size = 1;
	test_data2 = (rsu_data_t*)rsu_init(RSU_INST_DATA,&test_data_inf2);
    //Serial.println("\n[memCheck]");
    //Serial.println(freeRam());
	rsu_data_info_t test_data_inf3;
	test_data_inf3.checkpoint_interval = 0;
	test_data_inf3.error_detect = rsu_error_detection_off;
	test_data_inf3.pwr_down_mode = rsu_pwr_off_eeprom;
	test_data_inf3.bit_size = 8;
	test_data_inf3.encryption = rsu_enc_off;
	test_data_inf3.frame_size = 1;
	test_data3 = (rsu_data_t*)rsu_init(RSU_INST_DATA,&test_data_inf3);
    //Serial.println("\n[memCheck]");
    //Serial.println(freeRam());
	rsu_counter_info_t test_counter_inf0;
	test_counter_inf0.bit_size = 32;
	test_counter_inf0.checkpoint_interval = 0;
	test_counter_inf0.encryption = rsu_enc_off;
	test_counter_inf0.error_detect = rsu_error_detection_off;
	test_counter_inf0.gray_code = rsu_gray_coding_on;
	test_counter_inf0.offset = 0;
	test_counter_inf0.pwr_down_mode = rsu_pwr_off_eeprom;
	test_counter_inf0.frame_size = 1;
	test_counter = (rsu_counter_t*)rsu_init(RSU_INST_COUNTER,&test_counter_inf0);

	p_info = (rsu_data_enc_info_t*)test_data->data_info.p_encapsulated_info;
	p_info2 = (rsu_data_enc_info_t*)test_data2->data_info.p_encapsulated_info;
	p_info3 = (rsu_data_enc_info_t*)test_data3->data_info.p_encapsulated_info;
	p_counter_info = (rsu_data_enc_info_t*)test_counter->counter_info.p_encapsulated_info;

}


uint32_t data_test1 = 0x20326408;
uint8_t data_test2 = 0x30;
uint8_t data_test3 = 0x40;
uint32_t counter_test_data = 0x50;

uint8_t case_flag = 5;

void loop()
{

	switch(case_flag){
	case 0:
		test_case_0();
		break;
	case 1:
		test_case_1();
		break;
	case 2:
		test_case_2();
		break;
	case 3:
		test_at32u4_eeprom();
		break;
	case 4:
		test_at32u4_eeprom_counter();
		break;
	case 5:
		test_developer_lib();
		break;
	default:
		break;
	}



	//delay(5000);


}

void test_case_0(void){
    //Serial.println("\n[memCheck]");
   // Serial.println(freeRam());

	Serial.print("address:");
	Serial.println(p_info->address);
	Serial.print("page:");
	Serial.println(p_info->page);
	Serial.print("size:");
	Serial.println(p_info->size);
	Serial.print("frame size:");
	Serial.println(p_info->frame_size);
	Serial.print("current frame:");
	Serial.println(p_info->current_frame);
	Serial.print("usage rate:");
	Serial.println(p_info->usage_rate / p_info->frame_size);
	Serial.print("address:");
	Serial.println(p_info2->address);
	Serial.print("page:");
	Serial.println(p_info2->page);
	Serial.print("size:");
	Serial.println(p_info2->size);
	Serial.print("usage rate:");
	Serial.println(p_info2->usage_rate);
	Serial.print("address:");
	Serial.println(p_info3->address);
	Serial.print("page:");
	Serial.println(p_info3->page);
	Serial.print("size:");
	Serial.println(p_info3->size);
	Serial.print("usage rate:");
	Serial.println(p_info3->usage_rate);
	//optiboot_readPage(flashSpace, ramBuffer, 1);
	Serial.print("read_data:");
	Serial.println(read_data);
	Serial.print("read_data2:");
	Serial.println(read_data2);
	Serial.print("read_data3:");
	Serial.println(read_data3);
    //Serial.println("\n[memCheck]");
    //Serial.println(freeRam());
	test_data->data_write(&test_data->data_info,test);
	test_data2->data_write(&test_data2->data_info,&data_test2);
	test_data3->data_write(&test_data3->data_info,&data_test3);

	delay(5000);
	test_data->data_read(&test_data->data_info,test_read);
	test_data2->data_read(&test_data2->data_info,&read_data2);
	test_data3->data_read(&test_data3->data_info,&read_data3);

	Serial.print("read_data:");
	for(uint8_t i=0; i<11;i++){
		Serial.println(test_read[i]);
	}
	Serial.print("read_data2:");
	Serial.println(read_data2);
	Serial.print("read_data3:");
	Serial.println(read_data3);



   //Serial.println("\n[memCheck]");
    //println(freeRam());
}
void test_case_1(void){

	Serial.print("current frame:");
	Serial.println(p_counter_info->current_frame);
	test_counter->counter_inc(&test_counter->counter_info);
	value = test_counter->counter_get_value(&test_counter->counter_info);
	Serial.print("address:");
	Serial.println(p_counter_info->address);
	Serial.print("page:");
	Serial.println(p_counter_info->page);
	Serial.print("size:");
	Serial.println(p_counter_info->size);
	Serial.print("frame size:");
	Serial.println(p_counter_info->frame_size);
	Serial.print("current frame:");
	Serial.println(p_counter_info->current_frame);
	Serial.println("counter value");
	Serial.println(value);
	delay(2000);
	test_counter->counter_inc(&test_counter->counter_info);
	value = test_counter->counter_get_value(&test_counter->counter_info);
	Serial.print("address:");
	Serial.println(p_counter_info->address);
	Serial.print("page:");
	Serial.println(p_counter_info->page);
	Serial.print("size:");
	Serial.println(p_counter_info->size);
	Serial.println("counter value");
	Serial.print("current frame:");
	Serial.println(p_counter_info->current_frame);
	Serial.println(value);
	delay(2000);
	test_counter->counter_inc(&test_counter->counter_info);
	value = test_counter->counter_get_value(&test_counter->counter_info);
	Serial.print("address:");
	Serial.println(p_counter_info->address);
	Serial.print("page:");
	Serial.println(p_counter_info->page);
	Serial.print("size:");
	Serial.println(p_counter_info->size);
	Serial.println("counter value");
	Serial.print("current frame:");
	Serial.println(p_counter_info->current_frame);
	Serial.println(value);
	delay(1000);
	/*test_counter->counter_update(&test_counter->counter_info,counter_test_data);
	value = test_counter->counter_get_value(&test_counter->counter_info);
	Serial.print("address:");
	Serial.println(p_counter_info->address);
	Serial.print("page:");
	Serial.println(p_counter_info->page);
	Serial.print("size:");
	Serial.println(p_counter_info->size);
	Serial.println("counter value");
	Serial.print("current frame:");
	Serial.println(p_counter_info->current_frame);
	Serial.println(value);*/


}
uint32_t cycle_counter = 0;

void test_at32u4_eeprom(){
	cycle_counter++;
	test_data->data_write(&test_data->data_info,&test_eeprom1);
	test_data->data_read(&test_data->data_info,&read_data2);

	Serial.println("counter: " + String(cycle_counter));
	if(read_data2 == test_eeprom1){
		Serial.println("value: " + String(read_data2));
		Serial.println("----PASSED----");
	}else{
		Serial.println("----FAILED----");
		Serial.println("value: " + String(read_data2));
	}
	delay(10);
	cycle_counter++;

	test_data->data_write(&test_data->data_info,&test_eeprom2);
	test_data->data_read(&test_data->data_info,&read_data2);
	  Serial.println("counter: " + String(cycle_counter));
	if(read_data2 == test_eeprom2){
		Serial.println("value: " + String(read_data2));
		Serial.println("----PASSED----");
	}else{
		Serial.println("----FAILED----");
		Serial.println("value: " + String(read_data2));
	}
}

void test_at32u4_eeprom_counter(){
	cycle_counter++;
	test_counter->counter_inc(&test_counter->counter_info);
	value = test_counter->counter_get_value(&test_counter->counter_info);

	Serial.println("counter: " + String(cycle_counter));
	if(value == cycle_counter){
		Serial.println("value: " + String(value));
		Serial.println("----PASSED----");
	}else{
		Serial.println("----FAILED----");
		Serial.println("value: " + String(value));
	}
	delay(10);
}

void test_developer_lib(void){
	uint32_t dev_value_test;
	rsu_dev_storage_t p_storage,p_storage2;
	p_storage = rsu_dev_declare_counter(1,1,0,4);
	p_storage2 = rsu_dev_declare_data(1,1,0,11);

	while(1){
		Serial.println("Data test before write:");
		for(uint8_t i=0; i<11;i++){
			Serial.println(test_read[i]);
		}
		rsu_dev_data_write(p_storage2,test,sizeof(test));
		rsu_dev_data_read(p_storage2,test_read,11);
		for(uint8_t i=0; i<11;i++){
			Serial.println(test_read[i]);
		}
		rsu_dev_counter_increment(p_storage);
		dev_value_test = rsu_dev_counter_get_value(p_storage);
		Serial.print("Counter Value: ");
		Serial.println(dev_value_test);
		delay(2000);
	}
}

/*uint32_t test_gray = 18;
uint32_t test_2;
//char message[223] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec at mi ligula. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Ut vestibulum ut lorem a facilisis. Nullam quam sem metus";
char message2[24] = "Lorem ipsum dolor sit a";
char message1[60] = "Ernie, you have a banana in your ear!";
const int msglen = 24;  const uint8_t ECC_LENGTH = 8;  //Max message lenght, and "gurdian bytes", Max corrected bytes ECC_LENGTH/2
char message_frame[msglen]; // The message size would be different, so need a container
char repaired[msglen];    char encoded[msglen + ECC_LENGTH];

RS::ReedSolomon<msglen, ECC_LENGTH> rs;
void test_case_2(void){
	uint32_t start_enc,end_enc;
	//rslogtable_test();
	memset(message_frame, 0, sizeof(message_frame));        // Clear the array
	for(int i = 0; i < msglen; i++) {
	  message_frame[i] = message2[i];
	} // Fill with the message
	start_enc = micros();
	rs.Encode(message_frame, encoded);
	end_enc = micros();
	Serial.print("Elapsed time for encoding : ");
	Serial.println((end_enc - start_enc)/1000);
	Serial.print("Original: "); Serial.println(message_frame);
	Serial.print("Encoded:  ");        for(uint8_t i = 0; i < sizeof(encoded); i++) {    Serial.print(encoded[i]);    }    Serial.println("");
	for(uint8_t i = 5; i < 9; i++) {        encoded[i] = 'a';    } //Let's steal some byte from 20 to 25.
	Serial.print("Corrupted: ");      for(uint8_t i = 0; i < sizeof(encoded); i++) {     Serial.print(encoded[i]);    }    Serial.println("");
	start_enc = micros();
	rs.Decode(encoded, repaired);
	end_enc = micros();
	Serial.print("Elapsed time for decoding : ");
	Serial.println((end_enc - start_enc)/1000);
	String result;    memcmp(message_frame, repaired, msglen) == 0 ? result="SUCCESS" : result="FAILURE"; //Compare the arrays
	Serial.print("Result: ");   Serial.println(result);
	Serial.print("Repaired: "); Serial.println(repaired);
	Serial.println("TEST");
}*/
