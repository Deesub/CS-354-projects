/*  main.c  - main */


#include <xinu.h>
#include <stdio.h>


void messagesent(pid32,char,int);
void messagerec(int);
int myrecvhandler();

void asyncrec(void);
void asyncsend(pid32,umsg32);
void asyncrecv(void);
void bmstest1(void);
void bmstest2(void);
void bmstest3(void);
void bmstest4(void);
void printbsmtests(void);
void asmtest1(void);
void asmtest2(void);
void asmtest3(void);

int a = 0;
int b = 0;
int c = 0;
int d = 0;



// 
void messagesent(pid32 p, char msg,int maxwait) // function for bsmtests
{
	printf("Time is : %d && the PID is : %d \n",clktimefine,currpid);
	sendbt(p,msg,maxwait);
	//printf("Time is : %d && the PID is : %d \n",clktimefine,currpid);
}

void messagerec(int number) //  func for bsm tests
{
	umsg32 msg;
	int i;
	i = 0;
	while(i < number){
		msg = receiveblock();
		kprintf("Message received from sender: \"%c\".\n\r",msg);
		i++;
	}
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
	if (registercb(&myrecvhandler) != OK) {
      kprintf("recv handler registration failed\n");
      return 1;
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
void bmstest1() // test1
{
	pid32 receiver = create(messagerec,1024,20,"receiver",1,2);
	pid32 sender1 = create(messagesent,1024,20,"sender1",3,receiver,'D',2000);
	resume(sender1);
	sleepms(1000);
	resume(receiver);
	sleepms(1000);
	a = 1;


}

void bmstest2() // test 3
{
	pid32 receiver1 = create(messagerec,1024,20,"receiver",1,2);
	pid32 sender1 = create(messagesent,1024,20,"sender1",3,receiver1,'D',2000);
	pid32 receiver2 = create(messagerec,1024,20,"receiver",1,2);
	pid32 sender2 = create(messagesent,1024,20,"sender2",3,receiver2,'E',2000);
	resume(sender1);
	sleepms(1000);
	resume(sender2);
	sleepms(1000);
	resume(receiver1);
	sleepms(1000);
	resume(receiver2);
	sleepms(1000);

	b = 1;
}
void bmstest3() // test3
{
	pid32 receiver1 = create(messagerec,1024,20,"receiver",1,10);
	pid32 sender1 = create(messagesent,1024,20,"sender1",3,receiver1,'D',2000);
	resume(sender1);
	sleepms(1000);
	resume(receiver1);
	sleepms(1000);
	pid32 receiver2 = create(messagerec,1024,20,"receiver",0,NULL);
	sender1 = create(messagesent,1024,20,"sender1",3,receiver2,'E',2000);
	resume(sender1);
	sleepms(1000);
	resume(receiver2);
	sleepms(1000);

	c = 1;
}
void bmstest4() // test 4
{
	pid32 receiver = create((void*)messagerec,1024,20,"receiver",1,5);
	pid32 sender1 = create((void*)messagesent,1024,20,"sender1",3,receiver,'D',2000);
	pid32 sender2 = create((void*)messagesent,1024,20,"sender2",3,receiver,'E',2000);	
	
	resume(sender1);
	sleepms(1000);
	resume(sender2);
	sleepms(1000);
	resume(receiver);
	sleepms(1000);

	d = 1;
}



void printbsmtests()  // prints test for blocked message testing
{
	if(a == 1){
		printf("Test 1 passed!\n");
	}
	else{
		printf("Test 1 failed\n");
	}

	if(b == 1){
		printf("Test 2 passed!\n");
	}
	else{
		printf("Test 2 failed\n");
	}

	if(c == 1){
		printf("Test 3 passed!\n");
	}
	else{
		printf("Test 3 failed\n");
	}

	if(d == 1){
		printf("Test 4 passed!\n");
	}
	else{
		printf("Test 4 failed\n");
	}
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






process	main(void)
{

	//printf("SLUT\n");

	
	recvclr();

	//printf("----------Timed blocking message send TEST for PART 1-------\n");

	//TEST1

	//printf("TEST1 started\n");
	
	//bmstest1();

	//TEST2

	//printf("TEST2 started\n");

	//bmstest2();

	//TEST3

	//printf("TEST3 started\n");

	//bmstest3();

	//TEST4

	//printf("TEST4 started\n");

	//bmstest4();


	
	//printbsmtests(); // prints the result of the tests


	

	printf("---------------ASYNC MSG RECV TEST CASES FOR PART2------------\n");

	//TEST 1 - 

	printf("TEST1 started\n");

	asmtest1();
	printf("TEST1 finished\n");

	//TEST2 - 

	printf("TEST2 started\n");
	asmtest2();
	printf("TEST2 finished\n");


	printf("TEST3 started\n");
	//TEST3 - 


	asmtest3();

	printf("TEST3 finished\n");
	
	/*pid32 ar1 = create(asyncrec,1024,20,"asyncreceiver",0,NULL);
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
	resume(as4);*/
	
	




	return OK;
}
