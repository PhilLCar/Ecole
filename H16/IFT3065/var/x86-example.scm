;; File: x86-example.scm

;; This is an example of dynamic code generation for the x86 architecture.

;; To try this example, run it with the Gambit compiler from the shell with
;; this command:
;;
;;   % gsc -i x86-example.scm

;;-----------------------------------------------------------------------------

;; Some functions for generating and executing machine code.

(include "~~lib/_asm#.scm")
(include "~~lib/_x86#.scm")
(include "~~lib/_codegen#.scm")

;; The function u8vector->procedure converts a u8vector containing a
;; sequence of bytes into a Scheme procedure that can be called.
;; The code in the u8vector must obey the C calling conventions of
;; the host architecture.

(define (u8vector->procedure code)
  (machine-code-block->procedure
   (u8vector->machine-code-block code)))

(define (u8vector->machine-code-block code)
  (let* ((len (u8vector-length code))
         (mcb (##make-machine-code-block len)))
    (let loop ((i (fx- len 1)))
      (if (fx>= i 0)
          (begin
            (##machine-code-block-set! mcb i (u8vector-ref code i))
            (loop (fx- i 1)))
          mcb))))

(define (machine-code-block->procedure mcb)
  (lambda (#!optional (arg1 0) (arg2 0) (arg3 0))
    (##machine-code-block-exec mcb arg1 arg2 arg3)))

(define (create-procedure arch gen #!optional (show-listing? #t))
  (let* ((cgc (make-codegen-context))
         (endianness 'le))

    (asm-init-code-block cgc 0 endianness)
    (codegen-context-listing-format-set! cgc 'nasm)
    (x86-arch-set! cgc arch)

    (gen cgc)

    (let ((code (asm-assemble-to-u8vector cgc)))
      (if show-listing?
          (asm-display-listing cgc (current-output-port) #t))
      (u8vector->procedure code))))

;;-----------------------------------------------------------------------------

;; The function auto-detect-arch automatically detects the host architecture
;; (provided it is either an ARM, an X86-32 or X86-64 architecture).
;; It executes a fixed piece of code which represents different machine
;; instructions depending on the architecture executing the code.

(define (auto-detect-arch)

  (define auto-detect-arch-code '#u8(
                    ;;       ARM              X86-32            X86-64
                    ;;
#xEB #x0A #xA0 #xE3 ;;      mov r0,#962560    jmp x86           jmp x86
#x03 #x00 #xE0 #xE3 ;;      mov r0,#-4        ...               ...
#x1E #xFF #x2F #xE1 ;;      bx  lr            ...               ...
                    ;; x86:
#x48                ;;                        dec eax           xor rax,rax
#x31 #xC0           ;;                        xor eax,eax
#x48                ;;                        dec eax           inc rax
#xFF #xC0           ;;                        inc eax
#xC1 #xE0 #x02      ;;                        shl eax,0x2       shl eax,0x2
#xC3                ;;                        ret               ret
                    ;;
                    ;;      returns -4        returns 0         returns 4
))

  (case ((u8vector->procedure auto-detect-arch-code))
    ((-1) 'arm)
    ((0)  'x86-32)
    ((1)  'x86-64)
    (else #f)))

(pp (list 'arch= (auto-detect-arch)))

;;-----------------------------------------------------------------------------

;; Tests.

(println "foo")

(case (auto-detect-arch)

  ((x86-32)

   (let ()

     ;; Test the X86-32 assembler.

     (define f
       (create-procedure
        'x86-32
        (lambda (cgc)
          (x86-mov  cgc (x86-eax) (x86-mem 4 (x86-esp))) ;; [esp+4] = arg1 (on x86-32)
          (x86-imul cgc (x86-eax) (x86-eax) (x86-imm-int 7))
          (x86-ret  cgc))))

     (define g
       (u8vector->procedure
        '#u8(
             #x8b #x44 #x24 #x04 ;;     mov     eax,[esp+4]
             #x6b #xc0 #x0a      ;;     imul    eax,eax,byte 10
             #xc3                ;;     ret
             )))

     (pp (f 10))
     (pp (g 10))

     ;; Output :
     ;;
     ;; 000000               bits 32
     ;; 000000 8b442404      mov     eax,[esp+4]
     ;; 000004 6bc007        imul    eax,eax,byte 7
     ;; 000007 c3            ret
     ;; 70
     ;; 100
     ))

  ((x86-64)

   (let ()

     ;; Test the X86-64 assembler.

     (define f
       (create-procedure
        'x86-64
        (lambda (cgc)
          (x86-mov  cgc (x86-mem 23 (x86-bpl)) (x86-cl)) ;; rdi = arg1 (on x86-64)
          (x86-imul cgc (x86-rax) (x86-rax) (x86-imm-int 7))
          (x86-ret  cgc))))

     (define g
       (u8vector->procedure
        '#u8(
             #x48 #x89 #xf8      ;;     mov     rax,rdi
             #x48 #x6b #xc0 #x0a ;;     imul    rax,rax,byte 10
             #xc3                ;;     ret
             )))

     (pp (f 10))
     (pp (g 10))

     ;; Output :
     ;;
     ;; 000000               bits 64
     ;; 000000 4889f8        mov     rax,rdi
     ;; 000003 486bc007      imul    rax,rax,byte 7
     ;; 000007 c3            ret
     ;; 70
     ;; 100
     )))

;;-----------------------------------------------------------------------------
