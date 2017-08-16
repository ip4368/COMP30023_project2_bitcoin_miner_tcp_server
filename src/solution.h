/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     componenet to make solution checking easier                           */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>

#include "sha256.h"

#ifndef SOLUTION

#define SOLUTION

#ifndef SEED_LEN
#define SEED_LEN 32
#endif

#define HASH_LEN 32
#define TARGET_LEN 32
#define PAD_LEN 8

/*
 * This hash the seed to a half-done seed, need to update with the nonce
 * then do second hash
 */
SHA256_CTX hashed_seed(BYTE *seed);

/*
 * this will check the target, the nonce, and half-done first seed
 * this can be used straight ahead after the hashed_seed is used
 * to gain the target, please use init_target in hashcash.h
 */
int is_valid_solution(BYTE *target, uint64_t nonce, SHA256_CTX hash_ctx);

#endif
