# Orca Programming Language

Orca is a neat little programming language that offers a blend of object-oriented and functional programming. Below is a brief overview of its core concepts, illustrated by examples.

## Comments

In Orca, comments begin with `//`.

```orca
// This is a comment
```

## Variables

Variables can be declared using the `var` keyword. They can hold both string and number values.

```orca
var name = "Erlich Bachman"; // string variable
var age = 99; // number variable
```

## Functions

To define a function, use the `fun` keyword. Function parameters are passed in parentheses, and the function body is enclosed within curly braces.

```orca
fun printCommentAboutAge(name, age) {
    ...
}
```

Functions can also return values using the `return` keyword. If no value is returned, the function returns `null` by default.

```orca
fun add(a, b) {
    return a + b;
}
```

## Conditional Statements

Orca provides an `if-else` structure for conditional statements.

```orca
if (age > 80) {
    ...
} else {
    ...
}
```

## Classes

Orca supports object-oriented programming through classes. Use the `class` keyword to define a class.

### Constructor

The constructor method is named `init`. It gets called automatically when an instance of the class is created.

```orca
class Human {
    init(name, age, occupation) {
        this.name = name;
        this.age = age;
        this.occupation = occupation;
    }
    ...
}
```

### Methods

Methods can be defined within classes. The `this` keyword refers to the instance itself.

```orca
setName(newName) {
    this.name = name;
}

getName() {
    return this.name;
}
```

### Instantiation

To create an instance of a class:

```orca
var erlich = Human(name, age, "Former child");
```

## Loops

### While Loop

The `while` loop keeps executing as long as the condition remains true.

```orca
while (erlich.age > 80) {
    ...
}
```

### For Loop

The `for` loop is structured with an initializer, condition, and iterator.

```orca
for (var i = 0; i < 100; i = i + 1) {
    ...
}
```

## Property Access and Assignment

In Orca, object properties can be accessed and assigned using the dot notation. You can also add new properties to an existing object.

```orca
erlich.isOld = erlich.age > 80;
```

## Print Statement

The `print` keyword is used to output values to the console.

```orca
print "Erlich is now young.";
```

## Arrays

An array can be defined and accessed as follows:

```orca
var names = ["George", "James", "Harry", "William"];
```

You can also specify the initial array's length. The provided initializer will be filled with nil values to meet the length specifier requirements.

```orca
var numbers[10] = [1,2,3];
var people[100] = []; // An empty array of size 100.
```

## In Summary

Orca is a versatile language with features from both object-oriented and functional paradigms. This documentation provides a brief introduction to its main concepts, making it easy for developers to get started. 
