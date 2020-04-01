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
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sched.h"

/* --Scheduler random--*/
tproc * randomscheduler(tlist * procs, tlist * ready, int * delta) {
    int length = len(ready);
    int selected = rand()%length;
    tnode * p = ready->first;
    for (int i=0; i < selected; i++) {
        p = p->next;
    }
    *delta = rand()%p->proc->remaining + 1;
    return p->proc;
}
/* --Scheduler random--*/

/* --Scheduler fcfs-- */
tproc * fcfs(tlist * procs, tlist * ready, int * delta) {
    /* FIXED : first come first served */
    /* Chose the first ready process in ready tlist*/
    tnode* chosen=ready->first;
    *delta = chosen->proc->remaining; // how much time is left
    return chosen->proc;
}
/* --Scheduler fcfs-- */

/* --Scheduler rr-- */
tproc * rr(tlist * procs, tlist * ready, int * delta) {
    /* FIXED : Round-Robin */
    // Time quantum
    int quantum = 1;
    
    tnode * chosen = ready->first; /* Chose the first ready process in ready tlist*/
    tproc * chosenP = chosen->proc;   /* The chosen process*/
    
    del(ready,chosenP); /*  Remove the process chosenP from the list "ready" */
	add(procs,chosenP); /* Add the procces into the end of procs list */
    
    *delta = quantum; 

    return  chosenP;
}
/* --Scheduler rr-- */

/* --Scheduler sjf-- */
tproc * sjf(tlist * procs, tlist * ready, int * delta) {
    /* FIXED Shortest Job First */
    
    tnode * chosen = ready->first; /* Chose the first ready process in ready tlist*/
    tnode * sortestP =  ready->first; /* Process with the smallest execution time*/
    /*  Find process with minimum remaining time at every single time lap */
    while(chosen != NULL)
    {
        /* Selects for execution the waiting process with the shortest execution time */
		if((chosen->proc->remaining) < (sortestP->proc->remaining))
        {
            
		 	sortestP = chosen; /* The process that will be executed*/
        }
		chosen = chosen->next;  /* Chose the next process */
	}

    *delta = sortestP->proc->remaining;      
    
    return sortestP->proc;
}
/* --Scheduler sjf-- */

/* --Scheduler srtf-- */
tproc * srtf(tlist * procs, tlist * ready, int * delta) {
    /* FIXED Shortest remaining time first */
   
   // Time quantum
    int quantum = 1;
    
    tnode * chosen = ready->first; /* Chose the first ready process in ready tlist*/
    tnode * shortestP =  ready->first; /* Process with the smallest execution time*/
    /*  Find process with minimum remaining time at every single time lap */
    while(chosen != NULL)
    {
        /* Selects for execution the waiting process with the smallest execution time */
		if((chosen->proc->remaining) < (shortestP->proc->remaining))
        {
		 	shortestP = chosen;
        }
		chosen = chosen->next;
	}

    *delta = quantum;      
    
    return shortestP->proc;
}
/* --Scheduler srtf-- */


/* --Scheduler edf-- */
tproc * edf(tlist * procs, tlist * ready, int * delta) {
    /* FIXED Earliest deadline first scheduling */

    
    int quantum = 1;
    
    tnode * chosen = ready->first; /* Chose the first ready process in ready tlist*/
    tproc * earliestP =  chosen->proc; /* Process with the earliest deadline */
    /*  Find process with the earliest deadline */
    while(chosen != NULL)
    {
        
        /* Selects for execution the process with the earliest deadline */
		if( ((chosen->proc->period + chosen->proc->activation)) < ((earliestP->period + earliestP->activation)))
        {
		 	earliestP = chosen->proc;
        }

		chosen = chosen->next;
	}

    *delta = quantum;      
    
    return earliestP;
}
/* --Scheduler edf-- */



/* List of ready procs */
tlist ready;

/* List of other procs */
tlist procs;

/* The selected scheduler */
tscheduler scheduler;

/* The scheduling statistics */
tstats stats = {0} ;

/* display usage message */
void usage() {
    fail("usage: sched [fcfs, rr, sjf, srtf, edf, rm]\n");
}

