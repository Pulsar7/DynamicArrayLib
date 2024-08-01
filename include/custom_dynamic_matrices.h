#ifndef CUSTOM_DYNAMIC_MATRICES_H
#define CUSTOM_DYNAMIC_MATRICES_H

#include "constants.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h> // For debugging


typedef enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE
} DataType;


typedef struct MultiDimensionalMatrix {
    void* data;
    size_t* dimensions;          // For example 3 x 2 x 2 matrix has the dimensions := {3, 2, 2}
    size_t number_of_dimensions; // `len(dimensions)`
    DataType data_type; 
    size_t data_size;            // Size of the data-array (based on the data-type)
} MultiDimensionalMatrix;   

typedef struct IndexCalcReturn {
    size_t index;
    ErrorCode error_code;
} IndexCalcReturn;

typedef struct ArithmeticOperationReturn {
    MultiDimensionalMatrix* result_matrix;
    ErrorCode error_code;  
} ArithmeticOperationReturn;



// Functions

MultiDimensionalMatrix* create_matrix(size_t number_of_dimensions, size_t* dimensions, DataType data_type);
void clear_matrix(MultiDimensionalMatrix* matrix);
static IndexCalcReturn calc_index(MultiDimensionalMatrix* matrix, size_t* indices);
void* get_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices);
ErrorCode set_element_by_indices(MultiDimensionalMatrix* matrix, size_t* indices, void* value);
ErrorCode fill_matrix_from_static_array(MultiDimensionalMatrix* matrix, void* static_array, size_t* static_dimensions, size_t number_of_dimensions, DataType data_type);
ArithmeticOperationReturn add_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B);
ArithmeticOperationReturn multiply_2d_matrices(const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B);


#endif // CUSTOM_DYNAMIC_MATRICES_H