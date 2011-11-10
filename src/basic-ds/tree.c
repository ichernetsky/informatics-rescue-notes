#include <stdio.h>
#include <stdlib.h>

struct tree {
    int data;
    struct tree *left;
    struct tree *right;
};

struct tree *make_node(int data)
{
    struct tree *node = (struct tree *) malloc(sizeof(struct tree));

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

struct tree *insert(struct tree *root, struct tree *node)
{
    if (NULL == root)
        return node;

    if (root->data < node->data)
        root->right = insert(root->right, node);
    else if (root->data > node->data)
        root->left = insert(root->left, node);
    /* skipping items that are already in the struct tree */

    return root;
}

void apply_fn_preorder(struct tree *root, void (*fn)(struct tree *))
{
    if (NULL == root)
        return;

    fn(root);
    apply_fn_preorder(root->left, fn);
    apply_fn_preorder(root->right, fn);
}

void print_node(struct tree *node) {
    printf("%d\n", node->data);
}

void print_tree(struct tree *root)
{
    apply_fn_preorder(root, &print_node);
}

int main(void)
{
    struct tree *root = NULL;

    root = insert(root, make_node(8));
    root = insert(root, make_node(3));
    root = insert(root, make_node(1));
    root = insert(root, make_node(6));
    root = insert(root, make_node(10));

    print_tree(root);

    return 0;
}
