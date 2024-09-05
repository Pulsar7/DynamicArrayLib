#include "test.h"
#include "stdlib.h"


int main(int argc, const char** argv) {
    for (size_t i = 0; i < 20; i++) {
        printf("-");
    }
    printf("Test MultiDimensionalMatrices\n\n");
    printf("Testing `create_matrix`...\n");
    test_create_matrix();
    printf("Testing set and get element...\n");
    test_set_and_get_element();
    printf("Testing `add_matrices`...\n");
    test_add_matrices();
    printf("Testing `fill_matrix_from_static_array`...\n");
    test_fill_matrix_from_static_array();
    printf("Testing `scalar_multiply_matrix`...\n");
    test_scalar_multiply_matrix();
    printf("Testing `multiply_2d_matrices`...\n");
    test_multiply_2d_matrices();

    printf("\n");
    for (size_t i = 0; i < 20; i++) {
        printf("-");
    }
    printf("Test DynamicArrays\n\n");
    printf("Testing `initialize_list`...\n");
    test_initialize_list();
    printf("Testing `append`...\n");
    test_append_to_list();
    printf("Testing `count_elements`...\n");
    test_count_elements();
    printf("Testing `clear_list`...\n");
    test_clear_list();
    printf("Testing `get_list_element_by_index`...\n");
    test_get_list_element_by_index();
    printf("Testing `set_list_element_by_index`...\n");
    test_set_list_element_by_index();

    printf("\nAll tests passed successfully!\n");

    return 0;
}