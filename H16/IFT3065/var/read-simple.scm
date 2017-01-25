;; File: read-simple.scm

(define (read)
  (let ((c (peek-char-non-whitespace)))
    (cond ((eof-object? c)
           c)
          ((char=? c #\()
           (read-char) ;; skip "("
           (read-list))
          (else
           (read-char) ;; skip first char
           (let ((s (list->string (cons c (read-symbol)))))
             (or (string->number s)
                 (string->symbol s)))))))

(define (read-list)
  (let ((c (peek-char-non-whitespace)))
    (if (char=? c #\))
        (begin
          (read-char) ;; skip ")"
          '())
        (let ((first (read)))
          (let ((rest (read-list)))
            (cons first rest))))))

(define (read-symbol)
  (let ((c (peek-char)))
    (if (or (eof-object? c)
            (char=? c #\()
            (char=? c #\))
            (char<=? c #\space))
        '()
        (begin
          (read-char)
          (cons c (read-symbol))))))

(define (peek-char-non-whitespace)
  (let ((c (peek-char)))
    (if (or (eof-object? c)
            (char>? c #\space))
        c
        (begin
          (read-char)
          (peek-char-non-whitespace)))))

;; try it...

(pp (read))
