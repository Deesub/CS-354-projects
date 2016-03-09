#include <xinu.h>

pid32 multileveldq(qid16 q[]){
	int i = 59;
	for(i; i >= 0 ;i--)
	{
		if(!isempty(q[i])) //  checks to see if empty
		{
			pid32 dq = dequeue(q[i]);// removes and returns the ith element of the mlfq
			return dq;// returns it.
		}
	}
	return -1;
}