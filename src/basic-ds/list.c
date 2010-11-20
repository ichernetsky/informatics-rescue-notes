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
list *remove_item (list *, int);
void print_backwards (list *);

int main (void) {
  list *head = NULL;

  head = add_front (head, new_item (1));
  head = add_front (head, new_item (2));
  head = add_front (head, new_item (3));
  head = add_end (head, new_item (4));
  head = add_end (head, new_item (5));

  print_backwards (head);
  printf ("\n");

  head = remove_item (head, 3);
  head = remove_item (head, 1);
  head = remove_item (head, 5);

  print_backwards (head);
  printf ("\n");

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
  list *p;

  if (NULL == head)
    return new;

  for (p = head; p->next != NULL; p = p->next)
    ;
  p->next = new;
  return head;
}

list *remove_item (list *head, int data) {
  list *current, *prev = NULL;

  for (current = head; current != NULL; current = current->next) {
    if (data == current->data) {
      if (NULL == prev)
	head = current->next;
      else
	prev->next = current->next;
      free (current);
      return head;
    }
    prev = current;
  }
  return head;
}

void print_backwards (list *elem) {
  if (NULL == elem)
    return;

  print_backwards (elem->next);
  printf ("%x ", elem->data);
}
