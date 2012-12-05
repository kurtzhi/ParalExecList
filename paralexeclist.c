/*****************************************************************************
 * paralexeclist.c - Implementation of parallel execution list
 *
 *
 *    Created on: Oct 16, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#include <stdlib.h>
#include "rdl.h"
#include "paralexeclist.h"

/*
 * Parallel execution list
 */
typedef struct paralexeclist {
    rdl* enrolled;
    rdl* idle;
    void (*job_handle)(void *);
    rdl _enrolled;
    rdl _idle;
} paralexeclist;

extern int paralexeclist_create(paralexeclist_t *plist, int list_size,
        void (*consume_routine)(void *), int *mem_len) {
    int lt_size = sizeof(paralexeclist);
    int dl_size = sizeof(rdl_element) * list_size;

    paralexeclist *plt = 0;
    if (0 == (plt = (paralexeclist *) calloc(1, lt_size + dl_size))) {
        return -1;
    }

    plt->job_handle = consume_routine;

    plt->enrolled = &(plt->_enrolled);
    plt->enrolled->head = &(plt->enrolled->nil);
    plt->enrolled->type = RDL_TYPE_ENROLLED;
    rdl_init_head(plt->enrolled->head);

    plt->idle = &(plt->_idle);
    plt->idle->head = &(plt->idle->nil);
    plt->idle->type = RDL_TYPE_IDLE;
    rdl_init_head(plt->idle->head);

    rdl_element *e = (rdl_element *) ((void *) plt + lt_size);
    int i = 0;

    rdl_element *h = plt->idle->head;
    while (i < list_size) {
        rdl_add_elmt(h, e, h);
        i++;
        e++;
    }

    *mem_len = lt_size + dl_size;
    *plist = (paralexeclist_t) plt;

    return 0;
}

extern int paralexeclist_produce(paralexeclist_t list, void *data) {
    if (0 == list) {
        return -1;
    }

    paralexeclist *plt = (paralexeclist *) list;
    rdl_element *e;
    rdl_result res;

    while (RDL_RET_FAIL == (res = rdl_remove(plt->idle, &e))) {
    }
    if (RDL_RET_ERROR == res) {
        return -1;
    }

    e->data = data;

    while (RDL_RET_FAIL == (res = rdl_add(plt->enrolled, e))) {
    }
    if (RDL_RET_ERROR == res) {
        return -1;
    }

    return 0;
}

extern int paralexeclist_consume(paralexeclist_t list) {
    if (0 == list) {
        return -1;
    }

    paralexeclist *plt = (paralexeclist *) list;
    rdl_element *e;
    rdl_result res;

    while (RDL_RET_FAIL == (res = rdl_remove(plt->enrolled, &e))) {
    }
    if (RDL_RET_ERROR == res) {
        return -1;
    }

    plt->job_handle(e->data);
    rdl_element_reset(e);

    while (RDL_RET_FAIL == (res = rdl_add(plt->idle, e))) {
    }
    if (RDL_RET_ERROR == res) {
        return -1;
    }

    return 0;
}

extern int paralexeclist_destroy(paralexeclist_t *plist) {
    if (0 == *plist) {
        return -1;
    }

    free(*plist);
    *plist = 0;
    return 0;
}
