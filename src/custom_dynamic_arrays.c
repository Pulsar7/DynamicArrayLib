#include "custom_dynamic_arrays.h"


// Initialize list by adding the first-element and creating the head-pointer.
ErrorCode initialize_list(DynamicArray* dynamic_array, void* first_element, size_t element_size) {
    /*

        Returns an ErrorCode, which should be `ERR_NONE` if no error occured.
   
	    ERR_INVALID_ARGS	= Given element does not exist; Element size is invalid;

    */

    // Check arguments
    if (!first_element || element_size == 0) {
        return ERR_INVALID_ARGS;
    }

    dynamic_array->head_ptr = NULL;
    dynamic_array->tail_ptr = NULL;
    
    ErrorCode response = add_node(dynamic_array, first_element, element_size, LIST_START_POS);
    return response;
}

// Adds a node to the list at a given position.
ErrorCode add_node(DynamicArray* dynamic_array, void* element, size_t element_size, int index) {
    /*

        Returns an ErrorCode, which should be `ERR_NONE` if no error occured.
        Edits the head_ptr/tail_ptr if needed.
    
    */

    // Checks arguments
    if (!element || element_size == 0 || !dynamic_array || index < LIST_END_POS) {
        return ERR_INVALID_ARGS;
    }

    if (!dynamic_array->head_ptr) {
        // List is empty
        // A head-pointer has to be created

        DynamicArrayNode* new_head_ptr = (DynamicArrayNode*) malloc(sizeof(DynamicArrayNode));

        if (!new_head_ptr) {
            // allocation error
            return ERR_MALLOC_FAILED;
        }

        // Allocating space for the new element

        new_head_ptr->element = malloc(element_size);

        if (!new_head_ptr->element) {
            // allocation error
            free(new_head_ptr);
            return ERR_MALLOC_FAILED;
        }

        memcpy(new_head_ptr->element, element, element_size);

        new_head_ptr->next_ptr = NULL;
        new_head_ptr->previous_ptr = NULL;

        dynamic_array->head_ptr = new_head_ptr; // Update the real head-pointer
        dynamic_array->tail_ptr = new_head_ptr; // Tail-pointer is the head-pointer

        // Operation went successful
        return ERR_NONE;
    }

    //
    // Head-Pointer already exists
    //

    if (dynamic_array->head_ptr->previous_ptr != NULL) {
        // Invalid head-pointer
        // A head-pointer does not have any previous-nodes
        return ERR_INVALID_HEAD_PTR;
    }

    if (dynamic_array->tail_ptr->next_ptr != NULL) {
        // Invalid tail-pointer
        // A tail-pointer does not have any next-nodes
        return ERR_INVALID_TAIL_PTR;
    }

    if (index == LIST_START_POS) {
        // New node should be the new head-pointer
        DynamicArrayNode* new_head_ptr = (DynamicArrayNode*) malloc(sizeof(DynamicArrayNode));

        if (!new_head_ptr) {
            // allocation error
            return ERR_MALLOC_FAILED;
        }

        // Allocating space for the new element

        new_head_ptr->element = malloc(element_size);

        if (!new_head_ptr->element) {
            // allocation error
            free(new_head_ptr);
            return ERR_MALLOC_FAILED;
        }

        memcpy(new_head_ptr->element, element, element_size);

        dynamic_array->head_ptr->previous_ptr = new_head_ptr; // Connect old head-pointer with the new one
        dynamic_array->head_ptr = new_head_ptr; // Update the real head-pointer


        // Operation went successful
        return ERR_NONE;
    }

    if (index == LIST_END_POS) {
        // New node should be the new tail-pointer
        DynamicArrayNode* new_tail_ptr = (DynamicArrayNode*) malloc(sizeof(DynamicArrayNode));

        if (!new_tail_ptr) {
            // allocation error
            return ERR_MALLOC_FAILED;
        }

        // Allocating space for the new element

        new_tail_ptr->element = malloc(element_size);

        if (!new_tail_ptr->element) {
            // allocation error
            free(new_tail_ptr);
            return ERR_MALLOC_FAILED;
        }

        memcpy(new_tail_ptr->element, element, element_size);

        dynamic_array->tail_ptr->next_ptr = new_tail_ptr; // Connect old tail-pointer with the new one
        new_tail_ptr->previous_ptr = dynamic_array->tail_ptr; // Old tail-pointer is now the new previous-ptr of the new tail-pointer
        dynamic_array->tail_ptr = new_tail_ptr; // Update the real tail-pointer

        // Operation went successful
        return ERR_NONE;
    }

    // Iterate list

    DynamicArrayNode* current_ptr = dynamic_array->head_ptr;
    int counter = 0;

    while (current_ptr != NULL && counter != index) {
        current_ptr = current_ptr->next_ptr;
        counter++;
    }

    if (current_ptr == NULL) {
        // Index is out of boundaries
        return ERR_INVALID_INDEX;
    }

    // Given index is valid
    DynamicArrayNode* new_node = (DynamicArrayNode*) malloc(sizeof(DynamicArrayNode));
    
    if (!new_node) {
        // allocation error
        return ERR_MALLOC_FAILED;
    }

    // Allocating space for the new element

    new_node->element = malloc(element_size);

    if (!new_node->element) {
        // allocation error
        free(new_node);
        return ERR_MALLOC_FAILED;
    }

    memcpy(new_node->element, element, element_size);

    if (current_ptr->next_ptr == NULL) {
        // Current pointer has to be the tail-pointer
        if (current_ptr != dynamic_array->tail_ptr) {
            return ERR_INVALID_TAIL_PTR;
        }
    }

    // Save new-node behind new tail-node
    current_ptr->previous_ptr->next_ptr = new_node;
    new_node->previous_ptr = current_ptr->previous_ptr;
    current_ptr->previous_ptr = new_node;
    new_node->next_ptr = current_ptr;

    // Operation went successful
    return ERR_NONE;

}

