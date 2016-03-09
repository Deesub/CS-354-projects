/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

extern uint32 clktimefine;
extern uint32 prcpuused;
extern uint32 optarg;
uint32 initial;
uint32 final;
int a = 0;
int b = 0;


int myrecvhandler(void);
void asyncrec(void);
void asyncsend(pid32,umsg32);
void asmtest1(void);
void asmtest2(void);
void asmtest3(void);
int myalrmhandler1(void);
void alrmtest1(void);
void alrmtest2(void);
int myalrmhandler2(void);
void printloop(char);
void sigxcputest1(void);
int  sigxcpuhandler1(void);
void helper(void);

pid32 pidchecker;
umsg32 msgglob = ' ';
pid32 xcpu = 0;

void cpuintensive(){
	struct procent	*proc = &proctab[currpid];
	int i,j;
	char dest[5];
	const char src[5] = "Hi";
	for (i=0; i<5; i++) {
  		for (j=0; j<100000; j++) {
  			memcpy(dest,src,3);
  		}
  		//kprintf("PID:%d ,i:%d ,priority is:%d ,time slice:%d\n",currpid,i,1,QUANTUM);
	}
	printf("CPU time:%d\n",proc->prcpuused);
}


int myrecvhandler() // callback func for asm tests
{
   	  msgglob = receive();
   	  printf("Time after receive is : %d && the PID is : %d \n",clktimefine,currpid);
   	  kprintf("Message received from sender: \"%c\".\n\r",msgglob);
      return(OK);
}

void asyncrec() //  function for asmtests
{
	if (registercbsig(MYSIGRECV,&myrecvhandler,50) != OK) {
      kprintf("recv handler registration failed\n");
      //return 1;
   	}
   while(1)
   	;
}

void asyncsend(pid32 pid,umsg32 msg)  // function for asmtests
{
	printf("Time before send is : %d && the PID is : %d \n",clktimefine,currpid);
	pidchecker = pid;
	send(pid,msg);
	
}

void asmtest1() // asynchronous message test 1
{
	pid32 ar1 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 as1 = create(asyncsend,1024,20,"asyncsender1",2,ar1,'A');
	resume(ar1);
	sleepms(25);
	resume(as1);
	sleepms(25);
	if(msgglob != 'A' || pidchecker != ar1){
		kprintf("Test1 failed\n");
		printf("Currpid is %d\n",currpid );
	}
	else{
		kprintf("Test1 passed!\n");
	}
}

void asmtest2() //asynchronous message test 2
{
	int alla = 0;
	int allb = 0;
	int allc = 0;
	int alld = 0;
	pid32 ar1 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 ar2 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 ar3 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 ar4 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 as1 = create(asyncsend,1024,20,"asyncsender1",2,ar1,'A');
	pid32 as2 = create(asyncsend,1024,20,"asyncsender2",2,ar2,'B');	
	pid32 as3 = create(asyncsend,1024,20,"asyncsender3",2,ar3,'C');
	pid32 as4 = create(asyncsend,1024,20,"asyncsender4",2,ar4,'D');

	resume(ar1);
	sleepms(25);
	resume(as1);
	sleepms(25);
	if(msgglob != 'A' || pidchecker != ar1){
		kprintf("Test2 failed\n");
		//printf("Currpid is %d\n",currpid );
	}
	else{
		alla = 1;
	}
	resume(ar2);
	sleepms(25);
	resume(as2);
	sleepms(25);
	if(msgglob != 'B' || pidchecker != ar2){
		kprintf("Test2 failed\n");
		//printf("Currpid is %d\n",currpid );
	}
	else{
		allb = 1;
	}
	resume(ar3);
	sleepms(25);
	resume(as3);
	sleepms(25);
	if(msgglob != 'C' || pidchecker != ar3){
		kprintf("Test2 failed\n");
		//printf("Currpid is %d\n",currpid );
	}
	else{
		allc = 1;
	}
	resume(ar4);
	sleepms(25);
	resume(as4);
	sleepms(25);
	if(msgglob != 'D' || pidchecker != ar4){
		kprintf("Test2 failed\n");
		//printf("Currpid is %d\n",currpid );
	}
	else{
		alld = 1;
	}

	if((alla == 1) && (allb == 1) && (allc == 1) && (alld == 1)){
		kprintf("Test2 passed!\n");
	}
	
}

