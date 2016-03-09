#include <xinu.h>

char *getmemgc(uint32 nbytes)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	//uint32 nbytes;
	char *savedcurrptr;
	
	struct procent *prptr = &proctab[currpid];
	

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	//prptr->memlength = nbytes;
	//printf("NBYTES is : %d\n",nbytes);

	prev = &memlist;
	curr = memlist.mnext;
	
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			break;
		} 
		else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			break;
		} 
		else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	//printf("Curr is 0x%08X\n",curr);
	savedcurrptr = curr;

		/* Use memblk multiples	*/

	//prev = &memlist;
	//curr = memlist.mnext;

	if( curr->mlength == nbytes || curr->mlength > nbytes) // if there is block space in the free list
	{
		//prptr->prhasmemblk = TRUE; //  there is enough space, ie the block size is sufficient or greater than required
		
		struct memblkentry *memblkptr = (struct memblkentry*)curr;
		struct memblkentry *memblktmp ;
		struct memblkentry *memblknext;

		if(prptr->prmemblklist == NULL){
			memblktmp = memblkptr;
			memblktmp->next = prptr->prmemblklist;
			memblktmp->size = nbytes;
			prptr->prmemblklist = memblktmp;

		}
		else{
			memblkptr = prptr->prmemblklist;
			memblknext = memblkptr->next;
			while(memblknext != NULL){
				memblkptr = memblknext;
				memblknext = memblknext->next;
			}
			memblktmp = memblkptr;
			memblktmp->next = memblknext;
			memblktmp->size = nbytes;
			memblkptr->next = memblktmp;

		}
		restore(mask);
		return (char *)(curr);	
	}

	else{
		restore(mask);
		return (char *)SYSERR;
	}
}



		

		//kprintf("Currfuck is %s\n",savedcurrptr);
		//kprintf("The size is %d\n",memblkptr->size);
		//kprintf("Memblk pointer is 0x%08X\n",memblkptr);
		//kprintf("Memblk->ptr pointer is 0x%08X\n",memblkptr->ptr);
		//kprintf("The memblk->next is 0x%08X\n",memblkptr->next);
		//kprintf("proc table entry for memblklist is 0x%08X\n",prptr->prmemblklist);
		//kprintf("proc table entry for memblklist->next is 0x%08X\n",prptr->prmemblklist->next);

		
		//kprintf("proc table entry for memblklist is 0x%08X\n",prptr->prmemblklist);
		//kprintf("proc table entry for memblklist->next is 0x%08X\n",prptr->prmemblklist->next);*/
		

	
		
	
	