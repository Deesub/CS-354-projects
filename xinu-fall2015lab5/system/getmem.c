/* getmem.c - getmem */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */

//void updateList(char * pointerToMem, uint32 size);

char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;

	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

			updateList((char *) curr, nbytes); //calls updatelist since there is enough space 
			restore(mask);
			return (char *)(curr);
		} 
		else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			updateList((char *) curr, nbytes); //calls updatelist since there is enough space 
			restore(mask);
			return (char *)(curr);
		} 
		else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	
	restore(mask);
	return (char *)SYSERR;
}


void updateList(char * ptr, uint32 size) // function to set up and populate the linked list with the amount of memory required
{
	struct procent *prptr = &proctab[currpid];//pointer to the table entry for a new process
	struct memblkentry *memblkptr;// new linked list of type struct memblkentry
	memblkptr = (struct memblkentry*)ptr; //the linked list struct used to store the block allocated from the heap
	memblkptr->next = prptr->prmemblklist->next; //populating the linked lists next node
	memblkptr->size = size; //saves the size
	prptr->prmemblklist->next = memblkptr; // stores the next node to keep track of allocated memory for the process
	prptr->prmemblklist->size += size; // updating the size based on the number of bytes requested
}