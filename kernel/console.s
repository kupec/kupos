	.file	"console.c"
	.text
	.globl	strlen
	.type	strlen, @function
strlen:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L3:
	addl	$1, -4(%ebp)
.L2:
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L3
	movl	-4(%ebp), %eax
	subl	8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	strlen, .-strlen
	.globl	outb
	.type	outb, @function
outb:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movw	%ax, -4(%ebp)
	movl	%edx, %eax
	movb	%al, -8(%ebp)
	movzbl	-8(%ebp), %eax
#APP
# 4 "asm/io.h" 1
	movw -4(%ebp), %dx
	outb %al, %dx
	
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	outb, .-outb
	.globl	inb
	.type	inb, @function
inb:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$20, %esp
	movl	8(%ebp), %eax
	movw	%ax, -20(%ebp)
#APP
# 13 "asm/io.h" 1
	movw -20(%ebp), %dx
	inb %dx, %al
	
# 0 "" 2
#NO_APP
	movb	%al, -1(%ebp)
	movzbl	-1(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	inb, .-inb
	.globl	current_page
	.bss
	.align 4
	.type	current_page, @object
	.size	current_page, 4
current_page:
	.zero	4
	.text
	.globl	console_set_cursor
	.type	console_set_cursor, @function
console_set_cursor:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movw	%dx, -20(%ebp)
	movw	%ax, -24(%ebp)
	movl	current_page, %eax
	imulw	$2000, %ax, %cx
	movzwl	-20(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	leal	(%ecx,%eax), %edx
	movzwl	-24(%ebp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%ebp)
	pushl	$14
	pushl	$980
	call	outb
	addl	$8, %esp
	movzwl	-2(%ebp), %eax
	shrw	$8, %ax
	movzbl	%al, %eax
	pushl	%eax
	pushl	$981
	call	outb
	addl	$8, %esp
	pushl	$15
	pushl	$980
	call	outb
	addl	$8, %esp
	movzwl	-2(%ebp), %eax
	movzbl	%al, %eax
	pushl	%eax
	pushl	$981
	call	outb
	addl	$8, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	console_set_cursor, .-console_set_cursor
	.globl	console_get_cursor
	.type	console_get_cursor, @function
console_get_cursor:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 3, -12
	pushl	$14
	pushl	$980
	call	outb
	addl	$8, %esp
	pushl	$981
	call	inb
	addl	$4, %esp
	movb	%al, -5(%ebp)
	pushl	$15
	pushl	$980
	call	outb
	addl	$8, %esp
	movzbl	-5(%ebp), %eax
	sall	$8, %eax
	movl	%eax, %ebx
	pushl	$981
	call	inb
	addl	$4, %esp
	movzbl	%al, %eax
	addl	%ebx, %eax
	movw	%ax, -8(%ebp)
	movl	current_page, %eax
	imulw	$2000, %ax, %ax
	subw	%ax, -8(%ebp)
	movzwl	-8(%ebp), %eax
	movzwl	%ax, %eax
	imull	$52429, %eax, %eax
	shrl	$16, %eax
	movl	%eax, %edx
	shrw	$6, %dx
	movl	8(%ebp), %eax
	movw	%dx, (%eax)
	movzwl	-8(%ebp), %ecx
	movzwl	%cx, %eax
	imull	$52429, %eax, %eax
	shrl	$16, %eax
	movl	%eax, %edx
	shrw	$6, %dx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	12(%ebp), %eax
	movw	%dx, (%eax)
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	console_get_cursor, .-console_get_cursor
	.globl	console_select_page
	.type	console_select_page, @function
console_select_page:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	imulw	$2000, %ax, %ax
	movw	%ax, -2(%ebp)
	pushl	$12
	pushl	$980
	call	outb
	addl	$8, %esp
	movzwl	-2(%ebp), %eax
	shrw	$8, %ax
	movzbl	%al, %eax
	pushl	%eax
	pushl	$981
	call	outb
	addl	$8, %esp
	pushl	$13
	pushl	$980
	call	outb
	addl	$8, %esp
	movzwl	-2(%ebp), %eax
	movzbl	%al, %eax
	pushl	%eax
	pushl	$981
	call	outb
	addl	$8, %esp
	movl	8(%ebp), %eax
	movl	%eax, current_page
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	console_select_page, .-console_select_page
	.globl	get_video_page_ptr
	.type	get_video_page_ptr, @function
get_video_page_ptr:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	imull	$4000, %eax, %eax
	addl	$753664, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	get_video_page_ptr, .-get_video_page_ptr
	.globl	console_print_line
	.type	console_print_line, @function
console_print_line:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$48, %esp
	leal	-48(%ebp), %eax
	pushl	%eax
	leal	-46(%ebp), %eax
	pushl	%eax
	call	console_get_cursor
	addl	$8, %esp
	movzwl	-46(%ebp), %eax
	cmpw	$22, %ax
	jbe	.L14
	movzwl	-46(%ebp), %eax
	movzwl	%ax, %eax
	subl	$23, %eax
	jmp	.L15
.L14:
	movl	$0, %eax
.L15:
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, -20(%ebp)
	movzwl	-46(%ebp), %eax
	movzwl	%ax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, -24(%ebp)
	movzwl	-46(%ebp), %eax
	movl	-16(%ebp), %edx
	subl	%edx, %eax
	movw	%ax, -46(%ebp)
	movl	current_page, %edx
	movl	$1, %eax
	subl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	current_page, %eax
	pushl	%eax
	call	get_video_page_ptr
	addl	$4, %esp
	movl	%eax, -32(%ebp)
	pushl	-28(%ebp)
	call	get_video_page_ptr
	addl	$4, %esp
	movl	%eax, -36(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L16
.L17:
	movl	-4(%ebp), %eax
	leal	(%eax,%eax), %edx
	movl	-36(%ebp), %eax
	addl	%edx, %eax
	movw	$1824, (%eax)
	addl	$1, -4(%ebp)
.L16:
	cmpl	$1999, -4(%ebp)
	jle	.L17
	movl	-20(%ebp), %eax
	movl	%eax, -8(%ebp)
	jmp	.L18
.L19:
	movl	-8(%ebp), %eax
	leal	(%eax,%eax), %edx
	movl	-32(%ebp), %eax
	leal	(%edx,%eax), %ecx
	movl	-8(%ebp), %eax
	subl	-20(%ebp), %eax
	leal	(%eax,%eax), %edx
	movl	-36(%ebp), %eax
	addl	%eax, %edx
	movzwl	(%ecx), %eax
	movw	%ax, (%edx)
	addl	$1, -8(%ebp)
.L18:
	movl	-8(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jl	.L19
	pushl	8(%ebp)
	call	strlen
	addl	$4, %esp
	movzwl	-48(%ebp), %edx
	movzwl	%dx, %ecx
	movl	$80, %edx
	subl	%ecx, %edx
	cmpl	%edx, %eax
	jge	.L20
	pushl	8(%ebp)
	call	strlen
	addl	$4, %esp
	jmp	.L21
.L20:
	movzwl	-48(%ebp), %eax
	movzwl	%ax, %edx
	movl	$80, %eax
	subl	%edx, %eax
.L21:
	movl	%eax, -40(%ebp)
	movzwl	-46(%ebp), %eax
	movzwl	%ax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movzwl	-48(%ebp), %eax
	movzwl	%ax, %eax
	addl	%edx, %eax
	movl	%eax, -44(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L22
.L23:
	movl	-12(%ebp), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	-44(%ebp), %ecx
	movl	-12(%ebp), %edx
	addl	%ecx, %edx
	addl	%edx, %edx
	movl	%edx, %ecx
	movl	-36(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%eax), %eax
	movb	%al, (%edx)
	addl	$1, -12(%ebp)
.L22:
	movl	-12(%ebp), %eax
	cmpl	-40(%ebp), %eax
	jl	.L23
	pushl	-28(%ebp)
	call	console_select_page
	addl	$4, %esp
	movzwl	-46(%ebp), %eax
	addl	$1, %eax
	movzwl	%ax, %eax
	pushl	$0
	pushl	%eax
	call	console_set_cursor
	addl	$8, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	console_print_line, .-console_print_line
	.ident	"GCC: (Ubuntu 11.2.0-7ubuntu2) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
