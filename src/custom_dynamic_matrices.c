#include "custom_dynamic_matrices.h"


ErrorCode create_matrix(MultiDimensionalMatrix* matrix, size_t number_of_dimensions, size_t* dimensions, DataType data_type) {
    /*
        Create a multidimensional matrix

        Returns a custom defined ErrorCode, which should be `ERR_NONE` if no error occured.
    */

    if (number_of_dimensions == 0) {
        return ERR_INVALID_ARGS;
    }

    if (!dimensions || !matrix) {
        // No dimension/matrix given
        return ERR_NULL_PTR;
    }

    // Allocate space for the Matrix-Node itself
    MultiDimensionalMatrixNode* head_ptr = (MultiDimensionalMatrixNode*) malloc(sizeof(MultiDimensionalMatrixNode));

    if (!head_ptr) {
        // Allocation-Error
        return ERR_MALLOC_FAILED;
    }

    matrix->head_ptr = head_ptr;

    head_ptr->number_of_dimensions = number_of_dimensions;

    // Allocate space for the dimensions-array
    head_ptr->dimensions = (size_t*) malloc(number_of_dimensions * sizeof(size_t));

    if (!head_ptr->dimensions) {
        // Allocation-Error
        clear_matrix(matrix);
        return ERR_MALLOC_FAILED;
    }

    memcpy(head_ptr->dimensions, dimensions, number_of_dimensions * sizeof(size_t));

    // Get total size of dimensions
    size_t total_size = 1;

    for (size_t i = 0; i < number_of_dimensions; i++) {
        total_size *= dimensions[i];
    }

    switch(data_type) {
        case TYPE_INT:
            head_ptr->data = malloc(total_size * sizeof(int));
            head_ptr->data_size = total_size * sizeof(int);
            break;
        
        case TYPE_FLOAT:
            head_ptr->data = malloc(total_size * sizeof(float));
            head_ptr->data_size = total_size * sizeof(float);
            break;

        case TYPE_DOUBLE:
            head_ptr->data = malloc(total_size * sizeof(double));
            head_ptr->data_size = total_size * sizeof(double);
            break;

        default:
            // Should be unreachable
            // Not supported Data-Type
            head_ptr->data = NULL;
            break;
    }

    if (!head_ptr->data) {
        // Allocation-Error or invalid Data-Type
        clear_matrix(matrix);
        return ERR_MALLOC_FAILED;
    }

    head_ptr->data_type = data_type;

    // Operation was successful
    return ERR_NONE;
}


void clear_matrix(MultiDimensionalMatrix* matrix) {
    /*
        Free all allocated space
    */

    if (!matrix->head_ptr) {
        // Invalid matrix
        return;
    }

    if (matrix->head_ptr->data) {
        free(matrix->head_ptr->data);
        matrix->head_ptr->data = NULL;
    }

    if (matrix->head_ptr->dimensions) {
        free(matrix->head_ptr->dimensions);
        matrix->head_ptr->dimensions = NULL;
    }

    free(matrix->head_ptr);
    matrix->head_ptr = NULL;

    return;
}

static IndexCalcReturn calc_index(MultiDimensionalMatrix* matrix, size_t* indices) {
    /*
        Calculate the index of the 1-Dimensional-array with the given multidimensional indices

        Returns the custom `IndexCalcReturn`-struct in order to return the index and
        a self-defined Error-Code, if something went wrong.
    */

    IndexCalcReturn return_data;

    if (!indices || !matrix || !matrix->head_ptr) {
        // No indices given or matrix does not exist
        return_data.error_code = ERR_INVALID_ARGS;
        return return_data;
    }

    size_t index = 0; // Calculated index
    /*
    
        The offset is used to account for the size of each dimension and to calculate the 
        correct position of an element in a flattened version of the multidimensional array.
    
    */

    size_t offset = 1;

    /*

        The value `i` can't be a `size_t`, 
        because the expression `i >= 0` would be always true.
    
    */

    for (int i = matrix->head_ptr->number_of_dimensions - 1; i >= 0; --i) {
        index += indices[i] * offset;
        offset *= matrix->head_ptr->dimensions[i];
    }

    return_data.error_code = ERR_NONE;
    return_data.index = index;
    
    return return_data;
}

