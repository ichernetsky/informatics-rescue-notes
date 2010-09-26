#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 50

typedef struct binheap binheap;
struct binheap {
  int data[HEAP_SIZE];
  int size;
};

binheap heap = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		 9 };

inline int topos (int);
inline int toindex (int);
inline int parent (int);
inline int left (int);
inline int right (int);
void max_heapify (binheap *, int);
void build_max_heap (binheap *);

int main (void) {
  int i;
  build_max_heap (&heap);

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
    tmp = heap->data[i];
    heap->data[i] = heap->data[largest];
    heap->data[largest] = tmp;

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
