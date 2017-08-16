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

#include "solution.h"
#include "hashcash.h"
#include "sha256.h"
#include "uint256.h"

SHA256_CTX hashed_seed(BYTE *seed){
	SHA256_CTX hash;
	sha256_init(&hash);
	sha256_update(&hash, seed, SEED_LEN);
	return hash;
}

int is_valid_solution(BYTE *target, uint64_t nonce, SHA256_CTX hash_ctx){
	BYTE pad[PAD_LEN];
	init_bytes(pad, PAD_LEN);
	int64_to_bytes(nonce, &pad[PAD_LEN - 1], PAD_LEN);

	sha256_update(&hash_ctx, pad, PAD_LEN);

	BYTE first_hash[HASH_LEN];
	sha256_final(&hash_ctx, first_hash);

	SHA256_CTX second_hash_ctx;
	sha256_init(&second_hash_ctx);
	sha256_update(&second_hash_ctx, first_hash, HASH_LEN);

	BYTE second_hash[HASH_LEN];
	sha256_final(&second_hash_ctx, second_hash);

	/*
	 * if second hash is smaller than target
	 */
	if(sha256_compare(second_hash, target) < 0){
		return 1;
	}
	return 0;
}
