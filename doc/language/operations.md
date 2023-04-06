# Operations

Operations are arguably the most important part of the language, I would define the term "operation" as any method of changing data. The basic operations all have different syntax.

# Array initialisation

This is the most basic one, and is used to allocate a new array object in memory. Array init is always done with the syntax: ``datatype[...]``, what is inside the brackets can change depending on what type of initalisation you want. Once initialized, the array's size cannot be changed, it's value is mutable, unless specifed otherwise.

## Init by shape

Size inition will allocate an array of the specified shape and size, but by itself, will not set their value. Shapes can be defined as ``expressions`` separated by ``l``. The expression must always return an ``int`` value, or a value that can be cast to int.

``
int[2;2]
``

## Init by literal

The  only place where a literal array can be done is inside an inition. It looks like this ``{1,2,3}``, where the each braces is a dimension. The values inside must be of the same type as the array itself. 

## Init by copy

Init by copy, is what it says, it copies an existing object, it is of the syntax ``[identifier]``. If you want to copy the array that a pointer is pointing to, use ``[@identifier]``. Init by copy will still require that a type is specified, if the type is different that that of the object being copied, the values will be cast.

# Arrows

Definitevely the most cool looking part of the language, are the arrows ``->`` and ``=>``. These operations can be used with other operations to behave like loops.

## Scalar Arrow

The scalar arrow `->`, is used to set to a scalar. If applied to an array, it will define each elements of that array to the specifed scalar, unless a selector is applied.

```
int[5]-> 0 # 0 0 0 0 0
```

## Reshape Arrow

The reshape arrow `=>`, sets an array's value to another array's. It also reshapes it to the size which is specified. The reshape operation will not create a new array, so the size must of the array most be correct. The size of an  array is equal to every dimensions multiplied, or the number of elements.

The right-side must be an ``array_expression``(any expression that results into an array) and the left-side must be a one dimensional int array, where each element is equal to a dimension. It is not an init by shape array, because this way saves more memory, and also makes it possible to have determine the size to reshape at runtime.

```
arr int cube:range(3*3)=>int[{3;3}]
```

# Selection

Selection is a mechanism for being able to select elements from an array. It is rarely used alone, and is most usefull when paired with the scalar or the reshape arrow.

The syntax for selection is done by adding a parenthesis next to an *array inition*, or *array expression*. The inside of the parenthesis is called a *selector*. The code below sets the first element of an array to 0.

## Index Selection

```
int[5](0)->0
```

## Dimension Selection

Selection does not neccesarily select elements, it can also select specific dimensions. Using the syntax `{dimension}:index`.

```
arr int a:int[3;3]
a({0}:0) # returnrs the first row
a({1}:0) # returns the first column
```

## Chunk Selection

Selection can also be done by selecting chunks of *n*-size of an array

```
arr int a:int[6]
a([3]:0)  # select the first 3-large chunk of the array
```

If the array can not be split into *n*-chunks of specified size, then one of the chunks will not be of the same size as the others. This must be taken into account when writing code that splits arrays.

The parameter for a chunk selection is a `scalar_expression` which must return a `int` value or something that can be cast to an `int`. If the result is of type `float` or `double`, the result is rounded.

## Iterative Selection

Paired with the scalar arrow `->`, selection can be used for iterating through an array. This can be done using any selection method, and by changing the number in the selection to an identifier. The identifier must not already be associated to a variable.

```
int[10](x)->index(x) # 0 1 2 3 ...
```

The variable will be a ``selection_object``. The properties of which are not yet defined.