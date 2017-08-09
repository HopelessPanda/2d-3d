/*
***********************************************************************************
**                                                                                *
** INTEL CORPORATION                                                              *
** Copyright 2008-2009 Intel Corporation All Rights Reserved.                     *
**                                                                                *
** The source code contained or described herein and all documents related to     *
** the source code ("Material") are owned by Intel Corporation or its suppliers   *
** or licensors. Title to the Material remains with Intel Corporation or its      *
** suppliers and licensors. The Material contains trade secrets and proprietary   *
** and confidential information of Intel or its suppliers and licensors. The      *
** Material is protected by worldwide copyright and trade secret laws and         *
** treaty provisions. No part of the Material may be used, copied, reproduced,    *
** modified, published, uploaded, posted, transmitted, distributed, or            *
** disclosed in any way without Intel's prior express written permission.         *
**                                                                                *
** No license under any patent, copyright, trade secret or other intellectual     *
** property right is granted to or conferred upon you by disclosure or delivery   *
** of the Materials, either expressly, by implication, inducement, estoppel or    *
** otherwise. Any license under such intellectual property rights must be         *
** express and approved by Intel in writing.                                      *
**                                                                                *
***********************************************************************************
*/
#ifndef __AIO_INCLUDED
#define __AIO_INCLUDED

#ifdef  __cplusplus
extern "C" {
#endif


/*
**  INCLUDE FILES
**  -------------
*/
#include <windows.h>
#include <stddef.h>


/*
**  CONSTANTS
**  ---------
*/

/* Possible values of sigev_notify field (see structure sigevent below)     */

        /* A queued signal, with an application-defined value, is generated
           when the event of interest occurs.                               */
#define SIGEV_SIGNAL    0
        /* No asynchronous notification is delivered when the event of 
           interest occurs.                                                 */
#define SIGEV_NONE      1
        /* A notification function is called to perform notification.       */
#define SIGEV_THREAD    2


/* Return values of aio_cancel() */
        /* none of the requested operations could be canceled since they are 
           already complete.                                                */
#define AIO_ALLDONE       0
        /* all requested operations have been canceled.                     */
#define AIO_CANCELED      1
        /* some of the requested operations could not be canceled since 
           they are in progress.                                            */
#define AIO_NOTCANCELED   2


/* Operation codes for lio_listio           */
#define LIO_NOP         0
#define LIO_READ        1
#define LIO_WRITE       2

/* Synchronization options for lio_listio   */
#define LIO_WAIT        0
#define LIO_NOWAIT      1

/* Additional mode flags */
#define O_SYNC    010000
#define O_DSYNC   O_SYNC


/*
**  TYPES
**  -----
*/


/* 
** Signal value 
*/
typedef union sigval {
    int     sival_int;      /* Integer signal value     */
    void   *sival_ptr;      /* Pointer signal value     */
} sigval_t;


/*
** Notification info
*/
typedef void    (*notify_func_t)(sigval_t);
typedef int     pthread_attr_t; 


/*
** Signal event descriptor
*/
typedef struct sigevent {
    int        sigev_notify;    /* Notification type    */
    int        sigev_signo;     /* Signal number        */
    sigval_t   sigev_value;     /* Signal value         */
    
    notify_func_t   sigev_notify_function;      /* Notification function    */
    pthread_attr_t *sigev_notify_attributes;    /* Notification attributes  */
} sigevent_t;


/* 
** Space for internal state 
*/
#ifndef addinfo_t
    /* This file is included in user code - use opaque type */
    typedef intptr_t         addinfo_t[10];
    typedef volatile char*   aio_buf_t;
    #pragma comment(lib, "libicaio.lib")
#endif


/*
** Asynchronous I/O control block 
*/
struct aiocb {
    HANDLE           aio_fildes;      /* File desriptor             */
    intptr_t         aio_offset;      /* File offset                */
    aio_buf_t        aio_buf;         /* Location of buffer         */
    intptr_t         aio_nbytes;      /* Length of transfer         */
    int              aio_reqprio;     /* Request priority offset    */
    struct sigevent  aio_sigevent;    /* Signal number and value    */
    int              aio_lio_opcode;  /* Operation to be performed  */

    addinfo_t        ctrl;            /* Internal state             */
};


/*
** Time specificator
*/
typedef struct timespec {
    long    tv_sec;             /* seconds      */
    long    tv_nsec;            /* nanoseconds  */
} timespec_t;



/*
**  FUNCTIONS
*/

/* asynchronous read */
int     aio_read    (struct aiocb *aiocbp);
 
/* asynchronous write */
int     aio_write   (struct aiocb *aiocbp);

/* cancel one or more asynchronous I/O request */
int     aio_cancel  (HANDLE fh, struct aiocb *aiocbp);

/* suspend the calling thread until at least one I/O operations has completed */
int     aio_suspend (const struct aiocb * const cblist[],
                     int n, const struct timespec *timeout);

/* return status of an asynchronous I/O operation  */
intptr_t aio_return (struct aiocb *aiocbp);

/* list directed I/O */
int     lio_listio  (int mode, struct aiocb *list[],
                     int nent, struct sigevent *sig);

/* retrieve errors status for an asynchronous I/O operation  */
int     aio_error   (const struct aiocb *aiocbp);

/* asynchronous file synchronization */
int     aio_fsync   (int op, struct aiocb *aiocbp);



/*
** ERROR HANDLING
** --------------
*/

/* routine returning errno value */
int*    aio_errno();


/* errno redifinition */
#if defined(errno)
   #undef errno
#endif
#define errno   (*aio_errno())


/* Posix-AIO error codes */
#if !defined (EAGAIN)
    #define EAGAIN        11
#endif
#if !defined (EINVAL)
    #define EINVAL        22
#endif


/* Other Posix-AIO errors compatible with Windows errors */
#define ECANCELED         ERROR_OPERATION_ABORTED
#define EINPROGRESS       ERROR_IO_INCOMPLETE



#ifdef  __cplusplus
}
#endif

#endif /* _AIO_H_ */
