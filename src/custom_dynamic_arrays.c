#include "custom_dynamic_arrays.h"


static DynamicArray* add_node(DynamicArray* head_ptr, void* element, size_t size, int node_location) {
    /*
        Adding node to a specific location of the list

        When node-location is neither `LIST_START` nor `LIST_END`, 
        the node-location will be interpreted as an index.
    
    */

    // Checking given node-location
    if (node_location < LIST_END) {
        return NULL;
    }
    
    // Allocate space for the struct using `malloc`
    // Allocate space for the Node itself
    DynamicArray* current_ptr = (DynamicArray*)malloc(sizeof(DynamicArray));
    
    if (current_ptr == NULL) {
        // Allocation failed
        return NULL;
    }

    // Allocate space for the element using `malloc`

    current_ptr->element = malloc(size);
    
    if (current_ptr->element == NULL) {
        // Allocation failed
        free(current_ptr);
        return NULL;
    }

    memcpy(current_ptr->element,element,size);
    

    if (head_ptr == NULL || node_location == LIST_START) {
        // Assuming, that current node is required to be the (new) HEAD-Node

        current_ptr->previous_ptr = NULL;

        if (head_ptr != NULL) {
            // New HEAD-Node
            head_ptr->previous_ptr = current_ptr;
            current_ptr->next_ptr = head_ptr;

        } else {
            current_ptr->next_ptr = NULL;
        }

        return current_ptr;
    }

    // Aussuming, that the HEAD-Node is not NULL
    
    int counter = 0;
    DynamicArray* parse_ptr = head_ptr;

    // Node-location is any index
    while (parse_ptr->next_ptr != NULL && (counter != node_location || node_location != LIST_END)) {
        parse_ptr = parse_ptr->next_ptr;
        counter++;
    }

    if (counter != node_location && node_location != LIST_END) {
        // Given Node-Location is invalid
        // Index is larger than the list
        free(current_ptr->element);
        free(current_ptr); // Free allocated space
        return NULL;
    }

    if (parse_ptr->next_ptr == NULL || node_location == LIST_END) {
        // `parse_ptr` is the last element
        // Appending Node at the end

        parse_ptr->next_ptr = current_ptr;
        current_ptr->previous_ptr = parse_ptr;

        return current_ptr;
    }

    // Given Node-Location is another INDEX
    // In the middle of two Nodes

    current_ptr->next_ptr = parse_ptr->next_ptr;
    parse_ptr->next_ptr->previous_ptr = current_ptr;
    parse_ptr->next_ptr = current_ptr;
    current_ptr->previous_ptr = parse_ptr;
    
    return current_ptr;
}


DynamicArray* initialize_list(void* first_element, size_t size) {
    /*
        Initializes the Dynamic-Array by allocating the first-element
    
        Returns the NULL-Pointer if something went wrong
    */
    if (first_element == NULL) {
        // Cannot create list
        return NULL;
    }

    DynamicArray* head_ptr = add_node(NULL, first_element, size, LIST_START);

    // Created HEAD-Node
    return head_ptr;
}

ErrorCode append(DynamicArray* head_ptr, void* element, size_t size) {
    /*
        Add node to initialized Dynamic-Array at the end of the list
    
        Returns a self-defined `ErrorCode`
    */
    if (head_ptr == NULL || element == NULL) {
        // Neither the head_ptr nor the element must be NULL
        return ERR_NULL_PTR;
    }

    DynamicArray* current_ptr = add_node(head_ptr, element, size, LIST_END);
    
    if (current_ptr == NULL) {
        // Something while allocating new space went wrong
        return ERR_MALLOC_FAILED;
    }

    // Appended new element at the end of the list
    return ERR_NONE;
    
}

DynamicArray* delete_node(DynamicArray* head_ptr, DynamicArray* node) {
    /*
        Delete a single node

        Returns the new `head-pointer` if requied
        Returns otherwise `NULL`
    */

    if (head_ptr == NULL || node == NULL) {
        // HEAD-Pointer/Node cannot be NULL
        return NULL;
    }

    if (head_ptr->next_ptr != NULL) {
        // Invalid HEAD-Pointer
        // No Node should exist after the HEAD-Node
        return NULL;
    }

    // Get position of given Node

    if (node == head_ptr) {
        // Delete HEAD-Node
        DynamicArray* new_head_pointer = head_ptr->previous_ptr;
        head_ptr->previous_ptr->next_ptr = NULL;
        free(head_ptr->element);
        free(head_ptr);

        return new_head_pointer;
    }

    if (node->previous_ptr == NULL) {
        // Node is at the END of the List
        node->next_ptr->previous_ptr = NULL;
        free(node->element);
        free(node);

        return NULL;
    }

    // Node has to be in the middle of two Nodes
    // Despite this, checking if it's the case

    if (node->previous_ptr != NULL && node->next_ptr != NULL) {
        // Node is between two Nodes
        node->next_ptr->previous_ptr = node->previous_ptr;
        node->previous_ptr->next_ptr = node->next_ptr;
        free(node->element);
        free(node);

        return NULL;
    }

    // Something went wrong
    return NULL;

}

