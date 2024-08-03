#include "custom_dynamic_matrices.h"


MultiDimensionalMatrix* create_matrix(size_t number_of_dimensions, size_t* dimensions, DataType data_type) {
    /*
        Create a multidimensional matrix

        Returns the matrix as a pointer
        Returns a NULL-Pointer if an error occured
    */

    if (number_of_dimensions == 0) {
        return NULL;
    }

    if (!dimensions) {
        // No dimensions given
        return NULL;
    }

    // Allocate space for the Matrix-Node itself
    MultiDimensionalMatrix* matrix = (MultiDimensionalMatrix*) malloc(sizeof(MultiDimensionalMatrix));

    if (!matrix) {
        // Allocation-Error
        return NULL;
    }

    matrix->number_of_dimensions = number_of_dimensions;

    // Allocate space for the dimensions-array
    matrix->dimensions = (size_t*) malloc(number_of_dimensions * sizeof(size_t));

    if (!matrix->dimensions) {
        // Allocation-Error
        free(matrix);
        return NULL;
    }

    memcpy(matrix->dimensions, dimensions, number_of_dimensions * sizeof(size_t));

    // Get total size of dimensions
    size_t total_size = 1;

    for (size_t i = 0; i < number_of_dimensions; i++) {
        total_size *= dimensions[i];
    }

    switch(data_type) {
        case TYPE_INT:
            matrix->data = malloc(total_size * sizeof(int));
            matrix->data_size = total_size * sizeof(int);
            break;
        
        case TYPE_FLOAT:
            matrix->data = malloc(total_size * sizeof(float));
            matrix->data_size = total_size * sizeof(float);
            break;

        case TYPE_DOUBLE:
            matrix->data = malloc(total_size * sizeof(double));
            matrix->data_size = total_size * sizeof(double);
            break;

        default:
            // Should be unreachable
            // Not supported Data-Type
            matrix->data = NULL;
            break;
    }

    if (!matrix->data) {
        // Allocation-Error or invalid Data-Type
        free(matrix->dimensions);
        free(matrix);
        return NULL;
    }

    matrix->data_type = data_type;

    // Operation was successful
    return matrix;
}


void clear_matrix(MultiDimensionalMatrix* matrix) {
    /*
        Free all allocated space
    */

    if (!matrix) {
        // Invalid matrix
        return;
    }

    if (matrix->data) {
        free(matrix->data);
    }

    if (matrix->dimensions) {
        free(matrix->dimensions);
    }

    free(matrix);

    return;
}

