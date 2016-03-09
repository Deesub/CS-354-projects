#include <xinu.h>
#include <stdio.h>


	unsigned long *ebp;
	unsigned long *esp;


int stackdepth(){
	int count = 0;
	

	

	struct procent	*proc = &proctab[currpid];

	asm("movl %esp,esp");
	asm("movl %ebp,ebp");
	unsigned long *sp = esp;
	unsigned long *fp = ebp;

	

	while (sp < (unsigned long *)proc->prstkbase) {
		
		++count;
		kprintf("Count is %d\n",count);

		kprintf("SP is(%08X)\n", sp);
		
		kprintf("FP is (%08X)\n", fp);
		int len = (uint32)fp - (uint32)sp;

		kprintf("The difference is %d\n",len);

		for (; sp < fp; sp++);
		fp = (unsigned long *) *sp++;
		sp++;
	}	
		kprintf("final base pointer(%08X)\n", proc->prstkbase);
		kprintf("\n");
		//kprintf("Count is a mothafucka %d\n",count);
		return count;
}