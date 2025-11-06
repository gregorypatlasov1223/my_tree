#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_error_type.h"

tree_error_type tree_destroy_recursive(node_t* node)
{
    if (node == NULL)
        return TREE_NO_ERROR; // для рекурсии это норм

    tree_destroy_recursive(node -> left);
    tree_destroy_recursive(node -> right);

    free(node);

    return TREE_NO_ERROR;
}


tree_error_type print_tree_node(const node_t* node)
{
    if (node == NULL)
        return TREE_NO_ERROR;

    printf("(");
    printf("%d", node -> data);

    if (node -> left)
        print_tree_node(node -> left);

    if (node -> right)
        print_tree_node(node -> right);

    printf(")");

    return TREE_NO_ERROR;
}


tree_error_type tree_insert_element_recursive(node_t** node_ptr, tree_element value)  // иначе меняем копию node_ptr
{
    if (*node_ptr == NULL)
    {
        *node_ptr = (node_t*)calloc(1, sizeof(node_t));
        if (*node_ptr == NULL)
            return TREE_ERROR_ALLOCATION;

        (*node_ptr) -> data  = value;
        (*node_ptr) -> left  = NULL;
        (*node_ptr) -> right = NULL;

        return TREE_NO_ERROR;
    }

    if (value <= (*node_ptr) -> data)
        return tree_insert_element_recursive(&((*node_ptr) -> left), value);

    if (value > (*node_ptr) -> data)
        return tree_insert_element_recursive(&((*node_ptr) -> right), value);

    return TREE_NO_ERROR;
}


tree_error_type tree_insert_element(tree_t* tree, int value)
{
    if (tree == NULL)
        return TREE_ERROR_NULL_PTR;

    tree_error_type result = tree_insert_element_recursive(&(tree -> root), value);
    if (result == TREE_NO_ERROR)
        tree -> size++;

    return TREE_NO_ERROR;
}


tree_error_type tree_constructor(tree_t* tree)
{
    tree -> root = NULL;
    tree -> size = 0;

    return TREE_NO_ERROR;
}


tree_error_type tree_destructor(tree_t* tree)
{
    if (tree == NULL)
        return TREE_ERROR_NULL_PTR;

    tree_destroy_recursive(tree -> root);

    tree -> root = NULL;
    tree -> size = 0;

    return TREE_NO_ERROR;
}


tree_error_type tree_common_dump(tree_t* tree)
{
    if (tree == NULL)
    {
        printf("Tree is NULL");
        return TREE_ERROR_NULL_PTR;
    }

    printf("=====TREE DUMP=====\n");
    printf("Tree size = %zu\n", tree -> size);
    printf("Tree structure\n");

    if (tree -> root == NULL)
        printf("EMPTY TREE\n");

    else print_tree_node(tree -> root);

    putchar('\n');

    return TREE_NO_ERROR;
}



