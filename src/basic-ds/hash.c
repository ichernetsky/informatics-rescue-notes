#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HTABLE_SIZE   20
#define MULTIPLIER    31

struct item {
    char *name;
    int value;
    struct item *next;
};

struct item *make_item(char *name, int value)
{
    struct item *item = (struct item *) malloc(sizeof(struct item));

    item->name = name;
    item->value = value;
    item->next = NULL;

    return item;
}

unsigned int hash(char *str)
{
    unsigned int h = 0;
    unsigned char *p = (unsigned char *) str;

    for (; *p != '\0'; p++)
        h = MULTIPLIER * h + *p;
    return h % HTABLE_SIZE;
}

struct item *lookup(struct item *htable[], char *name, bool create, int value)
{
    int h = hash(name);
    struct item *i = htable[h];

    for (; i != NULL; i = i->next)
        if (strcmp (name, i->name) == 0)
            return i;

    if (create) {
        i = make_item(name, value);
        i->next = htable[h];
        htable[h] = i;
    }
    return i;
}

int main (void)
{
    struct item *htable[HTABLE_SIZE] = { NULL };

    char *strings[] = { "string1", "string2", "string3",
                        "string4", "string5" };
    int i;
    struct item *it;

    for (i = 0; i < 3; i++)
        lookup(htable, strings[i], true, i + 1);
    for (i = 0; i < 5; i++) {
        it = lookup(htable, strings[i], false, 0);
        if (it)
            printf ("%s in hash table, value = %d\n", it->name, it->value);
        else
            printf ("%s somewhere else\n", strings[i]);
    }

    return 0;
}
