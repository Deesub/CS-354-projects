#include <xinu.h>
#include <stdio.h>


extern uint32 prcpuused;

void iointensive(void);

void iointensive(){
	struct procent	*proc = &proctab[currpid];
	int i,j;
	for (i=0; i< 50; i++) {
  		for (j=0; j<50; j++) {
  			sleepms(1000);
  		}
  		kprintf("IO Intensive -------------PID:%d, i:%d, The process' priority:%d, slice:%d\n",currpid,i,proc->prprio,preempt);
	}
	//printf("The CPU time consumed is : %d\n",proc->prcpuused);
}