ErrorCode clear_list(DynamicArray* head_ptr) {
    /*
        Completely clear the Dynamic-Array

        Returns a custom `ErrorCode`
    */

    if (head_ptr == NULL) {
        // HEAD-Pointer/Node must not be NULL
        return ERR_NULL_PTR;
    }

    DynamicArray* current_ptr = head_ptr;

    while (current_ptr->next_ptr != NULL) {
        current_ptr = current_ptr->next_ptr;
        free(current_ptr->previous_ptr->element);
        free(current_ptr->previous_ptr);
    }

    free(current_ptr->element);
    free(current_ptr);

    return ERR_NONE;
}

unsigned int count_elements(DynamicArray* head_ptr) {
    /*
        Count all Nodes in the list

        Returns the amount
        Returns `UINT_MAX` if something went wrong
    */

    if (head_ptr == NULL) {
        // List does not exist
        return UINT_MAX; // Error-signal
    }

    DynamicArray* current_ptr = head_ptr;
    int counter = 0;

    while (current_ptr != NULL) {
        counter++;
        current_ptr = current_ptr->next_ptr;
    }

    return counter;
}

DynamicArray* dynamic_array_from_whole(void* static_array, size_t element_size, size_t elements_amount) {
    /*
        Tranfer a static-array as a whole into a custom dynamic array

        Returns the HEAD-Pointer if the transfer was successfull
        Returns the NULL-Pointer if the transfer failed
    */

    if (elements_amount <= 0 || static_array == NULL) {
        // No elements
        return NULL;
    }

    // Create HEAD-Node
    DynamicArray* head_ptr = initialize_list(static_array, element_size*elements_amount);

    if (head_ptr == NULL) {
        // Initialization failed
        return NULL;
    }
    
    // Transfer completed
    return head_ptr;
}


DynamicArray* dynamic_array_from_elements(void* static_array, size_t element_size, size_t elements_amount) {
    /*
        Tranfer a static-array (its elements) into a custom dynamic array

        Returns the HEAD-Pointer if the transfer was successfull
        Returns the NULL-Pointer if the transfer failed
    */

    if (elements_amount <= 0 || static_array == NULL) {
        // No elements
        return NULL;
    }

    unsigned char* ptr = (unsigned char*) static_array; // Cast to `unsigned char*` for byte-wise access

    // Get first element and create HEAD-Node
    DynamicArray* head_ptr = initialize_list(ptr,element_size); // „i = 0: ptr + (0 * element_size)”

    if (head_ptr == NULL) {
        // Initialization failed
        return NULL;
    }

    // Iterrate the static-array
    for (size_t i = 1; i < elements_amount; i++) {
        void* element_ptr = ptr + (i * element_size);

        if (append(head_ptr,element_ptr,element_size) != ERR_NONE) {
            clear_list(head_ptr);
            return NULL;
        }
    }
    
    // Transfer completed
    return head_ptr;
}

ErrorCode append_static_array_to_dynamic(DynamicArray* head_ptr, void* static_array, size_t element_size, size_t elements_amount) {
    /*
        Append the each element of a static-array to an existing custom dynamic array
        The elements are added at the end of the given dynamic-array

        Returns a custom `ErrorCode`
    */

    if (elements_amount <= 0) {
        // No elements
        return ERR_LIST_EMPTY;
    }

    if (head_ptr == NULL) {
        // List does not exist
        return ERR_NULL_PTR;
    }

    unsigned char* ptr = (unsigned char*) static_array; // Cast to `unsigned char*` for byte-wise access

    ErrorCode resp;

    // Iterrate the static-array
    for (size_t i = 0; i < elements_amount; i++) {
        void* element_ptr = ptr + (i * element_size);

        resp = append(head_ptr,element_ptr,element_size);

        if (resp != ERR_NONE) {
            // Not necessary
            // clear_list(head_ptr); // Clear the whole list!

            return resp;
        }
    }

    // Operation was successful
    return ERR_NONE;
}

void* get_element_by_index(DynamicArray* head_ptr, unsigned int index) {
    /*
        Get element of dynamic-array by its index

        Returns the element as a void-pointer
        Returns NULL if the index is invalid or an error occured
    */

    if (head_ptr == NULL) {
        // List does not exist
        return NULL;
    }

    // Iterate list

    DynamicArray* current_ptr = head_ptr;
    unsigned int counter = 0;

    while (current_ptr != NULL && counter != index) {
        current_ptr = current_ptr->next_ptr;
        counter++;
    }

    if (current_ptr == NULL) {
        // Index is out of bounds
        return NULL;
    }

    return current_ptr->element;
}

ErrorCode change_element_by_index(DynamicArray* head_ptr, unsigned int index, size_t element_size, void* new_element) {
    /*
        Change an existing elment in the List by its Index

        Returns a custom `ErrorCode`
    */

    if (head_ptr == NULL || new_element == NULL) {
        // List does not exist or new_element is Null
        return ERR_NULL_PTR;
    }

    // Iterate list

    DynamicArray* current_ptr = head_ptr;
    unsigned int counter = 0;

    while (current_ptr != NULL && counter != index) {
        current_ptr = current_ptr->next_ptr;
        counter++;
    }

    if (current_ptr == NULL) {
        // Index is out of bounds
        return ERR_INVALID_INDEX;
    }

    // Replace the element
    memcpy(current_ptr->element,new_element,element_size);

    // Changed element
    return ERR_NONE;
}