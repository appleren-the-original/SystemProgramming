segment .data
; To use For loops
i	dd 0
j	dd 0
z	dd 0
;###
intersections	dd 0
union		dd 0
segment .bss
;For taking function Parameters.
st_1		resd 1
st_2		resd 1
size1  	resd 1
size2  	resd 1
n 	  	resd 1
;##########
;For loops limit for st_1 and st_2
lim1  	resd 1
lim2 	resd 1
;###########

segment .text
global n_gram



n_gram:
	push ebp
	mov ebp, esp
	push ebx; ebx should not change
	
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
	
	
	
	mov dword [intersections], 0
;for (i = 0; i < lim1; i++ ); //lim1 = size1 - n + 1
	mov dword [i], 0
FOR1:
	mov ecx, dword [i]
	cmp ecx, dword [lim1]
	je son ; if i == lim1 return;
	inc ecx
	mov dword [i], ecx
; for (j = 0; j < lim2; j++);
	mov dword [j], 0
FOR2:
	mov ecx, dword [j]
	cmp ecx, dword [lim2]
	je FOR1 ; if j == lim2 you should go upper for.
	inc ecx
	mov dword [j], ecx
;for (z = 0; z < n; z++);
	mov dword [z], 0
	
FOR3:
	mov ecx, dword [z]
	cmp ecx, dword [n]
	; if z == n then we have equal substrings so we should break this for to not get 
	; more than one intersections for the same substring. mar, marmar should return 1.
	je buldu 
	
	mov ecx, eax ; ecx = &st1[0]
	add ecx, dword [i] ; ecx = &st1[0 + i]
	add ecx, dword [z] ; ecx = &st1[0 + i + z]
	sub ecx, 1		 ; ecx = &st1[0 + i + z - 1] // -1 because I increased i before this.
	mov dl, byte [ecx] ; dl = st1[i + z -1] // ecx has address but dl has real char.
	
	; Same Operation for st2 as above.
	mov ecx, ebx
	add ecx, dword [j]
	add ecx, dword [z]
	sub ecx, 1
	mov dh, byte [ecx]
	
	; IF both char are equal you can continue to look.
	cmp dl, dh
	jne FOR2 ; If they are not equal break this loop since there is no meaning to continue from there.
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

	
	
	pop ebx
	pop ebp
	ret


