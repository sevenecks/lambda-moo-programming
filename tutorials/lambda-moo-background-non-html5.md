
# Background

## Environment

C++ is a programming language. There is no seperate operating system,
or user environment associated with C++. For LambdaMOO, like Java, there
is a blurring of the distinction. LambdaMOO refers to the system running
(in this case) on a UNIX machine, using an interpreted language normally referred
to as either "The LambdaMOO programming language" or LambdaMOO. Is very
much like the problem of discussing Java the language, Java the virtual
machine, or Java the operating environment.

For the curious, I myself have no idea what the internal representation
of the code for the LambdaMOO system is. All I prove from my current knowledge
is that there are functions built into the system for changing the code
used in verbs, and for changing every attribute of objects in the 
system, including changing the inheritence tree.

## Classes and Objects

In C++, the difference between a class and an object is a concrete one.
Objects are instances of classes, and in general one manipulates objects.
In the LambaMOO system, there is very little (if any) distinction between
a class and an object. It is possible to create an object which inherits
from another object, add properties and verbs to that object, and then create 
other objects which inherit from that newly created object. Essentially,
LambdaMOO allows for the dynamic creation and changing of classes, rendering
them indistinguishable from objects.

## Functions and Verbs

In C++, an object is said to have member functions. In LambdaMOO, an object
is said to have verbs. As a side effect of the interactive nature of
LambdaMOO, verbs can be invoked either by the built-in parser, or by
other verbs. LambdaMOO's interpreter does have certain functions, which
can be called inside of verbs, but these functions have been compiled
directly into the interpreter.

## new and delete

C++ uses the new and delete operators for dynamically creating objects
during program execution. The equivalents for LambdaMOO are the built-in
functions create() and delete().

## Types

C++ has static type checking. With the partial exception of virtual functions
(since the compiler still checks that the variables can be converted), all variable
variables and members are constrained in type at compile time. LambdaMOO, as
an interpreted system, does run time type checking. As such, it also has
provisions for checking the type of a variable.

## Using variables
In C++, a variable cannot be declared after it is used. Or rather, in C++
a variable in scope before it is used, either declared inside a function,
as a member of a class, or otherwise in scope to be used. For LambdaMOO,
variables aren't declared. Variables which haven't been assigned to essentially
have no type, and as such any attempt (other than assignment) will result in
the expression evaluating to an error (variable type ERR).

In C++, global variables are simply variables declared outside of all other
scopes. In LambdaMOO, the properties of the System Object (Object \#0) are
the closest thing to truly global variables, and special access rights are
required to create or change those values.

### Variable Scope

In C++, variables have lexical scope, meaning it is possible to use
variables which have not been declared inside a function, but which an expression
can "see". By the scoping rules of C++, it is possible to use the same
notation, or lack thereof, to refer to a variable which is local to function, 
a class data member, or a global variable, a named contant, et al.

For LambaMOO, the scoping rules are much simpler. Or rather, the notation
is much more distinct. $GlobalVariable is a global variable named
GlobalVariable. this:MemberVariable is a member variable of the object
'this', and OtherVariable is a local variable named OtherVariable.
Member variables follow the reasonable approach to maintaining their
values, namely that the values of properties inherited from other objects
becomes distinct upon creating the derived object.

If you have experienced a language like Smalltalk, LambdaMOO doesn't seem much
like an object oriented system. In LambdaMOO, you have objects, and then you
have primitive types. Objects persist, everything else either gets stored on
an object or else eventually gets garbage collected into oblivion.
Aside from this issue, LambdaMOO is just as object oriented as C++, or Java.

# The source code for this file was taken from [http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/background.html][0] and is included in this repository for posterity. It is not HTML5\.

[0]: http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/background.html
