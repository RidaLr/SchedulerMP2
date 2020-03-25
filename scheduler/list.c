/* Copyright (c) 2013 Pablo Oliveira <pablo.oliveira@prism.uvsq.fr>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.  All rights reserved.
 */

#include <stdlib.h> 
#include <stdio.h>
#include "sched.h"

/* Abort the program with an error message */
void fail(char * message) {
    fprintf(stderr, "%s", message);
    abort();
}

/* Add element `p` to list `l` */
void add(tlist * l, tproc * p) {
    tnode * new = calloc(1, sizeof(tnode));
    new->proc = p;
    
    if (l->first == NULL) {
        l->first = new;
        l->last  = new; 
    } else {
        l->last->next = new;
        l->last = new;
    }
}

/* Remove element `p` from list `l`,
 * if `p` is missing from `l`, this function does nothing.
 * WARNING: del frees the tnode when removing `p`,
 * this must be taken into account if used while iterating 
 * a list. */
void del(tlist * l, tproc * p) {
    /* l is empty do nothing */
    if (l->first == NULL) {
        return;
    }

    /* special case when p is the head of the list */
    if (l->first->proc == p) {
        tnode * succesor = l->first->next;
        free(l->first);
        l->first = succesor;
        return;
    } 

    tnode * cur = l->first;
    while (1) {
        /* p was not found, return */
        if (cur->next == NULL) {
            return;
        } 

        /* p is found, remove it, and reconnect the list */
        if (cur->next->proc == p) {
            /* special case when p is the tail of the list */
            if (cur->next == l->last) {
                l->last = cur;
            }
            tnode * succesor = cur->next->next;
            free(cur->next);
            cur->next = succesor;
            return;
        }
        cur = cur->next;
    }
}

/* Remove all the elements from list `l` */
void del_all(tlist * l) {
    while(l->first != NULL) {
        del(l, l->first->proc);
    }
}

/* Returns the length of list `l` */
int len(tlist *l) {
    int count = 0;
    for (tnode * p = l->first; p != NULL; p = p->next)
        count++;
    return count;
}

/* Prints the list to stdout. Used to debug. */
void dump(tlist *l) {
    tnode * cur = l->first;
    printf("[ ");
    while(cur != NULL) {
        if (cur->proc == NULL) {
            fail("dump: Invalid process found");
        }
        printf("%d ", cur->proc->pid);
        cur = cur->next;
    }
    printf("]\n");
}
