segment .data
i	dd 0
j	dd 0
z	dd 0
intersections	dd 0
union		dd 0
segment .bss
st_1		resd 1
st_2		resd 1
size1  	resd 1
size2  	resd 1
n 	  	resd 1
lim1  	resd 1
lim2 	resd 1


segment .text
global n_gram



n_gram:
	push ebp
	mov ebp, esp
	
	; clear variables
	mov dword [intersections], 0
	mov dword [i], 0
	
	; Taking parameters to variables.
	;###########################
	mov ecx, [ebp+8]
	mov dword [st_1], ecx
	
	mov ecx, [ebp+12]
	mov dword [size1], ecx
	
	mov ecx, [ebp+16]
	mov [st_2], ecx
	
	mov ecx, [ebp+20]
	mov dword [size2], ecx

	mov ecx, [ebp+24]
	mov dword [n], ecx
	;#######################
	
	xor ecx, ecx
	add ecx, 2
	add ecx, dword [size1]
	add ecx, dword [size2]
	sub ecx, dword [n]
	sub ecx, dword [n]
	mov dword [union], ecx
	
	mov ecx, dword [size1]
	sub ecx, dword [n]
	inc ecx
	mov dword [lim1], ecx
	
	mov ecx, dword [size2]
	sub ecx, dword [n]
	inc ecx
	mov dword [lim2], ecx
	
	;mov eax, 4
	;mov ebx, 1
	;mov ecx, [st_1]
	;mov edx, [size1]
	;int 80h

	;eax = address of first byte of first string
	;ebx = address of first byte of second string
	mov eax, dword [st_1]
	mov ebx, dword [st_2]
	;#########################
	
	
	
FOR1:
	mov ecx, dword [i]
	cmp ecx, dword [lim1]
	je son
	inc ecx
	mov dword [i], ecx
	mov dword [j], 0
	
FOR2:
	mov ecx, dword [j]
	cmp ecx, dword [lim2]
	je FOR1
	inc ecx
	mov dword [j], ecx
	mov dword [z], 0
	
FOR3:
	mov ecx, dword [z]
	cmp ecx, dword [n]
	je buldu
	
	mov ecx, eax
	add ecx, dword [i]
	add ecx, dword [z]
	sub ecx, 1
	mov dl, byte [ecx]
	
	mov ecx, ebx
	add ecx, dword [j]
	add ecx, dword [z]
	sub ecx, 1
	mov dh, byte [ecx]
	
	cmp dl, dh
	jne FOR2
don:
	add dword [z], 1
	jmp FOR3


buldu:
	add dword [intersections], 1
	jmp FOR2


son:
	mov ecx, dword [union]
	sub ecx, dword [intersections]
	mov dword [union], ecx
	
	mov eax, 100
	mul dword [intersections]
	div ecx

	
	
	pop ebp
	ret


