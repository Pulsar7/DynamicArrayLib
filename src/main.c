#include "main.h"


/*int main(int argc, const char** argv) {
    DynamicArray* head_ptr = NULL;

    // Copy every single element of a static-array in a dynamic-array
    int numbers[] = {1,2,3,4,5};

    size_t element_size = sizeof(int);

    head_ptr = dynamic_array_from_elements(numbers, element_size, sizeof(numbers) / element_size);

    if (head_ptr == NULL) {
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        void* current_element = get_element_by_index(head_ptr,i);
        if (current_element == NULL) {
            // Probably an invalid index
            break;
        }
        printf("(i=%d) ELEMENT=%d\n",i,*(int*)current_element);
    }

    numbers[0] = 9;
    
    for (int i = 0; i < 10; i++) {
        void* current_element = get_element_by_index(head_ptr,i);
        if (current_element == NULL) {
            // Probably an invalid index
            break;
        }
        printf("(i=%d) ELEMENT=%d\n",i,*(int*)current_element);
    }

    // Append the same static-array to the dynamic-list

    if (append_static_array_to_dynamic(head_ptr,numbers,element_size,sizeof(numbers) / element_size) != ERR_NONE) {
        clear_list(head_ptr);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        void* current_element = get_element_by_index(head_ptr,i);
        if (current_element == NULL) {
            // Probably an invalid index
            break;
        }
        printf("(i=%d) ELEMENT=%d\n",i,*(int*)current_element);
    }

    clear_list(head_ptr);



    // Copy a static-array as a whole in a dynamic-array

    head_ptr = dynamic_array_from_whole(numbers, element_size, sizeof(numbers) / element_size);

    if (head_ptr == NULL) {
        return 1;
    }

    for (int i = 0; i < sizeof(numbers) / element_size; i++) {
        int* this_elements = (int*)head_ptr->element;
        printf("(i=%d) ELEMENT=%d\n",i,this_elements[i]);
    }

    clear_list(head_ptr);



    // Test with string

    char* message = "Hello World";

    element_size = sizeof(char);

    head_ptr = dynamic_array_from_whole(message, element_size, strlen(message));

    if (head_ptr == NULL) {
        return 1;
    }

    printf("%s\n",(char*)head_ptr->element);

    // Or accessing it via `get_element_by_index`

    void* selected_element = get_element_by_index(head_ptr,0);

    if (selected_element != NULL) {
        printf("%s\n",(char*)selected_element);
    }

    clear_list(head_ptr);



    // Additional string-tests

    head_ptr = dynamic_array_from_elements(message, element_size, strlen(message));

    if (head_ptr == NULL) {
        return 1;
    }

    for (int i = 0; i < strlen(message); i++) {
        void* current_element = get_element_by_index(head_ptr,i);
        if (current_element == NULL) {
            // Probably an invalid index
            break;
        }
        printf("%c",*(char*)current_element);
    }

    printf("\n");


    // Edit a character

    char new_element = 'R';

    if (change_element_by_index(head_ptr,1,sizeof(char),(void*)&new_element) != ERR_NONE) {
        clear_list(head_ptr);
        return 1;
    }

    for (int i = 0; i < strlen(message); i++) {
        void* current_element = get_element_by_index(head_ptr,i);
        if (current_element == NULL) {
            // Probably an invalid index
            break;
        }
        printf("%c",*(char*)current_element);
    }

    printf("\n");

    clear_list(head_ptr);

    return 0;
}*/


