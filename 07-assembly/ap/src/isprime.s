	.file	"main.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	isprime
	.type	isprime, @function
isprime:
.LFB0:
	.cfi_startproc
	lea	ecx, -1[rdi]
	mov	eax, 1
	cmp	edi, 2
	jne	.L3
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L4:
	mov	eax, edi
	xor	edx, edx
	div	ecx
	sub	ecx, 1
	test	edx, edx
	je	.L6
.L3:
	cmp	ecx, 1
	jg	.L4
	mov	eax, 1
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	xor	eax, eax
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	ret
	.cfi_endproc
.LFE0:
	.size	isprime, .-isprime
	.ident	"GCC: (GNU) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
