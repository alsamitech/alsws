	.text
	.file	"tcpserver.c"
	.globl	err_n_die               # -- Begin function err_n_die
	.p2align	4, 0x90
	.type	err_n_die,@function
err_n_die:                              # @err_n_die
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$464, %rsp              # imm = 0x1D0
	testb	%al, %al
	movaps	%xmm7, -240(%rbp)       # 16-byte Spill
	movaps	%xmm6, -256(%rbp)       # 16-byte Spill
	movaps	%xmm5, -272(%rbp)       # 16-byte Spill
	movaps	%xmm4, -288(%rbp)       # 16-byte Spill
	movaps	%xmm3, -304(%rbp)       # 16-byte Spill
	movaps	%xmm2, -320(%rbp)       # 16-byte Spill
	movaps	%xmm1, -336(%rbp)       # 16-byte Spill
	movaps	%xmm0, -352(%rbp)       # 16-byte Spill
	movq	%rdi, -360(%rbp)        # 8-byte Spill
	movq	%r9, -368(%rbp)         # 8-byte Spill
	movq	%r8, -376(%rbp)         # 8-byte Spill
	movq	%rcx, -384(%rbp)        # 8-byte Spill
	movq	%rdx, -392(%rbp)        # 8-byte Spill
	movq	%rsi, -400(%rbp)        # 8-byte Spill
	je	.LBB0_4
# %bb.3:                                # %entry
	movaps	-352(%rbp), %xmm0       # 16-byte Reload
	movaps	%xmm0, -176(%rbp)
	movaps	-336(%rbp), %xmm1       # 16-byte Reload
	movaps	%xmm1, -160(%rbp)
	movaps	-320(%rbp), %xmm2       # 16-byte Reload
	movaps	%xmm2, -144(%rbp)
	movaps	-304(%rbp), %xmm3       # 16-byte Reload
	movaps	%xmm3, -128(%rbp)
	movaps	-288(%rbp), %xmm4       # 16-byte Reload
	movaps	%xmm4, -112(%rbp)
	movaps	-272(%rbp), %xmm5       # 16-byte Reload
	movaps	%xmm5, -96(%rbp)
	movaps	-256(%rbp), %xmm6       # 16-byte Reload
	movaps	%xmm6, -80(%rbp)
	movaps	-240(%rbp), %xmm7       # 16-byte Reload
	movaps	%xmm7, -64(%rbp)
