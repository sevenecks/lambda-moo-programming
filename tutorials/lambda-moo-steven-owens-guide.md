# LambaMOO Programming Tutorial (very rough draft)

_by Steven J. Owens (unless otherwise attributed)_

MOO, of which LambaMOO is the first and biggest, is an interactive,
multiplayer online game that includes the ability for players to build
and even program new things or features into the game. The language
lacks a formal name but is generally referred to as "moo code" or
sometimes "moocode".

While there is a fair amount of information about LambdaMOO
programming in various places, there is also a lack of a comprehensive
explanation, especially of much of the programming environment. These
incomplete notes are the start of an attempt to remedy that. This is
just a beginning, but maybe someday it'll be an end.

There are a bunch of other resources out there, starting with the LambdaMOO
Programmer's Manual:

[LambdaMOO Programmer's Manual](moo-programmers-manual-updated.md)

Also there are a variety of tutorials. I'm not trying to duplicate or
replace them. Instead, this is a combination of filling in the gaps
about the MOO programming environment and jumpstarting you into MOO
programming.

### Outline of Current Draft

* Basic Terminology
* Verb Invocation and Property Access
* Data Types
* Lists
* Eval
* Comments
* Variable Declaration and Scoping
* Perms and Args
* Flow Control, for, if, while, suspend, fork
* Type Conversion
* Threading, Ticks and Tasks
* Core Classes
* Sight and Sound in MOOCode
* Location in MOOCode

**Note:**_See end for draft outline of planned revision_

## Basic Terminology

I'm going to assume you have a basic familiarity with programming
concepts, but don't get too stressed out if you don't, you'll be able
to pick that up as you go along. If you're totally, completely
new at this, I explain some general concepts at the end of this page,
in a section titled:
[Real Basics of Programming in Moocode](#real-basics-of-programming-in-moocode)

I'm going to assume you have a basic familiarity with moo concepts,
but just to be clear:

### The Server and the Db

You have the server and the database (colloquially "the db"). The server
is the foundation, it is written in C and compiled.  The compiled binary 
server code loads and runs the database, which is written in moocode and
defines the objects and their properties and verbs. Much of the code that 
makes the up the basic moo-ishness of things is implemented in the db, in
moocode.

**Note:**_Most people tend to associate the term "database" with a
"relational database", and most relational database programs tend to
keep most of their data in disk storage. The purely technical meaning
of "database" is "an organized collection of information."
LambdaMOO's database is not relational, it is an object database, and
it is kept entirely in-memory. The only reason I'm pointing this out
is to head off any chance of you confusing the moo database for a
relational database._

#### Core Db, MinimalCore and LambdaCore

The "core db" is the moocode infrastructure that most moos start from, 
the initial set of objects, verbs and properties that provide handy
utilities and predefined types of objects. These are all that exists
when a new MOO is first started up, until the users start adding custom
objects and moocode. For example, player objects, room objects, exit 
objects and thing objects are all part of the moo core db.

There are, generally speaking, two different versions of the core db
in use. The minimal db and the lambdacore. The minimaldb is just
about exactly what the name suggests, just the bare bones minimum of
objects to get your MOO up and running. The lambdacore has a whole
bunch of additional objects and features that the lambdamoo wizards
found useful.

#### Objects, Verbs and Properties

MOO consists of objects. 

MOO objects have properties and verbs.

Properties are values stored on the object and referenced in code via
the "." syntax, for example:

    somevariable = object.propertyname ;

Verbs are code stored on the object and invoked in code via the ":"
syntax, for example:

    somevariable = object:verbname() ;

#### Object Oriented

MOO stands for "Mud, Object-Oriented". If you don't understand what
"object oriented" means, it's an approach for organizing the code and
data in a system. 

MOO's object-oriented approach is slightly different from many
object-oriented programming langauges. In most object-oriented
languages, you have a division between object definition (its
blueprint, so to speak) and instances of the object in the system. 
The object definitions (called classes) exist off in some abstract
place, but your code in the system never deals with them directly.
Instead you create "instances" of a given class and use the instance.

In the MOO world, the object is defined by example. You create an
object instance in the system and then dynamically (aka "on the fly")
add verbs and properties to make your prototype. Then you can create
a new object that is "descended" from the first object. The new
object in turn can be dynamically modified with more verbs and
properties, and then you can create more objects that descend from the
second object.

If object-oriented programming is new to you, here's a brief
explanation:

The simplest approach is procedural - a sequential series of
instructions. Procedural languages have concepts for organizing the
code, like subroutines (also called functions or methods, depending on
what programming language you're talking about).

Object-oriented languages usually have their own version of
subroutines (typically they're called methods, or in MOOcode, verbs),
but they also have the concept of organizing groups of subroutines
(methods, verbs) as well as data (instance variables, or properties).
This conceptual grouping is what we call an "object".

#### Built-in Functions, Utils and the System Object

The moo language has a number of predefined functions, which are
generally referred to as functions or sometimes as "built-in
functions" or "built-ins" for short.

Most built-ins are documented in MOO's online help Sometimes there's
more than one help entry that matches a built-in function name, so as
a rule of thumb append an empty parentheses to the functionname() when
you use the help command. For example "help tonum()" will get you
help on the built-in function named tonum (see "Data Types", below).

In addition to the built-ins, there are a large number of handy little
utility verbs. Since verbs have to be defined on objects, these
utility verbs are organized on several objects, one object per
category of verbs. They are generally referred to as "the utils" or
sometimes "the $utils", because the individual grouping objects tend
to be kept track of in properties on the \#0 object, and you can refer
to these properties by using $propertyname, like $string\_utils, or
$list\_utils.

Speaking of object \#0, object \#0 is The System Object. This is used
as a central place to stick values that the wizards want to keep track
of on a moo wide basis, in properties on \#0\. You can reference these
properties with \#0.propertyname, but there's a shortcut, as I used
above, the dollar sign, $, so you can reference the properties in your
code by simply using $propertyname.

Anytime you see $ in a moo verb, mentally translate it to "\#0.", i.e.:

    $string_utils:english_ordinal

is really

    #0.string_utils:english_ordinal

There are a great many such useful properties on \#0 besides the utils,
but here are the utils defined on LambdaMOO's \#0 as of this writing:
  * \#0.building\_utils
  * \#0.byte\_quota\_utils
  * \#0.code\_utils
  * \#0.command\_utils
  * \#0.convert\_utils
  * \#0.gender\_utils
  * \#0.generic\_utils
  * \#0.list\_utils
  * \#0.lock\_utils
  * \#0.match\_utils
  * \#0.math\_utils
  * \#0.matrix\_utils
  * \#0.object\_quota\_utils
  * \#0.object\_utils
  * \#0.perm\_utils
  * \#0.quota\_utils
  * \#0.seq\_utils
  * \#0.set\_utils
  * \#0.string\_utils
  * \#0.time\_utils
  * \#0.trig\_utils
  * \#0.wiz\_utils

There are also several verbs defined directly on \#0, but most of them
are system related and are not particularly relevant to your average
moo coder.

#### The Root Class, Nothing and $nothing

Object \#1 is The Root Class. All objects are descended from Object \#1\.

Object \#-1 is "nothing" , the null object, the object that does not
exist. Things that are not located anywhere in particular have their
location property set to \#-1\. Verbs or functions that do not come up
with a valid result typically return the value \#-1\.

There's actually a root property, $nothing, that contains the
value \#-1, but I'm pretty sure that $nothing does not define \#-1, it
merely points to it and gives you a more expressive way to use the
value \#-1\. 

### Verb Invocation and Property Access

In code you invoke verbs with the ":" operator.

    foo = object:methodname() ;

In code you reference properties to get the values in them with the "." operator.

    foo = object.propertyname ;

You can generally chain properties for verb or property reference,
assuming that each link in the chain is a valid object reference,
e.g.:

    title = player.foo.bar:title();

    name = player.foo.bar.name ;

### Data Types

MOO has four data types: 
  * numbers (integers and floats)
  * strings (i.e. text)
  * lists of elements
  * moo object numbers (sometimes called objnums).

Numbers default to integers. Integers is a math term commonly used in
programming languages, it means no fractions or decimal places. If
you divide 1/2 in MOO, you get 0\. Surprisingly, you get get a heck of
a lot done with just integers.

Originally MOO only did integers, but these days MOO also does floats,
aka floating point numbers, i.e. numbers with a decimal point. That's
how MOO knows that a number is a float, by the presence of a decimal
point:

    somefloat = 5.0 ;
    someotherfloat = 5.0/2.0;
    otherotherfloat = somefloat/someotherfloat ;
    

In the example above, "someotherfloat" will contain the value "2.5"
and "otherotherfloat" will contain the value "2.0".

You can't mix integers and floats, so in MOOcode you need
to either include the decimal point in all the numbers in a statement
or use the "tofloat()" built-in function to convert the value in a
variable into a float (see "Type Conversion", below). Otherwise you
get a type mismatch error:

    ;1/2.0
    #-1:Input to EVAL, line 3:  Type mismatch
    ... called from built-in function eval()
    ... called from #217:eval_cmd_string (this == #1449), line 19
    ... called from #217:eval*-d (this == #1449), line 13
    (End of traceback)

You can use two periods to indicate a range of values, i.e. 1..10
means 1, 2, 3, 4, 5, 6, 7, 8, 9 and 10\. The two periods are called
the "range operator" and this mostly comes up in lists (although
can also use it in other places, like foreach loops) so I'll talk
about it more down in the section titled "Lists".

Strings is just programmerspeak for "text", i.e. "a string of
characters". The term dates all the way back to 1959 and the really
early computer languages. Moo knows something is text because it's
surrounded by double-quotes:

    astringvariable = "some text"

Lists are really fancy and get their own section (below).

Object numbers are references to objects in the MOO database. Moo
recognizes object numbers number because they're preceded by a pound
sign \#, also called a hash sign and officially named the octothorpe :-).

    puffsobjectnumber = #1449

### Lists

You define a list by enclosing a comma-separated series of
elements in curly braces 

    foo = {"a", "b", "c"} ;

You reference a specific element in a list by appending a number in
\[square brackets\] to the variable name. The number is called an
index, and accessing a list element this way is called indexing:

    xyzzy = foo[2] ;

Note that lists can contain any sort of data, including other lists:

    foo = {"a", b", c" {1, 2, 3} } ;

Nested lists can get quite complicated. One particularly common form
of nested lists is called associative lists, or alists for short, and
the $list\_utils have a number of special verbs for dealing wtih
alists.

The examples above are lists with elements that are all the same data
type, but lists can contain mixed data types:

    {"foo", 1, "bar", 1.5, #-1}

Sometimes you'll have a list of mixed data types like above, but of
course the order of the data types is something your code has to keep
track of. For example, you might have a message system where each
list contains the time received as an integer, the person the message
was from as an objectnumber, and the message text as a string. And
then you'd store all of your {time, personnumber, messagetext} lists
in another big list.

The trick with lists is not the syntax, it's the way lists are
used in day-to-day MOO coding. You expand a list with the at sign @:

    
    foo = {1, 2, 3}  ;
    bar = {4, 5} ;
    baz = {@foo, @bar} ;
    

baz will now contain {1, 2, 3, 4, 5} ;

While you only use { and } when you're creating a list, and @ when you
expand a list, the "moo way" is that you do a whole lot of list
creation and expansion. For example the "right" way to append to a
list is:

    baz = {@baz, 6} ;

As mentioned above, you refer to specific elements in a list by their **index**,
their numeric location in the list:

    foo = {"a", "b", "c"} ;
    thelettera = foo[1] ;

Note that in moocode indexes start at 1\. The first element in the
list is \[1\], the second element in the list is \[2\], etc. This is
unlike many C-derived languages where an array or list index starts
with 0, i.e. the index value is an _offset_.

You can use the range operator (..) in a list index to indicate a
range of elements. So for example if you wanted to get the second,
third and fourth element of a list, you'd index mixedlist\[2..4\].

    foo = {"a", "b", "c", "d", "e", "f"}  ;
    bar = foo[2..4] ;

bar will now contain {"b", "c", "d"}.

MOO list indexes and ranges can't do the things that fancier languages
can, they have to be just a single number \[8\] or two numbers separated
by two periods \[2..8\]. You have to do any fancy math yourself. For
example, if you want to get from element 3 to the end of the list, you
have to use the length() built-in function:

    foo = {"a", "b", "c", "d", "e", "f"}  ;
    bar = foo[2..length(foo)] ;

You'll also want to take a close look at the $list_utils functions ("help $list_utils").

### Eval

Eval is a really, really handy command that basically allows you to write little one-line MOOCode programs and execute them. It works just the same as say or emote, e.g. 

Enter: "foo<enter\>  
See: You say, "Foo"

Enter: :grins toothily<enter\>  
See: Puff grins toothily.

Enter: ;1+1<enter\>  
See: =\>2

One minor but important distinction is that only you see the results
of your eval.

Eval is handy for exploring and experimenting, and also really useful
for setting and looking at property values on your own objects, and
directly invoking verbs.

One of the first property values you should set with eval is your
eval\_env property. This may already be set for you; to check,
just type the following:

    ;player.eval_env<enter>

You should see:

    => "here=player.location;me=player;"

If it's not set, type the following:

    ;player.eval_env="here=player.location;me=player;"<enter>

Once you execute this command, you'll be able to use the eval
shortcuts "me" for your player object, and "here" for your player
object's current location. This turns out to be incredibly handy.

There's also a nifty little property, player.eval\_ticks. See the
section below "Threads, Ticks and Tasks" for more on that.

I never really got in the habit of using multi-statement
evals. Generally I find when I hit that point that it's simpler/easier
to just create a real verb to mess with. "Help eval" will explain how
it works, but here's the basics:

You start a multi-line eval with a double-semicolon, i.e.:

    ;;foo = {1, 2, 3}; bar = {4, 5};  for x in ({@foo, @bar}) me:tell(x) ; endfor<enter>
    

Also note that multi-line evals don't automatically print the return
value of the statement, so you need to explicitly print whatever you
want to see in the body of your eval statement by calling me:tell(). The built-in function
toliteral() is helpful for this, you can just call:me:tell(toliteral(whatevervariable)):

    >;;foo = {"a", "b", "c", "d", "e", "f"}  ; bar = foo[2..length(foo)] ; me:tell(toliteral(bar)) ;
    {"b", "c", "d", "e", "f"}
    => 0
    [used 161 ticks, 0 seconds.]
    

## Basic MOOCode Gotchas

### Comments

Effectively, MOOCode doesn't have comments. MOO actually has
comments, but nobody uses them and nobody, AFAIK, ever has. I can't
even remember what the comment character is - // maybe?

The problem is, early MOO was designed with the idea that the users
would store/maintain their own source code, and thus the compiler
discarded comments, didn't bother to save them in the compiled verb
bytecodes. When users used the in-server editing tools, which decompiled
the code for the user to edit it, presto, the comments were gone.
(This compiling/decompiling also lead to the happy accident that code
indenting/formatting is defacto specified by the decompiler.)

As a result of this, the custom of using string literals for comments
arose. E.g.:

    "this is a string literal" ;

This is so ingrained that by default the help command "help
objectname:verbname" will give the user any string-literal comments at
the top of of the verb source.

So, current usage is to use string-literals, as follows:

    
    "this is a MOOCode faux comment." ;
    

Of course, note that you need a semi-colon to terminate the line, and
of course any embedded quotes must be escaped with a backslash, i.e.:

    
    "this is a MOOCode faux comment with some \"embedded quotes\" in it." ;
    

## Variable Declaration and Scoping

In the MOO world you have objects, which have verbs and properties.
A verb is a method, function, subroutine, whatever. It's invoked on
an object using the ":", like so:

    
    object:verb(argument) 
    

Objects, and data stored in properties on objects are the only
persistent things in the moo world. 

Within a verb, variables are dynamically declared, and exist only
within the scope of the verb. If you call out to another verb and
include parameters in that call, the value is passed, not the original
variable (i.e. all verb arguments are pass-by-value). The second
verb can modify the parameter value all it wants, but the original
variable, in the first verb, will remain unchanged.

Note that although a verb call can pass an object, it's really passing
an object reference by value. The second verb can't do anything to
that object reference to change the object reference in the first
verb. But the second verb _can_ use that reference to access verbs
and properties on the object, thus changing the persistent object.

### A Short Digression on Dynamically Typed Variables

Moo code is dynamically typed. This means that you don't have to go
through a lot of bureacracy to set up a variable, you can just throw a
variable assignment in anywhere and presto! It's a variable.

    
    foo = 3 ;
    

This is a lot more convenient (and fun) but it also gives you a lot
more rope to hang yourself. You can be in the middle of a verb and
accidentally typo for variable name as "foop" instead of "foot" (don't
ask me how you typoed a "p" instead of a "t", maybe you have a weird
keyboard layout) and moo will happily create the new variable "foop"
and assign it, leading to all sorts of unexpected craziness.

Fortunately, hanging yourself isn't all that dangerous in the MOO
world, so you'll usually get a chance to spot the problem and fix it.

However, unlike a lot of dynamically typed languages, moocode isn't as
friendly at letting you mix types. In perl, for example, you could do:

    $message = "The number is " + $somenumvariable; 

And perl will try to Do Something Useful and assume that you really
meant to convert $somenumbervariable into a string and concatenate
them. However, in moocode, the compiler will complain because
somenumvariable is a number, not a string. You have to use the
builtin function tostr() to convert it to a string:

    message = "The number is " + tostr(somenumvariable); 

There are other towhatever() builtins, mainly tonum() which converts
to a numeric value and toobj() which converts to an object reference
number. See "Type Conversion" below.

## Perms and Args

You can see the existing verbs on your object using the @display
command (I consider this command indispensable as an object browser).

You can set the permissions using @chmod.

You can set the parser arguments using @args.

Now we get into how the args work. This immediately gets into parsing
and matching, since that's the whole point of verb args. These topics
deserve a section of their own, and I hope to find time to write it.
However, you can't really do any moocoding without some arg basics, so
here's a short primer meant to give you just sufficient info to get
started.

In moo programming, "arguments" generally refers to the somewhat
Zork-like command-line parser, though "@args" refers to the verb
arguments (when your verb code runs, any arguments included in
the invocation will be stored in the list named args, and you use
@args to expand that list). 

Command-line arguments consist of three elements, (after the verbname) e.g.:
  * verbname any in this
  * verbname this to any
  * verbname any in any
  * verbname none none none
  * verbname any any any

And so forth. The "foo preposition bar" form gets automatically
parsed by the MOO's command-line parser; it finds the verb name and
argument structure that best matches, and invokes it. 

"this" is a string that matches of the object the verb is defined on,
i.e. the object name or one of the object aliases.

"any" is any string.

There are some rules for precedence - the parser will try to match to
player:verbname before trying player.location:verbname, and so forht.

When the parser invokes the verbs, it pre-populates several special
context variables that each verb starts with; argstr, dobjstr,
iobjstr, and several others. Let's look at an example:

You have an object named "box" with a "put" verb and the
arguments "any in any"

You type "put gold in box".

The parser looks for a player:put command, doesn't find one, then
looks for a room:put command, doesn't fine one, then looks for a box
with a put command and gets a match.

The parser invokes box:put().

The code inside the verb box:put can assume that there will be a
dobjstr (direct object string) variable containing the string "gold"
and an iobjstr (indirect object sring) variable containing the string
"box".

The parser will also try to match the dobjstr (direct object string)
and iobjstr (indirect object string) and set the dobj variable and
iobj variable for the verb to any matched objects.

Note that verbs with args of "none none none" aren't meant to have
parameters at all. 

Args of "any any any" is for verbs that expect to do some sort of
custom parsing on their own.

There's also a special set of arguments for verbs meant to be invoked
only by other verbs:
  * this none this

Generally speaking, if your verb doesn't have its args set to "this
none this", you won't be able to invoke it from another verb.

In addition, such not-to-be-directly-invoked verbs must be @chmodded
to +x, to allow them to be invoked. If they're not +x, the moo will
report a rather confusing "verb not found" error. 

The reason for this is that not-to-be-directly-invoked verbs are
expected (in well-designed code) to do the heavy lifting, the
important and tricky stuff, and Pavel (or possibly Ghond) wanted to
avoid having lots of half-done code be invokable by anybody.

### Flow Control: if, for, while, suspend, fork

A simple list of instructions, one after the other, is technically a
program, but you probably want some more sophisticated options than
just a straight one-after-the-other list of instructions. You want
things like being able to check something and execute one set of
instructions or the other (if/endif), or being able to repeat an
instruction some number of times (for/endfor) .

The built-in functions suspend() and fork() aren't really technically
flow control operators, but they play a similar role, so I'm going to
talk about them here.

Some general rules of thumb:
  * Every flow control structure starts with a keyword (if, for, while).
  * Every flow control structure ends with a matching keyword that starts with "end" (endif, endfor, endwhile).
  * The lines between the start and end are called a "block" of code.
  * The line that starts the flow control structure usually contains an expression. 

Let's jump right in and show some examples of if, while, for, suspend
and fork. I'll get into them in more detail after.

Use "if" for conditionals - test some condition, and either carry out
the set of commands in the block or not. 

    ---
    
    if (some expression that evaluates to 0 or not-0)
      "do something" ;
    endif

You can also use if/else for when you want have a set of instructions
for when the if test results in not-true.

    ---
    
    if (some expression that evaluates to 0 or not-0)
      "do something" ;
    else
      "do something else" ;
    endif

The while and for flow controls are for looping, i.e. doing something one or more times:

    ---
    
    while (some expression that evaluates to 0 or not-0)
      "do something repetitive";
    endwhile

    ---
    
    for foo in (some expression that produces a list)
      "do something with foo" ;
    endfor

The built-in function suspend() is for pausing.

    ---
    
    suspend(some number of seconds you want the task to pause) ;

The built-in function fork() is for starting a new, separate task that
continues on and does its own thing.

    ---
    
    fork (somenumber of seconds) 
      "do something somenumber of seconds later, in a separate task" ;
    endfork
    "immediately after scheduling the fork, continue on with the rest of the program" ;
    

Sometimes that separate task can run for a long, long time, in which case it might
be handy to have the taskid:

    fork taskidvariable (somenumber of seconds) 
      "do something somenumber of seconds later, in a separate task" ;
    endfork
    "immediately after scheduling the fork, continue on with the rest of the program" ;

The taskidvariable is optional. If it's there, it gets filled with the taskid for the task that the fork creates. Typically the next thing you do is store the taskidvariable's contents in some property, so you can later on use it to check on, or if necessary kill, the task.

#### tests

The if and while flow control structures have a "test". In general,
this test an expression that evaluates to 0 for false or not-0 for
true. Not-0 includes negative values and strings and object numbers.
Basically, it includes anything but the number 0\.

    if (0)
      player:tell("This line never gets executed") ;
    endif
    if (1)
      player:tell("This line always gets executed.") ;
    endif
    if (player.iftest)
      player:tell("This line only gets executed if player.iftest contains a non-zero value.") ;
    endif
    while (1 == player.iftest)
      player:tell("This line keeps getting executed until somebody sets player.iftest to something besides 1.") ;
    endwhile
    

The test expression is often a comparison, using the double-equal-sign
comparison operator (x == y). It returns 1 for true if the two
arguments are equal, otherwise returns 0 for false. There's also !=
for not-equal, which returns 1 for true if they are not equal,
otherwise returns 0 for false.

    
    if (1 == player.iftest)
      player:tell("This line only gets executed if player.iftest contains the value 1.") ;
    endif
    

The test expression can also invoke a verb:

    
    if (player:iftestverbname())
      player:tell("This line only gets executed if the verb  player:iftestverbname() returns a non-zero value.") ;
    endif
    

#### for loop

For loops in moocode are kinda neat; "for x in (y)" is nice and
readable. Hey, Java added something similar (foreach) in 2004, it
only took them an extra 17 years (ooh that makes me feel all
smalltalkerly/LISPerly inside). The y part of "for x in (y)" can be
anything that returns a list, including:

A list slice reference...

    
    >;for x in (me.owned_objects[1..10]) me:tell(x) ; endfor
    #1449
    #1560
    #1565
    #1570
    #1573
    #1574
    #1575
    #1578
    #1579
    #1708
    => 0
    [used 4001 ticks, 0 seconds.]

Or even just a range...

    
    >;for x in [1..10] me:tell(x) ; endfor
    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    => 0
    [used 3776 ticks, 0 seconds.]
    

**suspend**

Suspend is pretty straight-forward; it just pauses the execution for
however many seconds.

    ;;suspend(20) ; me:tell("boo!") ;
    ...20 seconds pass...
    boo!
    => 0

**fork**

Fork takes a little more discussion. Fork fires off a separate task.
In this example I used all of the optional arguments. The somenumber
of seconds part delays the new task from starting until some number of
seconds in the future. Meanwhile, the rest of the verb continues on
without delay. 

You're allowed to set the delay at 0 if you just want to fire off the
new task immediately. This is useful when you want to do some things
that require time (perhaps a scripted set of comments by a robot) but
meanwhile you want the rest of the verb to proceed as usual.

The body of the fork is what gets executed by the new task (I usually
just put a one-line callout to some other verb in there and put all
the gory stuff in the separate verb).

The taskidvariable part lets you define a new variable to hold the
taskid of the new task. Of course, you have to do something with the
taskid that gets put in that variable, like store it on an object
property so you can kill it later with @kill, when it's sucking down
all of your ticks like a great tick-sucking vortex of doom.

(But, if you messed up and forgot to save the taskid, check out the @forked command).

Here are some examples of if/for/while/forked in action. Note that I'm
demonstrating all of them via eval, so each example is all in one
line. In normal code you'd have each bit on a separate line. for
example, the first if/else/endif example would be five lines in a
normal verb (if, the body of the if, else, the body of the else,
endif).

    
    >;if(1) player:tell("foo") ; else player:tell("bar") ; endif
    foo
    => 0
    >
    >;if(0) player:tell("foo") ; else player:tell("bar") ; endif
    bar
    => 0
    >
    >;if(-1) player:tell("foo") ; else player:tell("bar") ; endif
    foo
    => 0
    >
    >;; i = 3 ; while(i) me:tell("i is ", i) ; i = i -1 ; endwhile
    i is 3
    i is 2
    i is 1
    => 0
    >
    >;fork taskidvar (2) me:tell("later.") ; endfork
    => 0
    (two seconds pass)
    later.
    >
    

### Type Conversion

Moocode isn't friendly enough to auto-convert between types for you,
most of the time. You have objects references, ints, strings, floats.
Since it's MOO, by definition a lot of the time you're going to be
dealing wtih other people's data. 

Use the typeof() function to see what type a particular parameter is.

Use the toliteral() function to convert it to a string you can print
out (especially useful for looking at hairy lists-of-lists).

Use the various tofoo() functions to convert back and forth:
  * toobj() (converts a string like "\#1449" into an objectnumber reference variable)
  * toint() or tonum() (converts "1" to the value 1; originally ints were the only numerics in moo)
  * tostr() (converts the numeric value 1 to "1" or an objectnumber variable \#1449 to "\#1449")
  * tofloat() (converts the string "1.1" to the float value 1.1 or the int value 1 to the float value 1.0)
  * toliteral() (converts whatever value to a print-friendly value)
  * typeof() (returns an int value corresponding to the type of the value you passed in)

Note that typeof returns an int value, but there are several standard
values that are defined in moocode. INT is 0, OBJ is 1, STR is 2, ERR
is 3, etc. See "help typeof" for more info. You could just check to
see if the return value is 0 or 1, etc, but it's a lot smarter to use
those predefined variables. That way, when you look at a bunch of
code you wrote in a drunken binge, you'll have some vague idea wtf you
were intending to do.

## MOOCode In The Large

### Threading, Ticks and Tasks

Like just about all computers, MOO doesn't really do many things at
once, it just fakes it. Unlike other systems, MOO uses a rather
distinctive approach to faking it. It's not _really_ multitasking,
but it fakes it rather well, especially for a multi-user, multi-coder
system.

At any given point in time, the MOO server is running only one verb
invocation. This is called a task. What happens if that task gets
out of hand? What if some idiot codes a verb to calculate pi to the
final decimal place?

To keep this from causing problems, MOOcode has some internal
cost-accounting. Each task is given a certain number of "clock ticks"
at the start; last I looked I think it was 40,000 ticks. Each
operation inside the task costs some number of ticks, arbitrarily
decided by the original developers of MOO, based on how expensive they
thought that operation would be. If the task uses up all of the
ticks, the verb invocation dies with an error about being out of
ticks.

Ticks get renewed over time, so verbs that have to do a whole lot of
stuff will use suspend() to pause for some period of time and let
their ticks recharge. $command\_utils:suspend\_if\_needed(10) 
is often used for this, to only suspend() if the verb is in danger of
running out of ticks. Of course, suspend\_if\_needed() burns up ticks
in itself, so it's wise to always give it an argument of at least 5,
more like 10 seconds for the pause.

There's also a player-wide tick allocation, so if a particular player
has lots of verbs that have heavy tick usage, the performance impact
will fall more on that player, and less on the rest of the MOO.

If you want to learn more about just what costs how many ticks, do:

    ;me.eval\_env=1

And now the examples I gave you in the Flow Control section will
produce output like:

    
    >;if(1) player:tell("foo") ; else player:tell("bar") ; endif
    foo
    => 0
    [used 377 ticks, 0 seconds.]
    >;if(0) player:tell("foo") ; else player:tell("bar") ; endif
    bar
    => 0
    [used 382 ticks, 0 seconds.]
    >;if(-1) player:tell("foo") ; else player:tell("bar") ; endif
    foo
    => 0
    [used 377 ticks, 0 seconds.]
    

Note that there are a lot of odd variables that go into actual tick
costs, so they will tend to vary a little from invocation to
invocation. Don't get hung up on that facet.

## Core Classes

The real trick to moocoding is, just as with Java, learning the APIs.
With MOOCode, learning how the core infrastructure objects hook into
each other (and hence how you can code an object and drop it into this
system of interacting objects) is critical.

The @classes command displays a list of the "important" classes for
one of about five or six subsets that the wizards have decided are
worth keeping track of. One of the most important is the Generics,
and the key generics are:

* $player
* $room
* $exit
* $thing

You should probably also study up on object \#1, The Root Class, to see
what verbs you can expect to see on all objects, since The Root Class
is the grandaddy object from whom all objects are descended.

While these classes aren't the only important classes, they account
for the fundamental MOO experience. The vast majority of
player-created objects in the MOO database are descendants of $player, $room, $exit or $thing.

### Overview

There are help entries for $room ("help $room") and $exit ("help
$exit") but not for $player and $thing. Here's a quick overview of
all four.

Okay, so players are roughly self-explanatory, but the real essence of
a player is that player objects have incoming telnet connections
attached to them, and will feed incoming text to the MOO parser, and
feed output text back into the outgoing telnet. 

Outgoing text usually reaches the player object via player:tell() and
usually is passed to the outgoing telnet connection when player:tell()
calls player:notify(). Generally speaking most code isn't supposed to
call player:notify() directly.

In addition, players are generally descendants of various classes that
provide useful basic commands like say, emote, etc, and for other MOO
features that players care about, like the in-MOO mail system, @who,
some old features like setting line length and linewrapping (mostly
irrelevant now due to decent MUD client programs) and so forth.

Rooms are locations; like any MOO object they have a contents property
containing a list of objects they contain, but they also have lots of
verbs oriented towards players who are contained in them. Players
and rooms both work closely with the MOO parser to create the whole
MOO experience, both in terms of making the verbs work and in terms of
creating the sense of space and place.

Room:look() is the verb that the parser invokes if you simply type "look" with no arguments. 

Room:look() calls room:look\_self() to return text describing the room. 

Room:look\_self() calls room:description() and
room:tell\_contents() and sends the results to player:tell().

Room:description() defaults to just returning the text in the property room.description

Room:tell\_contents() defaults to just listing the objects in the property room.contents, but they can
and do often work differently. 

There are some room generics, for example, that dynamically compose
the description from objects that are in the room that are designed to
work with such rooms, and then the room leaves any such objects out of
the room:tell\_contents() output.

Rooms also have two properties, room.entrances and room.exits. Both
properties contain a list of $exits; an entrance is just an exit from
some other room that has this room as a destination. When you enter a
command, if the MOO parser can't find a verb whose name and arguments
match the command, it then tries to match an exit. If it does, it
calls the exit's :invoke() verb, which then moves you from that room
to the destination room.

Finally we have $thing, which provides the basic features you expect
to find in well, a thing; something that is neither intrinsically a
part of the room, nor an autonomous entity like a player, nor an
abstraction for movement like an exit. You can pick up a $thing using
its get verb. You can drop a thing using its drop verb, and so forth.
Things are kind of obvious and kind of minimal, but you have to start
somewhere. A lot of stuff in LambdaMOO is ultimately a descendant of
$thing.

## Detailed Examples

The two examples that come to mind are also probably the two most
common cases as well as fundamental to that sense of there-ness that
makes MOO work, which are: sight/sound and location.

## Sight and Sound in MOOCode

Sight/sound is pretty straight-forward. I lump them together because
really, they're just lines of text that are being sent to the player's
network connection. 

Everything has a :tell() verb, starting with \#1:tell(), which just
does nothing. This makes it very convenient to code verbs that emit
text and just send it to all objects in the room.

The player object has a special verb, player:notify(), that sends a
line of text to the player's network connection. player:notify() is
so special that nobody actually invokes it directly; it's almost only
ever used by player:tell() (and occasionally by player:tell\_lines(),
see note below). In fact, player:notify() has a special permission
check in its code to prevent it from being called from anything else
but verbs on the player.

player:tell(), at its simplest, relays lines to player:notify(). More
complicated versions of :tell() do checking for unwanted messages or
attempts to "spoof" the player.

(Note: spoofing is when you send text to a player with the intent to
deceive, usually by having the text not identify its source or
pretending to be output that resulted from a player command; it's
generally considered rude on its own, and when done for malicious
purposes extremely rude and/or socially objectionable).

**Note:**_player:tell\_lines() works much like player:tell() except it can
take a list of strings as an argument; it, in turn, calls
player:notify\_lines(), which loops through the list and calls
player:notify() with each individual line._

Okay, so now we know what happens to a line of text after it gets to
the player. This is the mechanism that everything in the MOO uses to
send text to your player object's network connection. When you look
at the room, for example, the room's verbs in turn call player:tell()
to feed you descriptions, a list of what's in the room, etc. This is
a fairly common practice and is considered normal.

## Location in MOOCode

Location in MOO is based on two special object properties:
"object.location" and "object.contents". All objects in MOO have these
two properties built into them. Every object always has a location
value (always a single object number), and every object has a contents
value (always a list, possibly empty). The server itself inherently
keeps track of these, so every object can only have one location at a
time, and will only (and can only) be in a single other
object.contents list.

The built-in function, "move()" updates three different object
properties in one atomic step, so they can never get out of sync:

* object.location
* source.contents
* destination.contents

However, you shouldn't be messing with move(), instead you'll use
object:moveto(\#destination).

When you move an object, the object's :moveto() verb may have some
special checks coded in it to prevent you moving it. Most players
don't like being moved around by random people.
See "help locking"; also, a lot of older players use a player class
that has a "sessile" property that, when set to 1, keeps anything from
moving them. These days players mostly just use @refuse moves to
protect themselves from idiots.

For the most part, objects are moved into/out of rooms, and into/out
of players (the stuff you're "carrying"). There are also generic
container objects - things that you can put stuff into and get stuff
out of. And, when all else fails, you can probably expect to find
your missing object off in \#-1, which appears to be where the server
sticks anything when it can't figure out where to put it.

For the most part what happens when you move something is that the
following verbs are called in something vaguely like this order:
  * call object:moveto(destinationobject)
  * moveto() calls destination:accept(objecttobemoved)
  * destination:accept() calls through to destination:acceptable(objecttobemoved) to figure out if an object actually is acceptable, then returns the value
  * destination:enterfunc(objecttobemoved) gives the destination an opportunity to react to the arrival of the object after it has entered the destination (though normally special effects are taken care of by the exits or by the verb that's calling object:moveto()).
  * source:exitfunc(objecttobemoved) gives the source an opportunity to do/say something after the object has left the source.

When you are in a room and you issue the "look" command, the parser
finds $room:look(), which in turn calls $room:look\_self().
look\_self() is the generic verb for an object to assemble what a
player sees when the object is looked at. As is often the case in
moocode, the look\_self verb doesn't just return a description, it in
turn calls player:tell().

What follows is pretty much the same process for most objects:
  * room:look\_self() is called.
  * room:look\_self() calls room:title()
  * room:look\_self() calls room:description()
  * room:look\_self() calls room:tell\_contents()

As it turns out, room:description() is not part of $room:look\_self().
I'm not sure where that get added to look\_self() (though the
:description() verb is defined on the Root Class, \#1).

### The End of the MooCode Stuff

That's all for now. The sections below are two things. 

At the very end is an outline for my next draft of this tutorial.
Really, this draft was just a brain dump of a lot of different topics
that I thought were underexplored by the current docs, back when I
wrote this. The brain dump is a bit jumbled and some things get
talked about multiple times, etc. The outline is for a more
organized attack on the whole topic.

In between the outline and here is a section adapted from an article I
originally wrote about Java, that tries to help an absolute beginner
and gets into things like "what's a statement" and "what is syntax",
etc.

### Real Basics of Programming in Moocode

These details get skipped a lot. If you've played a little bit
with some programming language, like BASIC, you may want to skip this,
but unless you're confident, I suggest you at least skim it. Even if
you are confident, I get into some programming in-jokes and stuff,
further on, that might help ease the shock of getting into the
programming world.

If you're confident, go back to the top of the article.

**Note:** _The following is focused on really basic stuff, but
it still assumes the context is the moo programming language. For
example, when I give a bit of code as example I'm not going to stop
and point out the details that are particular to moocode and maybe don't
work the same way in some other programming language. Sorry, but
there's too much to explain here already :-)._

#### A Program

A program is a huge, complex list of step-by-step instructions that
the computer carries out. These instructions are written in a
particular combination of words and punctuation.

#### A Short Example

Okay, so let's give you a short example of what a bit of program
might look like:`

    
    player:tell("Hello.") ;
    somenumber = 2 ;
    anothernumber = 3 ; 
    theothernumber ;
    theothernumber = somenumber + anothernumber;
    message = "the other number is " + tostr(theothernumber) ;
    player:tell(message) ;
    theothernumber = theothernumber + 1 ;
    message2  = "the other number is now " + tostr(theothernumber) ;
    player:tell(message2) ;
    

`

This sort of human-readable stuff that makes up the program is
usually called the _source code_, not an important detail but
if you're curious as to why [read
this](#source-code-compiled-and-interpreted-programs).

If you open up the source code for a program in an editor, you see
a bunch of words, punctuation and _whitespace_ (whitespace is
spaces, tabs, or carriage returns/newlines, basically any character 
in the source that doesn't produce a visible letter, number or punctuation,
but instead just changes the location of other characters).

_By the way "editor" is the programmer term for a word processor.
Generally editors (classics like emacs, or vi, or programs like
(shudder) Notepad on windows) are much more focused on function and
less on form. Editors are about moving words around, word processors
are about making them print out pretty. And if you're wondering why
programmers have a special term for word processor programs, it's the
other way around - what do you think was used to write the source code
for the first word processor programs?_

_While we're at it, in the modern era a lot of programmers don't
use just plain old editors anymore, they use IDEs. IDE stands for
Integrated Development Environment and is a GUI programming editor
with a bunch of other extra tools and features. Some programmers
swear by them, some swear at them. MOO doesn't have any IDEs._

The first step the computer takes in converting the human-readable
words to computer instructions is breaking them down into
_tokens_, chunking them up according to certain rules. For
starts, whitespace separates the words, and a change from letters to
punctuation usually separates the words.

Almost nobody in the programming world talks about tokens, unless
they're messing around with learning how programming langauges are
built. It's really just a fancier word for "chunks", and the main
reason I'm bringing it up is because it lets me say that when you get
right down to it, the "instructions" are a series of tokens; and now I
get to explain the different kinds of tokens without having to waffle
about keywords versus punctuation and various silly crap like that.
They're all just tokens.

#### Identifiers: Keywords, Variables, Literals

Strings of letters like "somenumber", are _identifiers_.

An identifier is either:

* a _keyword_ (a predefined
word that is an instruction to the computer)
* a name for a _variable_ (a holder for some data)
* a _literal_ value

In the example, `player` is a keyword, while
`somenumber`,
`anothernumber`, and
`theothernumber` are variable names.

**Variable** names account for most of the identifiers in any large
given program, since there're a limited number of keywords. On the
other hand, those keywords get repeated a lot in any program.

**Keywords** in moocode are often called _built-ins_, as in, "this
feature is built into the moo server", as opposed to the various _$util
libraries_ that are written in moocode in the db but available to all
users.

In other languages keywords are sometimes called "reserved words"
(meaning that you can't use them for a variable name, because they're
reserved to mean something special). Every now and then you even run
into a reserved word that doesn't actually do anything, the people who
designed that programming language just thought it would be a good
idea to keep that keyword reserved for some reason.

There are also rules for what you can put in a variable name, but
for now just use normal words (nouns mostly), avoid using a keyword,
and avoid doing anything fancy with punctuation or numbers, and you'll
be fine.

A _literal_ means that instead of having a variable that
contains a value, you just literally have the value typed right there
in the code, like the number 343243, for example.

Programmer-speak for a bit of text is a _String_, as in "a
string of letters", or really "a string of characters", since text can
include number characters and punctuation characters, as well as
letter characters. To put a literal String in a program, put quotes
around it, "like this".

#### Punctuation

Most punctuation in the code is one of two things, either 1)
operators or 2) start/end indicators for an expression, statement or
block. See [Fun With Punctuation](#source-code-and-fun-with-punctuation) for a
little more detail.

##### Operators

An _operator_ is basically a bit of puncuation that
**does something**, like the plus character + adds to
values together, or the minus character - subtracts them.

Most of the usual math symbols do what you'd expect them to, except
for the equals sign =, which is an important exception I'll get to in
just a second.

Another exception is that the plus sign "+" can be used to add two
strings of characters together, so:

`"foo" + "bar"`

Works out to have the same value as `"foobar"`. There's
not much use for `"foo" + "bar"` in programming, but
there's a lot of use for gluing together strings.

Note that in moocode you can only add strings to strings and
numbers to numbers. If you want to concatenate a string to a number,
you have to use the built-in tostr() function, like this:
    
    
    message = "The number is:  " + tostr(somenumber) ;
    

Many programming languages have increment "++" and decrement "--",
but moocode does not.

##### The Equals Sign `=`, Assignment and Comparison

The equals sign is used for _assignment_, meaning, storing a
value into a variable. If you see:

`a = 5` 

That means "store the value 5 in variable a." When programmers
write this out, they usually say something like "let a equal five" or "a is
assigned the value five".

If you try to put a literal on the left side, like this:

`5 = b`

...the compiler yells at you. This turns out to be a very good
thing, we'll talk about it in a moment.

You can also assign one variable to another, which means that the
value in the second variable gets copied to the first variable as
well:

`a = b`

#### Equals Signs, Assignment and Comparison, or Stupid Programmer Mistakes 

Using a single equals sign for assignment is a very firmly
established tradition in programming. This is sort of unfortunate,
because it leads to one of the most common, and most frustrating typos
in the programming world, which is "confusing assignment (=) with
comparison (==)".

Most people first learn about the equals sign for comparison, like
"is a equal to b?", and for finding out the value, like "two plus two
equals 4." The motion sort of goes left-to-right, the same way you
read english. In programming, when you see the equals sign, the
motion goes the other way, from right to left - the value on the right
gets stored in the variable on the left.

You use a double-equals sign `==` to do comparison. You
write "is a equal to b?" like this:

`a == b`

It is very, very easy, even for an experienced programmer, to slip
and write this instead:

`a = b`

When this does happen, it can lead to very weird behavior, which is
frustratingly hard to track down, and when you finally find it, you
feel really stupid. Bear in mind, it's usually buried among pages
and pages of other code, so it's really easy to look at it and see
what you're expecting to see, not the error. 

One really good rule of thumb that I picked up somewhere is
"literal on the left". If you're doing a comparison with a literal
value, put the literal value on the left side. For example:

`5 == a`

This is good because it forces you to think in a slightly different
way about it, and because the compiler will yell if you slip and write
`5 = a`.

#### Syntax

There are rules about how tokens can be used and how they can go
together. These rules are called _syntax_. As you start to
program, you'll be hearing about syntax a lot, mainly because a lot of
the more common mistakes beginners make are syntax errors, usually
finicky typos that are just damned hard to remember, until they become
ingrained by habit. Don't get frustrated, it's not you; even
experienced programmers often (usually) make stupid typos in their
first draft of a piece of code.

##### expressions

An _expression_ is the smallest piece of code that can be
treated like a value. So a variable is an expression. So is a
literal. An operation, like `1 + 1` is an expression. So
is `somenumber + anothernumber`. I guess you could say, as
a rule of thumb, that anything that you could take out and replace
with a literal value, is an expression.

Another way to say that is, "anything that returns a value".
"Returns a value" is programmer-speak for "results in a value being
sent back". There's a `return` keyword that does this
from a method (which is basically a grouping of code, also sometimes
called procedures, subroutines, or functions). So a call to a method
that returns a value is also an expression.

##### statements

If an expression is a term, a _statement_ is a phrase.
Since it's a phrase, not a sentence, you don't put a period at the end
of it, you put a semi-colon `;` at the end of it.

For example: `int somenumber = 2 + 2 ;`

#### Source Code and Fun With Punctuation

Most punctuation is a single character, so there's nothing
complicated as to what separates different punctuation tokens. There
are some two-character combinations and there are some matched sets.
The matched sets are usually used for organizing things, to start and
end sections:

Usually a left parenthesis "(" has to have a right parenthesis ")"
somewhere. Mostly parentheses are used for verb calls and any verb
call parameters are put between the parentheses,
e.g. player:tell("hello").

The characters "{" and "}", most programmers call curly brackets
but appear to have a dozen or more possibly-correct names, depending
on who you ask. Some programming languages use curly brackets to
define the start and stop of a program block, for example the contents
of a while loop. Moocode uses "end-whatever" to indicate the end of a
block, for example if/endif, while/endwhile, for/endfor, and so forth.

What MOO actually uses curly brackets for is defining lists of
things. A list is pretty much just what it says, a sequential
collection of elements. If you're familiar with arrays, think of a
list as a smart array, that grows and shrinks as necessary. Here's an
example:
    
    stringslist = {"one", "two", "three"};

Here's another example: 
    
    numberslist = {1, 2, 3};

A list can contain mixed types of elements, for example: 
    
    mixedlist = {1, "two", 3};

Most languages use square brackets "\[" and "\]" to _index_ into
an array or list, for example if you one the "two" from the mixedlist
example, you'd use:
    
    twostring = mixedlist[2];

One exception to all this left-right nonsense is the angle brackets, "

Quotes " usually have to be in a matched set. Pretty much no
programming language uses "smart quotes", where the quotes are angled
opposite each other. When programmers specifically want to say
not-smart-quotes they usually say "straight quotes".

Some programming languages have special meanings for the
single-quote ' and back-tick \` characters. Moocode did not at first,
but have since used them for catching errors, see the section "Catching
Errors in Expressions" in the LambdaMOO Programmer's Tutorial.

#### Multitasking

When you get right down to it, computers can only do one thing at a
time. They can just do them so fast it looks like they're doing
several things at once. This is called "multitasking." Juggling is a
popular metaphor for this, but I prefer the chess master metaphor.

The computer acts sort of like a brilliant chess player who can
play against twenty ordinary people at once - make a move on this
chess board, move to the next chess board and make a move, move to the
next chess board, etc. It may look like the chess player is doing
twenty things at once, but really he's just walking up to a chess
board, studying the context - the layout of the chess pieces - in a
glance and deciding on the right move.

Moo uses a system called tasks and ticks to manage this. This is
discussed in more detail up above.

#### Source Code: Compiled and Interpreted Programs

The human-readable version of the program is called the _source
code_. Why this is, you don't really need to know, nor do you
really need to know what compiled or interpreted mean. But if
you're interested, read on.

The computer itself only understands numbers. Everything
that you see a computer do that looks like it deals with words, is in
the end boiled down to numbers. The program is too - the
human-readable version is **purely** for your
convenience. And believe me, it is one heck of a convenience.

Something has to convert that human-readable stuff to machine code
(pure 1s and 0s). This is called a compiler, or sometimes an
interpreter. A compiler converts just once. The result is a set of
1s and 0s that the machine runs. An interpreter converts on the fly;
it's a program that runs your program, interpreting your words and
issuing the right 1s-and-0s version of the commands as necessary.

But in real life, nothing is that simple.

First of all, there's a lot of give and take in terms of whether a
particular program is a compiler, per se, or an interpreter. Many
"interpreters" actually compile the program right when you run the
program. Some compilers (like the moo compiler, and also the java
compiler) compile the program halfway, to a generic format called
bytecode, and then it's run on a _virtual machine_ (like the
moo server, ro the Java Virtual Machine, JVM for short), which is
basically another take on the interpreter, a program that runs a
program.

Second of all, there is seldom a one-for-one word-to-code
translation. Most often, a short set of words converts into dozens or
more machine codes. On top of that, many compilers and interpreters
then rearrange commands to make it all run faster, while still doing
exactly what you said to do.

#### RTFM, "Use The Source, Luke", and "ask smart questions"

As you learn and ask people for help, you'll hear two
phrases very often, RTFM (Read The Fucking Manual) and "Use the
Source, Luke" (sometimes abbreviated UTSL). A growing trend is the
third phrase, [learn how
to ask smart questions](http://www.catb.org/~esr/faqs/smart-questions.html), after an essay written by Eric S. Raymond
about how to do just that.

All of these boil down to "do your own homework before you ask me
for help." Like reading this page. All of us have been there before,
and it sucks, we'll spend tons of energy to **help** you
figure out how to do it yourself, but we're not going to do it
**for** you.

#### Foo, Bar, Baz: Metasyntactic Variables and Stupid Programmer Jokes

A lot of programmers use the words _foo_, _bar_, and
_baz_ a lot in examples. This is so common that there's
actually now a jargon phrase for it, the metasyntactic variables.

The reason these show up so often is that often you need to explain
something, and you need to explain it with an example, and the example
has to refer to some things, purely for the purpose of the example.
So programmers use "foo" and "bar" (from the military slang FUBAR, for
"Fucked Up Beyond All Recognition). The third variable "baz", is just
a corruption of "bar", because you often need a third example
variable.

Why not use real words? Three reasons:

* Because you may not want to distract yourself from the main
problem long enough to think of a good, plausible example.
* Because you want your reader/listener to be concentrating on HOW
you're doing whatever, not on WHAT you're doing it to.
* Because "foobar" is a stupid programmer joke of long-standing tradition.

#### The End

That's the end of our little "the really real realest basics of programming"
section, so you can go back to the top and read the regular tutorial.

### Outline of Planned Revision 

* Basic MOOCode Gotchas
  * Comments  
  * Variable Declaration and Scoping
  * Lists
  * Flow Control: for, if, while, suspend, fork
  * Types, typeof(), toobj(), toint() or tonum(), tostr(), toliteral(), tofloat()
* MOOCode In The Large
  * Object References (Object Numbers)
  * Implicit Environment References: this, player, caller, callers()
  * Verb Invocation, Property Access 
  * Perms and Args
  * Help
  * Parameter Passing
  * Threading, Ticks and Tasks
* The MOOCoder's Toolbox
  * @display, @args, @chmod
  * Eval ;
  * Eval Environment: ;player.eval\_env="here=player.location;me=player;"
  * Multi-Line Evals ;;
* The Framework
  * Parsing
  * Matching
  * Feature Objects 
  * Core Classes
    * $room
    * $player
    * $exit
    * $thing
    * $features
    * $utils
    * \#0
    * \#-1
* Some "interesting" functions and properties
  * max\_object()
  * $playerdb
  * player.owned\_objects

---

# This source code was pulled from [http://www.darksleep.com/notablog/articles/LambdaMOO\_Programming\_Tutorial](http://www.darksleep.com/notablog/articles/LambdaMOO_Programming_Tutorial) and is included in this repository for posterity. It is not currently HTML.

[5]: 
[6]: 
