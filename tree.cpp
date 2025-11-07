#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

// ============================GRAPHIC_DUMP===========================================

void write_dump_header(FILE* htm_file, time_t now)
{
    fprintf(htm_file, "<div style='border:2px solid #ccc; margin:10px; padding:15px; background:#f9f9f9;'>\n");
    fprintf(htm_file, "<h2 style='color:#333;'>Tree Dump at %s</h2>\n", ctime(&now));
}


void write_information_about_tree(FILE* htm_file, tree_t* tree)
{
    assert(tree     != NULL);
    assert(htm_file != NULL);

    fprintf(htm_file, "<div style='margin-bottom:15px;'>\n");
    fprintf(htm_file, "<p><b>Tree size:</b> %zu</p>\n", tree -> size);
    fprintf(htm_file, "<p><b>Root address:</b> %p</p>\n", (void*)tree -> root); // для чего тут void
    fprintf(htm_file, "</div>\n");
}


tree_error_type write_tree_nodes_table_recursive(node_t* node, FILE* htm_file)
{
    if (node == NULL)
        return TREE_NO_ERROR;

    fprintf(htm_file, "<tr><td>%p</td><td>%d</td><td>%p</td><td>%p</td></tr>\n",
            (void*)node, node -> data, (void*)node -> left, (void*)node -> right);

    tree_error_type result = TREE_NO_ERROR;

    result = write_tree_nodes_table_recursive(node -> left,  htm_file);
    if (result != TREE_NO_ERROR) return result;

    result = write_tree_nodes_table_recursive(node -> right,  htm_file);
    if (result != TREE_NO_ERROR) return result;

    return TREE_NO_ERROR;
}


void write_tree_nodes_table(FILE* htm_file, tree_t* tree)
{
    assert(tree     != NULL);
    assert(htm_file != NULL);

    fprintf(htm_file, "<table border='1' style='border-collapse:collapse; width:100%%; margin-top:15px;'>\n");
    fprintf(htm_file, "<tr><th>Address</th><th>Data</th><th>Left</th><th>Right</th></tr>\n");

    if (tree -> root != NULL)
        write_tree_nodes_table_recursive(tree -> root, htm_file);

    fprintf(htm_file, "</table>\n");
}


int is_root_node(tree_t* tree, node_t* node)
{
    return (tree -> root == node) ? 1 : 0;
}


tree_error_type create_dot_tree_recursive(tree_t* tree, node_t* node, FILE* dot_file)
{
    if (node == NULL)
        return TREE_ERROR_NULL_PTR;

    const char* fill_color = is_root_node(tree, node) ? "lightblue" : "white";
    const char* shape = "Mrecord";

    fprintf(dot_file, "    node_%p [label=\"{<f0> data: %d | <f1> left | <f2> right}\", shape=%s, style=filled, fillcolor=%s, color=black];\n",
            (void*)node, node->data, shape, fill_color); // <f0> данные узла, <f1> указатель на left, <f2> = указатель на right

    if (node -> left != NULL)
    {
        fprintf(dot_file, "    node_%p:<f1> -> node_%p:<f0> [color=blue];\n",
                                (void*)node, (void*)node -> left);
        create_dot_tree_recursive(tree, node -> left, dot_file);
    }

    if (node -> right != NULL)
    {
        fprintf(dot_file, "    node_%p:<f2> -> node_%p:<f0> [color=red];\n",
                               (void*)node, (void*)node -> right);
        create_dot_tree_recursive(tree, node -> right, dot_file);
    }

    return TREE_NO_ERROR;
}


tree_error_type create_dot_file_tree(tree_t* tree, const char* filename)
{
    assert(filename != NULL);
    if (tree == NULL)
        return TREE_ERROR_NULL_PTR;

    FILE* dot_file = fopen(filename, "w");
    if (dot_file == NULL)
        return TREE_ERROR_OPENING_FILE;

    fprintf(dot_file, "digraph BinaryTree {\n");
    fprintf(dot_file, "    rankdir=TB;\n");
    fprintf(dot_file, "    node [shape=Mrecord, color=black];\n\n");

    if (tree -> root == NULL)
        fprintf(dot_file, "    empty [label=\"Empty tree\"];\n");
    else
        create_dot_tree_recursive(tree, tree->root, dot_file);

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    return TREE_NO_ERROR;
}


