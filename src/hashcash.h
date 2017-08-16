/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     hashcash that provide aid for sha256                                  */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "uint256.h"

#ifndef HASHCASH

#define HASHCASH

/*
 * unitialise the uint256 by alpha and beta
 */
void init_target(BYTE *uint256, uint32_t alpha, uint32_t beta);

/*
 * turn 64-bit int into a array of bytes of length 8, it could use less than 8
 */
void int64_to_bytes(uint64_t num, BYTE *bytes, int len);

/*
 * initialise the value in the array to 0 for every bytes
 */
void init_bytes(BYTE *bytes, int len);

#endif
