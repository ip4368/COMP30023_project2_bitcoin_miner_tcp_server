/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "hashcash.h"
#include "uint256.h"

#define BYTE_MASK_ON 0xFF
#define BIT_PER_BYTE 8

void init_target(BYTE *uint256, uint32_t alpha, uint32_t beta){
	uint256_init(uint256);

	uint32_t exp = 8 * (alpha - 3);

	/*
	 * init the base to 2 first
	 */
	BYTE base[32];
	uint256_init(base);
	base[31] = (BYTE)2;

	/*
	 * the right hand side of the equation
	 * (actually doesn't matter left or right, just easier to mention)
	 */
	BYTE right[32];
	uint256_init(right);
	uint256_exp(right, base, exp);

	/*
	 * the left hand side of the equation
	 */
	BYTE left[32];
	uint256_init(left);
	int64_to_bytes((uint64_t)beta, &left[31], 3);

	/*
	 * just multiply to get target
	 */
	uint256_mul(uint256, left, right);
}

void int64_to_bytes(uint64_t num, BYTE *bytes, int len){
	int i;
	for(i=0; i<len; i++){
		bytes[-i] = (num >> (i * BIT_PER_BYTE)) & BYTE_MASK_ON;
	}
}

void init_bytes(BYTE *bytes, int len){
	int i;
	for(i=0; i<len; i++){
		bytes[i] = (BYTE)0;
	}
}
