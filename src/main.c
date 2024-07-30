#include "main.h"


int main(int argc, const char** argv) {
    DynamicArray* head_ptr = NULL;

    // Copy every single element of a static-array in a dynamic-array
    int numbers[] = {1,2,3,4,5};

    size_t element_size = sizeof(int);

    head_ptr = dynamic_array_from_elements(numbers, element_size, sizeof(numbers) / element_size);

    if (head_ptr == NULL) {
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        printf("(i=%d) ELEMENT=%d\n",i,*(int*)get_element_by_index(head_ptr,i));
    }

    numbers[0] = 9;
    
    for (int i = 0; i < 5; i++) {
        printf("(i=%d) ELEMENT=%d\n",i,*(int*)get_element_by_index(head_ptr,i));
    }
    
    clear_list(head_ptr);



    // Copy a static-array as a whole in a dynamic-array

    head_ptr = dynamic_array_from_whole(numbers, element_size, sizeof(numbers) / element_size);

    if (head_ptr == NULL) {
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        int* this_elements = (int*)head_ptr->element;
        printf("(i=%d) ELEMENT=%d\n",i,this_elements[i]);
    }

    clear_list(head_ptr);



    // Test with string

    char* message = "Hello World";

    head_ptr = dynamic_array_from_whole(message, element_size, strlen(message));

    if (head_ptr == NULL) {
        return 1;
    }

    printf("%s\n",(char*)head_ptr->element);

    // Or accessing it via `get_element_by_index`

    printf("%s\n",(char*)get_element_by_index(head_ptr,0));

    clear_list(head_ptr);

    return 0;
}