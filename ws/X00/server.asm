; alsws Assembly Web Server
; Sami Alameddine, 2020
; This is under the eXPAT (MIT) Liscense

section .bss
	fcontent resb 1000

section .data
	file db 'index.html'	; Defines the file path

section .text
	global _start
_start:
	call _rras		; calls RRAS




_rras:
	; Reads file
	mov rax, 2
	mov rdi, file
	mov rsi, 0		; This module opens the file
	mov rdx, 0644o
	syscall			; requests service from kernel

	push rax
	mov rdi, rax
	mov rax, 1		; This module reads the file
	mov rsi, fcontent
	mov rdx, 1000
	syscall			; requests service from kernel

	mov rax, 3
	pop rdi			; This module closes the file
	syscall			; requests service from Kernel
