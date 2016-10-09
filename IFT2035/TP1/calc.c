#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} stack = { malloc(sizeof(bigint*), 0, 1 };

void parsenumber(int c);
void parseoperator(int c);
void parsevariable();
void checkstack();
void add();
void print();

void alert(char* message)
{
  printf(message);
  exit(1);
}

void freeb(bigint *b) {
  digits *next, *val = b->value;
  free(b);
  while ((next = val->next) != NULL) {
    free(val);
    val = next;
  }
}

void push(bigint *elem)
{
  bigint **tmp;
  stack.len++;
  if (stack.len > stack.cap) {
    stack.cap = stack.len;
    tmp = realloc(stack.ptr, stack.cap * sizeof(bigint*));
    if (tmp = NULL) alert("STACK FAIL");
    stack.ptr = tmp;
  }
  *(stack.ptr + stack.len - 1) = elem;
}

bigint *pop()
{
  bigint  *ret = *(stack.ptr + stack.len - 1);
  stack.len--;
  bigint **tmp = realloc(stack.ptr, stack.len * sizeof(bigint*));
  if (tmp != NULL) {
    stack.cap--;
    stack.ptr = tmp;
  }
  return ret;
}

bigint *peak()
{
  return *(stack.ptr + stack.len - 1);
}

void set(bigint *b)
{
  *(stack.ptr + stack.len - 1) = b;
}

void checkstack()
{
}

void add()
{
  fprintf(stderr, "YOOOO");
  bigint *n1 = pop();
  bigint *n2 = peak();
  digits *v1 = n1->value;
  digits *v2 = n2->value;
  
  bigint *r  = malloc(sizeof(bigint));
  digits *res = malloc(sizeof(digits));
  
  int ret = 0, sum;

  r->value  = res;
  
  goto adding;
  
  while (v1->next != NULL && v2->next != NULL) {
    
    res->next = malloc(sizeof(digits));
    if (res->next == NULL)
      alert("FAIL!");
    else res = res->next;
  adding:
    res->digit = (sum = v1->digit + v2->digit + ret) % 10;
    ret = sum / 10;
    
  } while (v1->next != NULL) {
    
    res->next = malloc(sizeof(digits));
    if (res->next == NULL)
      alert("FAIL!");
    else res = res->next;
    res->digit = (sum = v1->digit + ret) % 10;
    ret = sum / 10;
    
  } while (v2->next != NULL) {
    
    res->next = malloc(sizeof(digits));
    if (res->next == NULL)
      alert("FAIL!");
    else res = res->next;
    res->digit = (sum = v2->digit + ret) % 10;
    ret = sum / 10;
    
  }
  res->next = NULL;

  freeb(n1);
  freeb(n2);
  set(r);
}

void print(bigint b)
{
  digits *i = b.value;
  digits *a = NULL, *p;
  do {
    p = i->next;
    i->next = a;
    a = i;
    i = p;
  } while (i->next != NULL);
  i = a;
  a = NULL;
  do {
    printf("%d", i->digit);
    p = i->next;
    i->next = a;
    a = i;
    i = p;
  } while (i->next != NULL);
}

void parse()
{
  int c;
  while ((c = getchar()) != '\n') {
    if (c >= '0' && c <= '9')
      parsenumber(c);
    else if (c == '*' || c == '+' || c == '-' || c == '/')
      parseoperator(c);
    else if (c == '=')
      parsevariable();
    else if (c == ' ' || c == '\t')
      continue;
  }
}

void parsenumber(int c)
{
  bigint *big = malloc(sizeof(bigint));
  digits *dig = malloc(sizeof(digits)), *tmp;
  if (big == NULL || dig == NULL)
    alert("Memory error!");
  big->flags = 0;
  dig->digit = c & 0x0F;
  dig->next  = NULL;
  while ((c = getchar()) >= '0' && c <= '9') {
    tmp = dig;
    dig = malloc(sizeof(digits));
    if (dig == NULL)
      alert("ULTIMATE FAIL");
    dig->digit = c & 0x0F;
    dig->next = tmp;
  }
  push(big);
}

void parseoperator(int c)
{
  switch(c) {
  case '+':
    add();
    break;
  case '-':
    //sub();
    break;
  case '*':
    //mul();
    break;
  case '/':
    //div();
    break;
  }
}

void parsevariable()
{
}

int main()
{
  stack.ptr = malloc(sizeof(bigint*));
  while(1) {
    printf("> ");
    parse();
    print(*peak());
  }
  return 0;
}
