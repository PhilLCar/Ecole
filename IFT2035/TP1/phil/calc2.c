#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCTURES
typedef struct Digits {
  int            digit;
  struct Digits *next;
} digits;

typedef struct {
  int     flags; // compteur de référence : flags >> 1
                 // flag de négativité    : flags &  1
  digits *value;
} bigint;

struct Stack {
  bigint **ptr;
  int      len;
  int      cap;
} stack = { NULL, 0, 0 };

bigint *variables[52];
int instr = 1;

// FONCTIONS
void     alert(char*);
void     print(bigint);
void     freeb(bigint*);
void     emptystack();
bigint **stackat(int);
void     shrink();
void     push();
bigint  *pop();
void     swap();
int      var(int);
void     emptyvars();
void     newvar(int);
void     ref();
void     add();
void     sub();
void     mul();
void     parse();
void     parsenumber(int);
void     parseoperator(int);
void     parsevariable(int);

//////////////////// IMPRESSION ////////////////////
/* ALERT
 *   Affiche un message d'erreur avant de rétablir la pile
 */
void alert(char* message)
{
  while(getchar() != '\n'); // vide le buffer
  ungetc('\n', stdin);
  printf(message);
  printf("\n");
  emptystack();
  // Averti l'utilisateur de la possibilité de libérer de la mémoire (juste une fois)
  if (instr) {
    printf("L'opération en cours a du être annulée mais les variables ont été conservées.\n");
    printf("Si vous voulez libérer plus de mémoire faites «free» pour libérer les variables.\n");
    instr = 0;
  }
}

void reverse(bigint *b)
{
  digits *i = b->value;
  digits *a = NULL, *p;
  do {
    p = i->next;
    i->next = a;
    a = i;
    i = p;
  } while (i != NULL);
  b->value = a;
}

/* PRINT
 *   Inverse les pointeurs mettre le nombre à l'endroit et les inverses à nouveau au moment de 
 * l'impression.
 */
void print(bigint b)
{
  digits *i = b.value;
  digits *a = NULL, *p;
  if (b.flags & 1) printf("-");
  do {
    p = i->next;
    i->next = a;
    a = i;
    i = p;
  } while (i != NULL);
  i = a;
  a = NULL;
  do {
    printf("%d", i->digit);
    p = i->next;
    i->next = a;
    a = i;
    i = p;
  } while (i != NULL);
}

//////////////////// MÉMOIRE ////////////////////
/* FREEB
 *   Libère la mémoire d'un nombre si celui-ci n'est pas référencé
 */
void freeb(bigint *b) {
  if (b->flags >> 1) return;
  digits *next, *val = b->value;
  free(b);
  while (val != NULL) {
    next = val->next;
    free(val);
    val = next;
  }
}

/* EMPTYSTACK
 *   Vide le stack en laissant la dernière valeur dessus
 */
void emptystack() {
  bigint *b;
  while (stack.len > 1) {
    b = pop();
    freeb(b);
  }
}

/* STACKAT
 *   Retourne le nombre à la position spécifiée en paramètre, 
 */
bigint **stackat(int index) {
  return (stack.ptr + stack.len - 1 - index);
}

/* SHRINK
 *   Conserve le nombre du dessus et libère celui d'en dessous
 */
void shrink() {
  // Dans le cas de ANS
  if (stack.len < 2) {
    printf("(ANS) ");
    return;
  }
  // Ne devrait pas arriver
  if (stack.len > 2) {
    alert("Il semble y avoir des problème avec la pile, le précédent résultat est peut-être erroné");
    return;
  }
  // Cas normal
  freeb(*stack.ptr);
  *stack.ptr = *(stack.ptr + 1);
  stack.len = 1;
}

/* PUSH
 *   Empile un nouveau nombre
 */
void push(bigint *elem)
{
  bigint **tmp;
  stack.len++;
  if (stack.len > stack.cap) {
    stack.cap = stack.len;
    tmp = realloc(stack.ptr, stack.cap * sizeof(bigint*));
    if (tmp == NULL) {
      alert("Mémoire insuffisante, opération annulée");
      return;
    }
    stack.ptr = tmp;
  }
  *stackat(0) = elem;
}

