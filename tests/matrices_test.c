#include "matrices_test.h"


void test_create_matrix() {
    MultiDimensionalMatrix matrix;
    size_t dimensions[3] = {3, 2, 2};
    
    // Test valid matrix creation
    ErrorCode err = create_matrix(&matrix, 3, dimensions, TYPE_INT);
    assert(err == ERR_NONE);
    assert(matrix.head_ptr->data != NULL);
    assert(matrix.head_ptr->dimensions[0] == dimensions[0]);
    assert(matrix.head_ptr->dimensions[1] == dimensions[1]);
    assert(matrix.head_ptr->dimensions[2] == dimensions[2]);

    
    clear_matrix(&matrix);
}

void test_set_and_get_element() {
    MultiDimensionalMatrix matrix;
    size_t dimensions[2] = {2, 2};
    create_matrix(&matrix, 2, dimensions, TYPE_INT);
    
    int value = 42;
    size_t indices[2] = {1, 1};
    ErrorCode err = set_element_by_indices(&matrix, indices, (void*)&value);
    assert(err == ERR_NONE);
    
    int* retrieved_value = (int*)get_element_by_indices(&matrix, indices);
    assert(*retrieved_value == 42);

    // Test if errors are handled properly

    // Invalid data-type
    float float_value = 3.14;
    err = set_element_by_indices(&matrix, indices, (void*)&float_value);
    assert(err == ERR_NONE);

    void* error_result = get_element_by_indices(&matrix, indices);
    assert(*(int*)error_result != (int)float_value);

    // Index should be out of range
    error_result = get_element_by_indices(&matrix, (size_t[]){2, 2});
    assert(error_result == NULL);
    
    
    clear_matrix(&matrix);
}

void test_add_matrices() {
    MultiDimensionalMatrix matrix_A, matrix_B;
    size_t dimensions[2] = {2, 2};
    create_matrix(&matrix_A, 2, dimensions, TYPE_INT);
    create_matrix(&matrix_B, 2, dimensions, TYPE_INT);
    
    int val1 = 1, val2 = 2;
    set_element_by_indices(&matrix_A, (size_t[]){0, 0}, (void*)&val1);
    set_element_by_indices(&matrix_B, (size_t[]){0, 0}, (void*)&val2);
    
    ArithmeticOperationReturn result = add_matrices(&matrix_A, &matrix_B);
    assert(result.error_code == ERR_NONE);
    int* result_value = (int*)get_element_by_indices(&result.result_matrix, (size_t[]){0, 0});
    assert(*result_value == 3);

    // Add matrices with different dimension-sizes
    MultiDimensionalMatrix third_matrix;
    dimensions[0] = 1;
    create_matrix(&third_matrix, 2, dimensions, TYPE_INT);
    set_element_by_indices(&third_matrix, (size_t[]){0, 0}, (void*)&val2);
    result = add_matrices(&matrix_A, &third_matrix);
    assert(result.error_code == ERR_DIMENSION_SIZE_MISMATCH);

    // Add matrices with different data-types
    dimensions[0] = 2;
    MultiDimensionalMatrix fourth_matrix;
    ErrorCode error = create_matrix(&fourth_matrix, 2, dimensions, TYPE_FLOAT);
    error = set_element_by_indices(&fourth_matrix, (size_t[]){0, 0}, (void*)&val2);
    assert(error == ERR_NONE);
    result = add_matrices(&matrix_A, &fourth_matrix);
    assert(result.error_code == ERR_DATATYPE_MISMATCH);

    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    clear_matrix(&result.result_matrix);
    clear_matrix(&third_matrix);
    clear_matrix(&fourth_matrix);
}

void test_fill_matrix_from_static_array() {
    MultiDimensionalMatrix matrix;
    size_t dimensions[2] = {2, 2};
    create_matrix(&matrix, 2, dimensions, TYPE_INT);
    
    int static_array[2][2] = {
        {1, 2},
        {3, 4}
    };

    ErrorCode err = fill_matrix_from_static_array(&matrix, static_array);
    
    assert(err == ERR_NONE);
    int* first = (int*)get_element_by_indices(&matrix, (size_t[]){0, 0});
    assert(*first == 1);
    int* second = (int*)get_element_by_indices(&matrix, (size_t[]){0, 1});
    assert(*second == 2);
    int* third = (int*)get_element_by_indices(&matrix, (size_t[]){1, 0}); 
    assert(*third == 3);
    int* fourth = (int*)get_element_by_indices(&matrix, (size_t[]){1, 1});
    assert(*fourth == 4);

    clear_matrix(&matrix);
}

void test_scalar_multiply_matrix() {
    MultiDimensionalMatrix matrix;
    size_t dimensions[2] = {2, 2};
    create_matrix(&matrix, 2, dimensions, TYPE_INT);
    
    int static_array[2][2] = {
        {1, 2},
        {3, 4}
    };

    ErrorCode err = fill_matrix_from_static_array(&matrix, static_array);
    
    assert(err == ERR_NONE);
    
    int scalar = 2;
    ArithmeticOperationReturn resp = scalar_multiply_matrix(&matrix, (void*)&scalar);

    assert(resp.error_code == ERR_NONE);
    assert(resp.result_matrix.head_ptr != NULL);

    // Check elements of result-matrix
    int* first = (int*)get_element_by_indices(&resp.result_matrix, (size_t[]){0, 0});
    assert(*first == 2);
    int* second = (int*)get_element_by_indices(&resp.result_matrix, (size_t[]){0, 1});
    assert(*second == 4);
    int* third = (int*)get_element_by_indices(&resp.result_matrix, (size_t[]){1, 0}); 
    assert(*third == 6);
    int* fourth = (int*)get_element_by_indices(&resp.result_matrix, (size_t[]){1, 1});
    assert(*fourth == 8);

    clear_matrix(&matrix);
    clear_matrix(&resp.result_matrix);
}

void test_multiply_2d_matrices() {
    MultiDimensionalMatrix matrix_A, matrix_B;
    size_t dimensions[2] = {2, 2};
    create_matrix(&matrix_A, 2, dimensions, TYPE_INT);
    create_matrix(&matrix_B, 2, dimensions, TYPE_INT);
    
    int static_array[2][2] = {
        {1, 2},
        {3, 4}
    };

    fill_matrix_from_static_array(&matrix_A, static_array);
    fill_matrix_from_static_array(&matrix_B, static_array);
    
    ArithmeticOperationReturn result = multiply_2d_matrices(&matrix_A, &matrix_B);
    assert(result.error_code == ERR_NONE);
    assert(result.result_matrix.head_ptr->dimensions[0] == dimensions[0]);
    assert(result.result_matrix.head_ptr->dimensions[1] == dimensions[1]);
    assert(result.result_matrix.head_ptr->number_of_dimensions == 2);
    
    // Check elements of result-matrix
    int* first = (int*)get_element_by_indices(&result.result_matrix, (size_t[]){0, 0});
    assert(*first == 7);
    int* second = (int*)get_element_by_indices(&result.result_matrix, (size_t[]){0, 1});
    assert(*second == 10);
    int* third = (int*)get_element_by_indices(&result.result_matrix, (size_t[]){1, 0}); 
    assert(*third == 15);
    int* fourth = (int*)get_element_by_indices(&result.result_matrix, (size_t[]){1, 1});
    assert(*fourth == 22);
    
    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    clear_matrix(&result.result_matrix);
}