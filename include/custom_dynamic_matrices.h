#ifndef CUSTOM_DYNAMIC_MATRICES_H
#define CUSTOM_DYNAMIC_MATRICES_H

#include "constants.h"

#include <stdlib.h>
#include <string.h>


typedef enum DataType {
    TYPE_NOT_SET_YET,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE
} DataType;


typedef struct MultiDimensionalMatrixNode {
    void* data;
    size_t* dimensions;          // For example 3 x 2 x 2 matrix has the dimensions := {3, 2, 2}
    size_t number_of_dimensions; // `len(dimensions)`
    DataType data_type;
    size_t data_size;            // Size of the data-array (based on the data-type)
} MultiDimensionalMatrixNode;

typedef struct MultiDimensionalMatrix {
    MultiDimensionalMatrixNode* head_ptr;

} MultiDimensionalMatrix;

typedef struct IndexCalcReturn {
    size_t index;
    ErrorCode error_code;
} IndexCalcReturn;

// Return-Object for every matrix-related arithmetic operation
typedef struct ArithmeticOperationReturn {
    MultiDimensionalMatrix result_matrix;
    ErrorCode error_code;
} ArithmeticOperationReturn;


//
// Functions
//

ErrorCode create_matrix(MultiDimensionalMatrix* matrix, size_t number_of_dimensions, size_t* dimensions, DataType data_type);
void clear_matrix(MultiDimensionalMatrix* matrix);
//static IndexCalcReturn calc_index(MultiDimensionalMatrix* matrix, size_t* indices);
void* get_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices);
ErrorCode set_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices, void* value);
//static ErrorCode set_element_by_linear_index(MultiDimensionalMatrix* matrix, size_t index, void* value);
ErrorCode fill_matrix_from_static_array(MultiDimensionalMatrix* matrix, void* static_array);
ArithmeticOperationReturn add_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B);
ArithmeticOperationReturn multiply_2d_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B);
ArithmeticOperationReturn scalar_multiply_matrix(const MultiDimensionalMatrix* matrix, void* scalar);
ErrorCode resize_matrix(MultiDimensionalMatrix* matrix, size_t new_number_of_dimensions, size_t* new_dimensions);
//static ErrorCode update_data_type(MultiDimensionalMatrix* matrix, DataType data_type);
ErrorCode change_data_type(MultiDimensionalMatrix* matrix, DataType new_data_type);
ErrorCode printout_matrix(MultiDimensionalMatrix* matrix);

#endif // CUSTOM_DYNAMIC_MATRICES_H