void* get_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices) {
    /*
        Get single element by its indices (position in the matrix)

        Returns the element as a void-Pointer.
        Returns a NULL-Pointer if something went wrong.
    */

    if (!matrix || !indices || !matrix->head_ptr) {
        // Matrix does not exist or no indices are given
        return NULL;
    }

    IndexCalcReturn return_data = calc_index(matrix,indices);

    if (return_data.error_code != ERR_NONE) {
        // Something went wrong while trying to calculate the index
        // 
        return NULL;
    }

    size_t index = return_data.index;
    
    // Check if calculated index is out of bounds
    // And return requested value if index is valid
    switch(matrix->head_ptr->data_type) {
        case TYPE_INT:
            if (index >= matrix->head_ptr->data_size / sizeof(int)) {
                return NULL;
            }
            return (void*)((int*)matrix->head_ptr->data + index);
            break; // Unreachable?

        case TYPE_FLOAT:
            if (index >= matrix->head_ptr->data_size / sizeof(float)) {
                return NULL;
            }
            return (void*)((float*)matrix->head_ptr->data + index);
            break; // Unreachable?
        
        case TYPE_DOUBLE:
            if (index >= matrix->head_ptr->data_size / sizeof(double)) {
                return NULL;
            }
            return (void*)((double*)matrix->head_ptr->data + index);
            break; // Unreachable?

        default:
            // Unsupported Data-Type
            // Unreachable?
            return NULL;
    }

    // Unreachable
    return NULL;
}

static ErrorCode set_element_by_linear_index(MultiDimensionalMatrix* matrix, size_t index, void* value) {
    /*
        Set an element at the given position, but by its flat/linear index in the struct.

        Returns a custom `ErrorCode`
    */

    if (!matrix || !value || !matrix->head_ptr) {
        // One or both of matrix and value are the NULL-Pointer
        return ERR_NULL_PTR;
    }

    // Check if calculated index is out of bounds
    // Modifies the element with this index
    switch(matrix->head_ptr->data_type) {
        case TYPE_INT:
            if (index >= matrix->head_ptr->data_size / sizeof(int)) {
                return ERR_INVALID_INDEX;
            }
            ((int*)matrix->head_ptr->data)[index] = *(int*)value;
            break;

        case TYPE_FLOAT:
            if (index >= matrix->head_ptr->data_size / sizeof(float)) {
                return ERR_INVALID_INDEX;
            }
            ((float*)matrix->head_ptr->data)[index] = *(float*)value;
            break;
        
        case TYPE_DOUBLE:
            if (index >= matrix->head_ptr->data_size / sizeof(double)) {
                return ERR_INVALID_INDEX;
            }
            ((double*)matrix->head_ptr->data)[index] = *(double*)value;
            break;

        default:
            // Unsupported Data-Type
            // Unreachable?
            return ERR_INVALID_ARGS;
    }

    return ERR_NONE;

}

ErrorCode set_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices, void* value) {
    /*
        Set an element at the given position (indices)

        Returns a custom `ErrorCode`
    */

    if (!matrix || !indices || !matrix->head_ptr) {
        // Matrix/value/indices does not exist
        return ERR_NULL_PTR;
    }

    IndexCalcReturn return_data = calc_index(matrix,indices);

    if (return_data.error_code != ERR_NONE) {
        // An error occured while trying to calculate the index
        // Returns the received Error-Code from `calc_index`
        return return_data.error_code;
    }

    size_t index = return_data.index;

    return set_element_by_linear_index(matrix, index, value);
}


ErrorCode fill_matrix_from_static_array(MultiDimensionalMatrix* matrix, void* static_array) {
    /*
    
        Fill a given matrix with a static-array

        Assuming, that the given dimensions of the static-array are the same as of the matrix

        Returns a custom `ErrorCode` (should be `ERR_NONE` if no error occured)

    */

    if (!matrix || !static_array || !matrix->head_ptr) {
        // NULL-Pointer
        return ERR_NULL_PTR;
    }

    // Determine the size of each element
    size_t element_size = 0;

    switch (matrix->head_ptr->data_type) {
        case TYPE_INT:
            element_size = sizeof(int);
            break;
        case TYPE_FLOAT:
            element_size = sizeof(float);
            break;
        case TYPE_DOUBLE:
            element_size = sizeof(double);
            break;
        default:
            // Unsupported data_type
            // Reachable?
            return ERR_UNKNOWN;
    }

    size_t total_size = matrix->head_ptr->data_size / element_size;

    // Iterate through the array
    void* this_element;
    ErrorCode response;

    for (size_t i = 0; i < total_size; i++) {
        // Iterate byte-wise
        this_element = (void*) (((char*)static_array + (i * element_size)));

        // Set element at specific position
        response = set_element_by_linear_index(matrix, i, this_element);
        
        if (response != ERR_NONE) {
            // Couldn't set element in matrix
            return response;
        }

    }
    
    return ERR_NONE;

}


// Arithmetic Operations

