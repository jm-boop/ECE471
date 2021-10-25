
@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:
	mov	r0, #42			@ move value 7 to return into r0
	mov	r7, #SYSCALL_EXIT	@ put exit() syscall number in r7
	swi	0			@ execute syscall
