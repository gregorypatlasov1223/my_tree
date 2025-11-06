#ifndef TREE_H_
#define TREE_H_

#include "tree_error_type.h"

typedef int tree_element;

struct node_t
{
    tree_element data;
    node_t*      left;
    node_t*      right;
};

struct tree_t
{
    size_t  size;
    node_t* root;
};

tree_error_type print_tree_node(const node_t* node);
tree_error_type tree_insert_element(tree_t* tree, int value);
tree_error_type tree_insert_element_recursive(node_t** node_ptr, tree_element value);
tree_error_type tree_destroy_recursive(node_t* node);
tree_error_type tree_destructor(tree_t* tree);
tree_error_type tree_constructor(tree_t* tree);
tree_error_type tree_common_dump(tree_t* tree);


#endif // TREE_H_
