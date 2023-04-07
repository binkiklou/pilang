# Objects

Often the basic datatypes are not enough. ``typedef`` lets you define a new type, and the behavior of that new type. It gives some oop features to pilang, such as having derivable objects.

Definition of the data inside a ``typdef`` is done in the ``attr`` field, this defines the attributes of the type. Attributes are variables.

The behavior of the type is defined individually for each attributes. It is done in the ``impl`` field. Operations are considereed to be functions, so overloading consists of creating a function for  the operator.

```
typdef string
{
    attr{
        arr char data
    }
    impl {
        fn new data(): char[0]
    }
}
```

> A list of all operators is yet to be added

Using objects is the same as using any other type.