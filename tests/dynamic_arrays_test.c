#include "dynamic_array_test.h"


void test_initialize_list() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    clear_list(&list);
}

void test_append_to_list() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    clear_list(&list);
}

void test_count_elements() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 1);

    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 2);

    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 3);

    clear_list(&list);
}

void test_clear_list() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 1);

    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 2);

    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    assert(count_list_elements(&list) == 3);

    assert(clear_list(&list) == ERR_NONE);
}

void test_get_list_element_by_index() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    first_element++;
    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    void* element = get_list_element_by_index(&list, LIST_END_POS);
    assert(*(int*)element == 10);

    element = get_list_element_by_index(&list, LIST_START_POS);
    assert(*(int*)element == 9);
}

void test_set_list_element_by_index() {
    int first_element = 9;
    DynamicArray list;
    ErrorCode err = initialize_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    first_element++;
    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    first_element++;
    err = append_to_list(&list, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    first_element++;
    err = set_list_element_by_index(&list, LIST_START_POS, (void*)&first_element, sizeof(int));
    assert(err == ERR_NONE);

    void* element = get_list_element_by_index(&list, LIST_START_POS);
    assert(*(int*)element == 12);
}



