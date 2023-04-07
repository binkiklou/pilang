# Organisation

Even thought I doubt this language is a good choice, or even an ok choice to build large software, the language comes with features to make creating large programs easier. The first feature is `import` system. This allows code from other files to be imported, and is very useful. The other feature  is `namespace` which makes organizing code much more pleasant.

## Import

The `import` statement is used to import from other files. The file which is imported, can not contain an `entry`. Import must be followed by a string which specifies the name of the file being imported. Unlike in some other languages, the objects contained in the other file do not fall under a new namespace in the current file.

```
import "other.pi"
```

> The behavior of other files is yet to be fully-defined.

The import of another file should trigger the compilation of that file. Importing a file should trigger the compiler to save  the public properties of the file, such as function declarations and other top-level things. The compiler will use that data to compile the main file. The compiled imported files are then linked with the main file. 