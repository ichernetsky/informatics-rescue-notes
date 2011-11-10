#include <stdio.h>
#include <stdlib.h>

#define array_length(a)                         \
    (sizeof(a) / sizeof((a)[0]))                \

static inline void swap(int a[], int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void quicksort(int a[], int n)
{
    int i, last = 0;

    if (n <= 1)
        return;

    swap (a, 0, rand () % n);
    for (i = 1; i < n; i++)
        if (a[i] < a[0])
            swap(a, ++last, i);
    swap(a, 0, last);

    quicksort(a, last);
    quicksort(a + last + 1, n - last - 1);
}

int main(void)
{
    int a[] = { 7, 5, 1, 7, 5, 9, 0, 3, 6 };
    int length = array_length(a);

    quicksort(a, length);
    for (int i = 0; i < length; i++)
        printf("%d ", a[i]);

    return 0;
}
