	.text
	.file	"nod.cs"
	.globl	getnod                          # -- Begin function getnod
	.p2align	4, 0x90
	.type	getnod,@function
getnod:                                 # @getnod
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -12(%rbp)
	movl	%esi, -16(%rbp)
	movl	$0, -8(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
.LBB0_1:                                # %v9
                                        # =>This Inner Loop Header: Depth=1
	cmpl	$0, -4(%rbp)
	jle	.LBB0_8
# %bb.2:                                # %v13
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-12(%rbp), %eax
	cltd
	idivl	-4(%rbp)
	movl	%edx, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	.LBB0_6
# %bb.3:                                # %v20
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-16(%rbp), %eax
	cltd
	idivl	-4(%rbp)
	movl	%edx, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	.LBB0_5
# %bb.4:                                # %v27
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
	movl	$0, -4(%rbp)
.LBB0_5:                                # %v28
                                        #   in Loop: Header=BB0_1 Depth=1
	jmp	.LBB0_6
.LBB0_6:                                # %v21
                                        #   in Loop: Header=BB0_1 Depth=1
	jmp	.LBB0_7
.LBB0_7:                                # %v14
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.LBB0_1
.LBB0_8:                                # %v10
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	getnod, .Lfunc_end0-getnod
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movabsq	$.L.str.4, %rdi
	leaq	-12(%rbp), %rsi
	movb	$0, %al
	callq	scanf@PLT
	movabsq	$.L.str.4, %rdi
	leaq	-8(%rbp), %rsi
	movb	$0, %al
	callq	scanf@PLT
	movl	-12(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	getnod
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %esi
	movabsq	$.L.str, %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d\n"
	.size	.L.str, 4

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"%c\n"
	.size	.L.str.1, 4

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"%f\n"
	.size	.L.str.2, 4

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"%s\n"
	.size	.L.str.3, 4

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"%d"
	.size	.L.str.4, 3

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"%c"
	.size	.L.str.5, 3

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"%f"
	.size	.L.str.6, 3

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"%s"
	.size	.L.str.7, 3

	.section	".note.GNU-stack","",@progbits
