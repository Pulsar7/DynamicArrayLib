#include "custom_dynamic_matrices.h"


MultiDimensionalMatrix* create_matrix(size_t number_of_dimensions, size_t* dimensions, DataType data_type) {
    /*
        Create a multidimensional matrix

        Returns the Matrix as a pointer
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
        Set an element on the given position (indices)

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


MultiDimensionalMatrix* multiply_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B) {

}