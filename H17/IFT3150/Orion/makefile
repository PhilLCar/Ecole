# Makefile pour CH.  Rien de bien compliqué ici.

# Attention, dans les fichiers Make, les TAB ont une signification très
# spécifique.  Ne pas remplacer des TABs par des espaces (ou l'inverse).

CFLAGS=-Wall

CC=cc
LD=cc
RM=rm -f
PDFLATEX=pdflatex

# Point d'entrée principal
all: rapport.pdf #tp-shell.pdf

clean:
	$(RM) *.o ch *.aux *.log *.pdf

.SUFFIXES: .tex .pdf

# Règle simpliste pour générer le PDF à partir du source LaTeX.
.tex.pdf:
	$(PDFLATEX) $<

