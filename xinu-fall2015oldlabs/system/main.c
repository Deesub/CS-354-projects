/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INNERB 1000000
#define OUTERB 5
//#define LOOP1 5 
//#define LOOP2 100

extern syscall stacktrace(int);
extern uint32 prcpuused;

void printloop(char val);
void printnoloop(char val);
extern void myvictim(void);
extern void myattacker(void);
extern void func1(void);
extern void func2(void);
extern void myattackermalware(void);
extern void iointensive(void);
extern void cpuintensive(void);


pid32 myv = 0;
pid32 mya = 0;

int myvictimglobal = 0;
/*void printnoloop(char val){
	putc(CONSOLE,val);
	stackdepth();
	sleepms(10000);
}*/

void printloop(char val){
	int i;int j;
		for(i = 0; i < OUTERB; i++){
			putc(CONSOLE,val);
			for(j = 0; j < INNERB; j++){
				
			}
		}
}


/*void printnoloop(char val){
	kprintf("%c\n",val);
	int st = stackdepth();
	kprintf("%d\n",st);
}*/








process	main(void)
{
	/*kprintf("hhere\n");
 	myv = create((void *)myvictim, 4096, 20, "myvictim", 0 ,NULL);
	 mya = create((void *)myattacker, 4096, 20, "myattacker", 0 ,NULL);
	 kprintf("%d,%d xxx\n", myv, mya);
	resume(myv);
	resume(mya);

	sleepms(3000);
	kprintf("cccc%d\n",myvictimglobal);

	return OK; */
/*
	kprintf("-------------------Problem 3.1-------------------\n");
	long m = 1000;
	long n = host2netl(m);
	kprintf("Host byte order = 0x%08X\n, Network byte order = 0x%08X\n",m,n);
	kprintf("-------------------------------------------------\n");

	kprintf("-------------------Problem 3.2-------------------\n");
	printsegaddress();
	kprintf("-------------------------------------------------\n");

	kprintf("-------------------Problem 3.3-------------------\n");
	stackdepth();
	kprintf("-------------------------------------------------\n");*/


	/*resume (
	   create((void *)lab0app1, 65536, 20, "lab0app1", 0,
           NULL));// create is called on process lab0app1
	sleepms(200);//sleep
	resume (
	   create((void *)lab0app2, 65536, 20, "lab0app2", 0,
           NULL));//create is called on processlab0app2  
	sleepms(200);//sleep */

	kprintf("-------------------Question 4,Lab 2-------------------\n");
	kprintf("-------------------Running 4.1 and 4.3 for 4 of Lab 2-------------------\n");
	
	pid32 sa = create((void *)printloop, 1024, 20, "sendA", 1,'A');
	pid32 sb = create((void *)printloop, 1024, 20, "sendB", 1,'B');
	pid32 sc = create((void *)printloop, 1024, 20, "sendC", 1,'C');
	pid32 sd = create((void *)printloop, 1024, 20, "sendD", 1,'D');

	resume(sa);
	sleepms(1000);
	printf("P\n");
	resume(sb);
	sleepms(1000);
	printf("P\n");
	resume(sc);
	sleepms(1000);
	printf("P\n");
	resume(sd);
	sleepms(1000);

	
	printf("Time is %d\n",proctab[sa].prcpuused);
	printf("Time is %d\n",proctab[sb].prcpuused);
	printf("Time is %d\n",proctab[sc].prcpuused);
	printf("Time is %d\n",proctab[sd].prcpuused);

	pid32 sa2 = create((void *)printloop, 1024, 20, "sendA", 1,'A');
	pid32 sb2 = create((void *)printloop, 1024, 20, "sendB", 1,'B');
	pid32 sc2 = create((void *)printloop, 1024, 50, "sendC", 1,'C');
	pid32 sd2 = create((void *)printloop, 1024, 50, "sendD", 1,'D');

	resume(sa2);
	sleepms(1000);
	printf("P\n");
	resume(sb2);
	sleepms(1000);
	printf("P\n");
	resume(sc2);
	sleepms(1000);
	printf("P\n");
	resume(sd2);
	sleepms(1000);


	printf("Time is %d\n",proctab[sa2].prcpuused);
	printf("Time is %d\n",proctab[sb2].prcpuused);
	printf("Time is %d\n",proctab[sc2].prcpuused);
	printf("Time is %d\n",proctab[sd2].prcpuused);	

	kprintf("----------------------------------------------------------------\n");
	
	/*printf("P\n");
	resume(sa);
	sleepms(1000);
	printf("P\n");
	resume(sb);
	sleepms(1000);
	printf("P\n");
	resume(sc);
	sleepms(1000);
	printf("P\n");
	resume(sd);
	sleepms(1000);

	printf("\n");


	sleepms(4000);*/


	kprintf("--------------------Question 5 ( Part 1), Lab 2---------------------------\n");


	/*pid32 p1 = create((void *)cpuintensive, 1024, 1, "p1", NULL);
	pid32 p2 = create((void *)cpuintensive, 1024, 1, "p2", NULL);
	pid32 p3 = create((void *)cpuintensive, 1024, 1, "p3", NULL);
	pid32 p4 = create((void *)cpuintensive, 1024, 1, "p4", NULL);



	resume(p1);
	sleepms(1000);
	resume(p2);
	sleepms(1000);
	resume(p3);
	sleepms(1000);
	resume(p4);
	sleepms(1000);*/

	/*resume(create((void *)cpuintensive, 1024, 1, "p1", NULL));
	resume(create((void *)cpuintensive, 1024, 1, "p2", NULL));
	resume(create((void *)cpuintensive, 1024, 1, "p3", NULL));
	resume(create((void *)cpuintensive, 1024, 1, "p4", NULL));*/

	kprintf("-------------------------------------------------------------------------\n"); 

	

	kprintf("--------------------Question 5 (Part 2), Lab 2---------------------------\n");

	/*pid32 q1 = create((void *)iointensive, 1024, 1, "q1", NULL);
	pid32 q2 = create((void *)iointensive, 1024, 1, "q2", NULL);
	pid32 q3 = create((void *)iointensive, 1024, 1, "q3", NULL);
	pid32 q4 = create((void *)iointensive, 1024, 1, "q4", NULL);

	resume(q1);
	sleepms(1000);
	resume(q2);
	sleepms(1000);
	resume(q3);
	sleepms(1000);
	resume(q4);
	sleepms(1000);*/
	/*resume(create((void *)iointensive, 1024, 1, "p1", NULL));
	resume(create((void *)iointensive, 1024, 1, "p2", NULL));
	resume(create((void *)iointensive, 1024, 1, "p3", NULL));
	resume(create((void *)iointensive, 1024, 1, "p4", NULL));*/

	kprintf("-------------------------------------------------------------------------\n"); 



	kprintf("--------------------Question 5 (Part 3), Lab 2---------------------------\n");	

	/*pid32 r1 = create((void *)cpuintensive, 1024, 1, "r1", NULL);
	pid32 r2 = create((void *)cpuintensive, 1024, 1, "r2", NULL);
	pid32 r3 = create((void *)iointensive, 1024, 1, "r3", NULL);
	pid32 r4 = create((void *)iointensive, 1024, 1, "r4", NULL);

	resume(r1);
	sleepms(1000);
	resume(r2);
	sleepms(1000);
	resume(r3);
	sleepms(1000);
	resume(r4);
	sleepms(1000);*/
	 

	kprintf("-------------------------------------------------------------------------\n"); 


	/*kprintf("-------------------Problem 5-------------------\n");
	
	pid32 nsa = create((void *)printnoloop, 1024, 20, "sendA", 1,'A');
	pid32 nsb = create((void *)printnoloop, 1024, 20, "sendB", 1,'B');
	pid32 nsc = create((void *)printnoloop, 1024, 20, "sendC", 1,'C');
	pid32 nsd = create((void *)printnoloop, 1024, 20, "sendD", 1,'D');
	

	printf("P\n");
	resume(nsa);
	sleepms(1000);
	printf("P\n");
	resume(nsb);
	sleepms(1000);
	printf("P\n");
	resume(nsc);
	sleepms(1000);
	printf("P\n");
	resume(nsd);
	sleepms(1000);

	printf("\n");

	sleepms(10000);



	
	kprintf("----------main calling stackdepth, then sleeps for 10s\n");

	int s = stackdepth();
	kprintf("%d\n",s);
	sleepms(10000);

	kprintf("------------------------------------------------------\n");*/

	
	

	/* myv = create((void *)myvictim, 4096, 20, "myvictim", 0 ,NULL);
	 mya = create((void *)myattacker, 4096, 20, "myattacker", 0 ,NULL);

	resume(myv);
	resume(mya);

	sleepms(3000);*/

	

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));


	//attack();

	/* Wait for shell to exit and recreate it */

/*while(TRUE){
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}*/
	return OK;
}
