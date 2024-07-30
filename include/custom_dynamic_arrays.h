#ifndef CUSTOM_DYNAMIC_ARRAYS_H
#define CUSTOM_DYNAMIC_ARRAYS_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h> // For UINT_MAX
#include <string.h>


typedef struct DynamicArray {
    void* element;
    struct DynamicArray* next_ptr;
    struct DynamicArray* previous_ptr;
} DynamicArray;

typedef enum NodeLocation {
    LIST_START = 0,
    LIST_END = -1
} NodeLocation;


typedef enum ErrorCode {
    ERR_NONE = 0x00,
    ERR_NULL_PTR = 0x01,
    ERR_MALLOC_FAILED = 0x02,
    ERR_INVALID_INDEX = 0x03,
    ERR_LIST_EMPTY = 0x04,
    ERR_INVALID_HEAD_PTR = 0x05,
    ERR_UNKNOWN = 0xFF
} ErrorCode;



// Functions
DynamicArray* initialize_list(void* first_element, size_t size);
static DynamicArray* add_node(DynamicArray* head_ptr, void* element, size_t size, int node_location);
ErrorCode append(DynamicArray* head_ptr, void* element, size_t size);
DynamicArray* delete_node(DynamicArray* head_ptr, DynamicArray* node);
ErrorCode clear_list(DynamicArray* head_ptr);
unsigned int count_elements(DynamicArray* head_ptr);
DynamicArray* dynamic_array_from_elements(void* static_array, size_t element_size, size_t elements_amount);
DynamicArray* dynamic_array_from_whole(void* static_array, size_t element_size, size_t elements_amount);
ErrorCode append_static_array_to_dynamic(DynamicArray* head_ptr, void* static_array, size_t element_size, size_t elements_amount);
void* get_element_by_index(DynamicArray* head_ptr, unsigned int index);
ErrorCode change_element_by_index(DynamicArray* head_ptr, unsigned int index, size_t element_size, void* new_element);


#endif // CUSTOM_DYNAMIC_ARRAYS_H