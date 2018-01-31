# The source code for this file was taken from [http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/programming.html][0] and is included here for posterity. It is not HTML5\.

# Programming

First and foremost, the most important thing to realize about the LambdaMOO 
system is that there really isn't anything like a program to deal with.
In C++, every program has a main function. For LambdaMOO, any implicit main
program is safely concealed inside the internals of the system, and programmers
have no say in the matter unless they wish to start hacking away at server code.
For LambdaMOO, a programmer is restricted to constructing objects and their verbs.
Like any object oriented system, a programmer could argue that the execution paths
through the various verbs constitues a program, but unfortunately that argument tends
to break down quickly since the entire point of having functions on an object is so that
other objects can interact with each other. That's the point of object oriented
programming.

Unless otherwise noted, when specifying commands for performing some action,
any text enclosed in square brackets is optional. E.g. _FunctionName(Arg1 \[,Arg2)_
indicates that Arg2 is optional when calling the function Arg1\.

Having said that, the next issue is how one constructs objects to do meaningful
things in LambdaMOO. The major issues at this point are

* [Data Types][1]
* [Creating an object][2]
* [Creating properties][3]
* [Creating verbs][4]
* [Inside Verbs][5]
* [Fun with lists and strings in LambdaMOO][6]

---

Data Types in LambdaMOO

The data types for LambdaMOO are
* **Integers** Signed whole numbers. 
* **Object** Objects in the LambdaMOO database are referred to
by their object number. \#ObjectNumber denotes an object number
(for example \#0 is object number 0). The object number is
essentially a pointer to an object, since there is no such
notion as passing an object by value for the LambdaMOO system.
toobj(...) will convert either strings like "\#42" or 42 to objects,
and both result in \#42\.
* **Strings** Characters in LambdaMOO are strings of length 1\.
Like lists, LambdaMOO strings are immutable.
* **Error** Various run time errors, such as security violations,
type mismatches, unassigned variables, etc. Consult the
programmer's manual for more information.
* **Lists** In LambdaMOO, lists are a the fundamental
data type for constructing things like arrays and sets.
The elements in a list can be assigned to destructively,
but in order to change the length of a list it is necessary to
do something like 

MyList={@MyList,NewElement}

Elements of a list can be of any type allowed for variables in
LambdaMOO, including lists themselves.
See the section on [Lists for more details.][6]
* **Floating point numbers** Signed real numbers.
It is important to note that in LambdaMOO,
33 _is not equal_ to 33.0\.
Data Typetypeof(...)==Conversion Function\>Comments

IntegersINT or 0tonum(...) or toint(...)Signed whole numbers

ObjectOBJ or 1toobj(...)Reference number for an object

StringsSTR or 2tostr(...)String or character

ErrorERR or 3noneError condition. Used for various runtime errors

ListsLIST or 4noneImmutable list. Used for arrays and sets

Floating point numberFLOAT or 9tofloat(...)Signed floating point number

Creating Objects

The three major ways of creating objects in LambdaMOO are 
* 
@create ParentObject named "Object Names"

This first method is used by human beings to create objects,
and is something which would be entered on the command line.
For example: **@create $thing named "my very own object"**

will create an object derived from $thing (Generic Thing)
and set the new object's name to _my very own thing_
* 
create(ParentObject \[,OwnerObject\])

This method is intended for programmatically creating new
objects by invoking the interpreter's built in **create**
function. Both ParentObject and OwnerObject are required to
by [object references][7].
If OwnerObject is not specified, then the object will own itself
(see [object ownership
in the security section][8]). The return type of create will
be the object reference of the newly created object, or else
the appropriate error code.

For example: 

**create($npc,player)**

will create an object derived from $npc and owned by the
player object which caused this verb to be executed.
* 
$recycler:\_create(Parent, \[,OwnerObject)

This method is also intended for programmatically creating new objects
but in this case it differs from the built in function in a subtle
manner. When create() is used, the object reference will always be
unique. $recycler:\_create attempts to objects discarded with
$recycler:\_recycle(NoLongerNeededObject) by reconstructing a previously
used object into its new role. In a perfect world this verb is
indistinguishable from the create function, except that its friendlier to
longer lived systems which create many temporary objects.

Creating properties on an object

The major ways to add properties to an object in LambdaMOO are
* 
@property ObjectName.PropertyName \[PropertyValue\]

This first method is intended for use by human beings when constructing
objects before they are put into use. If PropertyValue is omitted,
the property is initialized to 0\.
* 
add\_property(TargetObject,PropertyName,InitialValue,Info)

This method invokes the built in add\_property function to add
the property PropertyName to the object TargetObject. For more
information, please consult the online help using
**@help add\_property**
* 
Use tkMOOlite or MacMOOSE

Andrew Wilson's tkMOOlite or the MacMOOSE programs each have
nice interactive object browser's which allow a programmer to 
interactively add properties and verbs to an object.
Needless to say, it is generally horribly complicated to add properties
to objects from within a verb, and for most purposes there isn't really
any need to. I'd personally recommend staying true to the rule of thumb that
say "If you think you need to add a property to an object from within a verb
you're probably wrong." To change the value (or type for that matter)
of a property, the easiest way is simply to assign it a new value. 
Programmatically, this is simply

    
        ObjectReferece.Property = NewValue;
    

and to change a value at the command line, the only difference is to add
a semicolon (to tell the command line interpreter that you are typing in
an expression and not a command), like so:

    
    ;ObjectReference.Property = NewValue;
    

Creating verbs on an object

The major ways of adding verbs to an object in LambdaMOO are 
Fun with lists and strings

##### Accessing and modifying elements of lists and strings

To access a particular element of a list or string, the square braces
are used. Elements are numbered sequentially from 1 to n, where n is the
length of the string or list.

Assigning to an element of a list or string modifies the corresponding
element of the list or string. 

For example:

    
        Test="George";	// Our example string
        Test[3]="4";	// Test is now "Ge4rge"
        Test[5..6]="56";    // Test is now "Ge456e"
        Test={"A","B","C"}; // Our example list
        Test[2]="BC";	// Test is now {"A","BC","C"};
        Test[2]="BC","DE";  // Error.  Invalid statement.
        Test[2]={"BC","DE"}; // Test is now {"A",{"BC","DE"},"C"};
    

As shown in the example, the elements selected from a list or string can
be more than one element long. However, those elements have to be
continuous. For example

    
        "George"[3..3]	// "o"
        "George"[3..5]	// "org"
        "George"[3..5,7..9] // syntax error
    

Strings can be costructed using the + operator to append two strings together.
Unfortunately, this modifies neither of the two original strings.

    
        OriginalName="George";
        LastName=" The Great";
        NewName=OriginalName+LastName;
    

Changes neither OriginalName or LastName. There is absolutely nothing
wrong with assigning like

    
        OriginalName="George";
        LastName=" The Great";
        OriginalName=OriginalName+LastName; // OriginalName = "George The Great"
        OriginalName="George"+LastName;     // Same result
        OriginalName="George"+" The Great"; // Same result
    

In fact, this way of appending to strings in absolutely necessary.
In LambdaMOO (as in Java), a string is only as long as it absolutely
has to be, and to append to a string (such as changing a string stored
in a property) requires such a construct.

Lists are constructed in a similar manner. The ampersand operator,
when applied to the front of a list name, expands out to the contents
of that list. For example:

    
        List={1,2,3,4,5};
        List={@List};			// Identity operation
        List={@List,6};			// List = {1,2,3,4,5,6};
        Counter=1;
        while(Counter



[0]: http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/programming.html
[1]: #data_types
[2]: #create_object
[3]: #create_properties
[4]: #create_verbs
[5]: #inside_verbs
[6]: #lists
[7]: syntax.html#object_reference
[8]: security.html#object_ownership