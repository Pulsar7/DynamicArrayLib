#ifndef CUSTOM_DYNAMIC_ARRAYS_H
#define CUSTOM_DYNAMIC_ARRAYS_H

#include <stdlib.h>
#include <limits.h> // For UINT_MAX
#include <string.h>

#include "constants.h"


typedef struct DynamicArrayNode {
    void* element;
    struct DynamicArrayNode* next_ptr;
    struct DynamicArrayNode* previous_ptr;
} DynamicArrayNode;

typedef enum SpecialNodePosition {
    LIST_START_POS = 0,
    LIST_END_POS = -1
} SpecialNodePosition;

typedef struct NodeOperationReturn {
    ErrorCode error_code;
    DynamicArrayNode* ptr;
} NodeOperationReturn;

typedef struct DynamicArray {
    DynamicArrayNode* head_ptr;
    DynamicArrayNode* tail_ptr;
} DynamicArray;


//
// Functions
//

ErrorCode initialize_list(DynamicArray* dynamic_array, void* first_element, size_t element_size);
ErrorCode add_node(DynamicArray* dynamic_array, void* element, size_t element_size, int index);
ErrorCode append_to_list(DynamicArray* dynamic_array, void* element, size_t element_size);
ErrorCode clear_list(DynamicArray* dynamic_array);
size_t count_list_elements(DynamicArray* dynamic_array);
void* get_list_element_by_index(DynamicArray* dynamic_array, int index);
ErrorCode set_list_element_by_index(DynamicArray* dynamic_array, int index, void* element, size_t element_size);


#endif // CUSTOM_DYNAMIC_ARRAYS_H