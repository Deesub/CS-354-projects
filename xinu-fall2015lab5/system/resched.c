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
			ptold->prcpuused += (clktimefine) - store; //updatings the cpuused
			store = clktimefine;	//updating the store variable
			return;
		}

		/* Old process will no longer remain current */
		
		ptold->prcpuused += (clktimefine) - store;	//updating the cpuused
		store = clktimefine;	//updating the global variable store 
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/

	ptold->prcpuused += (clktimefine) - store; //updating the cpuused
	store = clktimefine;	//updating the global variable store 
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/*if(ptold->fpointer != NULL){ // if the pointer isn't null
		if(ptold->prhasmsg == TRUE){ // check to see if there is a message 
			ptold->fpointer(); // call the function pointing to the callback function
		}
	}*/

	if(ptold->recvfunc!=NULL) {		// if the pointer isn't null
		if(ptold->prhasmsg == TRUE){	// check to see if there is a message 
    		ptold->recvfunc();	// call the ptr to callback function
		}
 	}
	else if(ptold->alrmfunc != NULL && ptold->mysigalrmtime >= 0) { // check if the function isn't null and the saved sigalrmtime in the process table isn't negative
		if(clktimefine >= ptold->mysigalrmtime){		//if the current clktimefine is greater than the saved sigalrmtime 
			ptold->alrmfunc();	//	call the function pointing to callback function
			ptold->alrmfunc = NULL; //set the ptr to function as NULL, so that the process can't activate another callback function
		}
	}
	else if(ptold->xcpufunc != NULL){	// check if the function isn't null 
		if(ptold->prcpuused >= ptold->optarg){	//check if the cpuused by the process exceeds the optional argument 
			ptold->xcpufunc();	//call the function pointing to callback function
			ptold->xcpufunc = NULL; //reset the the function ptr to NULL so that the process can't activate another callback function
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
