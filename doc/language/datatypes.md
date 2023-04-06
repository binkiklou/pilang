# Datatypes

There are few datatypes in the language currently, but these datatypes may behave differently depending on their *form*, forms are ``arr`` or ``scalar``. There is also possibility to have pointers.

## Basic Datatypes

| keyword | Values |
| --- | --- |
| `int` | Any whole number |
| `char` | Any character |
| `bool` | `true` or `false` |
| `double` | |

Their size is equivalent to their size in C, bool is turned into a `char`.

## Pointers

All pointers are smart-pointers. 

# Array Variable

As said in the statement documentation for variables, an array variable is a reference to where the array object is held in memory. A simple variable can be defined as such:
```
arr int: a
```

But you can also have pointers to arrays. This changes the data stored, from a smart reference to the array object, to a smart reference to the array_reference. The ``!`` is used to specify the arr as being a pointer to an array. If the symbol was next to the int, it would then be a normal array object but with data which is expected to be int pointers. 

```
arr int: a
arr! int: b = $a
arr int!: c = $a # Not the same, will cause an error
```