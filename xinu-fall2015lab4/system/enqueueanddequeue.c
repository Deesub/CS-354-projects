/*
#include <xinu.h>

umsg32 msgenqueue(pid32 pid,umsg32 msg,int32 maxwait){

	intmask	mask;			
	struct	procent *prptr;	

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	if(maxwait < 0){
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	prptr->msgqueue[prptr->numberofmessage] = msg;

	prptr->numberofmessage++; 
	return msg;
}

umsg32 msgdequeue(pid32 pid,int32 maxwait){

	intmask	mask;			
	struct	procent *prptr;		

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	if(maxwait < 0){
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	umsg32 msg = prptr->msgqueue[0];

	int i = 0;
	for(i;i < prptr->numberofmessage;i++){
		prptr->msgqueue[i] = prptr->msgqueue[i+1];
	}
	prptr->numberofmessage--;

	return msg;
}*/