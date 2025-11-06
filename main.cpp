#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_error_type.h"

int main()
{
    tree_t tree = {};
    tree_error_type error = TREE_NO_ERROR;

    error = tree_constructor(&tree);
    if (error != TREE_NO_ERROR)
    {
        printf("ERROR IN CONSTRUCTOR: %d\n", error);
        return TREE_ERROR_CONSTRUCTOR;
    }

    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65, 90, 5, 95};
    int count = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < count; i++)
    {
        error = tree_insert_element(&tree, values[i]);
        if (error != TREE_NO_ERROR)
            printf("ERROR: %d\n", error);
    }

    tree_common_dump(&tree);

    tree_destructor(&tree);

    return TREE_NO_ERROR;

}
