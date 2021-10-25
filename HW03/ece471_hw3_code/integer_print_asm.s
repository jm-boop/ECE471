.syntax unified

.arm
@.thumb

@ Syscall Definitions
.equ SYSCALL_EXIT,     1
.equ SYSCALL_WRITE,    4

@ Other Definitions
.equ STDOUT,	       1

        .globl _start
_start:

	mov	r0,#1978

	bl	print_integer

	ldr	r1,=linefeed
	bl	print_string

        @================================
        @ Exit
        @================================
exit:
	mov	r0,#0			@ Return a zero
        mov	r7,#SYSCALL_EXIT	@
        swi	0x0			@ Run the system call



	@====================
	@ print_string
	@====================
	@ Null-terminated string to print pointed to by r1
	@ the value in r1 is destroyed by this routine


@.thumb

print_string:

	push    {r0,r2,r7,r10,lr}	// Save r0,r2,r7,r10,lr on stack

	mov	r2,#3

	@======================
	@ Your code here
	@======================

@ 5 instructions

@	mov	r2,#0
@print_loop:
@	ldrb	r0,[r1,r2]
@	add	r2,r2,#1
@	cmp	r0,#0
@	bne	print_loop


	mov	r7,r1
print_loop:
	ldrb	r0,[r7,#1]!
	cmp	r0,#0
	bne	print_loop
	sub	r2,r7,r1

	@======================

	@ The length of the string pointed to by r1
	@ Should be put in r2 by your code above

	mov	r0,#STDOUT		// R0 Print to stdout
					// R1 points to our string
					// R2 is the length
	mov	r7,#SYSCALL_WRITE	// Load syscall number
	swi	0x0			// System call

	pop	{r0,r2,r7,r10,pc}       // pop r0,r2,r3,pc from stack

@.arm

        @#############################
	@ print_integer
	@#############################
	@ r0 = value to print
	@ r0 is destroyed by this routine

print_integer:
	push	{r10,LR}	// Comments removed for HW
        ldr	r10,=buffer	//
        add	r10,r10,#10	//

divide:
	bl	divide_by_10	//
	add	r8,r8,#0x30	//
	strb	r8,[r10],#-1	//
	adds	r0,r7,#0	//
	bne	divide		//

write_out:
        add	r1,r10,#1	//

        bl	print_string	//

        pop	{r10,LR}	//

        mov	pc,lr		//

	##############################
	# divide_by_10
	##############################
	# r0=numerator
        # r4,r5 trashed
	# r7 = quotient, r8 = remainder

divide_by_10:
	ldr	r4,=429496730		// 1/10 * 2^32
	sub	r5,r0,r0,lsr #30
	umull	r8,r7,r4,r5		// {r8,r7}=r4*r5

	mov	r4,#10			// calculate remainder
	mul	r8,r7,r4
	sub	r8,r0,r8

	mov	pc,lr

linefeed:
	.string	"\n"

@ BSS
.lcomm buffer,11
