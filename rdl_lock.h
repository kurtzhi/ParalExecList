/*****************************************************************************
 * rdl_lock.h - Lock utilities for rounded double-linked list
 *
 *
 *    Created on: Oct 18, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#ifndef RDL_LOCK_H_
#define RDL_LOCK_H_

/*
 *  Description: Test and set rounded double-linked list element lock to
 *               3 (locked) if it is 0 (unlocked).
 *    Parameter: lock  - Lock of rounded double-linked list element.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_elmt(volatile char *lock);

/*
 *  Description: Tries to test and set rounded double-linked list element lock
 *               multiple times.
 *    Parameter: lock  - Lock of rounded double-linked list element.
 *               tries - Time to try lock
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_elmt_n(volatile char *lock, int tries);

/*
 *  Description: Test and set rounded double-linked list element lock to
 *               0 (unlocked) if it is 3 (locked).
 *    Parameter: lock  - Lock of rounded double-linked list element.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Unlock an already unlocked lock will returns -1.
 */
extern inline int rdl_unlock_elmt(volatile char *lock);

/*
 *  Description: Test and set rounded double-linked list next link lock to
 *               3 (locked) if it is 0 (unlocked).
 *    Parameter: lock  - Lock of rounded double-linked list next link.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_next(volatile char *lock);

/*
 *  Description: Tries to test and set rounded double-linked list next link
 *               lock multiple times.
 *    Parameter: lock  - Lock of rounded double-linked list next link.
 *               tries - Time to try lock
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_next_n(volatile char *lock, int tries);

/*
 *  Description: Test and set rounded double-linked list next link lock to
 *               0 (unlocked) if it is 3 (locked).
 *    Parameter: lock  - Lock of rounded double-linked list next link.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Unlock an already unlocked lock will returns -1.
 */
extern inline int rdl_unlock_next(volatile char *lock);

/*
 *  Description: Test and set rounded double-linked list previous link lock to
 *               3 (locked) if it is 0 (unlocked).
 *    Parameter: lock  - Lock of rounded double-linked list previous link.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_prev(volatile char *lock);

/*
 *  Description: Tries to test and set rounded double-linked list previous link
 *               lock multiple times.
 *    Parameter: lock  - Lock of rounded double-linked list previous link.
 *               tries - Time to try lock
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Lock an already locked lock will returns -1.
 */
extern inline int rdl_trylock_prev_n(volatile char *lock, int tries);

/*
 *  Description: Test and set rounded double-linked list previous link lock to
 *               0 (unlocked) if it is 3 (locked).
 *    Parameter: lock  - Lock of rounded double-linked list previous link.
 * Return value: On success returns 0; on error, it returns -1.
 *               Warning: Unlock an already unlocked lock will returns -1.
 */
extern inline int rdl_unlock_prev(volatile char *lock);

#endif /* RDL_LOCK_H_ */
