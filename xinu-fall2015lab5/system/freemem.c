/* freemem.c - freemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall	freemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;
	int found = 0;
	struct procent *prptr = &proctab[currpid]; //pointer to the table entry for a new process
	struct memblkentry *prevmemblkptr = prptr->prmemblklist; // stores the pointeer to the previous node
	struct memblkentry *memblkptr = prevmemblkptr->next;// sets up the next node to keep track of memory allocated to the process

	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			  || ((uint32) blkaddr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes); /* Use memblk multiples	*/
	block = (struct memblk *)blkaddr;

	prev = &memlist;			/* Walk along free list	*/
	next = memlist.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlist) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlist) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/*freemem basically does the removing from the linked list the amount of the block when found*/

	if(memblkptr != NULL && memblkptr == (struct  memblkentry*)block) //if the list isn't empty and we found the block we want to free
	{	
		prevmemblkptr->next = memblkptr->next; //overwrite the next node over current
		prevmemblkptr->size -= nbytes; //decrement size
	}

	while((memblkptr != NULL)&&(memblkptr != (struct memblkentry*)block)) //while the linked list isn't empty and the block size isn't correct
	{
		prevmemblkptr = memblkptr; // store the next element in the currents position
		memblkptr = memblkptr->next; // updates the value of linked list
	}

	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}
	restore(mask);
	return OK;
}