/* POP
 *   Dépile un nombre et retourne un pointeur vers celui-ci
 */
bigint *pop()
{
  bigint *ret = *stackat(0);
  stack.len--;
  if (stack.len > 0) {
    bigint **tmp = realloc(stack.ptr, stack.len * sizeof(bigint*));
    if (tmp != NULL) {
      stack.cap--;
      stack.ptr = tmp;
    } else {
      // Si la réallocation échoue, le programme va continuer de fonctionner mais il est exposé à des fuites de mémoire
      printf("\nIl semble y avoir des problèmes avec la libération de mémoire, terminez vos opérations et redémarrez\n");
    }
  }
  return ret;
}

/* SWAP
 *   Échange les deux nombres au sommet de la pile
 */
void swap()
{
  bigint *tmp;
  tmp = *stackat(0);
  *stackat(0) = *stackat(1);
  *stackat(1) = tmp;
}

/* VAR
 *   Obtient l'index du tableau associé à une lettre
 */
int var(int c) {
  if (c > 0x5A)
    c -= 0x61;
  else c -= 0x41;
  if (c < 0 || c > 51) {
    alert("Nom de variable invalide!");
    return -1;
  }
  return c;
}

/* EMPTYVARS
 *   Vide le tableau des variables
 */
void emptyvars() {
  int i;
  for (i = 0; i < 52; i++)
    if (variables[i] != NULL) {
      if ((variables[i]->flags -= 2) < 2 && *stack.ptr != variables[i]) 
	freeb(variables[i]);
      variables[i] = NULL;
    }
}

/* NEWVAR
 *   Stocke la valeur au sommet de la pile dans une variable en s'assurant de disposer de la valeur
 * précédente.
 */
void newvar(int c) {
  if ((c = var(c)) == -1) return;
  if (variables[c] != NULL)
    if ((variables[c]->flags -= 2) < 2)
      freeb(variables[c]);
  variables[c] = *stackat(0);
  variables[c]->flags += 2; // laisse le marqueur de négativité intact
}

//////////////////// CALCULATRICE ////////////////////
/* REF
 *   Remplace le nombre au sommet de la pile par le nombre de références à ce nombre.
 */
void ref() {
  int     n   = pop()->flags >> 1;
  bigint *big = malloc(sizeof(bigint));
  digits *dig = malloc(sizeof(digits));
  if (big == NULL) {
    alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
    return;
  } else if (dig == NULL) {
    alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
    freeb(big);
    return;
  }
  big->flags = 0;
  big->value = dig;
  goto reference;
  while ((n = n / 10) > 0) {
    dig->next = malloc(sizeof(digits));
    if (dig == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(big);
      return;
    }
    else
      dig = dig->next;
  reference:
    dig->digit = n % 10;
  }
  dig->next = NULL;
  push(big);
}

/* ADD
 *   Ajoute les deux nombre au sommet de la pile et empile le résultat
 */
