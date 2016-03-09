#include <xinu.h>
#include <stdio.h>

/* I have made changes here and got the process ID of the lab0app1 process */

process lab0app1(void)
{
	kprintf("\n Philipp Lahm is a great Captain!\n");
	int pid = getpid();
	kprintf("\n Process ID is %d\n",pid);
	return OK;
	
}
