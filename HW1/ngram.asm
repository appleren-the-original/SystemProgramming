segment .data
; To use For loops
i	dd 0
j	dd 0
z	dd 0
dummy dd 0
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
	
	
	
; Finding Union
	mov ecx, dword [size1]
	add ecx, dword [size2]
	sub ecx, dword [n]
	sub ecx, dword [n]
	add ecx, 2
	mov dword [union], ecx
	mov dword [i], 0
aux2:
	mov ecx, dword [lim2]
	cmp ecx, dword [i]
	je aux2_end
	mov dword [j], 0
union1:
	mov ecx, dword [j]
	cmp ecx, dword [i]
	je st2_uniq
	mov dword [z], 0
union2:
	mov ecx, dword [z]
	cmp ecx, dword [n]
	je st2_same_before
	
	mov ecx, ebx ; ecx = &st1[0]
	add ecx, dword [i] ; ecx = &st1[0 + i]
	add ecx, dword [z] ; ecx = &st1[0 + i + z]; ecx = &st1[0 + i + z - 1] // -1 because I increased i before this.
	mov dl, byte [ecx] ; dl = st1[i + z -1] // ecx has address but dl has real char.
	
	; Same Operation but starting from index j.
	mov ecx, ebx
	add ecx, dword [j]
	add ecx, dword [z]
	mov dh, byte [ecx]
	cmp dl, dh
	je st2_ayni
	jne st2_farkli
st2_ayni:
	add dword [z], 1
	jmp union2
st2_farkli:
	add dword [j], 1
	jmp union1
	
st2_same_before:
	sub dword [union], 1
	add dword [i], 1
	jmp aux2

st2_uniq:
	add dword [i], 1
	jmp aux2

aux2_end:
;

	
;for (i = 0; i < lim1; i++ ); //lim1 = size1 - n + 1
	mov dword [intersections], 0
	mov dword [i], 0
FOR1:
	mov ecx, dword [i]
	cmp ecx, dword [lim1]
	je son ; if i == lim1 return;
	jmp aux1
uniq:
	add dword [i], 1
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
	; if z == n then we have equal substrings
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
	sub dword [union], 1
	jmp FOR1


; Look if there is same substr before me.
aux1:
	mov dword [j], 0
inter1:
	mov ecx, dword [i]
	cmp ecx, dword [j]
	je uniq	
	mov dword [z], 0
inter2:
	mov ecx, dword [z]
	cmp ecx, dword [n]
	je same_before
	
	mov ecx, eax ; ecx = &st1[0]
	add ecx, dword [i] ; ecx = &st1[0 + i]
	add ecx, dword [z] ; ecx = &st1[0 + i + z]; ecx = &st1[0 + i + z - 1] // -1 because I increased i before this.
	mov dl, byte [ecx] ; dl = st1[i + z -1] // ecx has address but dl has real char.
	
	; Same Operation but starting from index j.
	mov ecx, eax
	add ecx, dword [j]
	add ecx, dword [z]
	mov dh, byte [ecx]
	cmp dl, dh
	je ayni
	jne farkli
ayni:
	add dword [z], 1
	jmp inter2
farkli:
	add dword [j], 1
	jmp inter1
	

; Then we have same substr before us so do not look for this index.
same_before:
	add dword [i], 1
	sub dword [union], 1
	jmp FOR1
	

son:
	mov eax, 100
	mul dword [intersections]
	div dword [union]
	pop ebx
	pop ebp
	ret