tree_error_type create_graph_visualization_tree(tree_t* tree, FILE* htm_file, const char* folder_name, time_t now)
{
    assert(tree        != NULL);
    assert(htm_file    != NULL);
    assert(folder_name != NULL);

    static int number_of_pictures = 0;

    char temp_dot[MAX_LENGTH_OF_FILENAME] = {};
    char temp_svg[MAX_LENGTH_OF_FILENAME] = {};

    snprintf(temp_dot, sizeof(temp_dot), "%s/tree_temp_%d%ld.dot", folder_name, number_of_pictures, now);
    snprintf(temp_svg, sizeof(temp_svg), "%s/tree_temp_%d%ld.svg", folder_name, number_of_pictures, now);
    number_of_pictures++;

    tree_error_type dot_result = create_dot_file_tree(tree, temp_dot);
    if (dot_result != TREE_NO_ERROR)
        return dot_result;

    char command[MAX_LENGTH_OF_SYSTEM_COMMAND] = {};
    snprintf(command, sizeof(command), "dot -Tsvg %s -o %s", temp_dot, temp_svg);
    int result = system(command);

    if (result == 0)
    {
        fprintf(htm_file, "<div style='text-align:center;'>\n");
        fprintf(htm_file, "<img src='%s' style='max-width:100%%; border:1px solid #ddd;'>\n", temp_svg);
        fprintf(htm_file, "</div>\n");
    }

    else
        fprintf(htm_file, "<p style='color:red;'>Error generating SVG graph</p>\n");

    remove(temp_dot);

    return TREE_NO_ERROR;
}


tree_error_type tree_dump_to_htm(tree_t* tree, FILE* htm_file, const char* folder_name)
{
    assert(tree != NULL);
    assert(htm_file != NULL);

    time_t now = time(NULL); // получаем текущее время

    write_dump_header(htm_file, now);
    write_information_about_tree(htm_file, tree);
    create_graph_visualization_tree(tree, htm_file, folder_name, now);
    write_tree_nodes_table(htm_file, tree);

    fprintf(htm_file, "</div>\n\n");

    return TREE_NO_ERROR;
}


tree_error_type tree_dump(tree_t* tree, const char* filename)
{
    assert(tree     != NULL);
    assert(filename != NULL);

    char folder_name[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(folder_name, sizeof(folder_name), "%s_dump", filename);

    char command[MAX_LENGTH_OF_SYSTEM_COMMAND] = {};
    // флаг -p гарантирует, что папка создастся даже если родительские директории не существуют
    snprintf(command, sizeof(command), "mkdir -p %s", folder_name); // mkdir - это команда для создания директорий в OC
    system(command);

    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "a");
    if (htm_file == NULL)
        return TREE_ERROR_OPENING_FILE;

    tree_error_type result = tree_dump_to_htm(tree, htm_file, folder_name);

    fclose(htm_file);

    return result;
}


tree_error_type initialization_of_tree_log(const char* filename)
{
    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "w");
    if (htm_file == NULL)
        return TREE_ERROR_OPENING_FILE;

    fprintf(htm_file, "<!DOCTYPE html>\n"
                      "<html>\n"
                      "<head>\n"
                      "<title>Tree Dump Log</title>\n"
                      "<style>\n"
                      "body { font-family: Arial, sans-serif; margin: 20px; }\n"
                      "table { border-collapse: collapse; width: 100%%; }\n"
                      "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n"
                      "th { background-color: #f2f2f2; }\n"
                      "</style>\n"
                      "</head>\n"
                      "<body>\n"
                      "<h1>Tree Dump Log</h1>\n");
    fclose(htm_file);

    return TREE_NO_ERROR;
}


tree_error_type close_tree_log(const char* filename)
{
    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "a");
    if (htm_file == NULL)
        return TREE_ERROR_OPENING_FILE;

    fprintf(htm_file, "</body>\n");
    fprintf(htm_file, "</html>\n");
    fclose(htm_file);

    return TREE_NO_ERROR;
}



