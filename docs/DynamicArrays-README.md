<h1>Dynamic Arrays</h1>


<h2>Table of contents</h2>

- [`initialize_list`](#initialize_list)
  - [Usage \& Example](#usage--example)
- [`dynamic_array_from_elements`](#dynamic_array_from_elements)
  - [Usage \& Example](#usage--example-1)
- [`dynamic_array_from_whole`](#dynamic_array_from_whole)
  - [Usage \& Example](#usage--example-2)


## `initialize_list`

The function should be called to create a list and returns its head-pointer. If an error occurs, the function will return a `NULL`-Pointer.

### Usage & Example

```C
int numbber = 1337;

DynamicArray* head_ptr = initialize_list((void*)&number, sizeof(number));

if (head_ptr == NULL) {
    return 1;
}

// Deallocate DynamicArray
clear_list(head_ptr);
```



## `dynamic_array_from_elements`

This function copies the actual values of the given static-array.


### Usage & Example

```C
int numbers[] = {1,2,3,4,5};

size_t element_size = sizeof(int);

DynamicArray* head_ptr = dynamic_array_from_elements(numbers, element_size, sizeof(numbers) / element_size);

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

// Deallocate DynamicArray
clear_list(head_ptr);
```

The output would be:

```
(i=0) ELEMENT=1
(i=1) ELEMENT=2
(i=2) ELEMENT=3
(i=3) ELEMENT=4
(i=4) ELEMENT=5
(i=0) ELEMENT=1
(i=1) ELEMENT=2
(i=2) ELEMENT=3
(i=3) ELEMENT=4
(i=4) ELEMENT=5
```

While the static-array has been modified, the values in the dynamic-list remain unmodified.


## `dynamic_array_from_whole`

This function copies the the pointers to its elements and saves them into a dynamic-array.


### Usage & Example 

```C
int numbers[] = {1,2,3,4,5};

size_t element_size = sizeof(int);

DynamicArray* head_ptr = dynamic_array_from_whole(numbers, element_size, sizeof(numbers) / element_size);

if (head_ptr == NULL) {
    return 1;
}
```

Printing out its content:

```C
for (int i = 0; i < 5; i++) {
    int* this_elements = (int*)head_ptr->element;
    printf("(i=%d) ELEMENT=%d\n",i,this_elements[i]);
}
```

The output would be:

```
(i=0) ELEMENT=1
(i=1) ELEMENT=2
(i=2) ELEMENT=3
(i=3) ELEMENT=4
(i=4) ELEMENT=5
```

But when an element is modified in the static-array, it also will be modified „in the dynamic-array”. This happens, because `dynamic_array_from_whole` just saves the reference (the pointer) to the actual values and not their data.

```C
numbers[0] = 9;
```

The new output would be: 

```
(i=0) ELEMENT=9
(i=1) ELEMENT=2
(i=2) ELEMENT=3
(i=3) ELEMENT=4
(i=4) ELEMENT=5
```

And of course, don't forget to clear the allocated data:

```C
clear_list(head_ptr);
```