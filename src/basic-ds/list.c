#include <stdio.h>
#include <stdlib.h>

struct list {
    int data;
    struct list *next;
};

struct list *make_item(int data)
{
    struct list *item = (struct list *) malloc(sizeof(struct list));

    item->data = data;
    item->next = NULL;

    return item;
}

struct list *add_front(struct list *head, struct list *item)
{
    item->next = head;
    return item;
}

struct list *add_back(struct list *head, struct list *item) {
    struct list *p;

    if (NULL == head)
        return item;

    for (p = head; p->next != NULL; p = p->next)
        ;
    p->next = item;
    return head;
}

#define for_each(n, head)                       \
    for (n = (head); n != NULL; n = n->next)    \

struct list *remove_item(struct list *head, int data)
{
    struct list *current, *prev = NULL;

    for_each(current, head) {
        if (data == current->data) {
            if (NULL == prev)
                head = current->next;
            else
                prev->next = current->next;

            free(current);
            return head;
        }

        prev = current;
    }

    return head;
}

void print_forwards(struct list *head)
{
    struct list *next;

    for_each(next, head)
        printf("%x ", next->data);
}

void print_backwards(struct list *item)
{
    if (NULL == item)
        return;

    print_backwards(item->next);
    printf("%x ", item->data);
}

int main(void)
{
    struct list *head = NULL;

    head = add_front(head, make_item(1));
    head = add_front(head, make_item(2));
    head = add_front(head, make_item(3));
    head = add_back(head, make_item(4));
    head = add_back(head, make_item(5));

    print_forwards(head);
    printf("\n");
    print_backwards(head);
    printf("\n");

    head = remove_item(head, 3);
    head = remove_item(head, 1);
    head = remove_item(head, 5);

    print_forwards(head);
    printf("\n");
    print_backwards(head);
    printf("\n");

    return 0;
}
