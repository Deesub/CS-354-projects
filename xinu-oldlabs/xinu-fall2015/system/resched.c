/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;
extern uint32 clktimefine;
extern uint32 prcpuused;
extern void memalloc(void);

uint32 store = 0;


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

	
	//printf("PID is : %d and state is %d\n",currpid,ptold->prstate);

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prcpuused < firstkey(readylist)) {
			ptold->prcpuused += (clktimefine) - store;
			store = clktimefine;	
			return;
		}

		/* Old process will no longer remain current */
		
		ptold->prcpuused += (clktimefine) - store;
		store = clktimefine;
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/

	ptold->prcpuused += (clktimefine) - store;
	store = clktimefine;
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/*if(ptold->fpointer != NULL){ // if the pointer isn't null
		if(ptold->prhasmsg == TRUE){ // check to see if there is a message 
			ptold->fpointer(); // call the function pointing to the callback function
		}
	}*/

	if(ptold->recvfunc!=NULL) {
		if(ptold->prhasmsg == TRUE){
    		ptold->recvfunc();
		}
 	}
	else if(ptold->alrmfunc != NULL && ptold->mysigalrmtime >= 0) {
		if(clktimefine >= ptold->mysigalrmtime){
			ptold->alrmfunc();
			ptold->alrmfunc = NULL;
		}
	}
	else if(ptold->xcpufunc != NULL){
		if(ptold->prcpuused >= ptold->optarg){
			ptold->xcpufunc();
			ptold->xcpufunc = NULL;
		}
	}


	/* Old process returns here when resumed */

	return;
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
