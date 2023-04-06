# Language Reference

Very small reference for the language, that I use to not forget about stuff. This reference skips over a lot of stuff and it doesn't explain most things. Once a working version of the language is made, a more strict reference will be written.

## Array Declaration
Arrays are the base type of the language.
The declaration must either hint or contain the size of the array.
```
[3;3] # Array of size 3 x 3
[foo] # Copy of existing array
[{{1,2},{3,4}}] # Array literal
```
# Reshape Operator
The reshape arrow(name should be changed) is the equivalent of the ``->`` arrow(*see Item manipulation*) for full arrays, and is used to reshape the right array to the left array's shape.

> Not yet defined: The arrow could be ``=>`` or ``<=``, because the side of expression matters. For now ``=>`` is used even though the right expression is reshaped to the left, unlike what the arrow implies.

```
[3;3]=>range(3*3)
```

## Selection 

Selection is the mechanism for being able to selecting sections or parts of an array. It can also imply recursion. You can select specific dimensions(row,columns,...) or even specific items. You can also select non-specifically, so that paired with ``->`` or ``=>`` you can set a value according to an expression.


Selecting a dimension will return a vector(or matrix for higher dimensions) from that dimension
```
foo:int[3;3]<=range(9)

# Selects col from (col,row)
foo({0}:0)->0

# Selects row from (col, row)
foo({1}:0)->0
```

### Behaviour
In other languages selecting an out-of-range value will lead to a runtime error. But because of the fixed size of arrays, the error can be caught at runtime. **This also means that array selection is always a constant scalar.**

In the event of a out-of-range access, to avoid segfaults, the object returned will be a location to nowhere will no value.(This part isn't decided yet)

```
arr:int[5]->0
[arr](item 5) # Compile-Time error
```


## Item manipulation
Even though scalars aren't really a type in this language, individual items in the array can be maniupulated via the ``->`` arrow. It's actually is an ``=`` for scalars, but when applied on non-scalars it's behaviour is different. When applied to an array the arrow is equivalent to a ``=`` for each item, and is very powerfull.

This simple example, demonstrate an array being created and all it's items being set to 0
```
[5]->0
# 0 0 0 0 0
```

The right-side of an arrow is an expression, which means that it can be much more powerful than a simple scalar. Paired with selection(see above), this can be used to emulate for loops.

This example shows an DIY range function, it sets the value to the index of the item.
```
[5](x)->index(x)
# 1 2 3 4 5
```

It can also be used as a lambda function

## Variable
```
a:int[3;3]->0
b:range(3)
```

Variables store their arrays, they do not hold reference to it. As such their size is immutable. It will be possible to have structures with sizes that can change, but I have not thought of  an elegant way to make that work yet.