void add()
{
  if (stack.len < 2) {
    alert ("Format de l'opération invalide, veuillez réessayer.");
    return;
  }
  int ret;
  // Gère les cas négatifs et reroute la fonction dans la bonne direction
  if ((ret = ((*stackat(0))->flags & 1)) != ((*stackat(1))->flags & 1)) {
    if (!ret) {
      (*stackat(1))->flags &= ~1;
      swap();
    } else
      (*stackat(0))->flags &= ~1;
    sub();
    return;
  }
  bigint *n1 = pop();
  bigint *n2 = *stackat(0);
  digits *v1 = n1->value;
  digits *v2 = n2->value;
  
  bigint *r   = malloc(sizeof(bigint));
  digits *res = malloc(sizeof(digits));
  // Sentinelle du dernier chiffre non-nul
  digits *zero = res;

  if (r == NULL) {
    alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
    return;
  } else if (res == NULL) {
    alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
    freeb(r);
    return;
  }
  
  int sum;

  r->value = res;
  r->flags = n1->flags & n2->flags & 1;
  ret = 0;

  // Construction du nombre résultat avec l'algorithme classique d'addition
  goto adding;
  
  while (v1 != NULL && v2 != NULL) {   
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
  adding:
    res->digit = (sum = v1->digit + v2->digit + ret) % 10;
    ret = sum / 10;
    if (res->digit) zero = res;
    v1 = v1->next;
    v2 = v2->next;
  }
  while (v1 != NULL) {
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
    res->digit = (sum = v1->digit + ret) % 10;
    ret = sum / 10;
    if (res->digit) zero = res;
    v1 = v1->next;
  }
  while (v2 != NULL) {
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
    res->digit = (sum = v2->digit + ret) % 10;
    ret = sum / 10;
    if (res->digit) zero = res;
    v2 = v2->next;
  }
  if (ret) {
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
    res->digit = 1;
    zero = res;
  }
  res->next = NULL;
  
  freeb(n1);
  freeb(n2);

  // Si le nombre diminue de taille, enlève les zéros en trop;
  if (zero->next != NULL) {
    res = zero->next;
    zero->next = NULL;
    while (res->next != NULL) {
      zero = res->next;
      free(res);
      res = zero;
    }
  }
  
  *stackat(0) = r;
}

/* SUB
 *   Soustrait le nombre au sommet de la pile par celui juste avant et empile le résultat
 * De telle sorte que (infixe) A - B = (postfixe) A B -
 */
void sub()
{
  if (stack.len < 2) {
    alert ("Format de l'opération invalide, veuillez réessayer.");
    return;
  }
  // Gère les cas négatifs et redirige l'exécution dans la bonne direction
  if ((*stackat(0))->flags & 1) {
    if ((*stackat(1))->flags & 1) {
      (*stackat(0))->flags &= ~1;
      (*stackat(1))->flags &= ~1;
      swap();
    } else {
      (*stackat(0))->flags &= ~1;
      add();
      return;
    }
  } else if ((*stackat(1))->flags & 1) {
    (*stackat(0))->flags |= 1;
    add();
    return;
  }
  bigint *n1 = pop();
  bigint *n2 = *stackat(0);
  digits *v1 = n1->value;
  digits *v2 = n2->value;
  
  bigint *r   = malloc(sizeof(bigint));
  digits *res = malloc(sizeof(digits));
  // Sentinelle du dernier chiffre non-nul
  digits *zero = res;
  
  int ret = 0, diff;

  if (r == NULL) {
    alert("Mémoire insuffisante, essayez de fractionner vos opérations");
    return;
  } else if (res == NULL) {
    alert("Mémoire insuffisante, essayez de fractionner vos opérations");
    freeb(r);
    return;
  }
  
  r->value = res;

  // Création du nombre résultat
  goto substract;
  
  while (v1 != NULL && v2 != NULL) {   
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
  substract:
    if ((diff = v2->digit - v1->digit - ret) < 0) {
      res->digit = diff + 10;
      ret = 1;
    } else {
      res->digit = diff;
      ret = 0;
    }
    if (res->digit) zero = res;
    v1 = v1->next;
    v2 = v2->next;
  }
  while (v1 != NULL) {
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante! Essayez de fractionner vos opérations.");
      freeb(r);
      return;
    } else res = res->next;
    if ((diff = -v1->digit - ret) < 0) {
      res->digit = diff + 10;
      ret = 1;
    } else {
      res->digit = diff;
      ret = 0;
    }
    if (res->digit) zero = res;
    v1 = v1->next;
  }
  while (v2 != NULL) {
    res->next = malloc(sizeof(digits));
    if (res->next == NULL) {
      alert("Mémoire insuffisante, essayez de fractionner vos opérations");
      freeb(r);
      return;
    } else res = res->next;
    if ((diff = v2->digit - ret) < 0) {
      res->digit = diff + 10;
      ret = 1;
    } else {
      res->digit = diff;
      ret = 0;
    }
    if (res->digit) zero = res;
    v2 = v2->next;
  }
  res->next = NULL;

  // Libère la mémoire occupée par les opérandes
  freeb(n1);
  freeb(n2);

  // Si le nombre est négatif, inverse les digits
  if ((r->flags = ret)) {
    res = r->value;
    res->digit = 10 - res->digit;
    if (res->digit != 0) zero = res;
    res = res->next;
    while (res != NULL) {
      res->digit = 10 - res->digit - ret;
      if (res->digit) zero = res;
      res = res->next;
    }
  }
  
  // Si le nombre diminue de taille, enlève les zéros en trop;
  if (zero->next != NULL) {
    res = zero->next;
    zero->next = NULL;
    while (res->next != NULL) {
      zero = res->next;
      free(res);
      res = zero;
    }
  }
  
  *stackat(0) = r;
}

