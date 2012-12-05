/*****************************************************************************
 * paralexeclist.h - Parallel execution list and its utilities
 *
 *   Description: Parallel execution list can run in multi-thread
 *                and multi-process environment.
 *                While runs in  multi-process environment, you need map
 *                paralexeclist_t on shared memory so that every process
 *                can reach, and as well as data pointer registered to parallel
 *                execution list.
 *
 *    Created on: Oct 16, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#ifndef PARALEXECLIST_H_
#define PARALEXECLIST_H_

/*
 * Parallel execution list type
 */
typedef void * paralexeclist_t;

/*
 *  Description: Create Parallel execution list.
 *    Parameter: plist [out]            - Parallel execution list.
 *               list_size [in]         - Size of parallel execution list.
 *               consume_routine [in]   - Routine to consume data.
 *               mem_len [out]          - Memory length of list in bytes.
 * Return value: On success returns 0; on error, it returns -1.
 */
extern int paralexeclist_create(paralexeclist_t *plist, int list_size,
        void (*consume_routine)(void *), int *mem_len);

/*
 *  Description: Add data to parallel execution list for consuming later.
 *    Parameter: list [in]              - Parallel execution list.
 *               data [in]              - A void pointer to data.
 * Return value: On success returns 0; on error, it returns -1.
 */
extern int paralexeclist_produce(paralexeclist_t list, void *data);

/*
 *  Description: Consuming data on parallel execution list
 *    Parameter: list [in]              - Parallel execution list.
 * Return value: On success returns 0; on error, it returns -1.
 */
extern int paralexeclist_consume(paralexeclist_t list);

/*
 *  Description: Destroy parallel execution list
 *    Parameter: plist [in]              - Parallel execution list.
 * Return value: On success returns 0; on error, it returns -1.
 */
extern int paralexeclist_destroy(paralexeclist_t *plist);

#endif /* PARALEXECLIST_H_ */
