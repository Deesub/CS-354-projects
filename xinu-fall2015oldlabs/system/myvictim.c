#include <xinu.h>
#include <stdio.h>

extern int myvictimglobal;
void myvictim(void);
void func1(void);
void func2(void);

void myvictim(){

	
	func1();
	
	printf("Process ID is:%d,Myvictimglobal is:%d\n", currpid,myvictimglobal);
}
void func1(){
		int a = 8;
		int b = 5;
		func2();
		printf("-------------first function returns--------------- %d\n",a*b);
		printf("\n");
		
}

void func2(){
		//printf("Myvictimglobal is : %d\n",myvictimglobal);
		sleepms(4000);
		//stacktrace(currpid);
}