#include <xinu.h>

extern uint32 clktimefine;

syscall registercbsig(uint16 asig, int (*func)(void),uint32 optarg){

	if(func != NULL){
		if(asig == MYSIGRECV){
			intmask mask;
			mask = disable();
			struct procent *prptr = &proctab[currpid];
			prptr->recvfunc = func;
			restore(mask);
			return OK;
		}
		if(asig == MYSIGALRM){
			intmask mask;
			mask = disable();
			struct procent *prptr = &proctab[currpid];
			prptr->alrmfunc = func;
			prptr->mysigalrmtime = optarg + clktimefine;
			restore(mask);
			return OK;
		}
		if(asig == MYSIGXCPU){
			intmask mask;
			mask = disable();
			struct procent *prptr = &proctab[currpid];
			prptr->xcpufunc = func;
			prptr->optarg = optarg;
			restore(mask);
			return OK;
		}

	}
	else{
		//restore(mask);
		return SYSERR;
	}


	
}
