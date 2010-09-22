#include <stdio.h>
#include <stdlib.h>

typedef struct tree tree;
struct tree {
  int data;
  tree *left;
  tree *right;
};

tree *new_item (int);
tree *insert (tree *root, tree *new);
tree *insert (tree *root, tree *new);
void apply_preorder (tree *root, void (*fn) (tree *));
void print_tree (tree *root);
void print_tree_aux (tree *root);

int
main (void) {
  tree *root = NULL;

  root = insert (root, new_item (8));
  root = insert (root, new_item (3));
  root = insert (root, new_item (1));
  root = insert (root, new_item (6));
  root = insert (root, new_item (10));

  print_tree (root);

  return EXIT_SUCCESS;
}

tree *new_item (int data) {
  tree *new = (tree *) malloc (sizeof (tree));

  new->data = data;
  new->left = NULL;
  new->right = NULL;

  return new;
}

tree *insert (tree *root, tree *new) {
  if (NULL == root)
    return new;

  if (root->data < new->data)
    root->right = insert (root->right, new);
  else if (root->data > new->data)
    root->left = insert (root->left, new);
  /* skipping items that are already in the tree */

  return root;
}

void apply_preorder (tree *root, void (*fn) (tree *)) {
  if (NULL == root)
    return;

  (*fn) (root);
  apply_preorder (root->left, fn);
  apply_preorder (root->right, fn);
}

void print_tree (tree *root) {
  apply_preorder (root, &print_tree_aux);
}

void print_tree_aux (tree *root) {
  printf ("%d\n", root->data);
}