/*int main(int argc, const char** argv) {
    size_t dimensions[] = {2, 2};

    MultiDimensionalMatrix* matrixA = create_matrix(2,dimensions,TYPE_INT);

    if (!matrixA) {
        printf("Couldn't create matrixA\n");
        return 1;
    }

    printf("Created the matrixA\n");

    MultiDimensionalMatrix* matrixB = create_matrix(2,dimensions,TYPE_INT);

    if (!matrixB) {
        printf("Couldn't create matrixB\n");
        clear_matrix(matrixA);
        return 1;
    }

    printf("Created the matrixB\n");

    // Fill both matrices
    int elements[2][2] = {
        {1, 1},
        {1, 1}
    };

    size_t indices[2];
    ErrorCode error_code;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            indices[0] = i;
            indices[1] = j;
            error_code = set_element_by_indices(matrixA,indices,(void*)&elements[i][j]);

            if (error_code != ERR_NONE) {
                printf("Couldn't fill matrixA due to an error\n");
                break;
            }

            error_code = set_element_by_indices(matrixB,indices,(void*)&elements[i][j]);

            if (error_code != ERR_NONE) {
                printf("Couldn't fill matrixB due to an error\n");
                break;
            }
        }
    }
    //


    ArithmeticOperationReturn response = add_matrices(matrixA, matrixB);

    if (response.error_code == ERR_NONE) {
        // Successfully added both matrices
        MultiDimensionalMatrix* result_matrix = response.result_matrix;

        printf("Sucessfully added both matrices!\n");

        // printout the result-matrix
        size_t indices[2];
        void* element;

        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                indices[0] = i;
                indices[1] = j;

                element = get_element_by_indices(result_matrix, indices);
                
                if (!element) {
                    printf("Couldn't get element at result_matrix[%ld][%ld]\n",i,j);
                    break;
                }

                printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)element);
            }
        }

    } else {
        // An error occured
        printf("Couldn't add both matrices\n");
        
        // Handle the specific `ErrorCode`

        if (response.error_code == ERR_INVALID_ARGS) {
            printf("MatrixA and MatrixB have different dimensions or the `data_type` of MatrixA and MatrixB are not the same!\n");
        }

        if (response.error_code == ERR_NULL_PTR) {
            printf("One or both of MatrixA and MatrixB aren't valid matrices.\n");
        }

        if (response.error_code == ERR_UNKNOWN) {
            printf("An unkown error occured.\n");
        }
    }

    // Clear the allocated space
    clear_matrix(matrixA); 
    clear_matrix(matrixB);
    clear_matrix(response.result_matrix);

    return 0;
}*/


/*int main(int argc, const char** argv) {
    size_t dimensions[] = {2, 2, 2};

    MultiDimensionalMatrix* matrix = create_matrix(3,dimensions,TYPE_INT);

    if (!matrix) {
        printf("Couldn't create matrix\n");
        return 1;
    }

    printf("Created the matrix\n");


    int static_array[2][2][2] = {
        {
            {1, 2},
            {3, 4}
        },
        {
            {5, 6},
            {7, 8}
        }
    };

    ErrorCode response = fill_matrix_from_static_array(matrix, (void*)static_array, dimensions, 3);

    if (response == ERR_NONE) {
        // printout the matrix
        size_t indices[3];
        void* element;

        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                for (size_t k = 0; k < 2; k++) {
                    indices[0] = i;
                    indices[1] = j;
                    indices[2] = k;

                    element = get_element_by_indices(matrix, indices);
                    
                    if (!element) {
                        printf("Couldn't get element at matrix[%ld][%ld][%ld]\n",i,j,k);
                        break;
                    }

                    printf("matrix[%ld][%ld][%ld]=%d\n",i,j,k,*(int*)element);
                }
            }
        }
    } else {
        printf("THIS IS FINE.\n");
    }

    // Clear the allocated space
    clear_matrix(matrix); 

    return 0;
}*/

