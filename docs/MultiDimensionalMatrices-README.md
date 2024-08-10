<h1>Multidimensional Matrices</h1>

<h2>Table of contents</h2>

- [`create_matrix`](#create_matrix)
  - [Usage \& Example](#usage--example)
- [`clear_matrix`](#clear_matrix)
  - [Usage \& Example](#usage--example-1)
- [`get_element_by_indices`](#get_element_by_indices)
  - [Usage \& Example](#usage--example-2)
- [`set_element_by_indices`](#set_element_by_indices)
  - [Usage \& Example](#usage--example-3)
- [`fill_matrix_from_static_array`](#fill_matrix_from_static_array)
  - [Usage \& Example](#usage--example-4)
- [`add_matrices`](#add_matrices)
  - [Usage \& Example](#usage--example-5)
- [`multiply_2d_matrices`](#multiply_2d_matrices)
  - [Usage \& Example](#usage--example-6)
- [`scalar_multiply_matrix`](#scalar_multiply_matrix)
  - [Usage \& Example](#usage--example-7)


## `create_matrix`

Allocates the space for the given dimensions.

Required function parameters:

1. `MultiDimensionalMatrix* matrix`: A reference to a defined struct-element
2. `size_t number_of_dimensions`: The „depth” of the matrix
3. `size_t* dimensions`: Maximum indices of every dimension
4. `DataType data_type`: The data-type of the matrix-elements

Unfortunately, only __int__, __float__ & __double__ are supported currently.

The function returns an `ErrorCode`, which should be __ERR_NONE__ if the nothing went wrong.


### Usage & Example

```C
// Create matrix
MultiDimensionalMatrix matrix;
size_t dimensions[3] = {3, 2, 2};

ErrorCode err = create_matrix(&matrix, 3, dimensions, TYPE_INT);

if (err != ERR_NONE) {
    printf("Couldn't create matrix\n");
    return 1;
}

clear_matrix(&matrix);
```


## `clear_matrix`

Deallocates all space and sets the pointer to the node (`MultiDimensionalMatrixNode`) to __NULL__.

Required function parameters:

1. `MultiDimensionalMatrix* matrix`: A reference to the given matrix


### Usage & Example

See [Example for matrix-creation](#usage--example)



## `get_element_by_indices`

Gets the element by its multidimensional-indices.

Required function parameters:

1. `MultiDimensionalMatrix* matrix`: A reference to the given matrix
2. `size_t* indices`: Array of the requested indices

The function returns a __void__-Pointer, which should be __NULL__ if an error occured.


### Usage & Example

```C
// Create matrix
MultiDimensionalMatrix matrix;
size_t dimensions[2] = {2, 2};

ErrorCode err = create_matrix(&matrix, 2, dimensions, TYPE_INT);

if (err != ERR_NONE) {
    printf("Couldn't create matrix\n");
    return 1;
}

// Get element by its indices
size_t indices[2] = {1, 1};
void* retrieved_value = get_element_by_indices(&matrix, indices);

if (retrieved_value) {
    printf("The retrieved value is: %d\n", *(int*)retrieved_value);
} else {
    printf("Couldn't get value! Something went wrong.\n");
}

clear_matrix(&matrix);
```


## `set_element_by_indices`

Sets the value at a specific position in the matrix.

Required function parameters:

1. `MultiDimensionalMatrix* matrix`: A reference to the given matrix
2. `size_t* indices`: Array of the requested indices
3. `void* value`: A reference to the given value

The function returns an `ErrorCode`, which should be __ERR_NONE__ if the nothing went wrong.

### Usage & Example

```C
// Create matrix
MultiDimensionalMatrix matrix;
size_t dimensions[2] = {2, 2};

ErrorCode err = create_matrix(&matrix, 2, dimensions, TYPE_INT);

if (err != ERR_NONE) {
    printf("Couldn't create matrix\n");
    return 1;
}

int value = 42;
size_t indices[2] = {1, 1};
ErrorCode err = set_element_by_indices(&matrix, indices, (void*)&value); // or just `&value`, because it doesn't matter wether it's a int- or a void-pointer

if (err == ERR_NONE) {
    printf("Set element %d at matrix[%ld][%ld]\n",value,indices[0],indices[1]);
} else {
    printf("Couldn't set the value\n");
}

clear_matrix(&matrix);
```


## `fill_matrix_from_static_array`

Fills a matrix with the elements of a given static-array. 
__Caution__: The data_type of the static-array-elements has to be equal to the given matrix-data_type!

Required function parameters:

1. `MultiDimensionalMatrix* matrix`: A reference to the given matrix
2. `void* static_array`: The static-array

The function returns an `ErrorCode`, which should be __ERR_NONE__ if the nothing went wrong.

### Usage & Example

```C
// Create matrix
MultiDimensionalMatrix matrix;
size_t dimensions[2] = {2, 2};
create_matrix(&matrix, 2, dimensions, TYPE_INT);

int static_array[2][2] = {
    {1, 2},
    {3, 4}
};

// Fill the matrix
ErrorCode err = fill_matrix_from_static_array(&matrix, static_array);

if (err == ERR_NONE) {
    printf("Copied values of static-array into matrix\n");
} else {
    printf("Couldn't fill the matrix\n");
}

clear_matrix(&matrix);
```


## `add_matrices`

Calculates the sum of two matrices.
__Caution__: Both matrices have to have the same data_types.

Required function parameters:

1. `const MultiDimensionalMatrix* matrix_A`: Reference to the first given matrix
2. `const MultiDimensionalMatrix* matrix_B`: Reference to the second given matrix

The function returns an `ArithmeticOperationReturn`, which is a struct and contains the `result_matrix` and the `error_code` (which should be __ERR_NONE__ if the operation went successful).

### Usage & Example

```C
// Create matrix_A and matrix_B
MultiDimensionalMatrix matrix_A, matrix_B;
size_t dimensions[2] = {2, 2};
size_t number_of_dimensions = 2;
if (create_matrix(&matrix_A, number_of_dimensions, dimensions, TYPE_INT) != ERR_NONE) {
    printf("Couldn't create `matrix_A`\n");
    return 1;
}

if (create_matrix(&matrix_B, number_of_dimensions, dimensions, TYPE_INT) != ERR_NONE) {
    printf("Couldn't create `matrix_B`\n");
    clear_matrix(&matrix_A);
    return 1;
}

// Fill both matrices (with the same elements)
int static_array[2][2] = {
    {1, 2},
    {3, 4}
};

ErrorCode err = fill_matrix_from_static_array(&matrix_A, static_array);

if (err != ERR_NONE) {
    printf("Couldn't fill `matrix_A`\n");
    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    return 1;
}

err = fill_matrix_from_static_array(&matrix_B, static_array);

if (err != ERR_NONE) {
    printf("Couldn't fill `matrix_B`\n");
    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    return 1;
}

// Add both matrices
ArithmeticOperationReturn result = add_matrices(&matrix_A, &matrix_B);

if (result.error_code == ERR_NONE) {
    printf("Successfully added both matrices\n");
    
    // Printout result-matrix
    size_t indices[number_of_dimensions];

    for (size_t i = 0; i < dimensions[0]; i++) {
        for (size_t j = 0; j < dimensions[1]; j++) {
            indices[0] = i;
            indices[1] = j;
            void* retrieved_value = get_element_by_indices(&result.result_matrix, indices);
            if (!retrieved_value) {
                printf("Couldn't get value at result_matrix[%ld][%ld]\n",i,j);
                continue;
            }
            printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)retrieved_value);
        }
    }

} else {
    printf("Something went wrong while trying to add both matrices\n");
}

clear_matrix(&matrix_A);
clear_matrix(&matrix_B);
clear_matrix(&result.result_matrix);
```


## `multiply_2d_matrices`

Calculates the product of two 2-Dimensional matrices.
__Caution__: Both matrices have to have the same data_types.

Required function parameters:

1. `const MultiDimensionalMatrix* matrix_A`: Reference to the first given matrix
2. `const MultiDimensionalMatrix* matrix_B`: Reference to the second given matrix

The function returns an `ArithmeticOperationReturn`, which is a struct and contains the `result_matrix` and the `error_code` (which should be __ERR_NONE__ if the operation went successful).


### Usage & Example

```C
// Create matrix_A and matrix_B
MultiDimensionalMatrix matrix_A, matrix_B;
size_t dimensions[2] = {2, 2};
size_t number_of_dimensions = 2;
if (create_matrix(&matrix_A, number_of_dimensions, dimensions, TYPE_INT) != ERR_NONE) {
    printf("Couldn't create `matrix_A`\n");
    return 1;
}

if (create_matrix(&matrix_B, number_of_dimensions, dimensions, TYPE_INT) != ERR_NONE) {
    printf("Couldn't create `matrix_B`\n");
    clear_matrix(&matrix_A);
    return 1;
}

// Fill both matrices (with the same elements)
int static_array[2][2] = {
    {1, 2},
    {3, 4}
};

ErrorCode err = fill_matrix_from_static_array(&matrix_A, static_array);

if (err != ERR_NONE) {
    printf("Couldn't fill `matrix_A`\n");
    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    return 1;
}

err = fill_matrix_from_static_array(&matrix_B, static_array);

if (err != ERR_NONE) {
    printf("Couldn't fill `matrix_B`\n");
    clear_matrix(&matrix_A);
    clear_matrix(&matrix_B);
    return 1;
}

// Add both matrices
ArithmeticOperationReturn result = multiply_2d_matrices(&matrix_A, &matrix_B);

if (result.error_code == ERR_NONE) {
    printf("Successfully multiplied both matrices\n");
    
    // Printout result-matrix
    size_t indices[number_of_dimensions];

    for (size_t i = 0; i < dimensions[0]; i++) {
        for (size_t j = 0; j < dimensions[1]; j++) {
            indices[0] = i;
            indices[1] = j;
            void* retrieved_value = get_element_by_indices(&result.result_matrix, indices);
            if (!retrieved_value) {
                printf("Couldn't get value at result_matrix[%ld][%ld]\n",i,j);
                continue;
            }
            printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)retrieved_value);
        }
    }

} else {
    printf("Something went wrong while trying to multiply both matrices\n");
}

clear_matrix(&matrix_A);
clear_matrix(&matrix_B);
clear_matrix(&result.result_matrix);
```


## `scalar_multiply_matrix`

Calculates the product of a scalar (number) and a matrix.

Required function parameters:

1. `const MultiDimensionalMatrix* matrix`: Reference to a given matrix
2. `void* scalar`: A reference to the given value


The function returns an `ArithmeticOperationReturn`, which is a struct and contains the `result_matrix` and the `error_code` (which should be __ERR_NONE__ if the operation went successful).


### Usage & Example

```C
// Create matrix
MultiDimensionalMatrix matrix;
size_t dimensions[2] = {2, 2};

if (create_matrix(&matrix, 2, dimensions, TYPE_INT) != ERR_NONE) {
    printf("Couldn't create matrix\n");
    return 1;
}

int static_array[2][2] = {
    {1, 2},
    {3, 4}
};

ErrorCode err = fill_matrix_from_static_array(&matrix, static_array);

if (err != ERR_NONE) {
    printf("Couldn't fill matrix from static array\n");
    clear_matrix(&matrix);
    return 1;
}

int scalar = 2;
ArithmeticOperationReturn resp = scalar_multiply_matrix(&matrix, (void*)&scalar);

if (resp.error_code != ERR_NONE) {
    printf("Couldn't calculate the product of a value and a matrix!\n");
    clear_matrix(&matrix);
    return 1;
}

// Printout result-matrix
size_t indices[number_of_dimensions];

for (size_t i = 0; i < dimensions[0]; i++) {
    for (size_t j = 0; j < dimensions[1]; j++) {
        indices[0] = i;
        indices[1] = j;
        void* retrieved_value = get_element_by_indices(&result.result_matrix, indices);
        if (!retrieved_value) {
            printf("Couldn't get value at result_matrix[%ld][%ld]\n",i,j);
            continue;
        }
        printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)retrieved_value);
    }
}

clear_matrix(&matrix);
clear_matrix(&resp.result_matrix);
```