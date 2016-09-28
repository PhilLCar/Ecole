char *text;
char *target;
char 

char* addSegment(char *segment) {
  
}

void compress(char* text) {
  char *segment, c;
  int word = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\t' || c == '\n') {
      if (word) {
	segment = addSegment(segment);
      } else continue;
    } else addChar(c, segment);
  }
  addSegment(segment);
}

void expand(char* text) {
  
}

/*
  Le but est de remplacer tous les mots par un index d'un des deux tableaux, la forme est la suivante

  FLAG FORM:
  000000
  ^^^^^^
  |||||S TERMINATED
  ||||ALL CAPS
  |||CAPS
  ||TABLE TYPE
  |TABLE
  COMPRESSION SWITCH

  LONG WORD:  (3 bytes)   // When length > 2
  TYPE  FLAGS     POSITION
  00    FFFFFF    IIII IIII IIII IIII

  SHORT WORD: (2 byte)    // When length == 2
  TYPE  FLAGS     POSITION
  01    FFFFFF    IIII IIII

  LETTER:     (1 byte)   // When length < 2 && c >= 0x40
  TYPE  CAPS  LETTER
  10    C     LLLLL

  SYMBOL:     (1 byte)   // When length < 2 && c < 0x40
  TYPE  CAPS  SYMBOL
  11    C     SSSSS

  SEGMENT:    (~ bytes)  // When impossible to compress
  001X XXXX .... 001X XXXX
 */
