# File: exio.s
#
# Contient des fonctions standard pour l'I/O de data scheme

	.text

	.globl print_scm
	.globl print_ret

#print ret imprime le retour de chariot
print_ret:
	push	%rax
	push	$'\n'
	call	putchar
	pop	%rax
	ret

#print scm choisi quelle fonction utiliser en fonction du type

print_scm:
	pushf
	push	%rax
	mov	24(%rsp), %rax
	and	$7, %rax
	jz	print_scmint
	cmp	$1, %rax
	jz	print_scmpound
	cmp	$2, %rax
	jz	print_scmchar
	cmp	$3, %rax
	jz	print_scmstring
	cmp	$4, %rax
	jz	print_scmvector
	cmp	$5, %rax
	jz	print_scmstring
	cmp	$6, %rax
	jz	print_scmpair
	cmp	$7, %rax
	jz	print_scmproc
	jmp	print_void

# print_scmint fonction envoie une représentation décimale d'un entier scheme.
# (les trois bits de gauche servent à identifier le type)

print_scmint:

	mov	24(%rsp), %rax
	sar	$3, %rax
	push	%rax
	call	print_word_dec

	pop     %rax
	popf
	ret     $8	#Return and pop parameter


# print_scmbool fonction qui envoie #t, ou #f à stdout
# Un mot contenant la valeur 1 est true, la valeur 9 est false

print_scmpound:

	push	$'#'
	call	putchar
	mov	24(%rsp), %rax		#Gets the bool value
	cmp	$1, %rax		#The literal value 1 is false
	je	print_scmbool_false
################################################
# Check for special values here!
################################################
	cmp	$17, %rax
	jz	print_void
	cmp	$33, %rax
	je	print_eof
	
print_scmbool_true:
	push 	$'t'
	call 	putchar
	jmp 	print_scmbool_end
	
print_scmbool_false:
	push	 $'f'
	call 	putchar
	
print_scmbool_end:
        pop     %rax
        popf
        ret     $8    #Return and pop parameter

# print_scmchar fonction qui imprime le caractère précédé de '#\'

print_scmchar:

	mov	24(%rsp), %rax
	sar	$3, %rax
	push	%rax
#	push	$92
#	push	$'#'
#	call	putchar
#	call	putchar
	call	putchar
        pop     %rax
        popf
        ret     $8    #Return and pop parameter

# print_scmstring permet d'imprimer un string situé dans le heap

print_scmstring:

	mov	24(%rsp), %rax
	sar	$3, %rax
	add	$8, %rax
	push	%rax
	call	print_string
	pop	%rax
	popf
	ret	$8

# imprime les données d'un vecteur

print_scmvector:
	
	push	%rbx
	mov	32(%rsp), %rax
	sar	$3, %rax
	mov	(%rax), %rbx
	sar	$3, %rbx
print_vector_loop:
	cmp	$0, %rbx
	jz	end_vector_loop
	add	$8, %rax
	push	(%rax)
	call	print_scm
	dec	%rbx
	jmp	print_vector_loop
end_vector_loop:
	pop	%rbx
	pop	%rax
	popf
	ret	$8

#print_scmpair permet d'imprimer une paire située dans le heap

print_scmpair:
	
	mov	24(%rsp), %rax
	jmp	print_car

print_pairend:
	pop	%rax
	popf
	ret	$8

print_car:
	cmp	$6, %rax
	je	print_pairend
	sar	$3, %rax
	push	(%rax)
	call	print_scm

print_cdr:
	add	$8, %rax
	push	(%rax)
	call	print_scm
	jmp	print_pairend

# impression de procédure
// OLD doit être utilisé quand la closure-conversion n'est pas appliquée

print_scmproc_OLD:
	lea	string_proc(%rip), %rax
	push	%rax
	call	print_string
	mov	24(%rsp), %rax
	push	2(%rax)
	call	print_word_dec
	push	$'>'
	call	putchar
	pop	%rax
	popf
	ret	$8

print_scmproc:
	lea	string_proc(%rip), %rax
	push	%rax
	call	print_string
	mov	24(%rsp), %rax
	sar	$3, %rax
	mov	(%rax), %rax
	push	2(%rax)
	call	print_word_dec
	push	$'>'
	call	putchar
	pop	%rax
	popf
	ret	$8
	

# print_void imprime le résultat #!void

print_void:

	lea	string_void(%rip), %rax
	push	%rax
	call	print_string
	pop	%rax
	popf
	ret	$8

print_eof:
	
	lea	string_eof(%rip), %rax
	push	%rax
	call	print_string
	pop	%rax
	popf
	ret	$8


############################################################################
.data

string_void:
	.asciz "!void"
string_eof:	
	.asciz "!eof"
string_proc:
	.asciz "#<procedure #"
	
