/*****************************************************************************
 * rdl.h - Rounded double-linked list and its operations
 *
 *
 *    Created on: Oct 16, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#ifndef RDL_H_
#define RDL_H_
#include "rdl_lock.h"

/*
 * Type of rounded double-linked list
 */
typedef enum rdl_result {
    RDL_RET_SUCCESS             = 0,
    RDL_RET_FAIL                = -1,   // Operation failed, need try again
    RDL_RET_ERROR               = -2    // Fatal error
} rdl_result;

/*
 * Type of rounded double-linked list
 */
typedef enum rdl_type {
    RDL_TYPE_ENROLLED           = 0x01,
    RDL_TYPE_IDLE               = 0x02
} rdl_type;

/*
 * Element of rounded double-linked list
 */
typedef struct rdl_element {
    struct rdl_element*         next;
    struct rdl_element*         prev;
    void*                       data;
    char                        lock;
} rdl_element;

/*
 * Rounded double-linked list
 */
typedef struct rdl {
    rdl_element*                head;
    rdl_element                 nil;
    rdl_type                    type;
} rdl;

/*
 * To test if an element belongs to the rounded double-linked list
 * Parameters:  rdl - The rounded double-linked list
 *              e   - Element to test
 */
#define rdl_element_match(rdl, e)  (                                       \
                                      ( ( (rdl)->type == RDL_TYPE_ENROLLED  \
                                            && 0 != (e)->data               \
                                        )                                   \
                                        ||                                  \
                                        ( (rdl)->type == RDL_TYPE_IDLE      \
                                            && 0 == (e)->data               \
                                        )                                   \
                                        ||                                  \
                                        (rdl)->head == (e)                  \
                                      ) ? 0 : -1                            \
                                    )

/*
 * Initialize rounded double-linked list head
 * Parameters:  h   - Head of rounded double-linked list
 */
#define rdl_init_head(h)           ({                                      \
                                        (h)->prev = (h);                    \
                                        (h)->next = (h);                    \
                                        (h)->data = 0;                      \
                                        (h)->lock = 0;                      \
                                    })
/*
 * Add a rounded double-linked list element between two elements
 * Parameters:  p   - Previous element
 *              e   - Element to add
 *              n   - Next element
 */
#define rdl_add_elmt(p, e, n)      ({                                      \
                                        (e)->prev = (p);                    \
                                        (e)->next = (n);                    \
                                        (p)->next = (e);                    \
                                        (n)->prev = (e);                    \
                                    })

/*
 * Remove a rounded double-linked list element between two elements
 * Parameters:  p   - Previous element
 *              e   - Element to remove
 *              n   - Next element
 */
#define rdl_remove_elmt(p, e, n)   ({                                      \
                                        (p)->next = (n);                    \
                                        (n)->prev = (p);                    \
                                    })

/*
 * Clear data pointer of an element of rounded double-linked list
 * Parameters:  e   - Element on which data pointer to clear
 */
#define rdl_element_reset(e)       ({                                      \
                                        (e)->data = 0;                      \
                                    })

/*
 *  Description: Add an element for a rounded double-linked list.
 *    Parameter: rdl  - Rounded double-linked list.
 *               elmt - Element to add.
 * Return value: On success returns RDL_RET_SUCCESS;
 *               on fail, it returns RDL_RET_FAIL.
 *               on error, it returns RDL_RET_ERROR.
 *
 */
extern inline rdl_result rdl_add(rdl *rdl, rdl_element *elmt);

/*
 *  Description: Remove an element from a rounded double-linked list
 *    Parameter: rdl  - Rounded double-linked list.
 *               elmt - Element removed.
 * Return value: On success returns RDL_RET_SUCCESS;
 *               on fail, it returns RDL_RET_FAIL.
 *               on error, it returns RDL_RET_ERROR.
 */
extern inline rdl_result rdl_remove(rdl *rdl, rdl_element **elmt);

#endif /* RDL_H_ */
