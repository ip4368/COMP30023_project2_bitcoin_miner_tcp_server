/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     this is just the worker for multi-threading solution finding          */
/*     used by work_daemon while job is being dispatched                     */
/*                                                                           */
/*****************************************************************************/

#ifndef WORK_WORKER

#define WORK_WORKER

/*
 * this is a worker thread function to actually try to find solution
 * and this respond to client as well
 * thread safe on responding (no 2 worker will be responding for 1 message)
 */
void *work_worker(void *data);

#endif
