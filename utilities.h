/*
 * utilities.h
 *
 *  Created on: Sep 13, 2019
 *      Author: emekcan
 */

uint32_t gray_encode(uint32_t raw_value){
	return raw_value ^ (raw_value >> 1);
}

uint32_t gray_decode(uint32_t gray_encoded_value){
	uint32_t mask = ((uint32_t)1) << 31;
	uint32_t binary = gray_encoded_value & mask; // get the MSB
    while ((mask >>= 1) != 0) // step through the bits...
    {
        binary |= mask & ((binary >> 1) ^ gray_encoded_value); // compute the new result
    }
    return binary;
}
