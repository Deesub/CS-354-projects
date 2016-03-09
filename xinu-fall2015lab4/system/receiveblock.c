/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32 receiveblock(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	struct procent *send;
	umsg32	msg;			/* Message to return		*/
	int i = 0;

	mask = disable(); // disable interrupts
	pid32 sendpid = 0;
	prptr = &proctab[currpid];

	if (prptr->prhasmsg == FALSE) {  // if there is no message
		if(!isempty(prptr->qofsenders)){ // if the queue of senders isn't empty
			sendpid = dequeue(prptr->qofsenders); // get the pid of the item on the queue by removing it
			send = &proctab[sendpid]; // declare the send pointer to the proc table entry
			send->sndflag = FALSE; // set the flag to false since there is no valid message yet
			prptr->prmsg = send->sndmsg; 
			prptr->prhasmsg = TRUE; //  set the prhasmsg flag to true
			//ready(sendpid);
			prptr->prstate = PR_READY; // set the state to ready
			insert(sendpid, readylist, prptr->prprio); // insert to readylist
			
		}
		else{
			prptr->prstate = PR_RECV;
			resched();		/* Block until message arrives	*/
		}
	}

		msg = prptr->prmsg;		/* Retrieve message		*/
		prptr->prhasmsg = FALSE;	/* Reset message flag		*/

	
	restore(mask);
	return msg;
}