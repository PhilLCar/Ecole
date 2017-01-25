(define ifnum 0)

(define (gensym type)
  (cond
   ((eqv? type "if")
    (inc! ifnum)
    (string-append "false" (number->string ifnum)))
   ((eqv? type "endif")
    (string-append "endif" (number->string ifnum)))))