ArithmeticOperationReturn add_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B) {
    /*

        Addition of two multidimensional-matrices

        Returns a custom-defined `ArithmeticOperationReturn`-struct, which contains the result-matrix (NULL-Pointer if an error occured)
        and an `ErrorCode`.
    
    */

    ArithmeticOperationReturn response;
    response.error_code = ERR_NONE;

    if (!matrix_A || !matrix_B || !matrix_A->head_ptr || !matrix_B->head_ptr) {
        // Wether `matrix_A` or `matrix_B` (or both) is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Check dimensions
    if (matrix_A->head_ptr->number_of_dimensions != matrix_B->head_ptr->number_of_dimensions) {
        // Cannot add two matrices with different dimensions
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    for (size_t i = 0; i < matrix_A->head_ptr->number_of_dimensions; i++) {
        if (matrix_A->head_ptr->dimensions[i] != matrix_B->head_ptr->dimensions[i]) {
            // Mismatch
            response.error_code = ERR_INVALID_ARGS;
            return response;
        }
    }
    
    // Check data_type
    if (matrix_A->head_ptr->data_type != matrix_B->head_ptr->data_type) {
        // Cannot add two matrices with different data-types
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    // Creating the `result_matrix`
    MultiDimensionalMatrix result_matrix;
    
    ErrorCode matrix_creation_resp = create_matrix(&result_matrix, matrix_A->head_ptr->number_of_dimensions, matrix_A->head_ptr->dimensions, matrix_A->head_ptr->data_type);

    if (matrix_creation_resp != ERR_NONE) {
        // Something went wrong while trying to create the matrix
        response.error_code = matrix_creation_resp;
        return response;
    }

    response.result_matrix = result_matrix;

    // Calculate the sum of both matrices
    
    size_t total_elements;

    switch(matrix_A->head_ptr->data_type) {
        case TYPE_INT:
            total_elements = matrix_A->head_ptr->data_size / sizeof(int);
            int* int_dataA = (int*)matrix_A->head_ptr->data;
            int* int_dataB = (int*)matrix_B->head_ptr->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((int*)result_matrix.head_ptr->data)[i] = int_dataA[i] + int_dataB[i];
            }
            break;
        
        case TYPE_FLOAT:
            total_elements = matrix_A->head_ptr->data_size / sizeof(float);
            float* float_dataA = (float*)matrix_A->head_ptr->data;
            float* float_dataB = (float*)matrix_B->head_ptr->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((float*)result_matrix.head_ptr->data)[i] = float_dataA[i] + float_dataB[i];
            }
            break;

        case TYPE_DOUBLE:
            total_elements = matrix_A->head_ptr->data_size / sizeof(double);
            double* double_dataA = (double*)matrix_A->head_ptr->data;
            double* double_dataB = (double*)matrix_B->head_ptr->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((double*)result_matrix.head_ptr->data)[i] = double_dataA[i] + double_dataB[i];
            }
            break;

        default:
            // Unsupported Data_Type
            // This section shouldn't be reached
            response.error_code = ERR_UNKNOWN;
            clear_matrix(&result_matrix);
            return response;
    }

    // Successfully added two matrices
    return response;

}


