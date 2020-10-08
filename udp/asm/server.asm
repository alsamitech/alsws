; Assembly Web Server, Sami Alameddine, 2020

; This web Server is part of ALSWS

section .data
	msg db 'alsws'	;defines msg as alsws
	s dw 2		; AF_INET
	db 10h, 00h;=4096
	db 7fh, 0h,0h,01h	;127.0.01
	db 0,0,0,0,0,0,0,0

section .text
	global _start

_start:
	; To do this, we will need to get the linux kernel syscall documentation at 
	; http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
	mov rax, 41	; moves rax to 41, same thing as the socket part of socket()
	mov rdi, 2	; essentailly does mov rdi, AF_INET.
	mov rsi, 2 	; mov rsi, SOCK_DGRAM
	mov rdx, 0	; flags
	syscall

	mov rdi, rdx	; socket file descriptor
	mov rax, 44	; sendto()
	mov rdi, msg	; Moves the RDI Register to the ASCII Value of msg
	mov rsi, 5	; Moves the RSI Register to the length of msg
	mov r8, s	; makes structure
	mov r9, 16
	syscall
	
	; END OF PROGRAM
