; Assembly WebServer
; Sami Alameddine, 2020

;		CREDITS TO:
;			FUTURE CONTRIBUTORS HERE!!!

section .data
	file db "index.html",0		; Defines file path

section .bss
	fcontent resb 18

section .text
	global _start

_start:
	mov rax, 2
	mov rdi, file			; This chunk opens the file
	mov rsi, 0
	syscall

	push rax
	mov rdi, rax
	mov rax, 0
	mov rsi, fcontent		; This chunk reads from the file
	mov rdx, 17
	syscall

	mov rax, 3
	pop rdi				; This chunk closes the file
	syscall

