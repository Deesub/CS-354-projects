#include <xinu.h>
#include <stdio.h>

/* I have made changes here and got the process ID of the lab0app2 process */

process lab0app2(void){
	int c = 3782 * 751;
	int pid = getpid();// gets the process ID
	kprintf("\nThe result is %d\n",c);
	kprintf("\nThe Process ID is %d\n",pid);
	return OK;	
}
