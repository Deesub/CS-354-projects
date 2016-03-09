#include <xinu.h>

syscall registercb(int (* func) (void)){
	intmask	mask;	
	mask = disable(); // disable interrupts
	struct	procent *prptr = &proctab[currpid];	
	prptr->fpointer = func; // sets pointer to the callback function
	restore(mask); // restore interrupts
	return OK;
}