void asmtest3() // asynchronous message test 3
{
	pid32 ar1 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
	pid32 as1 = create(asyncsend,1024,20,"asyncsender1",2,ar1,'A');
	pid32 as2 = create(asyncsend,1024,20,"asyncsender2",2,ar1,'B');	
	pid32 as3 = create(asyncsend,1024,20,"asyncsender3",2,ar1,'C');
	pid32 as4 = create(asyncsend,1024,20,"asyncsender4",2,ar1,'D');
	
	resume(ar1);
	sleepms(25);
	resume(as1);
	sleepms(25);
	resume(as2);
	sleepms(25);
	resume(as3);
	sleepms(25);
	resume(as4);
	sleepms(25);

	if(msgglob == 'D' || pidchecker == ar1){
		kprintf("Test3 passed!\n");
	}
	else{
		kprintf("Test3 failed\n");
	}
}

void alrmtest1(){
	initial = clktimefine;
	kprintf("The inital clocktime is : %d\n",initial);
	if(registercbsig(MYSIGALRM,&myalrmhandler1,1200) != OK) {
     	kprintf("alarm handler registration failed\n");
     	return 1;
    }
  
    while(1)
   	;
}

int myalrmhandler1(){
	final = clktimefine;
	kprintf("The current clocktime is : %d\n",final);
	if(final >= initial + 1200){
		kprintf("myalrmsignal handler test1 passed\n");
	}
	else{
		kprintf("myalrmsignal handler test1 failed\n");
	}
	return OK;
}

void alrmtest2(){
    initial = clktimefine;
    kprintf("The inital clocktime is : %d\n",initial);
    if(registercbsig(MYSIGALRM,&myalrmhandler2,0) != OK) {
     	kprintf("alarm handler registration failed\n");
     	//return 1;
    }

    while(1)
    ;
}

int myalrmhandler2(){
	final = clktimefine;
	kprintf("The current clocktime is : %d\n",final);
	if(final >= initial){
		kprintf("myalrmsignal handler test2 passed\n");
	}
	else{
		kprintf("myalrmsignal handler test2 failed\n");
	}
}

int sigxcpuhandler1(){
	struct procent *prptr = &proctab[currpid];
	initial = prptr->prcpuused;
	printf("Cpu Time is %d\n",initial);
	if(initial >= 500){
		kprintf("mysigxcpu handler test1 passed\n");
	}
	else{
		kprintf("mysigxcpu handler test1 failed\n");
	}
}

void sigxcputest1(){
	struct procent *prptr = &proctab[currpid];
	if(registercbsig(MYSIGXCPU,&sigxcpuhandler1,500) != OK) {
     	kprintf("sigxcpu handler registration failed\n");
    }

    while(1)
    ;
}
void helper(){
	uint32 sum = 0;
	struct memblk *tmp;
	tmp = memlist.mnext;
	while(tmp != NULL){
		//kprintf("temp length is %d\n",sum);
		sum += tmp->mlength;
		tmp = tmp->mnext;
	}
	kprintf("the total length  is %d\n",sum);
}

int gctest1(){
	struct procent *prptr = &proctab[currpid];
	char *addr;
	char *add;
	char *fboi;
	char *first;
	char *mid;
	char *t1;
	char *t2;
	char *t3;

	helper();
	addr = getmem(1250);
	add = getmem(200);
	fboi = getmem(250);
	first = getmem(1200);
	mid = getmem(1200);
	t1 = getmem(400);
	t2 = getmem(600);
	t3 = getmem(500);
	helper();
}

int gctest2(){

	struct procent *prptr = &proctab[currpid];
	char *addr;
	char *add;
	char *fboi;
	char *first;
	char *mid;
	

	helper();
	addr = getmem(250);
	add = getmem(200);
	fboi = getmem(250);
	first = getmem(200);
	mid = getmem(1000);
	

	helper();

	freemem(addr,250);
	freemem(add,200);
	freemem(fboi,250);
	freemem(first,200);
	freemem(mid,1000);

	

	helper();	
	
	//kprintf("Called from main : The size is %d\n",prptr->prmemblklist->size);
	return 0;
}



