<h1>DynamicArrayLib</h1>


<h2>Table of contents</h2>

- [Description](#description)
  - [Dynamic Arrays](#dynamic-arrays)
  - [Multidimensional Matrices](#multidimensional-matrices)
- [Testing](#testing)
- [Installation](#installation)


## Description

DynamicArrayLib is a C library that provides custom implementations for dynamic arrays and multidimensional matrices. This library demonstrates how to create, manage, and manipulate dynamic arrays and matrices efficiently in C.



### Dynamic Arrays

- Convert a static array to a dynamic array
- Retrieve an element from a dynamic array by its index
- Edit an element in a dynamic array by its index
- Append a static array to an existing dynamic array

Documentation: [DynamicArrays-README.md](./DynamicArrays-README.md)

### Multidimensional Matrices

- Create a multidimensional matrix
- Modify elements in a multidimensional matrix
- Retrieve an element from a matrix by its indices
- Calculate the sum of two multidimensional matrices
- Fill a matrix with a static array
- Calculate the product of two 2-Dimensional matrices
- Multiplication of scalar and matrix


Documentation: [MultiDimensionalMatrices-README.md](./MultiDimensionalMatrices-README.md)


## Testing

I've written some implementation-examples of this project in __src/main.c__. 

```BASH
make
./example_main
```

## Installation

To use this library in your project, clone the repository and include the relevant files in your project.

```bash
git clone https://github.com/Pulsar7/DynamicArrayLib/tree/main
cd DynamicArrayLib
```