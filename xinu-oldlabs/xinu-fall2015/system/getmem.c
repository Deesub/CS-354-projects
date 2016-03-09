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

	nbytes = (uint32) roundmb(nbytes); //+ sizeof(memblkentry);	/* Use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;

	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			updateList((char *) curr, nbytes);
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
			updateList((char *) curr, nbytes);
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


void updateList(char * ptr, uint32 size){
	struct procent *prptr = &proctab[currpid];
	struct memblkentry *memblkptr;
	memblkptr = (struct memblkentry*)ptr;
	memblkptr->next = prptr->prmemblklist->next;
	memblkptr->size = size;
	prptr->prhasmemblk = TRUE;
	//kprintf("memblkptr->size is %d\n",memblkptr->size);
	//kprintf("memblkptr is %08X\n",&memblkptr);
	//kprintf("memblkptr->next is %08X\n",&memblkptr->next);
	prptr->prmemblklist->next = memblkptr;
	//kprintf("prptr->prmemblklist->next is %08X\n",prptr->prmemblklist->next);
	//kprintf("prptr->prmemblklist->size before is %d\n",prptr->prmemblklist->size);
	prptr->prmemblklist->size += size;
	//kprintf("prptr->prmemblklist->size after is %d\n",prptr->prmemblklist->size);
}