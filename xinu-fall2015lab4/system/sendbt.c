#include <xinu.h>

syscall	sendbt(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg,		/* Contents of message		*/
	  int32     maxwait 	/* maxwait parameter*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	struct  procent *prsndmsg;	/*sending message ptr to process table entry*/

	mask = disable();

	prptr = &proctab[pid];
	prsndmsg = &proctab[currpid];

	if(maxwait < 0){				//error checking to see if maxwait is less than zero
		restore(mask);
		return SYSERR;
	}
	if(maxwait == 0){					// if the maxwait is zero, set the state to SENDING state.
		prsndmsg->prstate = PR_SEND;
		resched();
	}
	
	if (isbadpid(pid)) {			//badpid error checking
		restore(mask);
		return SYSERR;
	}
	if ((prptr->prstate == PR_FREE)) { // if the state of the process is free, restore mask and return syserr
		restore(mask);
		return SYSERR;
	}

	if(prptr->prhasmsg){  //if there is a message, ie prhasmsg parameter of proc table is TRUE
		
		prsndmsg->sndmsg = msg; // store its contents in the 1 word message parameter sndmsg
		prsndmsg->sndflag = TRUE; // set the flag to true since sndmsg is valid
		if (insertd(currpid,sleepq,maxwait) == SYSERR) // error checking, 
		{
			restore(mask);
			return SYSERR;
		}

		prsndmsg->prstate = PR_SEND; // sets the state to SENDING state 
		enqueue(currpid,prptr->qofsenders); // add the message item to the queue of senders
		resched(); //  calls reschedule
		

	}

	else{    // otherwise, if the prhasmsg flag is FALSE
		prptr->prmsg = msg; // store the contents of the message in prmsg parameter 
		prptr->prhasmsg = TRUE; // set the flag of prhasmsg to TRUE, now that we have a message
	}
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

