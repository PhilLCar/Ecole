# To compile and execute this program enter the shell commands:
#
#    gcc -o test stdio.s test.s
#    ./test

        .text
        .globl _main
        .globl main
_main:
main:

        push    $'h'
        call    putchar

        push    $'e'
        call    putchar

        push    $'l'
        call    putchar

        push    $'l'
        call    putchar

        push    $'o'
        call    putchar

        lea     world(%rip), %rax
        push    %rax
        call    print_string

        push    $-1000001
        call    print_word_dec

        push    $0
        call    print_word_dec

        lea     newline(%rip), %rax
        push    %rax
        call    print_string

	call    print_regs

        mov     $0, %rax
        ret

world:   .ascii " world!"
newline: .asciz "\n"
