/*****************************************************************************
 * rdl.c - Implementation of rounded double-linked list operations
 *
 * Memory barrier on element removal/add to prevent processes reordering:
 *  Enrolled: h -> a -> b -> c -> h && h <- a <- b <- c <- h
 *  Idle: h -> f -> g -> h && h <- f <- g <- h
 *  |       Process A       |       Process B       |
 *  |  lock(a.next)         |                       |
 *  |  lock(b)              |                       |
 *  |  lock(c.prev)         |                       |
 *  |  a.next = c           |                       |
 *  |  c.prev = a           |                       |
 *  |  unlock(c.prev)       |                       |
 *  |  unlock(a.next)       |                       |
 *  |  b.prev = ?           |                       |
 *  |  b.next = ?           |                       |
 *  |  b.data = NULL        |                       |
 *  |  unlock(b)            |                       |
 *  Enrolled: h -> a -> c -> h && h <- a <- c <- h
 *  Idle: h -> f -> b -> g -> h && h <- f <- b <- g <- h
 *  |                       |  lock(b.next)         |
 *  |                       |  memory barrier       |
 *  |                       |  if(b.data == NULL){  |
 *  |                       |    p = enrolled.h     |
 *  |                       |  } else {             |
 *  |                       |    lock(g)            |
 *  |                       |    lock(h.prev)       |
 *
 *    Created on: Oct 16, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#include "rdl.h"


/*
 * Remove element start from the head of rounded double-linked list.
 */
inline rdl_result rdl_remove(rdl *rdl, rdl_element **elmt) {
    rdl_result ret = RDL_RET_FAIL;
    rdl_element *h = rdl->head;
    rdl_element *p = h;
    rdl_element *l, *e;

    while (p) {
        if (h == h->next) {
            break;
        }

        if (0 == rdl_trylock_next(&(p->lock))) {
            l = p;  // Left bound
            if (0 == rdl_element_match(rdl, l)) {
                p = p->next;
                if (p != h && 0 == rdl_trylock_elmt_n(&(p->lock), 2)) {
                    e = p;  // Selected element
                    p = p->next;  // Right bound
                    if (0 == rdl_trylock_prev_n(&(p->lock), 4)) {
                        rdl_remove_elmt(l, e, p);
                        ret = RDL_RET_SUCCESS;
                        if (0 != rdl_unlock_prev(&(p->lock))) {
                            return RDL_RET_ERROR;
                        }
                    } else {
                        if (0 != rdl_unlock_elmt(&(e->lock))) {
                            return RDL_RET_ERROR;
                        }
                    }
                }

                p = l->next->next;
            } else {
                p = h;
            }

            if (0 != rdl_unlock_next(&(l->lock))) {
                return RDL_RET_ERROR;
            }

            if (RDL_RET_SUCCESS == ret) {
                *elmt = e;
                return ret;
            }
        } else {
            p = p->next->next;
        }
    }

    return ret;
}

/*
 * Add element start from the tail of rounded double-linked list.
 */
inline rdl_result rdl_add(rdl *rdl, rdl_element *elmt) {
    rdl_result ret = RDL_RET_FAIL;
    rdl_element *h = rdl->head;
    rdl_element *p = h;
    rdl_element *r;

    while (p) {
        if (0 == rdl_trylock_prev(&(p->lock))) {
            r = p;  // Right bound
            p = p->prev;  // Left bound
            if (0 == rdl_trylock_next(&(p->lock))) {
                rdl_add_elmt(p, elmt, r);
                ret = RDL_RET_SUCCESS;
                if (0 != rdl_unlock_elmt(&(elmt->lock))) {
                    return RDL_RET_ERROR;
                }
                if (0 != rdl_unlock_next(&(p->lock))) {
                    return RDL_RET_ERROR;
                }
            }

            if (0 != rdl_unlock_prev(&(r->lock))) {
                return RDL_RET_ERROR;
            }

            if (RDL_RET_SUCCESS == ret) {
                return ret;
            }
            p = r->prev;
        } else {
            p = p->prev;
        }
    }

    return ret;
}
