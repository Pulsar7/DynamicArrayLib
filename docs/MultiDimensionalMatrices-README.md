<h1>Multidimensional Matrices</h1>

<h2>Table of contents</h2>

- [`create_matrix`](#create_matrix)
  - [Usage \& Example](#usage--example)
- [`get_element_by_indices`](#get_element_by_indices)
  - [Usage \& Example](#usage--example-1)
- [`set_element_by_indices`](#set_element_by_indices)
  - [Usage \& Example](#usage--example-2)
- [`fill_matrix_from_static_array`](#fill_matrix_from_static_array)
  - [Usage \& Example](#usage--example-3)
- [`add_matrices`](#add_matrices)
  - [Usage \& Example](#usage--example-4)
- [`multiply_2d_matrices`](#multiply_2d_matrices)
  - [Important Note](#important-note)
  - [Usage \& Example](#usage--example-5)


## `create_matrix`

This function create a matrix with the given parameters:

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

The required parameters are:

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

The required parameters are:

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

## `fill_matrix_from_static_array`

This function is able to copy the values of a static-array into a given matrix.

The required parameters are:

- `MultiDimensionalMatrix* matrix`: *See description at [get_element_by_indices](#get_element_by_indices)*
- `void* static_array`: The given static-array
- `size_t* static_dimensions`: The amount of the elements of every dimension (has to be the same dimensions as the used dimensions-array while creating the matrix)
- `size_t number_of_dimensions`: The amount of dimensions of the static-array
- `DataType data_type`: *See description at [create_matrix](#create_matrix)*

The function returns a custom __ErrorCode__, which should be __ERR_NONE__ if no error occured.

### Usage & Example

```C
// size_t dimensions[] = {2, 2, 2};
// Matrix created successfully

int static_array[2][2][2] = {
  {
    {1, 2},
    {3, 4}
  },

  {
    {5, 6},
    {7, 8}
  }
};

ErrorCode response = fill_matrix_from_static_array(matrix, (void*)static_array, dimensions, 3, TYPE_INT);

if (response == ERR_NONE) {
  // printout the matrix
  size_t indices[3];
  void* element;

  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      for (size_t k = 0; k < 2; k++) {
        indices[0] = i;
        indices[1] = j;
        indices[2] = k;

        element = get_element_by_indices(matrix, indices);
        
        if (!element) { 
          printf("Couldn't get element at matrix[%ld][%ld][%ld]\n",i,j,k);
          break;
        }

        printf("matrix[%ld][%ld][%ld]=%d\n",i,j,k,*(int*)element);
      }
    }
  }
} else {
  printf("THIS IS FINE.\n");
}

// Clear the allocated space
clear_matrix(matrix); 
```


## `add_matrices`

With this function, you can calculate the sum of two matrices.

The required parameters are: `const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B`

The function returns an __ArithmeticOperationReturn__-struct, which contains both, the pointer to the result-matrix and an `ErrorCode`.
If something went wrong, the function should return the __NULL-Pointer__ as the result-matrix and an accordingly __ErrorCode__.


### Usage & Example

```C
// size_t dimensionsA[] = {1, 2, 3};
// size_t dimensionsB[] = {1, 2, 3};
// `matrixA` & `matrixB` created successfully

ArithmeticOperationReturn response = add_matrices(matrixA, matrixB);

if (response.error_code == ERR_NONE) {
    // Successfully added both matrices
    ArithmeticOperationReturn* result_matrix = response.result_matrix;

    printf("Sucessfully added both matrices!\n");
} else {
    // An error occured
    printf("Couldn't add both matrices\n");
    
    // Handle the specific `ErrorCode`

    if (response.error_code == ERR_INVALID_ARGS) {
        printf("MatrixA and MatrixB have different dimensions or the `data_type` of MatrixA and MatrixB are not the same!\n");
    }

    if (response.error_code == ERR_NULL_PTR) {
        printf("One or both of MatrixA and MatrixB aren't valid matrices.\n");
    }

    if (response.error_code == ERR_UNKNOWN) {
        printf("An unkown error occured.\n");
    }
}

// Clear the allocated space
clear_matrix(matrixA); 
clear_matrix(matrixB);
clear_matrix(response.result_matrix);
```


## `multiply_2d_matrices`

Multiply two given 2-Dimensional matrices.

The required parameters are: `const MultiDimensionalMatrix* matrix_A, const MultiDimensionalMatrix* matrix_B`

The function returns an __ArithmeticOperationReturn__-struct, which contains both, the pointer to the result-matrix and an `ErrorCode`.
If something went wrong, the function should return the __NULL-Pointer__ as the result-matrix and an accordingly __ErrorCode__.

### Important Note

Currently, the following calculation block is redundant and repeated for each data type in the code:

```C
for (size_t i = 0; i < rows_A; i++) {
  for (size_t j = 0; j < cols_B; j++) {
    int_result[i * cols_B + j] = 0; // Initialize result element
    for (size_t k = 0; k < cols_A; k++) {
        int_result[i * cols_B + j] += int_dataA[i * cols_A + k] * int_dataB[k * cols_B + j];
    }
  }
}
```

However, I haven't found a way to implement these calculations generically for all supported data types. The current implementation repeats the block for each data type (int, float, double), which is not ideal.

### Usage & Example

```C
// size_t dimensionsA[] = {2, 2};
// size_t dimensionsB[] = {2, 2};
// `matrixA` & `matrixB` created successfully

int static_array[2][2] = {
  {1, 2},
  {3, 4}
};

ErrorCode response = fill_matrix_from_static_array(matrix_A, (void*)static_array, dimensions, 2, TYPE_INT);

if (response != ERR_NONE) {
  printf("THIS IS FINE.\n");
  clear_matrix(matrix_A);
  return 1;
} 

// printout the matrix
size_t indices[3];
void* element;

for (size_t i = 0; i < 2; i++) {
  for (size_t j = 0; j < 2; j++) {
    indices[0] = i;
    indices[1] = j;

    element = get_element_by_indices(matrix_A, indices);
    
    if (!element) {
      printf("Couldn't get element at matrix_A[%ld][%ld]\n",i,j);
      break;
    }

  printf("matrix_A[%ld][%ld]=%d\n",i,j,*(int*)element);
  }
}

int _static_array[2][2] = {
  {5, 6},
  {7, 8}
};

response = fill_matrix_from_static_array(matrix_B, (void*)_static_array, dimensions, 2, TYPE_INT);

if (response != ERR_NONE) {
  printf("THIS IS FINE.\n");
  clear_matrix(matrix_A);
  clear_matrix(matrix_B);
  return 1;
} 

// printout the matrix

for (size_t i = 0; i < 2; i++) {
  for (size_t j = 0; j < 2; j++) {
    indices[0] = i;
    indices[1] = j;

    element = get_element_by_indices(matrix_B, indices);
    
    if (!element) {
      printf("Couldn't get element at matrix_B[%ld][%ld]\n",i,j);
      break;
    }

    printf("matrix_B[%ld][%ld]=%d\n",i,j,*(int*)element);
  }
}

ArithmeticOperationReturn operation_resp = multiply_2d_matrices(matrix_A, matrix_B);

if (operation_resp.error_code != ERR_NONE) {
  printf("THIS IS FINE.\n");
  clear_matrix(matrix_A);
  clear_matrix(matrix_B);
  return 1;
}

printf("Calculated the matrices-product of matrix_A and matrix_B\n");

// printout the matrix

MultiDimensionalMatrix* result_matrix = operation_resp.result_matrix;

for (size_t i = 0; i < 2; i++) {
  for (size_t j = 0; j < 2; j++) {
    indices[0] = i;
    indices[1] = j;

    element = get_element_by_indices(result_matrix, indices);
    
    if (!element) {
      printf("Couldn't get element at result_matrix[%ld][%ld]\n",i,j);
      break;
    }

    printf("result_matrix[%ld][%ld]=%d\n",i,j,*(int*)element);
  }
}

// Clear the allocated space
clear_matrix(matrix_A);
clear_matrix(matrix_B);
clear_matrix(result_matrix);
```