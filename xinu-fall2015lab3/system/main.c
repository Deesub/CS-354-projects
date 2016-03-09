/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INNERB 1000000
#define OUTERB 5

extern void iointensive(void);
extern void cpuintensive(void);

process	main(void)
{

	printf(" ----------------Lab 3, part 3.3 and 4-------------\n");

	pid32 p1 = create((void *)cpuintensive, 1024, 1, "p1", NULL);
	pid32 p2 = create((void *)cpuintensive, 1024, 1, "p2", NULL);
	pid32 p3 = create((void *)iointensive, 1024, 1, "p3", NULL);
	pid32 p4 = create((void *)iointensive, 1024, 1, "p4", NULL);



	resume(p1);
	resume(p2);
	resume(p3);
	resume(p4);
	 


	/*
	pid32 q1 = create((void *)iointensive, 1024, 1, "q1", NULL);
	pid32 q2 = create((void *)iointensive, 1024, 1, "q2", NULL);
	pid32 q3 = create((void *)iointensive, 1024, 1, "q3", NULL);
	pid32 q4 = create((void *)iointensive, 1024, 1, "q4", NULL);

	resume(q1);
	resume(q2);
	resume(q3);
	resume(q4);
	*/
	
	/*
	pid32 r1 = create((void *)cpuintensive, 1024, 1, "r1", NULL);
	pid32 r2 = create((void *)cpuintensive, 1024, 1, "r2", NULL);
	pid32 r3 = create((void *)iointensive, 1024, 1, "r3", NULL);
	pid32 r4 = create((void *)iointensive, 1024, 1, "r4", NULL);

	resume(r1);
	resume(r2);
	resume(r3);
	resume(r4);
	
	 

	sleepms(3000);
	*/
	
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	/*while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}*/
	return OK;
}
