;; File: "closure-conv.scm"

;; This file implements the steps of closure-conversion:
;;
;;   - alpha-conversion
;;   - assignment-conversion
;;   - closure-conversion

;;;----------------------------------------------------------------------------
(include "utils.scm")

;; Define some special forms to define macros that are usable
;; in macro definitions.

(define-macro (for-macro-expansion . body)
  (eval `(begin ,@body))
  #f)

(define-macro (for-here-and-macro-expansion . body)
  `(begin
     (for-macro-expansion ,@body)
     ,@body))

;;;----------------------------------------------------------------------------

;; Define the match special form.

(for-here-and-macro-expansion

  (define-macro (match sujet . clauses)

    (define (if-equal? var gab oui non)
      (cond ((and (pair? gab)
                  (eq? (car gab) 'unquote)
                  (pair? (cdr gab))
                  (null? (cddr gab)))
             `(let ((,(cadr gab) ,var))
                ,oui))
            ((null? gab)
             `(if (null? ,var) ,oui ,non))
            ((symbol? gab)
             `(if (eq? ,var ',gab) ,oui ,non))
            ((or (boolean? gab)
                 (char? gab))
             `(if (eq? ,var ,gab) ,oui ,non))
            ((number? gab)
             `(if (eqv? ,var ,gab) ,oui ,non))
            ((pair? gab)
             (let ((carvar (gensym))
                   (cdrvar (gensym)))
               `(if (pair? ,var)
                    (let ((,carvar (car ,var)))
                      ,(if-equal?
                        carvar
                        (car gab)
                        `(let ((,cdrvar (cdr ,var)))
                           ,(if-equal?
                             cdrvar
                             (cdr gab)
                             oui
                             non))
                        non))
                    ,non)))
            (else
             (error "unknown pattern"))))

    (let* ((var
            (gensym))
           (fns
            (map (lambda (x) (gensym))
                 clauses))
           (err
            (gensym)))
      `(let ((,var ,sujet))
         ,@(map (lambda (fn1 fn2 clause)
                  `(define (,fn1)
                     ,(if-equal? var
                                 (car clause)
                                 (if (and (eq? (cadr clause) 'when)
                                          (pair? (cddr clause)))
                                     `(if ,(caddr clause)
                                          ,(cadddr clause)
                                          (,fn2))
                                     (cadr clause))
                                 `(,fn2))))
                fns
                (append (cdr fns) (list err))
                clauses)
         (define (,err) (error "match failed"))
         (,(car fns)))))

  (define gensym ;; a version of gensym that creates easier to read symbols
    (let ((count 0))
      (lambda ()
        (set! count (+ count 1))
        (string->symbol (string-append "g" (number->string count))))))

)

;;;----------------------------------------------------------------------------

;; Alpha-conversion.

(define (alpha-conv expr)
  (alphac expr '()))

(define (alphac expr env)

  (define (ac e)
    (alphac e env))

  (define (rename v)
    (cond ((assq v env) => cdr)
          (else v)))

  (match expr

    (,c when (constant? c)
     expr)

    ((quote ,x)
     expr)

    (,v when (symbol? v)
     (rename v))

    ((set! ,v ,E1)
     `(set! ,(rename v) ,(ac E1)))

    ((define ,v ,body) when (pair? v)
     `(define ( ,(car v) ,@(map rename (if (not (list? (cdr v)))
					   (cons (cdr v) '())
					   (cdr v))
				)) ,(ac body)))
     
    ((define ,v ,E1)
     `(define ,(rename v) ,(ac E1)))

    ((lambda ,params ,E)
     (let* ((fresh-params
             (map (lambda (p) (cons p (gensym)))
                  params))
            (new-env
             (append fresh-params env)))
       `(lambda ,(map cdr fresh-params)
          ,(alphac E new-env))))

    ((let ,bindings ,E)
     (let* ((fresh-vars
             (map (lambda (b) (cons (car b) (gensym)))
                  bindings))
            (new-env
             (append fresh-vars env)))
       `(let ,(map (lambda (v e) `(,(cdr v) ,(ac (cadr e))))
                   fresh-vars
                   bindings)
          ,(alphac E new-env))))

    ((if ,E1 ,E2)
     `(if ,(ac E1) ,(ac E2)))
    ((if ,E1 ,E2 ,E3)
     `(if ,(ac E1) ,(ac E2) ,(ac E3)))

    ((,E0 . ,Es)
     `(,(if (primitive? E0) E0 (ac E0))
       ,@(map ac Es)))

    (,_
     (error "unknown expression" expr))))

;;;----------------------------------------------------------------------------

;; Assignment-conversion.

(define (assign-conv expr)
  (let ((globals (fv expr)))
    (assignc expr (difference (mv expr) globals))))

(define (assignc expr mut-vars)

  (define (ac e)
    (assignc e mut-vars))

  (define (mutable? v)
    (memq v mut-vars))

  (match expr

    (,c when (constant? c)
     expr)

    ((quote ,x)
     expr)

    (,v when (symbol? v)
     (if (mutable? v) `($car ,v) v))

    ((set! ,v ,E1)
     (if (mutable? v)
         `($set-car! ,v ,(ac E1))
         `(set! ,v ,(ac E1))))

    ((define ,v ,E1)
     `(define ,v ,(ac E1)))

    ((lambda ,params ,E)
     (let* ((mut-params
             (map (lambda (p) (cons p (gensym)))
                  (keep mutable? params)))
            (params2
             (map (lambda (p)
                    (if (mutable? p)
                        (cdr (assq p mut-params))
                        p))
                  params)))
       `(lambda ,params2
          ,(if (null? mut-params)
               (ac E)
               `(let ,(map (lambda (x) `(,(car x) ($cons ,(cdr x) '())))
                           mut-params)
                   ,(ac E))))))

    ((let ,bindings ,E)
     (let* ((vars
             (map car bindings))
            (mut-vars
             (map (lambda (v) (cons v (gensym)))
                  (keep mutable? vars)))
            (vars2
             (map (lambda (v)
                    (if (mutable? v)
                        (cdr (assq v mut-vars))
                        v))
                  vars)))
       `(let ,(map (lambda (v e) `(,v ,(ac (cadr e))))
                   vars2
                   bindings)
          ,(if (null? mut-vars)
               (ac E)
               `(let ,(map (lambda (x) `(,(car x) ($cons ,(cdr x) '())))
                           mut-vars)
                   ,(ac E))))))

    ((if ,E1 ,E2)
     `(if ,(ac E1) ,(ac E2)))
    ((if ,E1 ,E2 ,E3)
     `(if ,(ac E1) ,(ac E2) ,(ac E3)))

    ((,E0 . ,Es)
     `(,(if (primitive? E0) E0 (ac E0))
       ,@(map ac Es)))

    (,_
     (error "unknown expression" expr))))

;;;----------------------------------------------------------------------------

;; Closure-conversion.

(define (closure-conv expr)
  (let ((globals (fv expr)))
    (closurec expr '() globals)))

(define (closurec expr cenv globals)

  (define (cc e)
    (closurec e cenv globals))

  (define (pos id)
    (let ((x (memq id cenv)))
      (and x
           (- (length cenv)
              (length x)))))

  (match expr

    (,c when (constant? c)
     expr)

    ((quote ,x)
     expr)

    (,v when (symbol? v)
     (let ((p (pos v)))
       (if p
           `(closure-ref $this ,p)
           v)))

    ((set! ,v ,E1)
     `(set! ,v ,(cc E1)))

    ((define ,v ,E1)
     `(define ,v ,(cc E1)))

    ((lambda ,params ,E)
     (let ((new-cenv (difference (fv expr) globals)))
       `(make-closure
         (lambda ($this ,@params)
           ,(closurec E new-cenv globals))
         ,@(map cc new-cenv))))

    ((let ,bindings ,E)
     `(let ,(map (lambda (b) `(,(car b) ,(cc (cadr b)))) bindings)
        ,(cc E)))

    ((if ,E1 ,E2)
     `(if ,(cc E1) ,(cc E2)))
    ((if ,E1 ,E2 ,E3)
     `(if ,(cc E1) ,(cc E2) ,(cc E3)))
   
    ((begin . ,Es)
     `(begin ,@(map cc Es)))

    ((,E0 . ,Es)
     (if (primitive? E0)
         `(,E0 ,@(map cc Es))
         `(let (($clo ,(cc E0)))
            ((closure-code $clo)
             $clo
             ,@(map cc Es)))))

    (,_
     (error "unknown expression" expr))))

;;;----------------------------------------------------------------------------

;; Free-variable and mutable-variable analyses.

(define (fv expr)
  (match expr

    (,c when (constant? c)
     '())

    ((quote ,x)
     `())

    (,v when (symbol? v)
     (list v))

    ((set! ,v ,E1)
     (union (list v) (fv E1)))

    ((define ,v ,E1)
     (union (list v) (fv E1)))

    ((lambda ,params ,E)
     (difference (fv E) params))

    ((let ,bindings ,E)
     (union (apply union (map (lambda (b) (fv (cadr b))) bindings))
            (difference (fv E) (map car bindings))))

    ((if ,E1 ,E2)
     (union (fv E1) (fv E2)))
    ((if ,E1 ,E2 ,E3)
     (union (fv E1) (fv E2) (fv E3)))

    ((,E0 . ,Es)
     (union (if (primitive? E0)
                '()
                (fv E0))
            (apply union (map fv Es))))

    (,_
     (error "unknown expression" expr))))

(define (mv expr)
  (match expr

    (,c when (constant? c)
     '())

    ((quote ,x)
     `())

    (,v when (symbol? v)
     '())

    ((set! ,v ,E1)
     (union (list v) (mv E1)))

    ((define ,v ,E1)
     (union (list v) (mv E1)))

    ((lambda ,params ,E)
     (mv E))

    ((let ,bindings ,E)
     (union (apply union (map (lambda (b) (mv (cadr b))) bindings))
            (mv E)))

    ((if ,E1 ,E2)
     (union (mv E1) (mv E2)))
    ((if ,E1 ,E2 ,E3)
     (union (mv E1) (mv E2) (mv E3)))

    ((,E0 . ,Es)
     (union (if (primitive? E0) '() (mv E0))
            (apply union (map mv Es))))

    (,_
     (error "unknown expression" expr))))

;;;----------------------------------------------------------------------------

;; Set operations.

(define (union . ss)
  (let loop ((lst ss) (result '()))
    (if (null? lst)
        result
        (loop (cdr lst)
              (union2 result (car lst))))))

(define (union2 s1 s2)
  (cond ((null? s1)
         s2)
        ((member (car s1) s2)
         (union2 (cdr s1) s2))
        (else
         (cons (car s1)
               (union2 (cdr s1) s2)))))

(define (intersect s1 s2)
  (cond ((null? s1)
         '())
        ((member (car s1) s2)
         (cons (car s1)
               (intersect (cdr s1) s2)))
        (else
         (intersect (cdr s1) s2))))

(define (difference s1 s2)
  (cond ((null? s1)
         '())
        ((member (car s1) s2)
         (difference (cdr s1) s2))
        (else
         (cons (car s1)
               (difference (cdr s1) s2)))))

(define (set-equal? s1 s2)
  (and (null? (difference s1 s2))
       (null? (difference s2 s1))))

(define (keep f lst)
  (cond ((null? lst)   '())
        ((f (car lst)) (cons (car lst) (keep f (cdr lst))))
        (else          (keep f (cdr lst)))))

;;;----------------------------------------------------------------------------
