/*****************************************************************************
 * rdl_lock.c - Lock utilities for rounded double-linked list implemented in
 *              x86/x64 assembly instructions
 *
 *
 *    Created on: Oct 18, 2012
 *        Author: Kurt Zhi
 *        E-mail: kurtzhi@outlook.com
 *      Web site: www.kurtzhi.com
 *****************************************************************************/

#include "rdl_lock.h"

inline int rdl_trylock_elmt(volatile char *lock) {
    char gra;
    __asm__ __volatile__ (
            "movb       $0x0,       %%al    ;"
            "movb       $0x3,       %%bl    ;"
            "lock                           ;"
            "cmpxchgb   %%bl,       %1      ;"
            "movb       %%al,       %0      ;"
            : "=g"(gra)
            : "m"(*lock)
            : "%al", "%bl", "memory"
            );
    return gra == 0 ? 0 : -1;
}

inline int rdl_trylock_elmt_n(volatile char *lock, int tries) {
	int i = 0;
	while (i < tries) {
		if (0 == rdl_trylock_elmt(lock)) {
			return 0;
		}
		i++;
	}
	return -1;
}

inline int rdl_unlock_elmt(volatile char *lock) {
    char gra;
    __asm__ __volatile__ (
            "movb       $0x3,       %%al    ;"
            "movb       $0x0,       %%bl    ;"
            "lock                           ;"
            "cmpxchgb	%%bl,       %1      ;"
            "movb		%%al,       %0      ;"
            : "=g"(gra)
            : "m"(*lock)
            : "%al", "%bl", "memory"
            );
    return gra == 3 ? 0 : -1;
}

inline int rdl_trylock_next(volatile char *lock) {
    char orig;
    __asm__ __volatile__ (
            "movw       $0x0,       %%ax    ;"
            "movw       $0x1,       %%bx    ;"
            "lock                           ;"
            "bts        $0x0,       %1      ;"  // Set first bit
            "cmovcw     %%bx,       %%ax    ;"  // Move if CF equals 1
            "movb       %%al,       %0      ;"
            : "=g"(orig)
            : "m"(*lock)
            : "%ax", "%bx", "memory"
            );
    return orig == 0 ? 0 : -1;
}

inline int rdl_trylock_next_n(volatile char *lock, int tries) {
    int i = 0;
    while (i < tries) {
        if (0 == rdl_trylock_next(lock)) {
            return 0;
        }
        i++;
    }
    return -1;
}

inline int rdl_unlock_next(volatile char *lock) {
    char orig;
    __asm__ __volatile__ (
            "movw       $0x0,       %%ax    ;"
            "movw       $0x1,       %%bx    ;"
            "lock                           ;"
            "btr        $0x0,       %1      ;"  // Clear first bit
            "cmovcw     %%bx,       %%ax    ;"
            "movb       %%al,       %0      ;"
            : "=g"(orig)
            : "m"(*lock)
            : "%ax", "%bx", "memory"
            );
    return orig == 1 ? 0 : -1;
}

inline int rdl_trylock_prev(volatile char *lock) {
    char orig;
    __asm__ __volatile__ (
            "movw       $0x0,       %%ax    ;"
            "movw       $0x1,       %%bx    ;"
            "lock                           ;"
            "bts        $0x1,       %1      ;"  // Set second bit
            "cmovcw     %%bx,       %%ax    ;"
            "movb       %%al,       %0      ;"
            : "=g"(orig)
            : "m"(*lock)
            : "%ax", "%bx", "memory"
            );
    return orig == 0 ? 0 : -1;
}

inline int rdl_trylock_prev_n(volatile char *lock, int tries) {
    int i = 0;
    while (i < tries) {
        if (0 == rdl_trylock_prev(lock)) {
            return 0;
        }
        i++;
    }
    return -1;
}

inline int rdl_unlock_prev(volatile char *lock) {
    char orig;
    __asm__ __volatile__ (
            "movw       $0x0,       %%ax    ;"
            "movw       $0x1,       %%bx    ;"
            "lock                           ;"
            "btr        $0x1,       %1      ;"    // Clear second bit
            "cmovcw     %%bx,       %%ax    ;"
            "movb       %%al,       %0      ;"
            : "=g"(orig)
            : "m"(*lock)
            : "%ax", "%bx", "memory"
    );
    return orig == 1 ? 0 : -1;
}
