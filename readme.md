# Pi Lang

Pi lang is a compiled array programming language that is actually readable.


## Simple features

In pilang arrays are mutable, but their size stay  constant.

```
# Init an array with a default value of 0
int a:[2;2]->0

# Inits an array, without having to specify the sizes
int b:[{1,2,3},{4,5,6},{7,8,9}]

# Sums the first element of each row
int result:sum(col 0 of b)

# One liner of the above
int res:sum(col 0 of [3;3]=>range(9))

@print(result)
```