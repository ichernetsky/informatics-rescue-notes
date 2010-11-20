#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTABLE_SIZE 20
#define MULTIPLIER  31

typedef struct item item;
struct item {
  char *name;
  int value;
  item *next;
};

item *htable[HTABLE_SIZE];

item *new_item (char *, int);
item *lookup (char *, int, int);
unsigned int hash (char *);

int main (void) {
  char *strings[] = { "string1", "string2", "string3",
                      "string4", "string5" };
  int i;
  item *it;

  for (i = 0; i < 3; i++)
    lookup (strings[i], 1, i + 1);
  for (i = 0; i < 5; i++) {
    it = lookup (strings[i], 0, 0);
    if (it)
      printf ("%s in hash table, value = %d\n", it->name, it->value);
    else
      printf ("%s somewhere else\n", strings[i]);
  }

  return EXIT_SUCCESS;
}

item *new_item (char *name, int value) {
  item *new = (item *) malloc (sizeof (item));

  new->name = name;
  new->value = value;
  new->next = NULL;

  return new;
}

item *lookup (char *name, int create, int value) {
  int h = hash (name);
  item *i = htable[h];

  for (; i != NULL; i = i->next)
    if (strcmp (name, i->name) == 0)
      return i;

  if (create) {
    i = new_item (name, value);
    i->next = htable[h];
    htable[h] = i;
  }
  return i;
}

unsigned int hash (char *str) {
  unsigned int h = 0;
  unsigned char *p = (unsigned char *) str;

  for (; *p != '\0'; p++)
    h = MULTIPLIER * h + *p;
  return h % HTABLE_SIZE;
}