static IndexCalcReturn calc_index(MultiDimensionalMatrix* matrix, size_t* indices) {
    /*
        Calculate the index of the 1-Dimensional-array with the given multidimensional indices

        Returns the custom `IndexCalcReturn`-struct in order to return the index and
        a self-defined Error-Code, if something went wrong.
    */

    IndexCalcReturn return_data;

    if (!indices || !matrix) {
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

    for (int i = matrix->number_of_dimensions - 1; i >= 0; --i) {
        index += indices[i] * offset;
        offset *= matrix->dimensions[i];
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

    if (!matrix || !indices) {
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
    switch(matrix->data_type) {
        case TYPE_INT:
            if (index >= matrix->data_size / sizeof(int)) {
                return NULL;
            }
            return (void*)((int*)matrix->data + index);
            break; // Unreachable?

        case TYPE_FLOAT:
            if (index >= matrix->data_size / sizeof(float)) {
                return NULL;
            }
            return (void*)((float*)matrix->data + index);
            break; // Unreachable?
        
        case TYPE_DOUBLE:
            if (index >= matrix->data_size / sizeof(double)) {
                return NULL;
            }
            return (void*)((double*)matrix->data + index);
            break; // Unreachable?

        default:
            // Unsupported Data-Type
            // Unreachable?
            return NULL;
    }

    // Unreachable
    return NULL;
}

ErrorCode set_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices, void* value) {
    /*
        Set an element at the given position (indices)

        Returns a custom `ErrorCode`
    */

    if (!matrix || !indices) {
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

static ErrorCode set_element_by_linear_index(MultiDimensionalMatrix* matrix, size_t index, void* value) {
    /*
        Set an element at the given position, but by its flat/linear index in the struct.

        Returns a custom `ErrorCode`
    */

    if (!matrix || !value) {
        // One or both of matrix and value are the NULL-Pointer
        return ERR_NULL_PTR;
    }

    // Check if calculated index is out of bounds
    // Modifies the element with this index
    switch(matrix->data_type) {
        case TYPE_INT:
            if (index >= matrix->data_size / sizeof(int)) {
                return ERR_INVALID_INDEX;
            }
            ((int*)matrix->data)[index] = *(int*)value;
            break;

        case TYPE_FLOAT:
            if (index >= matrix->data_size / sizeof(float)) {
                return ERR_INVALID_INDEX;
            }
            ((float*)matrix->data)[index] = *(float*)value;
            break;
        
        case TYPE_DOUBLE:
            if (index >= matrix->data_size / sizeof(double)) {
                return ERR_INVALID_INDEX;
            }
            ((double*)matrix->data)[index] = *(double*)value;
            break;

        default:
            // Unsupported Data-Type
            // Unreachable?
            return ERR_INVALID_ARGS;
    }

    return ERR_NONE;

}


ErrorCode fill_matrix_from_static_array(MultiDimensionalMatrix* matrix, void* static_array, size_t* static_dimensions, size_t number_of_dimensions, DataType data_type) {
    /*
    
        Fill a given matrix with a static-array

        Returns a custom `ErrorCode` (should be `ERR_NONE` if no error occured)

    */

    if (!matrix || !static_array || !static_dimensions) {
        // NULL-Pointer
        return ERR_NULL_PTR;
    }

    // Check data type
    if (matrix->data_type != data_type) {
        // Matrix-data-type is not the same as the given static-array data-type
        return ERR_INVALID_ARGS;
    }

    // Check dimensions
    if (matrix->number_of_dimensions != number_of_dimensions) {
        // Dimension mismatch
        return ERR_INVALID_ARGS;
    }

    // Determine the size of each element
    size_t element_size = 0;

    switch (data_type) {
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

    size_t total_size = matrix->data_size / element_size;

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
    response.result_matrix = NULL;
    response.error_code = ERR_NONE;

    if (!matrix_A || !matrix_B) {
        // Wether `matrix_A` or `matrix_B` (or both) is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Check dimensions
    if (matrix_A->number_of_dimensions != matrix_B->number_of_dimensions) {
        // Cannot add two matrices with different dimensions
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    for (size_t i = 0; i < matrix_A->number_of_dimensions; i++) {
        if (matrix_A->dimensions[i] != matrix_B->dimensions[i]) {
            // Mismatch
            response.error_code = ERR_INVALID_ARGS;
            return response;
        }
    }
    
    // Check data_type
    if (matrix_A->data_type != matrix_B->data_type) {
        // Cannot add two matrices with different data-types
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    // Creating the `result_matrix`
    MultiDimensionalMatrix* result_matrix = create_matrix(matrix_A->number_of_dimensions, matrix_A->dimensions, matrix_A->data_type);

    if (!result_matrix) {
        // Couldn't create the `result_matrix`
        response.error_code = ERR_UNKNOWN;
        return response;
    }

    response.result_matrix = result_matrix;

    // Calculate the sum of both matrices
    
    size_t total_elements;

    switch(matrix_A->data_type) {
        case TYPE_INT:
            total_elements = matrix_A->data_size / sizeof(int);
            int* int_dataA = (int*)matrix_A->data;
            int* int_dataB = (int*)matrix_B->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((int*)result_matrix->data)[i] = int_dataA[i] + int_dataB[i];
            }
            break;
        
        case TYPE_FLOAT:
            total_elements = matrix_A->data_size / sizeof(float);
            float* float_dataA = (float*)matrix_A->data;
            float* float_dataB = (float*)matrix_B->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((float*)result_matrix->data)[i] = float_dataA[i] + float_dataB[i];
            }
            break;

        case TYPE_DOUBLE:
            total_elements = matrix_A->data_size / sizeof(double);
            double* double_dataA = (double*)matrix_A->data;
            double* double_dataB = (double*)matrix_B->data; 
            for (size_t i = 0; i < total_elements; i++) {
                ((double*)result_matrix->data)[i] = double_dataA[i] + double_dataB[i];
            }
            break;

        default:
            // Unsupported Data_Type
            // This section shouldn't be reached
            response.error_code = ERR_UNKNOWN;
            clear_matrix(result_matrix);
            response.result_matrix = NULL; // required?
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
    response.result_matrix = NULL;
    response.error_code = ERR_NONE;

    if (!matrix_A || !matrix_B) {
        // Wether `matrix_A` or `matrix_B` is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Check dimensions
    if (matrix_A->number_of_dimensions != 2 || matrix_B->number_of_dimensions != 2) {
        // The given matrices are not 2-Dimensional
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    if (matrix_A->dimensions[1] != matrix_B->dimensions[0]) {
        // Columns of matrix_A aren't equal to the rows of matrix_B
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }
    

    // Check data_type
    if (matrix_A->data_type != matrix_B->data_type) {
        // Cannot multiply two matrices with different data-types
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    // Creating the `result_matrix`
    size_t result_matrix_dimensions[] = {
        matrix_A->dimensions[0], matrix_B->dimensions[1]
    };
    MultiDimensionalMatrix* result_matrix = create_matrix((size_t)2, result_matrix_dimensions, matrix_A->data_type);

    if (!result_matrix) {
        // Couldn't create the `result_matrix`
        response.error_code = ERR_UNKNOWN;
        return response;
    }

    response.result_matrix = result_matrix;

    // Calculate the product of both matrices

    size_t rows_A = matrix_A->dimensions[0], cols_A = matrix_A->dimensions[1];
    size_t cols_B = matrix_B->dimensions[1];

    switch(matrix_A->data_type) {
        case TYPE_INT:
            int* int_dataA = (int*)matrix_A->data;
            int* int_dataB = (int*)matrix_B->data; 
            int* int_result = (int*)result_matrix->data;

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
            float* float_dataA = (float*)matrix_A->data;
            float* float_dataB = (float*)matrix_B->data; 
            float* float_result = (float*)result_matrix->data;

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
            double* double_dataA = (double*)matrix_A->data;
            double* double_dataB = (double*)matrix_B->data; 
            double* double_result = (double*)result_matrix->data;

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
            clear_matrix(result_matrix);
            response.result_matrix = NULL; // required?
            return response;
    }

    // Successfully multiplied two 2D-matrices
    return response;
}


ArithmeticOperationReturn scalar_multiply_matrix(const MultiDimensionalMatrix* matrix, void* scalar, DataType data_type) {
    /*

        Multiplication of a matrix and a scalar

        Returns a custom-defined `ArithmeticOperationReturn`-struct, which contains the result-matrix (NULL-Pointer if an error occured)
        and an `ErrorCode`.

    */

    ArithmeticOperationReturn response;
    response.error_code = ERR_NONE;
    response.result_matrix = NULL;

    if (!matrix || !scalar) {
        // Wether `matrix_A` or `matrix_B` is a NULL-Pointer
        response.error_code = ERR_NULL_PTR;
        return response;
    }

    // Check data-type
    if (matrix->data_type != data_type) {
        // Mismatch
        response.error_code = ERR_INVALID_ARGS;
        return response;
    }

    // Creating the `result_matrix`
    MultiDimensionalMatrix* result_matrix = create_matrix(matrix->number_of_dimensions, matrix->dimensions, data_type);

    if (!result_matrix) {
        // Couldn't create the `result_matrix`
        response.error_code = ERR_UNKNOWN;
        return response;
    }

    response.result_matrix = result_matrix;

    // Iterate through matrix and multiply each element with the scalar

    switch(data_type) {
        case TYPE_INT:
            for (size_t i = 0; i < (matrix->data_size/sizeof(int)); i++) {
                ((int*)result_matrix->data)[i] = ((int*)matrix->data)[i] * *((int*)scalar);
            }
            break;
        
        case TYPE_FLOAT:
            for (size_t i = 0; i < (matrix->data_size/sizeof(float)); i++) {
                ((float*)result_matrix->data)[i] = ((float*)matrix->data)[i] * *((float*)scalar);
            }
            break;
        
        case TYPE_DOUBLE:
            for (size_t i = 0; i < (matrix->data_size/sizeof(double)); i++) {
                ((double*)result_matrix->data)[i] = ((double*)matrix->data)[i] * *((double*)scalar);
            }
            break;
        
        default:
            // Unsupported Data_Type
            // This section shouldn't be reached
            response.error_code = ERR_UNKNOWN;
            clear_matrix(result_matrix);
            return response;
    }
    

    return response;
}