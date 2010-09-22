#include <stdio.h>
#include <stdlib.h>

typedef struct list list;
struct list {
  int data;
  list *next;
};

list *new_item (int);
list *add_front (list *, list *);
list *add_end (list *, list *);
void print_backwards (list *);

int main (void) {
  list *head = NULL;

  head = add_front (head, new_item (1));
  head = add_front (head, new_item (2));
  head = add_front (head, new_item (3));
  head = add_end (head, new_item (4));
  head = add_end (head, new_item (5));

  print_backwards (head);

  return EXIT_SUCCESS;
}

list *new_item (int data) {
  list *new = (list *) malloc (sizeof (list));

  new->data = data;
  new->next = NULL;

  return new;
}

list *add_front (list *head, list *new) {
  new->next = head;
  return new;
}

list *add_end (list *head, list *new) {
  if (NULL == head)
    return new;

  list *p;
  for (p = head; p->next != NULL; p = p->next)
    ;
  p->next = new;
  return head;
}

void print_backwards (list *elem) {
  if (NULL == elem)
    return;

  print_backwards (elem->next);
  printf ("%x\n", elem->data);
}
