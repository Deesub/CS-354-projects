#include <xinu.h>

extern uint32 clktimefine;

syscall registercbsig(uint16 asig, int (*func)(void),uint32 optarg){

	if(func != NULL){
		if(asig == MYSIGRECV){ //checks to see if the signal is MYSIGRECV
			intmask mask; //define mask
			mask = disable();// disable interrupts
			struct procent *prptr = &proctab[currpid]; //pointer to the table entry for a new process
			prptr->recvfunc = func; //sets pointer to the callback func
			restore(mask);//restore the mask
			return OK;
		}
		if(asig == MYSIGALRM){	//checks to see if the signal is MYSIGALRM
			intmask mask;	//define mask
			mask = disable();	// disable interrupts
			struct procent *prptr = &proctab[currpid]; //pointer to the table entry for a new process
			prptr->alrmfunc = func;	//sets pointer to the callback func
			prptr->mysigalrmtime = optarg + clktimefine;	//saves the sum of the initial clktime and the optarg for the particular process
			restore(mask);	//restore the mask
			return OK;
		}
		if(asig == MYSIGXCPU){	//checks to see if the signal is MYSIGXPU
			intmask mask;	//define mask
			mask = disable();	//disable interrupts
			struct procent *prptr = &proctab[currpid];	//pointer to the table entry for a new process
			prptr->xcpufunc = func; //sets pointer to callback function
			prptr->optarg = optarg; // saves the optional argument for the particular process
			restore(mask);	//restore the mask
			return OK;
		}

	}
	else{
		//restore(mask);
		return SYSERR;
	}


	
}