void gctest(){
	//resume(create(memdealloc,2048,20,"mem",1,1));

	printf(" memory test : xinu has free memory %d\r\n",memlist.mlength);

	//resume(create(memalloc,2048,20,"mem1",1,1));
	//resume(create(memalloc,2048,20,"mem2",0,NULL));
	/*resume(create(memalloc,2048,20,"mem3",1,3));
	resume(create(memalloc,2048,20,"mem4",1,4));
	resume(create(memalloc,2048,20,"mem5",1,5));
	resume(create(memalloc,2048,20,"mem6",1,6));
	resume(create(memalloc,2048,20,"mem7",1,7));
	resume(create(memalloc,2048,20,"mem8",1,8));
	resume(create(memalloc,2048,20,"mem9",1,9));
	resume(create(memalloc,2048,20,"mem10",1,10));*/

	/*sleepms(5000);
	printf(" memory test : xinu has free memory %d\r\n",memlist.mlength);
	printf("\n");

	resume(create(memalloc,2048,20,"mem11",1,1));
	/*resume(create(memalloc,2048,20,"mem12",1,2));
	resume(create(memalloc,2048,20,"mem13",1,3));
	resume(create(memalloc,2048,20,"mem14",1,4));
	resume(create(memalloc,2048,20,"mem15",1,5));
	resume(create(memalloc,2048,20,"mem16",1,6));
	resume(create(memalloc,2048,20,"mem17",1,7));
	resume(create(memalloc,2048,20,"mem18",1,8));
	resume(create(memalloc,2048,20,"mem19",1,9));
	resume(create(memalloc,2048,20,"mem20",1,10));*/

	sleepms(5000);
	printf(" memory test : xinu has free memory %d\r\n",memlist.mlength);
	printf("\n");

	printf("Main finished\n");


	while(1){
		;
	}
}

process main(void)
{
	
	recvclr();

	
	
	kprintf("--------------MYSIGRECV TEST CASES FOR PART1------------\n");

	printf("TEST1 started\n");
	asmtest1();
	printf("TEST1 finished\n");

	printf("TEST2 started\n");
	asmtest2();
	printf("TEST2 finished\n");

	printf("TEST3 started\n");
	asmtest3();
	printf("TEST3 finished\n");

	kprintf("--------------MYSIGALRM TEST CASES FOR PART1------------\n");

	printf("TEST1 started\n");
	sleepms(2000);
	pid32 alarm1 = create((void*)alrmtest1,1024,20,"alrmtest1",0,NULL);
	resume(alarm1);
	sleepms(2000);
	printf("TEST1 finished\n");
	
	
	printf("TEST2 started\n");
	sleepms(2000);
	pid32 alarm2 = create((void*)alrmtest2,1024,20,"alrmtest2",0,NULL);
	resume(alarm2);
	sleepms(2000);

	printf("TEST2 finished\n");

	kprintf("--------------MYSIGXCPU TEST CASES FOR PART1-------------\n");

    printf("TEST1 started\n");
    sleepms(2000);
    xcpu = create((void*)sigxcputest1,1024,20,"sigxcpu",0,NULL);
	resume(xcpu);
	sleepms(2000);
	printf("TEST1 finished\n");
	sleepms(2000);
	
	
	kprintf("--------------MEMORY GARBAGE COLLECTION TESTS-------------\n");


	

	//helper();
	kprintf("Test1 started\n");
	resume(create((void *)gctest1,2048,20,"mem1",0,NULL));
	sleepms(2000);
	printf("TEST1 finished\n");
	//helper();
	printf("TEST2 started\n");
	resume(create((void *)gctest2,2048,20,"mem1",0,NULL));
	sleepms(2000);
	printf("TEST2 finished\n");

	
	/*resume(create((void*)memalloc,2048,20,"mem2",0,NULL));
	sleepms(2000);
	resume(create(memalloc,2048,20,"mem3",0,NULL));
	sleepms(2000);
	resume(create(memalloc,2048,20,"mem4",0,NULL));
	sleepms(2000);*/
	/*resume(create(memalloc,2048,20,"mem5",0,NULL));
	resume(create(memalloc,2048,20,"mem6",0,NULL));
	resume(create(memalloc,2048,20,"mem7",0,NULL));
	resume(create(memalloc,2048,20,"mem8",0,NULL));
	resume(create(memalloc,2048,20,"mem9",0,NULL));
	resume(create(memalloc,2048,20,"mem10",0,NULL));*/

	/*sleepms(5000);
	printf(" memory test : xinu has free memory %d\r\n",memlist.mlength);
	printf("\n");

	resume(create(memalloc,2048,20,"mem11",1,1));
	/*resume(create(memalloc,2048,20,"mem12",1,2));
	resume(create(memalloc,2048,20,"mem13",1,3));
	resume(create(memalloc,2048,20,"mem14",1,4));
	resume(create(memalloc,2048,20,"mem15",1,5));
	resume(create(memalloc,2048,20,"mem16",1,6));
	resume(create(memalloc,2048,20,"mem17",1,7));
	resume(create(memalloc,2048,20,"mem18",1,8));
	resume(create(memalloc,2048,20,"mem19",1,9));
	resume(create(memalloc,2048,20,"mem20",1,10));*/

	/*sleepms(5000);
	printf(" memory test : xinu has free memory %d\r\n",memlist.mlength);
	printf("\n");
	
	printf("Main finished\n");*/


	while(1){
		;
	}



	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	return OK;
}