/* MUL
 *   Utilise l'algorithme classique de l'addition. L'opération est effectuée en 2 parties:
 *      (1) Pour chaque chiffre du nombre en haut de la pile, ajoute à la pile le résultat de la 
 *          multiplication entre le second nombre en haut de la pile et le chiffre en question
 *      (2) Additionne chacun des nombre créés entre eux
 */
void mul()
{
  if (stack.len < 2) {
    alert ("Format de l'opération invalide, veuillez réessayer.");
    return;
  }
  bigint *n1 = pop();
  bigint *n2 = pop();
  digits *v1 = n1->value;
  digits *v2;
  
  bigint *r;
  digits *res, *zero;
  
  int ret, prod, lvl = 0;
  int i;

  // (1) Création des nombres pour l'addition
  while (v1 != NULL) {
    v2   = n2->value;
    r    = malloc(sizeof(bigint));
    res  = malloc(sizeof(digits));
    zero = res;
    
    if (r == NULL) {
      alert("Mémoire insuffisante, essayez de fractionner vos opérations");
      return;
    } else if (res == NULL) {
      alert("Mémoire insuffisante, essayez de fractionner vos opérations");
      freeb(r);
      return;
    }
    
    r->value = res;
    r->flags = 0;
    ret = 0;
    for (i = 0; i < lvl; i++) {
      res->digit = 0;
      res->next = malloc(sizeof(digits));
      if (res->next == NULL) {
	alert("Mémoire insuffisante, essayez de fractionner vos opérations");
	freeb(r);
	return;
      } else res = res->next;
    }
    goto multiply;
    while (v2 != NULL) {
      res->next = malloc(sizeof(digits));
      if (res->next == NULL) {
	alert("Mémoire insuffisante, essayez de fractionner vos opérations");
	freeb(r);
	return;
      } else res = res->next;
    multiply:
      res->digit = (prod = v1->digit * v2->digit + ret) % 10;
      ret = prod / 10;
      if (res->digit) zero = res;
      v2 = v2->next;
    }
    if (ret) {
      res->next = malloc(sizeof(digits));
      if (res->next == NULL) {
	alert("Mémoire insuffisante, essayez de fractionner vos opérations");
	freeb(r);
	return;
      } else res = res->next;
      if ((res->digit = ret)) zero = res;
    }
    res->next = NULL;
    
    // Si le nombre diminue de taille, enlève les zéros en trop;
    if (zero->next != NULL) {
      res = zero->next;
      zero->next = NULL;
      while (res->next != NULL) {
	zero = res->next;
	free(res);
	res = zero;
      }
    }
    push(r);
    lvl++;
    v1 = v1->next;
  }
  // (2) Additione les nombres résultants de l'étape (1)
  while (--lvl > 0) {
    add();
  }
  // Empêche le cas "-0"
  if (res->digit)
    r->flags = (n1->flags ^ n2->flags) & 1;

  freeb(n1);
  freeb(n2);
}

//////////////////// PARSING ////////////////////
/* PARSE
 *   Dirige le parsing vers la bonne fonction
 */
