/* initialize.c - nulluser, sysinit, sizmem */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
extern	void xdone(void);	/* System "shutdown" procedure		*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/
struct  ts_disptb tsdtab[60];
qid16 qarray[60];



/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/


/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/
	

	
	/* Initialize the system */
		
	sysinit();

	kprintf("\n\r%s\n\n\r", VERSION);
	
	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\r\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)&text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&text, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&ebss - (uint32)&data);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&data, (uint32)&ebss - 1);

	/* Enable interrupts */

	enable();

	kprintf("\nHello World!\n");
	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
	kprintf("\n...creating a shell\n");
	kprintf("My name's Dhruv Subramanian\n");
	kprintf("Purdue Id : 0026458203\n");


	int i = 0;
	int c = 0;
	for(i = 0;i < 60;i++){
		if(i >= 0 && i < 10 ){
			tsdtab[i].ts_tqexp = 0;
			tsdtab[i].ts_slpret = 50;
			tsdtab[i].ts_quantum = 200;

		}
		else if(i >= 10 && i < 20){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 51;
			tsdtab[i].ts_quantum = 160;
		}
		else if(i >= 20 && i < 30){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 52;
			tsdtab[i].ts_quantum = 120;
		}
		else if(i >= 30 && i < 35){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 53;
			tsdtab[i].ts_quantum = 80;
		}
		else if(i >= 35 && i < 40){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 54;
			tsdtab[i].ts_quantum = 80;
		}
		else if(i >= 40 && i < 45){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 55;
			tsdtab[i].ts_quantum = 40;	
		}
		else if(i == 45){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 56;
			tsdtab[i].ts_quantum = 40;
		}
		else if(i == 46){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 57;
			tsdtab[i].ts_quantum = 40;
		}
		else if(i >= 47 && i < 59){
			tsdtab[i].ts_tqexp = c;
			c++;
			tsdtab[i].ts_slpret = 58;
			tsdtab[i].ts_quantum = 40;
		}
		else if(i == 59){
			tsdtab[i].ts_tqexp = c;
			tsdtab[i].ts_slpret = 59;
			tsdtab[i].ts_quantum = 20;	
		}

		kprintf("--------------------------The dispatch table is------------------------------\n");
		kprintf("TABLE is %d   %d   %d \n", tsdtab[i].ts_tqexp,tsdtab[i].ts_slpret,tsdtab[i].ts_quantum);//printed out table

	}

	int j = 0;

	for(j = 0; j < 60;j++){
		qarray[j] = newqueue();
	}

	

	/* Create a process to execute function main() */

	resume (
	   create((void *)main, INITSTK, INITPRIO, "Main process", 0,
           NULL));

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/

	while (TRUE) {
		;		/* Do nothing */
	}

	



	

}

/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/

	/* Platform Specific Initialization */

	platinit();

	/* Initialize the interrupt vectors */

	initevec();
	
	/* Initialize free memory list */
	
	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}

	/* Initialize the Null process entry */	

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	currpid = NULLPROC;
	
	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	/* Initialize buffer pools */

	bufinit();

	/* Create a ready list for processes */

	//readylist = newqueue();

	/* Initialize the real time clock */

	clkinit();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}

int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}
