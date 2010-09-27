#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a[0]))

void swap (int [], int, int);
void quicksort (int [], int);

int main (void) {
  int i;
  int a[] = { 7, 5, 1, 7, 5, 9, 0, 3, 6 };
  int length = ARRAY_LENGTH (a);

  quicksort (a, length);
  for (i = 0; i < length; i++)
    printf ("%d ", a[i]);

  return EXIT_SUCCESS;
}

void swap (int a[], int i, int j) {
  int tmp;

  tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void quicksort (int a[], int n) {
  int i, last;
  if (n <= 1)
    return;

  swap (a, 0, rand () % n);
  last = 0;
  for (i = 1; i < n; i++)
    if (a[i] < a[0])
      swap (a, ++last, i);
  swap (a, 0, last);

  quicksort (a, last);
  quicksort (a + last + 1, n - last - 1);
}
