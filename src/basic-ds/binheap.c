#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define HEAP_SIZE 50

struct binheap {
    int data[HEAP_SIZE];
    int size;
};

static inline int topos(int i)
{
    return i + 1;
}

static inline int toindex(int i)
{
    return i - 1;
}

static inline int parent(int i)
{
    return toindex(topos(i) / 2);
}

static inline int left(int i)
{
    return toindex(2 * topos(i));
}

static inline int right(int i)
{
    return toindex(2 * topos(i) + 1);
}

static inline void swap(int a[], int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void max_heapify(struct binheap *heap, int i)
{
    int l = left(i);
    int r = right(i);
    int largest = i;

    if (l < heap->size && heap->data[l] > heap->data[i])
        largest = l;
    if (r < heap->size && heap->data[r] > heap->data[largest])
        largest = r;

    if (largest != i) {
        swap(heap->data, i, largest);
        max_heapify(heap, largest);
    }
}

void build_max_heap(struct binheap *heap)
{
    if (heap->size <= 1)
        return;

    for (int i = toindex(heap->size / 2); i >= 0; i--)
        max_heapify(heap, i);
}

int heap_maximum(struct binheap *heap)
{
    return heap->data[0];
}

int heap_extract_max(struct binheap *heap)
{
    int max = heap->data[0];

    heap->size -= 1;
    heap->data[0] = heap->data[heap->size];
    max_heapify(heap, 0);

    return max;
}


int heap_increase_key(struct binheap *heap, int i, int k)
{
    if (k < heap->data[i])
        return -1;

    heap->data[i] = k;
    while (i > 0 && heap->data[parent(i)] < heap->data[i]) {
        swap(heap->data, i, parent(i));
        i = parent(i);
    }

    return 0;
}

void heap_insert(struct binheap *heap, int k)
{
    heap->data[heap->size] = INT_MIN;
    heap->size += 1;
    heap_increase_key(heap, heap->size - 1, k);
}

void heapsort(struct binheap *heap)
{
    int size = heap->size;

    build_max_heap(heap);
    for (int i = size - 1; i > 0; i--) {
        swap(heap->data, 0, i);
        heap->size -= 1;
        max_heapify(heap, 0);
    }

    heap->size = size;
}

int main(void)
{
    int i;
    struct binheap heap = {
        .data = { 9, 1, 11, 13, 7, 3, 15, 17, 5 },
        .size = 9
    };

    build_max_heap(&heap);
    for (i = 0; i < heap.size; i++)
        printf("%d ", heap.data[i]);
    printf("\n");

    heap_insert(&heap, 8);
    heap_insert(&heap, 19);
    (void) heap_extract_max(&heap);
    (void) heap_increase_key(&heap, 6, 12);
    for (i = 0; i < heap.size; i++)
        printf("%d ", heap.data[i]);
    printf("\n");

    heapsort(&heap);
    for (i = 0; i < heap.size; i++)
        printf("%d ", heap.data[i]);
    printf("\n");

    return 0;
}
