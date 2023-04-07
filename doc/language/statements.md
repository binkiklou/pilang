# Statement

This documents defines the basic behavior and syntax for statements. Statements in this case, is defined with anything which is parsed on the top-level, or inside a block ``{}``. Statements are "things that do stuff". 

# Variables

Variables are objects that hold references to array objects, variables are held on the stack but their reference is often, if not always held in heap memory. The array which is held in memory is immutable, but the variable itself can be changed.

## Declaration

Variable declarations needs to contain the type of variable(`arr` or `scalar`) and the datatype held.

```
arr int a
```

Declaration and assignments can be done in the same statement.

```
arr int a:int[3]->0
```

## Assignements

The right-side of an assignement is always the variable, and the right side. The ``:`` symbol is used to separate the left and right-side. The right-side, must be an identifier of a accessible variable. The left-side must be an `array-expression` or a `scalar-expression`, depending on the form of the variable.


# Functions

Functions should not be confused with *procedures*, functions return an array or scalar, and cannot have side-effects. Arguments of a function are pass by copy. Functions can be overloaded by arguments, but not by type.

```
fn arr int range(scalar int: r):int[r](x)->index(x)
```

In the future there may be function templating, but that is not yet in the language.

# Procedures

Procedures are like functions, but instead of being a big expression they contain a code block, and do not return a value. Their arguments are pass by copy.

```
proc loop(){
    # ...
}
```

Procedures are called using the ``%`` symbol.

```
%print("Hello, World!")
```

# Entry 

Entry is the entry-point of the program. It behaves like a procedure, but only one instance of entry can exist.

```
entry {...}
```

Optionally, the entry procedure can have arguments.

```
entry(arr string a) {...}
```