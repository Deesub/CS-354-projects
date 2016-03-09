#include <xinu.h>
#include <string.h>
#include <stdio.h>


void printsegaddress()
{	
	kprintf("---------TEXT-----------------------\n");
	kprintf("beginning of text is 0x%08x \n",(int)&text);
	kprintf("end of text is 0x%08x \n",(int)&etext);
	

	kprintf("-----------DATA-----------------------\n");
	kprintf("beginning of data is 0x%08x \n",(int)&data);
	kprintf("end of data is 0x%08x \n",(int)&edata);
	

	kprintf("------------BSS-----------------------\n");
	kprintf("beginning of bss is 0x%08x \n",(int)&bss);
	kprintf("end of bss is 0x%08x \n",(int)&ebss);
	

	kprintf("--------------The FIRST 4 BYTES--------------\n");
	kprintf("The first 4 bytes of text is 0x%08x \n",text);
	kprintf("The first 4 bytes of data is 0x%08x \n",(int)data);
	kprintf("The first 4 bytes of bss is 0x%08x \n",(int)bss);



	kprintf("--------------LENGTHS-----------------------\n");
	kprintf("Length of text is %10d\n",(uint32)&etext - (uint32)&text);
	kprintf("Length of data is %10d\n",(uint32)&edata - (uint32)&data);
	kprintf("Length of bss is %10d\n",(uint32)&ebss - (uint32)&bss);
}