void parse()
{
  int c;
  while ((c = getchar()) != '\n') {
    if (c >= '0' && c <= '9')
      parsenumber(c);
    else if (c == '*' || c == '+' || c == '-' || c == '/' || c == '=' || c == '?')
      parseoperator(c);
    else if (c == EOF) {
      printf("exit\n");
      emptystack();
      emptyvars();
      exit(0);
    }
    else if (c == ' ' || c == '\t')
      continue;
    else
      parsevariable(c);
  }
  if (stack.len > 2) {
    printf("Opération invalide.\n");
    emptystack();
  }
}

bigint *copy(bigint *c)
{
  bigint *b = malloc(sizeof(bigint));
  digits *a, *d = malloc(sizeof(digits));
  a = c->value;
  b->flags = c->flags;
  b->value = d;
  goto copying;
  while(a->next != NULL) {
    d->next = malloc(sizeof(digits));
    d = d->next;
    a = a->next;
  copying:
    d->digit = a->digit;
  }
  d->next = NULL;
  return b;
}

void idiv()
{
  bigint *n1 = pop();
  bigint *n2 = pop();
  //digits *v1 = n1->value;
  //digits *v2 = n2->value;

  bigint *sn1 = copy(n1);
  bigint *sn2 = copy(n2);
  reverse(sn1);
  reverse(sn2);
  digits *sv1 = sn1->value;
  digits *sv2 = sn2->value;

  bigint *b;
  digits *d;
  
  bigint *r   = malloc(sizeof(bigint));
  digits *res = malloc(sizeof(digits));

  int c;

  r->flags = 0;
  r->value = res;

  n1->flags += 2; // Garde n1 en vie

  // Trouver le sous nombre
  while(1) {
    b = malloc(sizeof(bigint));
    d = malloc(sizeof(digits));
    b->flags = 0;
    b->value = d;
    printf("hehe\n");
    while(1)
      if (sv1->digit < sv2->digit) {
	goto b1;
	while (sv2 != NULL && sv1 != NULL) {
	  d->next = malloc(sizeof(digits));
	  // check
	  d = d->next;
	b1:
	  d->digit = sv2->digit;
	  printf("d: %d\n", d->digit);
	  sv1 = sv1->next;
	  sv2 = sv2->next;
	}
	break;
      }
      else if (sv1->digit == sv2->digit)
	{
	  printf("test\n");
	  d->digit = sv2->digit;
	  if (sv1->next == NULL || sv2->next == NULL) 
	    d->next = malloc(sizeof(digits));
	  d = d->next;
	  sv1 = sv1->next;
	  sv2 = sv2->next;
	} //do watev
      else {
	goto b2;
	while (sv2 != NULL && sv1 != NULL) {
	  d->next = malloc(sizeof(digits));
	  // check
	  d = d->next;
	b2:
	  d->digit = sv2->digit;
	  sv1 = sv1->next;
	  sv2 = sv2->next;	
	}
	d->next = malloc(sizeof(digits));
	  // check
	d = d->next;
	d->digit = sv2->digit;
	sv2 = sv2->next;
	break;
      }
    d->next = NULL;
    reverse(b);
    push(b);
    c = -1;
    do {
      push(n1);
      sub();
      printf("stack: ");
      print(**stackat(0));
      printf("\n");
      c++;
    } while(!(*stackat(0))->flags);
    pop();
    freeb(b);
    res->digit = c;
    if (sv2 == NULL) break;
    res->next = malloc(sizeof(digits));
    res = res->next;
    sv1 = sn1->value;
  }
  res->next = NULL;
  
  reverse(r);
  push(r);

  n1->flags -= 2;
  
  freeb(n1);
  freeb(n2);
  freeb(sn1);
  freeb(sn2);
}

/* PARSENUMBER
 *   Crée un nombre et ajoute les chiffres au fur et a mesure de la lecture.
 * Appelée quand le parser rencontre un chiffre.
 */
