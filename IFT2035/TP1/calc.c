#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BigInt bigint;
typedef struct Digits digits;

struct BigInt {
  int flags;
  struct Digits *value;
};

struct Digits {
  int digit;
  struct Digits *next;
};

void **stack_ptr;
int    stack_len = 0;
int    stack_cap = 1;

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

void stack(void *elem)
{
  void **tmp;
  stack_len++;
  if (stack_len > stack_cap) {
    stack_cap = stack_len;
    tmp = realloc(stack_ptr, stack_cap * sizeof(void*));
    if (tmp = NULL) alert("STACK FAIL");
    stack_ptr = tmp;
  }
  *(stack_ptr + stack_len - 1) = elem;
}

void checkstack()
{
}

void add()
{
  
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
  stack(big);
}

void parseoperator(int c)
{
  switch(c) {
  case '+':
    add();
  }
}

void parsevariable()
{
}

int main()
{
  stack_ptr = malloc(sizeof(bigint*));
  while(1) {
    printf("> ");
    parse();
  }
  return 0;
}
