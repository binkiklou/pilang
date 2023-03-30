# Language Reference

Very small reference for the language, that I use to not forget about stuff. This reference skips over a lot of stuff and it doesn't explain most things. Once a working version of the language is made, a more strict reference will be written.

## Array Declaration
Arrays are the base type of the language.
The declaration must either hint or contain the size of the array.
```
[3;3] # Array of size 3 x 3
[foo] # Copy of existing array
[{1,2},{3,4}] # Array literal
```

## Selection 

Selection is the mechanism for being able to selecting sections or parts of an array. It can also imply recursion.


## Item manipulation
Even though scalars aren't really a type in this language, individual items in the array can be maniupulated via the ``->`` arrow. It's actually is an ``=`` for scalars, but when applied on non-scalars it's behaviour is different. When applied to an array the arrow is equivalent to a ``=`` for each item, and is very powerfull.

This simple example, demonstrate an array being created and all it's items being set to 0
```
[5]->0
# 0 0 0 0 0
```

The left-side of an arrow is an expression, which means that it can be much more powerful than a simple scalar. Paired with selection(see above), this can be used to emulate for loops.

This example shows an DIY range function, it sets the value to the index of the item.
```
[5](item x)->x
# 1 2 3 4 5
```

It can also be used as a lambda function

## Variable
```
a:int[3;3]->0
b:range(3)
```
Variables are mutable but their size is constant