void parsenumber(int c)
{
  bigint *big = malloc(sizeof(bigint));
  digits *dig = malloc(sizeof(digits)), *tmp;
  if (big == NULL) {
    alert("Mémoire insuffisante, essayez de fractionner vos opérations");
    return;
  } else if (dig == NULL) {
    alert("Mémoire insuffisante, essayez de fractionner vos opérations");
    freeb(big);
    return;
  }
  big->flags = 0;
  dig->digit = c & 0x0F;
  dig->next  = NULL;
  while ((c = getchar()) >= '0' && c <= '9') {
    tmp = dig;
    dig = malloc(sizeof(digits));
    if (dig == NULL) {
      alert("Mémoire insuffisante, essayez de fractionner vos opérations");
      freeb(big);
      return;
    }
    dig->digit = c & 0x0F;
    dig->next = tmp;
  }
  big->value = dig;
  ungetc(c, stdin);
  push(big);
}

/* PARSEOPERATOR
 *   Appelle la fonction correspondant à l'opérateur lu.
 */
void parseoperator(int c)
{
  switch(c) {
  case '+':
    add();
    break;
  case '-':
    sub();
    break;
  case '*':
    mul();
    break;
  case '?':
    ref();
    break;
  case '/': // peut-être un jour
    idiv();
    break;
  case '=':
    c = getchar();
    if (c < 0x41 || c > 0x7A) {
      ungetc(c, stdin);
      alert("Impossible de définir cette variable!");
      return;
    }
    newvar(c);
  }
}

/* PARSEVARIABLE
 *   Appelée quand le parser rencontre une lettre; accède à une variable ou aux commandes 
 *   "exit" et "free"
 */
void parsevariable(int c)
{
  int i, t, x = getchar();
  bigint *v;
  char message[50] = "La variable «i» n'est pas définie!";
  // Gère la commande "exit"
  if (c == 'e') {
    if (x == 'x') {
      if ((i = getchar()) == 'i') {
	if ((t = getchar()) == 't') {
	  emptystack();
	  emptyvars();
	  exit(0);
	} else {
	  ungetc(t, stdin);
	  ungetc(i, stdin);
	}
      } else
	ungetc(i, stdin);
    } // Gère la commande "free"
  } else if (c == 'f') 
    if (x == 'r') {
      if ((i = getchar()) == 'e') {
	if ((t = getchar()) == 'e') {
	  emptystack();
	  emptyvars();
	  while (getchar() != '\n');
	  ungetc('\n', stdin);
	  return;
	} else {
	  ungetc(t, stdin);
	  ungetc(i, stdin);
	}
      } else
	ungetc(i, stdin);
    }
  
  ungetc(x, stdin);
  
  if (x != ' ' && x != '\t' && x != '\n') {
    alert("Les variables ne peuvent contenir qu'un seul caractère!");
    return;
  }
  if ((t = var(c)) == -1) return;
  if ((v = variables[t]) == NULL) {
    message[14] = c;
    alert(message);
    return;
  }
  push(v);
}

int main()
{
  // Initialisation
  int i;
  for (i = 0; i < 52; i++) variables[i] = NULL;
  stack.ptr = malloc(sizeof(bigint*));
  if (stack.ptr != NULL)
    stack.cap = 1;
  else {
    alert("Erreur fatale: Incapable d'initialiser la calculatrice!");
    return 1;
  }
  bigint *b = malloc(sizeof(bigint*));
  digits *d = malloc(sizeof(digits*));
  if (b == NULL) {
    alert("Erreur fatale: Incapable d'initialiser la calculatrice!");
    return 1;
  } else if (d == NULL) {
    alert("Erreur fatale: Incapable d'initialiser la calculatrice!");
    freeb(b);
    return 1;
  }
  b->flags = 0;
  b->value = d;
  d->digit = 0;
  d->next = NULL;
  push(b);
  // Calculatrice
  while(1) {
    printf("> ");
    parse();
    shrink();
    print(**stackat(0));
    printf("\n");
  }
  return 0;
}
