#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 50

typedef struct binheap binheap;
struct binheap {
  int data[HEAP_SIZE];
  int size;
};

inline int topos (int);
inline int toindex (int);
inline int parent (int);
inline int left (int);
inline int right (int);
void swap (int [], int, int);
void max_heapify (binheap *, int);
void build_max_heap (binheap *);
void heapsort (binheap *);

int main (void) {
  binheap heap = { { 9, 1, 8, 2, 7, 3, 6, 4, 5 },
		   9 };
  int i;

  build_max_heap (&heap);
  for (i = 0; i < heap.size; i++)
    printf ("%d ", heap.data[i]);

  printf ("\n");

  heapsort (&heap);
  for (i = 0; i < heap.size; i++)
    printf ("%d ", heap.data[i]);

  return EXIT_SUCCESS;
}

inline int topos (int i) {
  return i + 1;
}

inline int toindex (int i) {
  return i - 1;
}

inline int parent (int i) {
  return toindex (topos (i) / 2);
}

inline int left (int i) {
  return toindex (2 * topos (i));
}

inline int right (int i) {
  return toindex (2 * topos (i) + 1);
}

void swap (int a[], int i, int j) {
  int tmp;

  tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void max_heapify (binheap *heap, int i) {
  int l = left (i);
  int r = right (i);
  int largest = i;
  int tmp;

  if (l < heap->size && heap->data[l] > heap->data[i])
    largest = l;
  if (r < heap->size && heap->data[r] > heap->data[largest])
    largest = r;

  if (largest != i) {
    swap (heap->data, i, largest);
    max_heapify (heap, largest);
  }
}

void build_max_heap (binheap *heap) {
  int i;
  if (heap->size == 0)
    return;
  for (i = toindex (heap->size / 2); i >= 0; i--)
    max_heapify (heap, i);
}

void heapsort (binheap *heap) {
  int i;
  int size = heap->size;

  build_max_heap (heap);
  for (i = size - 1; i > 0; i--) {
    swap (heap->data, 0, i);
    heap->size -= 1;
    max_heapify (heap, 0);
  }
  heap->size = size;
}