/*int main(int argc, const char** argv) {
    size_t dimensions[] = {2, 2};

    MultiDimensionalMatrix* matrix_A = create_matrix(2,dimensions,TYPE_INT);

    if (!matrix_A) {
        printf("Couldn't create matrix_A\n");
        return 1;
    }

    printf("Created the matrix_A\n");


    int static_array[2][2] = {
        {1, 2},
        {3, 4}
    };

    ErrorCode response = fill_matrix_from_static_array(matrix_A, (void*)static_array, dimensions, 2);

    if (response != ERR_NONE) {
        printf("THIS IS FINE.\n");
        clear_matrix(matrix_A);
        return 1;
    } 

    // printout the matrix
    size_t indices[3];
    void* element;

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            indices[0] = i;
            indices[1] = j;

            element = get_element_by_indices(matrix_A, indices);
            
            if (!element) {
                printf("Couldn't get element at matrix_A[%ld][%ld]\n",i,j);
                break;
            }

            printf("matrix_A[%ld][%ld]=%d\n",i,j,*(int*)element);
        }
    }

    MultiDimensionalMatrix* matrix_B = create_matrix(2,dimensions,TYPE_INT);

    if (!matrix_B) {
        printf("Couldn't create matrix_B\n");
        return 1;
    }

    printf("Created the matrix_B\n");


    int _static_array[2][2] = {
        {5, 6},
        {7, 8}
    };

    response = fill_matrix_from_static_array(matrix_B, (void*)_static_array, dimensions, 2);

    if (response != ERR_NONE) {
        printf("THIS IS FINE.\n");
        clear_matrix(matrix_A);
        clear_matrix(matrix_B);
        return 1;
    } 
    
    // printout the matrix

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            indices[0] = i;
            indices[1] = j;

            element = get_element_by_indices(matrix_B, indices);
            
            if (!element) {
                printf("Couldn't get element at matrix_B[%ld][%ld]\n",i,j);
                break;
            }

            printf("matrix_B[%ld][%ld]=%d\n",i,j,*(int*)element);
        }
    }

    ArithmeticOperationReturn operation_resp = multiply_2d_matrices(matrix_A, matrix_B);

    if (operation_resp.error_code != ERR_NONE) {
        printf("THIS IS FINE.\n");
        clear_matrix(matrix_A);
        clear_matrix(matrix_B);
        return 1;
    }

    printf("Calculated the matrices-product of matrix_A and matrix_B\n");

    // printout the matrix

    MultiDimensionalMatrix* result_matrix = operation_resp.result_matrix;

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            indices[0] = i;
            indices[1] = j;

            element = get_element_by_indices(result_matrix, indices);
            
            if (!element) {
                printf("Couldn't get element at result_matrix[%ld][%ld]\n",i,j);
                break;
            }

            printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)element);
        }
    }

    // Clear the allocated space
    clear_matrix(matrix_A);
    clear_matrix(matrix_B);

    return 0;
}*/




int main(int argc, const char** argv) {
    size_t dimensions[] = {2, 2};

    MultiDimensionalMatrix* matrix = create_matrix(2,dimensions,TYPE_INT);

    if (!matrix) {
        printf("Couldn't create matrix\n");
        return 1;
    }

    printf("Created the matrix\n");


    int static_array[2][2] = {
        {1, 2},
        {3, 4}
    };

    ErrorCode response = fill_matrix_from_static_array(matrix, (void*)static_array, dimensions, 2);

    if (response != ERR_NONE) {
        printf("THIS IS FINE.\n");
        clear_matrix(matrix);
        return 1;
    } 

    printf("Filled the matrix\n");

    int scalar = 2;

    ArithmeticOperationReturn arith_resp = scalar_multiply_matrix(matrix,(void*)&scalar);

    if (arith_resp.error_code != ERR_NONE) {
        printf("THIS IS FINE.\n");
        clear_matrix(matrix);
        return 1;
    }

    MultiDimensionalMatrix* result_matrix = arith_resp.result_matrix;

    // printout the matrix
    size_t indices[2];
    void* element;

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            indices[0] = i;
            indices[1] = j;

            element = get_element_by_indices(result_matrix, indices);
            
            if (!element) {
                printf("Couldn't get element at result_matrix[%ld][%ld]\n",i,j);
                break;
            }

            printf("matrix[%ld][%ld]=%d\n",i,j,*(int*)element);
        }
    }

    // Clear the allocated space
    clear_matrix(matrix);
    clear_matrix(result_matrix);

    return 0;
}