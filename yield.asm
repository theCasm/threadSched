extern	TTABLE
extern	currID
extern	scheduleNext

%define TSIZ	32
%define xarea  	16
%define SP	  	24


	section	.text

%macro	pushadq 0
	
	sub	rsp, 120
	mov	[rsp+112], rax
	mov	[rsp+104], rbx
	mov	[rsp+96], rcx
	mov	[rsp+88], rdx
	mov	[rsp+80], rbp
	mov	[rsp+72], rdi
	mov	[rsp+64], rsi
	mov	[rsp+56], r8
	mov	[rsp+48], r9
	mov	[rsp+40], r10
	mov	[rsp+32], r11
	mov	[rsp+24], r12
	mov	[rsp+16], r13
	mov	[rsp+8], r14
	mov	[rsp+0], r15

%endmacro

%macro	popadq 0
	
	mov	rax, [rsp+112]
	mov	rbx, [rsp+104]
	mov	rcx, [rsp+96]
	mov	rdx, [rsp+88]
	mov	rbp, [rsp+80]
	mov	rdi, [rsp+72]
	mov	rsi, [rsp+64]
	mov	r8,  [rsp+56]
	mov	r9,  [rsp+48]
	mov	r10, [rsp+40]
	mov	r11, [rsp+32]
	mov	r12, [rsp+24]
	mov	r13, [rsp+16]
	mov	r14, [rsp+8]
	mov	r15, [rsp+0]
	add	rsp, 120

%endmacro

	global yield

yield:	pushadq

	imul 	rcx, [rel currID], TSIZ	; rcx = sizeof(struct Thread)*currID

	lea	rbx, [rel TTABLE]	;
	add	rbx, rcx		; rbx = &TTABLE[currID]

	mov	rax, 0xFFFFFFFFFFFFFFFF	; we want to call xsave with
	mov	rdx, 0xFFFFFFFFFFFFFFFF	; all features enabled
	mov	rcx, [rbx + xarea]	; rcx = &TTABLE[currID]->xarea
	XSAVE	[rcx]

	mov	[rbx + SP], rsp	; &TTABLE[currID]->SP = rsp

	;; we just have to change currID to switch processes - but only
	;; if the new process is waiting on a yield. We don't need 
	;; to write everyting in asm anymore
	call	scheduleNext

	;; if all has gone well, scheduleNext has simply moved stuff around
	;; so currID now gives the new thread.

awaken:	imul	rcx, [rel currID], TSIZ

	lea	rbx, [rel TTABLE]
	add	rbx, rcx

	mov	rsp, [rbx + SP]
	
	mov	rax, 0xFFFFFFFFFFFFFFFF	; we want to call xrstor
	mov	rdx, 0xFFFFFFFFFFFFFFFF	; with all features enabled
	mov	rcx, [rbx + xarea]	; rcx = &TTABLE[currID]->xarea
	XRSTOR	[rcx]
	
	popadq
	ret
	
