/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Safe memory allocation library                                        */
/*                                                                           */
/*****************************************************************************/

#include <stdlib.h>

#ifndef MEMORY_ALLOCATOR

#define MEMORY_ALLOCATOR

#define MEMORY_FAILURE_MSG "memory allocation error\n"


void* safe_malloc(size_t size);
void* safe_realloc(void *data, size_t size);

#endif
