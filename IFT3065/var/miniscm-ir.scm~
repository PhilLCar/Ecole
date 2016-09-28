#! /usr/bin/env gsi

;;; File: "miniscm-ir.scm"

;; To compile the file "file.scm" to "file.ir" do:
;;
;;   ./miniscm-ir file.scm
;;
;; Before doing this, do a "chmod +x miniscm-ir.scm".

;;; Source language syntax:

;; <expr> ::=  <number>
;;         |   <var>
;;         |   (let ((<var> <expr>)) <expr>)
;;         |   (+ <expr> <expr>)
;;         |   (- <expr> <expr>)
;;         |   (* <expr> <expr>)
;;         |   (/ <expr> <expr>)
;;         |   (println <expr>)

;;; IR instructions:

;; #(label <string>)           label with name = <string>
;; #(push-constant <number>)   push <number> on stack
;; #(push-local <n>)           push stack slot <n> from top (0 is top of stack)
;; #(add)                      pop values a and b from stack and push a+b
;; #(sub)                      pop values a and b from stack and push a-b
;; #(mul)                      pop values a and b from stack and push a*b
;; #(div)                      pop values a and b from stack and push a/b
;; #(println)                  print value at top of stack
;; #(xchg)                     pop values a and b from stack and push a then b
;; #(pop)                      pop a value from stack and discard it
;; #(return)                   pop a value from stack (the result) and pop
;;                             the return address and jump to it

;;; Parsing

(define (parse source-filename)
  (with-input-from-file
      source-filename
    (lambda ()
      (read))))

;;; Translation of AST to IR

(define (translate-to-ir ast)
  (flatten (comp-function "main" ast)))

(define (comp-function name expr)
  (list
   (vector 'label name)
   (comp-expr expr 0 '((argc . 1)))
   (vector 'return)))

(define (comp-expr expr fs cte) ;; fs = frame size
                                ;; cte = compile time environment

  (cond
        ;; handle this form: 123
        ((number? expr)
         (vector 'push-constant expr))

        ;; handle this form: var
        ((symbol? expr)
         (let ((x (assoc expr cte)))
           (if x
               (let ((index (cdr x)))
                 (vector 'push-local (+ fs index)))
               (error "undefined variable" expr))))

        ;; handle this form: (let ((var expr)) body)
        ((and (list? expr)
              (= (length expr) 3)
              (eq? (list-ref expr 0) 'let))
         (let ((binding (list-ref (list-ref expr 1) 0)))
           (list
            (comp-expr (list-ref binding 1)
                       fs
                       cte)
            (comp-expr (list-ref expr 2)
                       (+ fs 1)
                       (cons (cons (list-ref binding 0)
                                   (- (+ fs 1)))
                             cte))
            (vector 'xchg)
            (vector 'pop))))

        ;; handle these forms:
        ;;   (+ expr1 expr2)
        ;;   (- expr1 expr2)
        ;;   (* expr1 expr2)
        ;;   (/ expr1 expr2)
        ((and (list? expr)
              (= (length expr) 3)
              (member (list-ref expr 0) '(+ - * /)))
         (list
          (comp-expr (list-ref expr 2) fs cte)
          (comp-expr (list-ref expr 1) (+ fs 1) cte)
          (case (list-ref expr 0)
            ((+) (vector 'add))
            ((-) (vector 'sub))
            ((*) (vector 'mul))
            ((/) (vector 'div)))))

        ;; handle these forms:
        ;;   (println expr)
        ((and (list? expr)
              (= (length expr) 2)
              (member (list-ref expr 0) '(println)))
         (list
          (comp-expr (list-ref expr 1) fs cte)
          (case (list-ref expr 0)
            ((println) (vector 'println)))))

        (else
         (error "comp-expr cannot handle expression"))))

;; Utility:

(define (flatten lst)
  (append-flatten lst '()))

(define (append-flatten lst rest)
  (cond ((null? lst)
         rest)
        ((pair? lst)
         (append-flatten (car lst)
                         (append-flatten (cdr lst)
                                         rest)))
        (else
         (cons lst rest))))

;; Main program:

(define (main source-filename)
  (let ((ast (parse source-filename)))
    (let ((ir (translate-to-ir ast)))
      (with-output-to-file
          (string-append (path-strip-extension source-filename) ".ir")
        (lambda ()
          (pretty-print ir))))))
