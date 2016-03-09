
	.text
	.globl	host2netl
host2netl:
	pushl   %ebp //points to the stack frame of the caller
	movl	%esp, %ebp // saves the callers  base pointer by pushing it onto the stack
	movl	8(%ebp),%eax // temporarily stores into the eax register
	bswap	%eax // reverses the byte order of a 32 bit register
	
	leave 
	ret

