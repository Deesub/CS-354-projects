/* resched.c - resched, resched_cntl */

#include <xinu.h>

extern qid16 qarray[];
extern struct ts_disptb tsdtab[];


struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

		if(ptold->prstate == PR_CURR) {  /* cpu intensive */

			pri16 old = ptold->prprio; // saves the old process priority
			pri16 newprio = tsdtab[old].ts_tqexp;//updates the new priority based on the old one from the dispatch table

			ptold->prprio = newprio;// updates the old priority

			int i = 59;
			for(i; i >= 0;i--) // iterates through the number of levels
			{
				if(!isempty(qarray[i])) // makes sure it isnt empty at the particular level
				{
					if(ptold->prprio > i) // checks to see if the old process priority is greater than the priority at level i
					{
						return;
					}
					else{
						break;
					}
				}
			}
						

		/* Old process will no longer remain current */

			ptold->prstate = PR_READY; // saves the state to ready

			enqueue(currpid,qarray[ptold->prprio]); // calls enqueue to add 
		}

		else if (ptold->prstate == PR_SLEEP) // io intensive
		{ 

			pri16 old = ptold->prprio; // saves the old process priority
			pri16 newprio = tsdtab[old].ts_slpret;//updates the new priority based on the old one from the dispatch table
			ptold->prprio = newprio;//updates the old priority

			
 		}
 			currpid = multileveldq(qarray);// calls dequeue multilevelfeedbackqueue

			ptnew = &proctab[currpid];
			ptnew->prstate = PR_CURR;//sets the state to current

			preempt = tsdtab[ptnew->prprio].ts_quantum; // updates preempt
			ctxsw(&ptold->prstkptr, &ptnew->prstkptr);// context switches
			return;
		
	

	/* Force context switch to highest priority ready process */


	
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