.LBB0_4:                                # %entry
	movq	-368(%rbp), %rax        # 8-byte Reload
	movq	%rax, -184(%rbp)
	movq	-376(%rbp), %rcx        # 8-byte Reload
	movq	%rcx, -192(%rbp)
	movq	-384(%rbp), %rdx        # 8-byte Reload
	movq	%rdx, -200(%rbp)
	movq	-392(%rbp), %rsi        # 8-byte Reload
	movq	%rsi, -208(%rbp)
	movq	-400(%rbp), %rdi        # 8-byte Reload
	movq	%rdi, -216(%rbp)
	movq	-360(%rbp), %r8         # 8-byte Reload
	movq	%r8, -8(%rbp)
	callq	__errno_location
	leaq	-48(%rbp), %rcx
	movl	(%rax), %r9d
	movl	%r9d, -12(%rbp)
	movq	%rcx, %rax
	leaq	-224(%rbp), %rdx
	movq	%rdx, 16(%rax)
	leaq	16(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movl	$48, 4(%rax)
	movl	$8, (%rax)
	movq	stdout, %rdi
	movq	-8(%rbp), %rsi
	movq	%rcx, %rdx
	callq	vfprintf
	movq	stdout, %rdi
	movabsq	$.L.str, %rsi
	movl	%eax, -404(%rbp)        # 4-byte Spill
	movb	$0, %al
	callq	fprintf
	movq	stdout, %rdi
	movl	%eax, -408(%rbp)        # 4-byte Spill
	callq	fflush
	cmpl	$0, -12(%rbp)
	movl	%eax, -412(%rbp)        # 4-byte Spill
	je	.LBB0_2
# %bb.1:                                # %if.then
	movq	stdout, %rdi
	movl	-12(%rbp), %edx
	movl	-12(%rbp), %eax
	movq	%rdi, -424(%rbp)        # 8-byte Spill
	movl	%eax, %edi
	movl	%edx, -428(%rbp)        # 4-byte Spill
	callq	strerror
	movq	-424(%rbp), %rdi        # 8-byte Reload
	movabsq	$.L.str.1, %rsi
	movl	-428(%rbp), %edx        # 4-byte Reload
	movq	%rax, %rcx
	movb	$0, %al
	callq	fprintf
	movl	-12(%rbp), %edi
	movl	%eax, -432(%rbp)        # 4-byte Spill
	callq	strerror
	movq	stdout, %rdi
	movabsq	$.L.str, %rsi
	movq	%rax, -440(%rbp)        # 8-byte Spill
	movb	$0, %al
	callq	fprintf
	movq	stdout, %rdi
	movl	%eax, -444(%rbp)        # 4-byte Spill
	callq	fflush
	movl	%eax, -448(%rbp)        # 4-byte Spill
.LBB0_2:                                # %if.end
	leaq	-48(%rbp), %rax
	movl	$1, %edi
	movq	%rax, -456(%rbp)        # 8-byte Spill
	callq	exit
.Lfunc_end0:
	.size	err_n_die, .Lfunc_end0-err_n_die
	.cfi_endproc
                                        # -- End function
	.globl	bin2hex                 # -- Begin function bin2hex
	.p2align	4, 0x90
	.type	bin2hex,@function
bin2hex:                                # @bin2hex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movabsq	$.L.str.2, %rsi
	movq	%rsi, -40(%rbp)
	cmpq	$0, -16(%rbp)
	je	.LBB1_2
# %bb.1:                                # %lor.lhs.false
	cmpq	$0, -24(%rbp)
	ja	.LBB1_3
.LBB1_2:                                # %if.then
	movq	$0, -8(%rbp)
	jmp	.LBB1_8
.LBB1_3:                                # %if.end
	imulq	$3, -24(%rbp), %rax
	addq	$1, %rax
	movl	%eax, %ecx
	movl	%ecx, -44(%rbp)
	movslq	-44(%rbp), %rdi
	callq	malloc
	xorl	%esi, %esi
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rdi
	movslq	-44(%rbp), %rdx
	callq	memset
	movl	$0, -48(%rbp)
.LBB1_4:                                # %for.cond
                                        # =>This Inner Loop Header: Depth=1
	movslq	-48(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jae	.LBB1_7
# %bb.5:                                # %for.body
                                        #   in Loop: Header=BB1_4 Depth=1
	movq	-40(%rbp), %rax
	movq	-16(%rbp), %rcx
	movslq	-48(%rbp), %rdx
	movzbl	(%rcx,%rdx), %esi
	sarl	$4, %esi
	movslq	%esi, %rcx
	movb	(%rax,%rcx), %dil
	movq	-32(%rbp), %rax
	imull	$3, -48(%rbp), %esi
	movslq	%esi, %rcx
	movb	%dil, (%rax,%rcx)
	movq	-40(%rbp), %rax
	movq	-16(%rbp), %rcx
	movslq	-48(%rbp), %rdx
	movzbl	(%rcx,%rdx), %esi
	andl	$15, %esi
	movslq	%esi, %rcx
	movb	(%rax,%rcx), %dil
	movq	-32(%rbp), %rax
	imull	$3, -48(%rbp), %esi
	addl	$1, %esi
	movslq	%esi, %rcx
	movb	%dil, (%rax,%rcx)
	movq	-32(%rbp), %rax
	imull	$3, -48(%rbp), %esi
	addl	$2, %esi
	movslq	%esi, %rcx
	movb	$32, (%rax,%rcx)
# %bb.6:                                # %for.inc
                                        #   in Loop: Header=BB1_4 Depth=1
	movl	-48(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -48(%rbp)
	jmp	.LBB1_4
.LBB1_7:                                # %for.end
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB1_8:                                # %return
	movq	-8(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	bin2hex, .Lfunc_end1-bin2hex
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$8336, %rsp             # imm = 0x2090
	xorl	%edx, %edx
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$2, %edi
	movl	$1, %esi
	callq	socket
	movl	%eax, -20(%rbp)
	cmpl	$0, %eax
	jge	.LBB2_2
# %bb.1:                                # %if.then
	movabsq	$.L.str.3, %rdi
	movb	$0, %al
	callq	err_n_die
.LBB2_2:                                # %if.end
	xorl	%eax, %eax
	movl	$16, %ecx
	movl	%ecx, %edx
	leaq	-48(%rbp), %rsi
	movq	%rsi, %rdi
	movl	%eax, %esi
	movl	%eax, -8296(%rbp)       # 4-byte Spill
	callq	memset
	movw	$2, -48(%rbp)
	movl	-8296(%rbp), %edi       # 4-byte Reload
	callq	htonl
	movl	%eax, -44(%rbp)
	movl	$18000, %edi            # imm = 0x4650
	callq	htons
	movw	%ax, -46(%rbp)
	movl	-20(%rbp), %edi
	leaq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movl	$16, %edx
	callq	bind
	cmpl	$0, %eax
	jge	.LBB2_4
# %bb.3:                                # %if.then5
	movabsq	$.L.str.4, %rdi
	movb	$0, %al
	callq	err_n_die
.LBB2_4:                                # %if.end6
	movl	-20(%rbp), %edi
	movl	$10, %esi
	callq	listen
	cmpl	$0, %eax
	jge	.LBB2_6
# %bb.5:                                # %if.then9
	movabsq	$.L.str.5, %rdi
	movb	$0, %al
	callq	err_n_die
.LBB2_6:                                # %if.end10
	jmp	.LBB2_7
.LBB2_7:                                # %for.cond
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB2_8 Depth 2
	movabsq	$.L.str.6, %rdi
	movl	$18000, %esi            # imm = 0x4650
	movb	$0, %al
	callq	printf
	movq	stdout, %rdi
	movl	%eax, -8300(%rbp)       # 4-byte Spill
	callq	fflush
	xorl	%esi, %esi
	movl	%esi, %edi
	movl	-20(%rbp), %esi
	movq	%rdi, -8312(%rbp)       # 8-byte Spill
	movl	%esi, %edi
	movq	-8312(%rbp), %rsi       # 8-byte Reload
	movq	-8312(%rbp), %rdx       # 8-byte Reload
	movl	%eax, -8316(%rbp)       # 4-byte Spill
	callq	accept
	xorl	%esi, %esi
	movl	$4096, %edi             # imm = 0x1000
	movl	%edi, %edx
	leaq	-8272(%rbp), %rdi
	movl	%eax, -24(%rbp)
	callq	memset
.LBB2_8:                                # %while.cond
                                        #   Parent Loop BB2_7 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	$4095, %eax             # imm = 0xFFF
	movl	%eax, %edx
	leaq	-8272(%rbp), %rsi
	movl	-24(%rbp), %edi
	callq	read
	movl	%eax, %edi
	movl	%edi, -28(%rbp)
	cmpl	$0, %edi
	jle	.LBB2_12
# %bb.9:                                # %while.body
                                        #   in Loop: Header=BB2_8 Depth=2
	leaq	-8272(%rbp), %rdi
	movq	stdout, %rax
	movslq	-28(%rbp), %rsi
	movq	%rax, -8328(%rbp)       # 8-byte Spill
	callq	bin2hex
	leaq	-8272(%rbp), %rcx
	movq	-8328(%rbp), %rdi       # 8-byte Reload
	movabsq	$.L.str.7, %rsi
	movq	%rax, %rdx
	movb	$0, %al
	callq	fprintf
	movl	-28(%rbp), %r8d
	subl	$1, %r8d
	movslq	%r8d, %rcx
	movzbl	-8272(%rbp,%rcx), %r8d
	cmpl	$10, %r8d
	movl	%eax, -8332(%rbp)       # 4-byte Spill
	jne	.LBB2_11
# %bb.10:                               # %if.then26
                                        #   in Loop: Header=BB2_7 Depth=1
	jmp	.LBB2_12
.LBB2_11:                               # %if.end27
                                        #   in Loop: Header=BB2_8 Depth=2
	xorl	%esi, %esi
	movl	$4096, %eax             # imm = 0x1000
	movl	%eax, %edx
	leaq	-8272(%rbp), %rdi
	callq	memset
	jmp	.LBB2_8
.LBB2_12:                               # %while.end
                                        #   in Loop: Header=BB2_7 Depth=1
	cmpl	$0, -28(%rbp)
	jge	.LBB2_14
# %bb.13:                               # %if.then31
                                        #   in Loop: Header=BB2_7 Depth=1
	movabsq	$.L.str.8, %rdi
	movb	$0, %al
	callq	err_n_die
.LBB2_14:                               # %if.end32
                                        #   in Loop: Header=BB2_7 Depth=1
	movl	$4097, %eax             # imm = 0x1001
	movl	%eax, %esi
	leaq	-4160(%rbp), %rdi
	movabsq	$.L.str.9, %rdx
	movb	$0, %al
	callq	snprintf
	movl	%eax, -8336(%rbp)       # 4-byte Spill
	jmp	.LBB2_7
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\n"
	.size	.L.str, 2

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"(eerno = %d) : %s\n"
	.size	.L.str.1, 19

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"0123456789ABCDEF"
	.size	.L.str.2, 17

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"socket error."
	.size	.L.str.3, 14

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"bind error."
	.size	.L.str.4, 12

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"listen error."
	.size	.L.str.5, 14

	.type	.L.str.6,@object        # @.str.6
.L.str.6:
	.asciz	"waiting for connection on port %d\n"
	.size	.L.str.6, 35

	.type	.L.str.7,@object        # @.str.7
.L.str.7:
	.asciz	"\n%s\n\n%s"
	.size	.L.str.7, 8

	.type	.L.str.8,@object        # @.str.8
.L.str.8:
	.asciz	"read error"
	.size	.L.str.8, 11

	.type	.L.str.9,@object        # @.str.9
.L.str.9:
	.asciz	"HTTP/1.0 200 OK\r\n\r\nEnterCodeHere"
	.size	.L.str.9, 33


	.ident	"clang version 7.0.0 (git@github.com:apple/swift-clang.git 5c9d04dc0697297a47b5edb0c1a581b306a42bdb) (git@github.com:apple/swift-llvm.git 34250a6eef79ee8a83c5cfb4deb1583176dcbb63)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym err_n_die
	.addrsig_sym __errno_location
	.addrsig_sym vfprintf
	.addrsig_sym fprintf
	.addrsig_sym fflush
	.addrsig_sym strerror
	.addrsig_sym exit
	.addrsig_sym bin2hex
	.addrsig_sym malloc
	.addrsig_sym main
	.addrsig_sym socket
	.addrsig_sym htonl
	.addrsig_sym htons
	.addrsig_sym bind
	.addrsig_sym listen
	.addrsig_sym printf
	.addrsig_sym accept
	.addrsig_sym read
	.addrsig_sym snprintf
	.addrsig_sym stdout
