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


int main(int argc, const char** argv) {
    size_t* dimensions = {1, 2, 3};

    MultiDimensionalMatrix* matrix = create_matrix(3,dimensions,TYPE_INT);

    if (!matrix) {
        printf("Couldn't create matrix\n");
        return 1;
    }

    int number = 9;

    size_t* indices = {1, 1, 1};

    if (set_element_by_indices(matrix,indices,&number) != ERR_NONE) {
        printf("Couldn't set element\n");
    }

    clear_matrix(matrix);

    return 0;
}