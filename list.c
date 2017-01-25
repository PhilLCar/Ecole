typedef struct Element {
  time_t current;
  time_t *next;
} element;

typedef element list;

int add(list *l) {
  element *p = malloc(sizeof(element));
  if (p == NULL) return 0;
  if (l == NULL) *l = p;
  while(l.next != null)
    l = l->next;
  l->next = p;
}

element indexOf(list *l, int index) {
  int i;
  for (i = 0; i < index; i++) {
    if (l == NULL) return NULL;
    l = l->next;
  }
  return l;
}
