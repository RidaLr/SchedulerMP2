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

/* process */
typedef struct {
    int pid;
    int activation;
    int length;
    int remaining;
} tproc;

/* node in a list of process */
typedef struct tnode {
    tproc * proc;
    struct tnode * next;
} tnode;

/* list of process */
typedef struct {
    tnode * first;
    tnode * last;
} tlist;

/* structure aggregating scheduler's statistics */
typedef struct {
    int completion;
    int waiting;
    int response;
} tstats;

/* Function pointer for scheduler methods */
typedef tproc* (*tscheduler)(tlist*, tlist*, int*); 

/*****************/
/* tlist methods */
/*****************/

/* Abort the program with an error message */
void fail(char * message);

/* Add element `p` to list `l` */
void add(tlist * l, tproc * p);

/* Remove element `p` from list `l`,
 * if `p` is missing from `l`, this function does nothing.
 * WARNING: del frees the tnode when removing `p`,
 * this must be taken into account if used while iterating 
 * a list. */
void del(tlist * l, tproc * p);

/* Remove all the elements from list `l` */
void del_all(tlist * l);

/* Returns the length of list `l` */
int len(tlist *l);

/* Prints the list to stdout. Used to debug. */
void dump(tlist * l);


