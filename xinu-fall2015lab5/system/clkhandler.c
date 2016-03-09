/* clkhandler.c - clkhandler */

#include <xinu.h>

extern uint32 clktimefine;
extern uint32 mysigalrmtime;
extern uint32 prcpuused;
extern uint32 optarg;
/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/

	/* Decrement the ms counter, and see if a second has passed */

	struct procent *prptr = &proctab[currpid]; //pointer to the table entry for current process
	clktimefine++; //incrementing the clktimefine
	
	if(prptr->alrmfunc != NULL && prptr->mysigalrmtime >= 0) {	// check if the function isn't null and the saved sigalrmtime in the process table isn't negative
		if(clktimefine >= prptr->mysigalrmtime){	//if the current clktimefine is greater than the saved sigalrmtime 
			prptr->alrmfunc();	//call the function pointing to callback function
			prptr->alrmfunc = NULL;	//set the ptr to function as NULL, so that the process can't activate another callback function
		}
	}

	else if(prptr->xcpufunc != NULL){	// check if the function isn't null 
		if(prptr->prcpuused >= prptr->optarg){	//check if the cpuused by the process exceeds the optional argument 
			prptr->xcpufunc();	//call the function pointing to callback function
			prptr->xcpufunc = NULL;	//reset the the function ptr to NULL so that the process can't activate another callback function
		}
	}

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		preempt = QUANTUM;
		resched();
	}
}
