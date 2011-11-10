#include <stdio.h>
#include <stdlib.h>

#define array_length(a)                         \
    (sizeof(a) / sizeof((a)[0]))                \

int binary_search(int elem, int array[], size_t length)
{
    int mid, min = 0, max = length - 1;

    do {
        mid = min + (max - min) / 2;
        if (elem > array[mid])
            min = mid + 1;
        else
            max = mid - 1;
    } while (min <= max && array[mid] != elem);

    if (array[mid] == elem)
        return mid;

    /* elem is not found */
    return -1;
}

int binary_search_aux(int elem, int array[], int low, int high)
{
    int mid;

    if (high < low)
        /* elem is not found */
        return -1;

    mid = low + (high - low) / 2;
    if (array[mid] > elem)
        return binary_search_aux(elem, array, low, mid - 1);
    if (array[mid] < elem)
        return binary_search_aux(elem, array, mid + 1, high);

    return mid;
}

int binary_search_rec (int elem, int array[], size_t length)
{
    return binary_search_aux(elem, array, 0, length - 1);
}

int main (void)
{
    int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int length = array_length(array);

    printf("%d\n", binary_search(3, array, length));
    printf("%d\n", binary_search(-3, array, length));
    printf("%d\n", binary_search_rec(3, array, length));
    printf("%d\n", binary_search_rec(-3, array, length));

    return 0;
}