ArithmeticOperationReturn multiply_2d_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B) {
    /*
        Multiplication of two 2-Dimensional-matrices

        Returns a custom-defined `ArithmeticOperationReturn`-struct, which contains the result-matrix (NULL-Pointer if an error occured)
        and an `ErrorCode`.
    */

    ArithmeticOperationReturn response;
    response.error_code = ERR_NONE;

    if (!matrix_A || !matrix_B || !matrix_A->head_ptr || !matrix_B->head_ptr) {
        // Wether `matrix_A` or `matrix_B` is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Check dimensions
    if (matrix_A->head_ptr->number_of_dimensions != 2 || matrix_B->head_ptr->number_of_dimensions != 2) {
        // The given matrices are not 2-Dimensional
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    if (matrix_A->head_ptr->dimensions[1] != matrix_B->head_ptr->dimensions[0]) {
        // Columns of matrix_A aren't equal to the rows of matrix_B
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }
    

    // Check data_type
    if (matrix_A->head_ptr->data_type != matrix_B->head_ptr->data_type) {
        // Cannot multiply two matrices with different data-types
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    // Creating the `result_matrix`
    size_t result_matrix_dimensions[] = {
        matrix_A->head_ptr->dimensions[0], matrix_B->head_ptr->dimensions[1]
    };

    MultiDimensionalMatrix result_matrix;

    ErrorCode matrix_creation_resp = create_matrix(&result_matrix, (size_t)2, result_matrix_dimensions, matrix_A->head_ptr->data_type);

    if (matrix_creation_resp != ERR_NONE) {
        // Something went wrong while trying to create the matrix
        response.error_code = matrix_creation_resp;
        return response;
    }

    response.result_matrix = result_matrix;

    // Calculate the product of both matrices

    size_t rows_A = matrix_A->head_ptr->dimensions[0], cols_A = matrix_A->head_ptr->dimensions[1];
    size_t cols_B = matrix_B->head_ptr->dimensions[1];

    switch(matrix_A->head_ptr->data_type) {
        case TYPE_INT:
            int* int_dataA = (int*)matrix_A->head_ptr->data;
            int* int_dataB = (int*)matrix_B->head_ptr->data; 
            int* int_result = (int*)result_matrix.head_ptr->data;

            for (size_t i = 0; i < rows_A; i++) {
                for (size_t j = 0; j < cols_B; j++) {
                    int_result[i * cols_B + j] = 0; // Initialize result element
                    for (size_t k = 0; k < cols_A; k++) {
                        int_result[i * cols_B + j] += int_dataA[i * cols_A + k] * int_dataB[k * cols_B + j];
                    }
                }
            }
            break;
        
        case TYPE_FLOAT:
            float* float_dataA = (float*)matrix_A->head_ptr->data;
            float* float_dataB = (float*)matrix_B->head_ptr->data; 
            float* float_result = (float*)result_matrix.head_ptr->data;

            for (size_t i = 0; i < rows_A; i++) {
                for (size_t j = 0; j < cols_B; j++) {
                    float_result[i * cols_B + j] = 0; // Initialize result element
                    for (size_t k = 0; k < cols_A; k++) {
                        float_result[i * cols_B + j] += float_dataA[i * cols_A + k] * float_dataB[k * cols_B + j];
                    }
                }
            }
            break;

        case TYPE_DOUBLE:
            double* double_dataA = (double*)matrix_A->head_ptr->data;
            double* double_dataB = (double*)matrix_B->head_ptr->data; 
            double* double_result = (double*)result_matrix.head_ptr->data;

            for (size_t i = 0; i < rows_A; i++) {
                for (size_t j = 0; j < cols_B; j++) {
                    double_result[i * cols_B + j] = 0; // Initialize result element
                    for (size_t k = 0; k < cols_A; k++) {
                        double_result[i * cols_B + j] += double_dataA[i * cols_A + k] * double_dataB[k * cols_B + j];
                    }
                }
            }
            break;

        default:
            // Unsupported Data_Type
            // This section shouldn't be reached
            response.error_code = ERR_UNKNOWN;
            clear_matrix(&result_matrix);
            return response;
    }

    // Successfully multiplied two 2D-matrices
    return response;
}


ArithmeticOperationReturn scalar_multiply_matrix(const MultiDimensionalMatrix* matrix, void* scalar) {
    /*

        Multiplication of a matrix and a scalar

        Returns a custom-defined `ArithmeticOperationReturn`-struct, which contains the result-matrix (NULL-Pointer if an error occured)
        and an `ErrorCode`.

    */

    ArithmeticOperationReturn response;
    response.error_code = ERR_NONE;

    if (!matrix || !scalar || !matrix->head_ptr) {
        // Wether `matrix` or `scalar` is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Creating the `result_matrix`
    MultiDimensionalMatrix result_matrix;

    ErrorCode matrix_creation_resp = create_matrix(&result_matrix,matrix->head_ptr->number_of_dimensions, matrix->head_ptr->dimensions, matrix->head_ptr->data_type);

    if (matrix_creation_resp != ERR_NONE) {
        // Something went wrong while trying to create the matrix
        response.error_code = matrix_creation_resp;
        return response;
    }

    response.result_matrix = result_matrix;

    // Iterate through matrix and multiply each element with the scalar

    switch(matrix->head_ptr->data_type) {
        case TYPE_INT:
            for (size_t i = 0; i < (matrix->head_ptr->data_size/sizeof(int)); i++) {
                ((int*)result_matrix.head_ptr->data)[i] = ((int*)matrix->head_ptr->data)[i] * *((int*)scalar);
            }
            break;
        
        case TYPE_FLOAT:
            for (size_t i = 0; i < (matrix->head_ptr->data_size/sizeof(float)); i++) {
                ((float*)result_matrix.head_ptr->data)[i] = ((float*)matrix->head_ptr->data)[i] * *((float*)scalar);
            }
            break;
        
        case TYPE_DOUBLE:
            for (size_t i = 0; i < (matrix->head_ptr->data_size/sizeof(double)); i++) {
                ((double*)result_matrix.head_ptr->data)[i] = ((double*)matrix->head_ptr->data)[i] * *((double*)scalar);
            }
            break;
        
        default:
            // Unsupported Data_Type
            // This section shouldn't be reached
            response.error_code = ERR_UNKNOWN;
            clear_matrix(&result_matrix);
            return response;
    }
    

    return response;
}