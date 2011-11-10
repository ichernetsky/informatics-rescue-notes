#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ea {
    int count;
    int max;
    int *a;
};

enum {
    EA_INIT = 1,
    EA_GROW = 2
};

int add(struct ea *ea, int value)
{
    if (NULL == ea->a) {
        ea->a = (int *) malloc(EA_INIT * sizeof(int));
        if (NULL == ea->a)
            return -1;

        ea->max = EA_INIT;
        ea->count = 0;
    } else if (ea->count == ea->max) {
        int *a = (int *) malloc(EA_GROW * ea->max * sizeof(int));
        if (NULL == a)
            return -1;

        memcpy(a, ea->a, ea->count * sizeof(int));
        free(ea->a);

        ea->max *= EA_GROW;
        ea->a = a;
    }

    ea->a[ea->count] = value;
    return ea->count++;
}

int del(struct ea *ea, int value)
{
    for (int i = 0; i < ea->count; i++)
        if (value == ea->a[i]) {
            memmove(ea->a + i, ea->a + i + 1,
                    (ea->count - i - 1) * sizeof(int));

            ea->count--;
            return 1;
        }

    return 0;
}

struct ea array;

int main (void)
{
    (void) add(&array, 5);
    (void) add(&array, 3);
    (void) add(&array, 2);
    (void) add(&array, 7);
    (void) add(&array, 6);

    for (int i = 0; i < array.count; i++)
        printf("%d ", array.a[i]);
    printf("\n");

    (void) del(&array, 2);
    (void) del(&array, 7);

    for (int i = 0; i < array.count; i++)
        printf("%d ", array.a[i]);
    printf("\n");

    return 0;
}
