#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_error_type.h"

void test_large_tree()
{
    tree_t tree = {};
    tree_constructor(&tree);

    const char* folder_name = "log_TestLargeTree";
    initialization_of_tree_log(folder_name);

    tree_dump(&tree, folder_name);

    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65, 90, 5, 95};
    int count = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < count; i++)
    {
        tree_insert_element(&tree, values[i]);
        if ((i + 1) % 3 == 0) // дампим каждый 3 элемент
        {
            printf("Inserted %d elements\n", i + 1);
            tree_dump(&tree, folder_name);
        }
    }

    printf("Final tree with %zu elements\n", tree.size);
    tree_dump(&tree, folder_name);

    close_tree_log(folder_name);
    tree_destructor(&tree);

    remove("log_TestLargeTree");
}


void all_tests()
{
    test_large_tree();
}
