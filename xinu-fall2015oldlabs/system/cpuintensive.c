#include <xinu.h>
#include <stdio.h>


#define LOOP1 5
#define LOOP2 1000000
extern uint32 prcpuused;



void cpuintensive(){
	struct procent	*proc = &proctab[currpid];
	int i,j;
	char dest[5];
	const char src[5] = "Hi";
	for (i=0; i<LOOP1; i++) {
  		for (j=0; j<LOOP2; j++) {
  			memcpy(dest,src,3);
  		}
  		kprintf("PID:%d ,i:%d ,priority is:%d ,time slice:%d\n",currpid,i,1,QUANTUM);
	}
	printf("CPU time:%d\n",proc->prcpuused);
}
