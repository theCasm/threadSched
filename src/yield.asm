/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

default	rel

extern	TTABLE
extern	currID
extern	scheduleNext

%define TSIZ	40
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
	global awaken
	global threadInit

yield:	
	pushfq
	pushadq

	mov	rbx, [rel currID wrt ..gotpc]
	imul 	rcx, [rbx], TSIZ	; rcx = sizeof(struct Thread)*currID

	mov	rbx, [rel TTABLE wrt ..gotpc]
	add	rbx, rcx

	mov	rax, 0xFFFFFFFFFFFFFFFF	; we want to call xsave with
	mov	rdx, 0xFFFFFFFFFFFFFFFF	; all features enabled
	mov	rcx, [rbx + xarea]	; rcx = &TTABLE[currID]->xarea
	XSAVE	[rcx]

	mov	[rbx + SP], rsp	; &TTABLE[currID]->SP = rsp

	;; we just have to change currID to switch processes - but only
	;; if the new process is waiting on a yield. We don't need 
	;; to write everyting in asm anymore
	mov	rax, [rel scheduleNext wrt ..gotpc]
	call	rax

	;; if all has gone well, scheduleNext has simply moved stuff around
	;; so currID now gives the new thread.

awaken:	mov	rbx, [rel currID wrt ..gotpc]
	imul	rcx, [rbx], TSIZ

	mov	rbx, [rel TTABLE wrt ..gotpc]
	add	rbx, rcx

	mov	rsp, [rbx + SP]
	
	mov	rax, 0xFFFFFFFFFFFFFFFF	; we want to call xrstor
	mov	rdx, 0xFFFFFFFFFFFFFFFF	; with all features enabled
	mov	rcx, [rbx + xarea]	; rcx = &TTABLE[currID]->xarea
	XRSTOR	[rcx]
	
	popadq
	popfq
	ret
	

;; necessary to "fake" call to yield so everything works
;; ASSUMES fake stack trace already done
;;  - pushes regs/flags
;;  - xsave
;;  - stores new rsp
;; input: 	rdi should hold pointer to thread struct
;;	rsi should hold SP
threadInit:	
	push	rax
	push	rbx
	push	rdx
	push	rcx
	push	r10

	mov	r10, rsp
	mov	rsp, rsi
	; we are now "in" target thread's stack

	pushfq
	pushadq

	mov	rax, 0xFFFFFFFFFFFFFFFF	; we want to call xsave with
	mov	rdx, 0xFFFFFFFFFFFFFFFF	; all features enabled
	mov	rcx, [rdi + xarea]	; rcx = &TTABLE[currID]->xarea
	XSAVE	[rcx]

	mov	[rdi + SP], rsp
	
	; remember to go back
	mov	rsp, r10

	pop	r10
	pop 	rcx
	pop 	rdx
	pop	rbx
	pop 	rax
	ret
