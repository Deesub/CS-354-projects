/* kill.c - kill */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
  */
 //extern void helper(void);

syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	int32	i;			/* Index into descriptors	*/
	struct	procent *prptr = &proctab[currpid];		/* Ptr to process' table entry	*/\
	struct memblkentry *prevmemblkptr = prptr->prmemblklist;
	struct memblkentry *memblkptr =  prevmemblkptr->next;

	//kprintf("in here homie bad zone\n");

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}
	
	while(memblkptr != NULL){
		prevmemblkptr = memblkptr;
		memblkptr = memblkptr->next;
	}

	//kprintf("prptr->prmemblklist->size before :%d\n",prptr->prmemblklist->size);
	prevmemblkptr->size = 0;
	prptr->prmemblklist->size = prevmemblkptr->size;
	prevmemblkptr->next = NULL;
	//kprintf("prptr->prmemblklist->size after :%d\n",prptr->prmemblklist->size);
	

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}
	freestk(prptr->prstkbase, prptr->prstklen);

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	kprintf("prptr->prmemblklist->size %d\n",prevmemblkptr->size);
	restore(mask);
	return OK;
}
