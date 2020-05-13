	.file	"base.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d aaaaaaaaaaaaaaaaaaaa\n"
.LC1:
	.string	"%d cccccccccccccccccccc\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$5, -24(%rbp)
	movl	$5, -4(%rbp)
	movl	-24(%rbp), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -20(%rbp)
	movl	$30, -16(%rbp)
	jmp	.L2
.L12:
	movl	$0, -12(%rbp)
	jmp	.L3
.L9:
	movl	-24(%rbp), %eax
	imull	-12(%rbp), %eax
	movl	%eax, -24(%rbp)
	movl	-20(%rbp), %eax
	subl	-24(%rbp), %eax
	movl	-12(%rbp), %edx
	imull	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jge	.L4
	movl	-12(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L5
.L8:
	movl	-4(%rbp), %eax
	subl	%eax, -20(%rbp)
	movl	-24(%rbp), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -20(%rbp)
	jge	.L6
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L7
.L6:
	movl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L7:
	addl	$1, -8(%rbp)
.L5:
	cmpl	$9, -8(%rbp)
	jle	.L8
.L4:
	addl	$1, -12(%rbp)
.L3:
	cmpl	$29, -12(%rbp)
	jle	.L9
	cmpl	$29, -16(%rbp)
	jg	.L10
	subl	$4, -16(%rbp)
	jmp	.L11
.L10:
	subl	$1, -16(%rbp)
.L11:
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L2:
	cmpl	$1, -16(%rbp)
	jg	.L12
	xor	    %eax, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
