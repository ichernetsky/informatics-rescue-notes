#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct extarray extarray;
struct extarray {
  int count;
  int max;
  int *array;
};

extarray arr;

enum { EXTARRAY_INIT = 1,
       EXTARRAY_GROW = 2 };

int add (int);

int main (void) {
  int i;

  (void) add (5);
  (void) add (3);
  (void) add (2);
  (void) add (7);
  (void) add (6);

  for (i = 0; i < arr.count; i++)
    printf ("%d ", arr.array[i]);

  return EXIT_SUCCESS;
}

int add (int value) {
  if (NULL == arr.array) {
    arr.array = (int *) malloc (EXTARRAY_INIT * sizeof (int));
    if (NULL == arr.array)
      return -1;
    arr.max = EXTARRAY_INIT;
    arr.count = 0;
  } else if (arr.count >= arr.max) {
    int i;

    int *newarr = (int *) malloc ((EXTARRAY_GROW * arr.max) * sizeof (int));
    if (NULL == newarr)
      return -1;
    memcpy (newarr, arr.array, arr.count * sizeof (int));
    free (arr.array);
    arr.max *= EXTARRAY_GROW;
    arr.array = newarr;
  }
  arr.array[arr.count] = value;
  return arr.count++;
}
