#ifndef TREE_H_
#define TREE_H_

#include "tree_error_type.h"

#define COEFFICIENT 2
#define MAX_LENGTH_OF_FILENAME 256
#define MAX_LENGTH_OF_SYSTEM_COMMAND 1024

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
// 1. Все что связано с деревом
tree_error_type print_tree_node(const node_t* node);
tree_error_type tree_insert_element(tree_t* tree, int value);
tree_error_type tree_insert_element_recursive(node_t** node_ptr, tree_element value);
tree_error_type tree_destroy_recursive(node_t* node);
tree_error_type tree_destructor(tree_t* tree);
tree_error_type tree_constructor(tree_t* tree);
tree_error_type tree_common_dump(tree_t* tree);

void write_dump_header(FILE* htm_file, time_t now);
void write_information_about_tree(FILE* htm_file, tree_t* tree);
tree_error_type write_tree_nodes_table_recursive(node_t* node, FILE* htm_file);
void write_tree_nodes_table(FILE* htm_file, tree_t* tree);
int is_root_node(tree_t* tree, node_t* node);
tree_error_type create_dot_tree_recursive(tree_t* tree, node_t* node, FILE* dot_file);
tree_error_type create_dot_file_tree(tree_t* tree, const char* filename);
tree_error_type create_graph_visualization_tree(tree_t* tree, FILE* htm_file, const char* folder_name, time_t now);
tree_error_type tree_dump_to_htm(tree_t* tree, FILE* htm_file, const char* folder_name);
tree_error_type tree_dump(tree_t* tree, const char* filename);
tree_error_type initialization_of_tree_log(const char* filename);
tree_error_type close_tree_log(const char* filename);

#endif // TREE_H_
