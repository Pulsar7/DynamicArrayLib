<h1>Multidimensional Matrices</h1>

<h2>Table of contents</h2>

- [`create_matrix`](#create_matrix)
  - [Usage \& Example](#usage--example)
- [`get_element_by_indices`](#get_element_by_indices)
  - [Usage \& Example](#usage--example-1)
- [`set_element_by_indices`](#set_element_by_indices)
  - [Usage \& Example](#usage--example-2)


## `create_matrix`

This function create a matrix with the given parameters: `size_t number_of_dimensions, size_t* dimensions, DataType data_type`.

- `size_t number_of_dimensions`: The „depth” of the matrix
- `size_t* dimensions`: The amount of the elements of every dimension
- `DataType data_type`: Which data-type the elements will have


Currently, only __int__, __float__ & __double__ are supported `DataType`s.

The function returns the pointer of the __MultiDimensionalMatrix__-struct, or, if something went wrong, the __NULL-Pointer__.

### Usage & Example

Creating a __1 x 2 x 3__ - matrix:

```C
size_t number_of_dimensions = 3; // 3-Dimensional matrix
size_t dimensions[] = {1, 2, 3};
DataType data_type = TYPE_INT;

MultiDimensionalMatrix* matrix = create_matrix(number_of_dimensions, dimensions, data_type);

if (!matrix) {
    // Handle error: Matrix couldn't be created
    printf("Failed to create matrix\n");
    return 1;
}

// Matrix created successfully
// Use the matrix (e.g., set or get elements)


// Clear the allocated space
clear_matrix(matrix); 
```


## `get_element_by_indices`

With this function, you're able to get a specific element in a given matrix.

The required parameters are: `MultiDimensionalMatrix* matrix, size_t* indices`

- `MultiDimensionalMatrix* matrix`: The given matrix
- `size_t* indices`: The „coordinates” to the requested element in the matrix

The function returns a __void-Pointer__, which wether points to the actual element or, if something went wrong, to __0__.

### Usage & Example

```C
// size_t dimensions[] = {1, 2, 3};
// Matrix created successfully

size_t indices[] = {0, 1, 1}; // matrix[0][1][1]

void* element_ptr = get_element_by_indices(matrix, indices);

if (!element_ptr) {
    // element_ptr = NULL
    printf("Couldn't get element at the specified indices: `[0][1][1]`\n");
    clear_matrix(matrix); 
    return 1;
}

printf("matrix[0][1][1] = %d\n",*(int*)element_ptr);

// Clear the allocated space
clear_matrix(matrix); 
```


## `set_element_by_indices`

With this function, you're able to set/modify a specific element in a given matrix.

The required parameters are: `MultiDimensionalMatrix* matrix, size_t* indices, void* value`

- `MultiDimensionalMatrix* matrix`: *See description at [get_element_by_indices](#get_element_by_indices)*
- `size_t* indices`: *See description at [get_element_by_indices](#get_element_by_indices)*
- `void* value`: The new element-value


It's important to note, that the type of the __void* value__ should be the given __DataType__ while creating the matrix.

The function returns an __ErrorCode__, which should be `ERR_NONE` if the operation was successful.


### Usage & Example

```C
// size_t dimensions[] = {1, 2, 3};
// Matrix created successfully

size_t indices[] = {0, 1, 1}; // matrix[0][1][1]
int new_value = 9;

ErrorCode resp_code = set_element_by_indices(matrix, indices, (void*)&new_value);

if (resp_code != ERR_NONE) {
    // An error occured
    printf("Couldn't set element at the specified indices: `[0][1][1]`\n");
    clear_matrix(matrix); 
    return 1;
}

// Successfully modified the value at `[0][1][1]`

// Clear the allocated space
clear_matrix(matrix); 
```