/* simulate a single core scheduler, from time 0 to `max_time` */
void simulate(int max_time) {
    int time=0;
    while(time < max_time) {
        /* Activate process */
        for (tnode * p = procs.first; p != NULL;) {
            tproc * proc = p->proc; 
            p = p->next;

            /* Move every process which should be activated,
             * from the procs list to the ready list */
            if (proc->activation <= time) {
                del(&procs, proc);
                add(&ready, proc);
            }
        }

        /* If any process is ready, then we can schedule! */
        if (ready.first != NULL) {

            int delta = 0;
            /* Call the scheduler */
            tproc * proc = scheduler(&procs, &ready, &delta);

            /* Ensure the scheduler has advanced at least one unit of time */
            assert(delta > 0);

            /* Output task execution */
            //printf("\\TaskExecution{%d}{%d}{%d}\n", proc->pid, time, time+delta);
            
            if (time+delta < proc->activation + proc-> period)
            	printf("\\TaskExecution{%d}{%d}{%d}\n", proc->pid, time, time+delta);
			else
				printf("\\TaskExecution[color=red]{%d}{%d}{%d}\n", proc->pid, time, time+delta);


               /* Calculate response time */
            if (proc->remaining == proc->length) 
            {
                /* The moment when we activate the thread - the moment when the thread start the execution*/
				stats.response += time - proc->activation;
			}

            /* Advance time by delta */
            time += delta;

            /* Remove delta from chosen process */
            proc->remaining-=delta;
 
          

            /* If the process remaining time is less zero or less, 
             * delete it */ 
            if (proc->remaining <= 0) {
                del(&ready, proc);
                del(&procs, proc);
                /* Completion time = the time when process arrives to the ends of execution - the activation time */
                stats.completion += time - proc->activation;
                /* Waiting time = the time when process arrives to the ends of execution - the length of the process 
                - the activation time */
                stats.waiting += time - proc->length - proc->activation;

                /* Manage the periodique process */
                if(proc->period)  // If the period of the process different to 0
                {
                	proc->activation += proc->period; //We update his activation time
					proc->remaining = proc->length;  // And his remaining time
					add(&procs,proc);             // And we add this process to the tasks list
                }
            }
        } 
        /* If no process is ready, just advance the simulation timer */
        else {
            time += 1;
        }

    }
}

int main(int argc, char * argv[]) {

    /* Parse arguments */
    if (argc != 2) usage(); 

    /* Seed random number generator */
    srand(time(NULL) ^ getpid());

    char * method = argv[1]; 

    /* The sched argument should be one of fcfs, rr, sjf, srtf, edf, rm */
    if (strcmp(method, "fcfs") == 0) {
        scheduler = fcfs;
    } 
    else if (strcmp(method, "rr") == 0) {
        scheduler = rr;
    }
    else if (strcmp(method, "sjf") == 0) {
        scheduler = sjf;
    }
    else if (strcmp(method, "srtf") == 0) {
        scheduler = srtf;
    } 
    else if (strcmp(method, "edf") == 0) {
        scheduler = edf;
    } 
   
    else {
        usage();
    }

    /* Compile the task descriptions */
    #include "task_description.h"

    /* Add all tasks to the procs queue */
    for(int i = 0; i < sizeof(tasks)/sizeof(tproc); i ++) {
        add(&procs, &(tasks[i]));
    }
    
    /* Output RTGrid header */
    printf("\\begin{RTGrid}[width=0.8\\textwidth]{%d}{%d}\n", len(&procs), max_time);

   
    /* Output task arrivals for all tasks */ 
    
    for (tnode *p = procs.first; p != NULL; p = p->next) {
        int n = p->proc->activation;
        while (p->proc->period && n < max_time)
        {
            printf("\\TaskArrival{%d}{%d}\n", p->proc->pid, n); 
            n += p->proc->period;
        }
    }

    /* Start scheduling simulation */
    simulate(max_time);

    /* Close RTGrid environment */
    printf("\\end{RTGrid}\\newline\\newline\n");

    /* Print statistics */
    printf("Total completion time = %d\\newline\n", stats.completion);
    printf("Total waiting time = %d\\newline\n", stats.waiting);
    printf("Total response time = %d\\newline\n", stats.response);

    /* Empty the lists if needed */
    del_all(&ready);
    del_all(&procs);

    return 0;
}