// Append element at the end of the list
ErrorCode append_to_list(DynamicArray* dynamic_array, void* element, size_t element_size) {
    /*

        Returns an ErrorCode, which should be `ERR_NONE` if no error occured.
        Updates the tail-pointer.
    
    */

    if (!dynamic_array || !element || element_size == 0) {
        // Invalid arguments
        return ERR_INVALID_ARGS;
    }

    ErrorCode response = add_node(dynamic_array, element, element_size, LIST_END_POS);
    return response;
}

// Deallocates the whole given list.
ErrorCode clear_list(DynamicArray* dynamic_array) {
    /*

        Returns an ErrorCode, which should be `ERR_NONE` if no error occured.
        Sets the head- & tail-pointer to `NULL`.
    
    */

    if (!dynamic_array) {
        // List doesn't exist
        return ERR_INVALID_ARGS;
    }

    if (!dynamic_array->head_ptr) {
        // Invalid head-pointer
        return ERR_INVALID_HEAD_PTR;
    }

    if (!dynamic_array->tail_ptr) {
        // Invalid tail-pointer
        return ERR_INVALID_TAIL_PTR;
    }

    DynamicArrayNode* current_ptr = dynamic_array->head_ptr;

    while (current_ptr->next_ptr != NULL) {
        if (current_ptr->element) {
            free(current_ptr->element);
        }
        current_ptr = current_ptr->next_ptr;
        free(current_ptr->previous_ptr);
    }

    if (current_ptr->element) {
        free(current_ptr->element);
    }

    free(current_ptr);

    dynamic_array->head_ptr = NULL;
    dynamic_array->tail_ptr = NULL;
    
    return ERR_NONE;
}

// Count all nodes in the list
size_t count_list_elements(DynamicArray* dynamic_array) {
    /*

        Returns the amount.
        Returns `0` if an error occured.

    */

    size_t counter = 0;

    if (!dynamic_array) {
        // List does not exist
        return counter;
    }

    if (!dynamic_array->head_ptr || !dynamic_array->tail_ptr) {
        // Invalid list
        return counter;
    }

    DynamicArrayNode* current_ptr = dynamic_array->head_ptr;

    while (current_ptr != NULL) {
        counter++;
        current_ptr = current_ptr->next_ptr;
    }

    return counter;
}

// Get element by index
void* get_list_element_by_index(DynamicArray* dynamic_array, int index) {
    /*

        Returns the reference to the element.
        Returns the NULL-pointer if something went wrong.
    
    */

    if (!dynamic_array || index < LIST_END_POS) {
        // List does not exist or given index is invalid
        return NULL;
    }

    if (index == LIST_START_POS) {
        return dynamic_array->head_ptr->element;
    }

    if (index == LIST_END_POS) {
        return dynamic_array->tail_ptr->element;
    }

    int counter = 0;
    DynamicArrayNode* current_ptr = dynamic_array->head_ptr;

    while (current_ptr != NULL && counter != index) {
        current_ptr = current_ptr->next_ptr;
        counter++;
    }

    if (current_ptr == NULL) {
        // Index is out of boundaries
        return NULL;
    }

    return current_ptr->element;
}

// Set element by index
ErrorCode set_list_element_by_index(DynamicArray* dynamic_array, int index, void* element, size_t element_size) {
    /*

        Returns an ErrorCode, which should be `ERR_NONE` if no error occured.
    
    */

    if (!dynamic_array || index < LIST_END_POS) {
        return ERR_INVALID_ARGS;
    }

    void* new_element_pointer = NULL;

    if (index == LIST_START_POS) {
        new_element_pointer = dynamic_array->head_ptr->element;
    }

    if (index == LIST_END_POS) {
        new_element_pointer = dynamic_array->tail_ptr->element;
    }


    if (!new_element_pointer) {
        DynamicArrayNode* current_ptr = dynamic_array->head_ptr;
        int counter = 0;
        while (current_ptr != NULL && counter != index) {
            current_ptr = current_ptr->next_ptr;
            counter++;
        }

        if (!current_ptr) {
            // Index is out of boundaries
            return ERR_INVALID_INDEX;
        }

        new_element_pointer = current_ptr->element;
    }

    void* this_element = realloc(new_element_pointer, element_size);
    if (!this_element) {
        // Reallocation-Error
        return ERR_REALLOC_FAILED;
    }
    memcpy(new_element_pointer, element, element_size);
    return ERR_NONE;
}
