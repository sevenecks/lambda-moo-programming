# LambdaMOO Programmer's Manual

## For LambdaMOO Version 1.8.0p6 (and beyond!)

Originally written March, 1997:

by Pavel Curtis, aka Haakon, aka Lambda

Updated January, 2018 ([CHANGE LOG][0]):

By [Brendan Butts][1], aka Slither, aka Fengshui

For older versions of this document please see the resources section.

## LambdaMOO Resources

* [lisdude MOO resources][2]
* [Unedited MOO Programmers Manual][3]
* [Older Unedited MOO Programmers Mnaual][4]
* [LambdaMOO Source (github)][5]
* [LambdaMOO Databases (and other resources)][6]
* [MOO Talk Mailing List][7]
* [Dome Client Web Socket MOO Client][8]

## Table of Contents

* [Introduction][9]

---

## Foreword

Hey, it's me: Brendan. The guy updating this document. The idea of updating the MOO Programming Manual 
has crossed my mind a few times. It's a bit of a daunting task. This document is pretty comprehensive and 
I'm endeavouring to make it even more so. I'm adding some much needed styling so that it looks more modern. 
Things were once hosted primarily on FTP servers and we now have GitHub, so many links and resources mentioned 
in this document have been updated and I've even added some additional ones.

If you want an up to date manual on MOO, this is the guide for you. If you want to read the historic document 
which contains the unedited text, I recommend reviewing the links toward the top of the page that point to unedited
versions of the guide.

## Introduction

LambdaMOO is a network-accessible, multi-user, programmable, interactive
system well-suited to the construction of text-based adventure games,
conferencing systems, and other collaborative software. Its most common use,
however, is as a multi-participant, low-bandwidth virtual reality, and it is
with this focus in mind that I describe it here.

Participants (usually referred to as _players_) connect to LambdaMOO using
Telnet or some other, more specialized, _client_ program. Upon
connection, they are usually presented with a _welcome message_ explaining
how to either create a new _character_ or connect to an existing one.
Characters are the embodiment of players in the virtual reality that is
LambdaMOO.

Note: No one really connects to a MOO over telnet these days. MOO Clients and MUD Clients are 
relatively common. See the resources section for more information on these. There are even some web based clients (dome-client) out there that use websockets to connect to a MOO directly from the browser.

Having connected to a character, players then give one-line commands that are
parsed and interpreted by LambdaMOO as appropriate. Such commands may cause
changes in the virtual reality, such as the location of a character, or may
simply report on the current state of that reality, such as the appearance of
some object.

The job of interpreting those commands is shared between the two major
components in the LambdaMOO system: the _server_ and the _database_.
The server is a program, written in a standard programming language, that
manages the network connections, maintains queues of commands and other tasks
to be executed, controls all access to the database, and executes other
programs written in the MOO programming language. The database contains
representations of all the objects in the virtual reality, including the MOO
programs that the server executes to give those objects their specific
behaviors.

Almost every command is parsed by the server into a call on a MOO procedure,
or _verb_, that actually does the work. Thus, programming in the MOO
language is a central part of making non-trivial extensions to the database
and thus, the virtual reality.

In the next chapter, I describe the structure and contents of a LambdaMOO
database. The following chapter gives a complete description of how the
server performs its primary duty: parsing the commands typed by players.
Next, I describe the complete syntax and semantics of the MOO programming
language. Finally, I describe all of the database conventions assumed by the
server.

Note: For the most part, this manual describes only those aspects of LambdaMOO that are
entirely independent of the contents of the database. It does not describe,
for example, the commands or programming interfaces present in the LambdaCore
database. There are exceptions to this, for situations where it seems prudent to delve deeper into these areas.

## The LambdaMOO Database

In this chapter, I begin by describing in detail the various kinds of data
that can appear in a LambdaMOO database and that, therefore, MOO programs can
manipulate. In a few places, I refer to the _LambdaCore_ database. This
is one particular LambdaMOO database, created every so often by extracting the
"core" of the current database for the original LambdaMOO.

Note: The original LambdaMOO resides on the host
lambda.parc.xerox.com (the numeric address for which is
192.216.54.2), on port 8888\. Feel free to drop by! A copy of the most
recent release of the LambdaCore database can be obtained by anonymous FTP from
host ftp.parc.xerox.com in the directory pub/MOO.

The above information may be out of date, but the most recent dump of the _LambdaCore_
can be found [here][6].

### MOO Value Types

There are only a few kinds of values that MOO programs can manipulate:

* integers (in a specific, large range)
* real numbers (represented with floating-point numbers)
* strings (of characters)
* objects (in the virtual reality)
* errors (arising during program execution)
* lists (of all of the above, including lists)

MOO supports the integers from -2^31 (that is, negative two to the power
of 31) up to 2^31 - 1 (one less than two to the power of 31); that's
from -2147483648 to 2147483647, enough for most purposes. In MOO
programs, integers are written just as you see them here, an optional minus
sign followed by a non-empty sequence of decimal digits. In particular, you
may not put commas, periods, or spaces in the middle of large integers, as we
sometimes do in English and other natural languages (e.g., \`2,147,483,647').

Real numbers in MOO are represented as they are in almost all other programming
languages, using so-called _floating-point_ numbers. These have certain
(large) limits on size and precision that make them useful for a wide range of
applications. Floating-point numbers are written with an optional minus sign
followed by a non-empty sequence of digits punctuated at some point with a
decimal point (\`.') and/or followed by a scientific-notation marker (the letter
\`E' or \`e' followed by an optional sign and one or more digits). Here are some
examples of floating-point numbers:

    325.0   325.  3.25e2   0.325E3   325.E1   .0325e+4   32500e-2

All of these examples mean the same number. The third of these, as an example
of scientific notation, should be read "3.25 times 10 to the power of 2".

Fine point: The MOO represents floating-point numbers using the local
meaning of the C-language double type, which is almost always equivalent
to IEEE 754 double precision floating point. If so, then the smallest positive
floating-point number is no larger than 2.2250738585072014e-308 and the
largest floating-point number is 1.7976931348623157e+308.

* IEEE infinities and NaN values are not allowed in MOO.
* The error E\_FLOAT is raised whenever an infinity would otherwise be computed.
* The error E\_INVARG is raised whenever a NaN would otherwise arise.
* The value 0.0 is always returned on underflow.

Character _strings_ are arbitrarily-long sequences of normal, ASCII
printing characters. When written as values in a program, strings are
enclosed in double-quotes, like this:

    "This is a character string."

To include a double-quote in the string, precede it with a backslash
(\\), like this:

    "His name was \"Leroy\", but nobody ever called him that."

Finally, to include a backslash in a string, double it:

    "Some people use backslash ('\\') to mean set difference."

MOO strings may not include special ASCII characters like carriage-return,
line-feed, bell, etc. The only non-printing characters allowed are spaces and
tabs.

Fine point: There is a special kind of string used for representing the
arbitrary bytes used in general, binary input and output. In a _binary
string_, any byte that isn't an ASCII printing character or the space character
is represented as the three-character substring "~XX", where XX is the
hexadecimal representation of the byte; the input character \`~' is represented
by the three-character substring "~7E". This special representation is used by
the functions encode\_binary() and decode\_binary() and by the
functions notify() and read() with network connections that are
in binary mode. See the descriptions of the set\_connection\_option(),
encode\_binary(), and decode\_binary() functions for more details.

_Objects_ are the backbone of the MOO database and, as such, deserve a
great deal of discussion; the entire next section is devoted to them. For now,
let it suffice to say that every object has a number, unique to that object.

In programs, we write a reference to a particular object by putting a hash mark
(\#) followed by the number, like this:

    #495

Note: Referencing object numbers in your code should be discouraged. An object only
exists until it is recycled. It is technically possible for an object number to change under some circumstances. Thus, you should use a corified reference to an object ($my\_special\_object) instead. More on corified references later.

Object numbers are always integers.

There are three special object numbers used for a variety of purposes:
\#-1, \#-2, and \#-3, usually referred to in the
LambdaCore database as $nothing, $ambiguous\_match, and
$failed\_match, respectively.

_Errors_ are, by far, the least frequently used values in MOO. In the
normal case, when a program attempts an operation that is erroneous for some
reason (for example, trying to add a number to a character string), the server
stops running the program and prints out an error message. However, it is
possible for a program to stipulate that such errors should not stop execution;
instead, the server should just let the value of the operation be an error
value. The program can then test for such a result and take some appropriate
kind of recovery action. In programs, error values are written as words
beginning with E\_. The complete list of error values, along with their
associated messages, is as follows:

**E\_NONE**

> No error

**E\_TYPE**

> Type mismatch

**E\_DIV**

> Division by zero

**E\_PERM**

> Permission denied

**E\_PROPNF**

> Property not found

**E\_VERBNF**

> Verb not found

**E\_VARNF**

> Variable not found

**E\_INVIND**

> Invalid indirection

**E\_RECMOVE**

> Recursive move

**E\_MAXREC**

> Too many verb calls

**E\_RANGE**

> Range error

**E\_ARGS**

> Incorrect number of arguments

**E\_NACC**

> Move refused by destination

**E\_INVARG**

> Invalid argument

**E\_QUOTA**

> Resource limit exceeded

**E\_FLOAT**

> Floating-point arithmetic error

Note: The MOO can be extended using extensions. These are plugins that usually contain patch files that are applied directly to your LambdaMOO source code. One very popular extension is the FileIO plugin, which allows you to read and write files to disk. Extensions can throw their own errors, though it's not as common. Old versions of FileIO threw a fake error "E\_FILE", though a recently updated version throws true E\_FILE errors.

The final kind of value in MOO programs is _lists_. A list is a sequence
of arbitrary MOO values, possibly including other lists. In programs,
lists are written in mathematical set notation with each of the elements
written out in order, separated by commas, the whole enclosed in curly
braces ({ and }). For example, a list of the names of
the days of the week is written like this:

    {"Sunday", "Monday", "Tuesday", "Wednesday",
     "Thursday", "Friday", "Saturday"}
    

Note: It doesn't matter that we put a line-break in the middle of
the list. This is true in general in MOO: anywhere that a space can go,
a line-break can go, with the same meaning. The only exception is
inside character strings, where line-breaks are not allowed.

### Objects in the MOO Database

Objects are, in a sense, the whole point of the MOO programming language.
They are used to represent objects in the virtual reality, like people, rooms,
exits, and other concrete things. Because of this, MOO makes a bigger deal
out of creating objects than it does for other kinds of value, like integers.

Numbers always exist, in a sense; you have only to write them down in order to
operate on them. With objects, it is different. The object with number
\#958 does not exist just because you write down its number. An
explicit operation, the create() function described later, is required
to bring an object into existence. Symmetrically, once created, objects
continue to exist until they are explicitly destroyed by the recycle()
function (also described later).

The identifying number associated with an object is unique to that
object. It was assigned when the object was created and will never be
reused, even if the object is destroyed. Thus, if we create an object
and it is assigned the number \#1076, the next object to be
created will be assigned \#1077, even if \#1076 is destroyed
in the meantime.

The above limitation led to design of systems to manage object reuse. The $recycler
is one example of such a system. This is **not** present in the minimal.db
which is included in the LambdaMOO source, however it is present in the latest dump of the 
[LambdaCore DB][6] which is the recommended starting point for new development.

Every object is made up of three kinds of pieces that together define its
behavior: _attributes_, _properties_, and _verbs_.

#### Fundamental Object Attributes

There are three fundamental _attributes_ to every object:

1. A flag (either true or false) specifying whether or not the object represents a player
2. The object that is its _parent_
3. A list of the objects that are its _children_; that is, those objects for which this object is their parent.

The act of creating a character sets the player attribute of an object and
only a wizard (using the function set\_player\_flag()) can change that
setting. Only characters have the player bit set to 1\.

The parent/child hierarchy is used for classifying objects into general classes
and then sharing behavior among all members of that class. For example, the
LambdaCore database contains an object representing a sort of "generic" room.
All other rooms are _descendants_ (i.e., children or children's children,
or ...) of that one. The generic room defines those pieces of behavior
that are common to all rooms; other rooms specialize that behavior for their
own purposes. The notion of classes and specialization is the very essence of
what is meant by _object-oriented_ programming. Only the functions
create(), recycle(), 
chparent(), and renumber() can
change the parent and children attributes.

#### Properties on Objects

A _property_ is a named "slot" in an object that can hold an arbitrary
MOO value. Every object has eight built-in properties whose values are
constrained to be of particular types. In addition, an object can have any
number of other properties, none of which have type constraints. The built-in
properties are as follows:

**name**

> a string, the usual name for this object

**owner**

> an object, the player who controls access to it

**location**

> an object, where the object is in virtual reality

**contents**

> a list of objects, the inverse of location

**programmer**

> a bit, does the object have programmer rights?

**wizard**

> a bit, does the object have wizard rights?

**r**

> a bit, is the object publicly readable?

**w**

> a bit, is the object publicly writable?

**f**

> a bit, is the object fertile?

The name property is used to identify the object in various printed
messages. It can only be set by a wizard or by the owner of the object. For
player objects, the name property can only be set by a wizard; this
allows the wizards, for example, to check that no two players have the same
name.

The owner identifies the object that has owner rights to this object,
allowing them, for example, to change the name property. Only a wizard
can change the value of this property.

The location and contents properties describe a hierarchy of
object containment in the virtual reality. Most objects are located
"inside" some other object and that other object is the value of the
location property.

The contents property is a list of those
objects for which this object is their location. In order to maintain the
consistency of these properties, only the move() function is able to
change them.

The wizard and programmer bits are only applicable to
characters, objects representing players. They control permission to use
certain facilities in the server. They may only be set by a wizard.

The r bit controls whether or not players other than the owner of this
object can obtain a list of the properties or verbs in the object.

Symmetrically, the w bit controls whether or not non-owners can add or
delete properties and/or verbs on this object. The r and w bits
can only be set by a wizard or by the owner of the object.

The f bit specifies whether or not this object is _fertile_, whether
or not players other than the owner of this object can create new objects with
this one as the parent. It also controls whether or not non-owners can use the
chparent() built-in function to make this object the parent of an
existing object. The f bit can only be set by a wizard or by the owner
of the object.

All of the built-in properties on any object can, by default, be read by any
player. It is possible, however, to override this behavior from within the
database, making any of these properties readable only by wizards. See the
chapter on server assumptions about the database for details.

As mentioned above, it is possible, and very useful, for objects to have other
properties aside from the built-in ones. These can come from two sources.

First, an object has a property corresponding to every property in its parent
object. To use the jargon of object-oriented programming, this is a kind of
_inheritance_. If some object has a property named foo, then so
will all of its children and thus its children's children, and so on.

Second, an object may have a new property defined only on itself and its
descendants. For example, an object representing a rock might have properties
indicating its weight, chemical composition, and/or pointiness, depending upon
the uses to which the rock was to be put in the virtual reality.

Every defined property (as opposed to those that are built-in) has an owner
and a set of permissions for non-owners. The owner of the property can get
and set the property's value and can change the non-owner permissions. Only a
wizard can change the owner of a property.

The initial owner of a property is the player who added it; this is usually,
but not always, the player who owns the object to which the property was
added. This is because properties can only be added by the object owner or a
wizard, unless the object is publicly writable (i.e., its w property is
1), which is rare. Thus, the owner of an object may not necessarily be the
owner of every (or even any) property on that object.

The permissions on properties are drawn from this set: r (read),
w (write), and c (change ownership in descendants). Read
permission lets non-owners get the value of the property and, of course, write
permission lets them set that value. The c permission bit is a little
more complicated.

Recall that every object has all of the properties that its parent does and
perhaps some more. Ordinarily, when a child object inherits a property from
its parent, the owner of the child becomes the owner of that property. This
is because the c permission bit is "on" by default. If the c
bit is not on, then the inherited property has the same owner in the child as
it does in the parent.

As an example of where this can be useful, the LambdaCore database ensures
that every player has a password property containing the encrypted
version of the player's connection password. For security reasons, we don't
want other players to be able to see even the encrypted version of the
password, so we turn off the r permission bit. To ensure that the
password is only set in a consistent way (i.e., to the encrypted version of a
player's password), we don't want to let anyone but a wizard change the
property. Thus, in the parent object for all players, we made a wizard the
owner of the password property and set the permissions to the empty string,
"". That is, non-owners cannot read or write the property and, because
the c bit is not set, the wizard who owns the property on the parent
class also owns it on all of the descendants of that class.

**Warning:** The MOO will only hash the first 8 characters of a password. In practice
this means that the passwords password and password12345
are exactly the same and either one can be used to login.

Another, perhaps more down-to-earth example arose when a character named Ford
started building objects he called "radios" and another character, yduJ,
wanted to own one. Ford kindly made the generic radio object fertile, allowing
yduJ to create a child object of it, her own radio. Radios had a property
called channel that identified something corresponding to the frequency
to which the radio was tuned. Ford had written nice programs on radios (verbs,
discussed below) for turning the channel selector on the front of the radio,
which would make a corresponding change in the value of the channel
property. However, whenever anyone tried to turn the channel selector on
yduJ's radio, they got a permissions error. The problem concerned the
ownership of the channel property.

As I explain later, programs run with the permissions of their author. So, in
this case, Ford's nice verb for setting the channel ran with his permissions.
But, since the channel property in the generic radio had the c
permission bit set, the channel property on yduJ's radio was owned by
her. Ford didn't have permission to change it! The fix was simple. Ford
changed the permissions on the channel property of the generic radio to
be just r, without the c bit, and yduJ made a new radio. This
time, when yduJ's radio inherited the channel property, yduJ did not
inherit ownership of it; Ford remained the owner. Now the radio worked
properly, because Ford's verb had permission to change the channel.

#### Verbs on Objects

The final kind of piece making up an object is _verbs_. A verb is a named
MOO program that is associated with a particular object. Most verbs implement
commands that a player might type; for example, in the LambdaCore database,
there is a verb on all objects representing containers that implements
commands of the form put _object_ in _container_.

It is also possible for MOO programs to invoke the verbs defined on objects. Some verbs,
in fact, are designed to be used only from inside MOO code; they do not
correspond to any particular player command at all. Thus, verbs in MOO are
like the _procedures_ or _methods_ found in some other programming languages.

Note: There are even more ways to refer to _verbs_ and
their counterparts in other programming language: _procedure_, _function_, _subroutine_, 
_subprogram_, and _method_ are the primary onces. However, in _Object Oriented Programming_
abbreviated _OOP_ you may primarily know them as methods.

As with properties, every verb has an owner and a set of permission bits. The
owner of a verb can change its program, its permission bits, and its argument
specifiers (discussed below). Only a wizard can change the owner of a verb.

The owner of a verb also determines the permissions with which that verb runs;
that is, the program in a verb can do whatever operations the owner of that
verb is allowed to do and no others. Thus, for example, a verb owned by a
wizard must be written very carefully, since wizards are allowed to do just
about anything.

Warning: This is serious business. The MOO has a variety of checks
in place for permissions (at the object, verb and property levels) that are all but ignored when a verb is 
executing with a wizard's permisisons. You may want to create a non-wizard character and give them the 
programmer bit, and write much of your code there, leaving the wizard bit for things that actually require
access to everything, despite permissions.

The permission bits on verbs are drawn from this set: r (read),
w (write), x (execute), and d (debug). Read permission
lets non-owners see the program for a verb and, symmetrically, write
permission lets them change that program. The other two bits are not,
properly speaking, permission bits at all; they have a universal effect,
covering both the owner and non-owners.

The execute bit determines whether or not the verb can be invoked from within
a MOO program (as opposed to from the command line, like the put verb
on containers). If the x bit is not set, the verb cannot be called
from inside a program. The x bit is usually set.

The setting of the debug bit determines what happens when the verb's program
does something erroneous, like subtracting a number from a character string.
If the d bit is set, then the server _raises_ an error value; such
raised errors can be _caught_ by certain other pieces of MOO code. If the
error is not caught, however, the server aborts execution of the command and,
by default, prints an error message on the terminal of the player whose command
is being executed. (See the chapter on server assumptions about the database
for details on how uncaught errors are handled.) If the d bit is not
set, then no error is raised, no message is printed, and the command is not
aborted; instead the error value is returned as the result of the erroneous
operation.

Note: The d bit exists only for historical reasons; it used to
be the only way for MOO code to catch and handle errors. With the introduction
of the try-except statement and the error-catching expression,
the d bit is no longer useful. All new verbs should have the d
bit set, using the newer facilities for error handling if desired. Over time,
old verbs written assuming the d bit would not be set should be changed
to use the new facilities instead.

In addition to an owner and some permission bits, every verb has three
_argument specifiers_, one each for the direct object, the preposition, and
the indirect object. The direct and indirect specifiers are each drawn from
this set: this, any, or none. The preposition specifier
is none, any, or one of the items in this list:

* with/using
* at/to
* in front of
* in/inside/into
* on top of/on/onto/upon
* out of/from inside/from
* over
* through
* under/underneath/beneath
* behind
* beside
* for/about
* is
* as
* off/off of

The argument specifiers are used in the process of parsing commands,
described in the next chapter.

## The Built-in Command Parser

The MOO server is able to do a small amount of parsing on the commands
that a player enters. In particular, it can break apart commands that
follow one of the following forms:

* _verb_
* _verb_ _direct-object_
* _verb_ _direct-object_ _preposition_ _indirect-object_

Real examples of these forms, meaningful in the LambdaCore database, are
as follows:

    
    look
    take yellow bird
    put yellow bird in cuckoo clock
    

Note that English articles (i.e., the, a, and an) are not
generally used in MOO commands; the parser does not know that they are not important parts of objects' names.

To have any of this make real sense, it is important to understand precisely how the server decides what to do when a player types a
command.

First, the server checks whether or not the first non-blank character in the command is one of the following:

* _"_
* _:_
* _;_

If so, that character is replaced by the corresponding command below, followed
by a space:

* _say_
* _emote_
* _eval_

For example this command:

"Hi, there.

will be treated exactly as if it were as follows:

say Hi, there.

The server next breaks up the command into words. In the simplest case,
the command is broken into words at every run of space characters; for example,
the command foo bar baz would be broken into the words foo,
bar, and baz. To force the server to include spaces in a
"word", all or part of a word can be enclosed in double-quotes. For example, the command:

foo "bar mumble" baz" "fr"otz" bl"o"rt

is broken into the words foo, bar mumble, baz frotz, and
blort.

Finally, to include a double-quote or a backslash in a word,
they can be preceded by a backslash, just like in MOO strings.

Having thus broken the string into words, the server next checks to see if the
first word names any of the six "built-in" commands: 

* .program
* PREFIX
* OUTPUTPREFIX
* SUFFIX
* OUTPUTSUFFIX
* or the connection's defined _flush_ command, if any (.flush by default).

The first one of these is only available to programmers, the next four are
intended for use by client programs, and the last can vary from database to
database or even connection to connection; all six are described in the final
chapter of this document, "Server Commands and Database Assumptions". If the
first word isn't one of the above, then we get to the usual case: a normal MOO
command.

The server next gives code in the database a chance to handle the command. If
the verb $do\_command() exists, it is called with the words of the
command passed as its arguments and argstr set to the raw command typed
by the user. If $do\_command() does not exist, or if that verb-call
completes normally (i.e., without suspending or aborting) and returns a false
value, then the built-in command parser is invoked to handle the command as
described below. Otherwise, it is assumed that the database code handled the
command completely and no further action is taken by the server for that
command.

If the built-in command parser is invoked, the server tries to parse the
command into a verb, direct object, preposition and indirect object. The first
word is taken to be the verb. The server then tries to find one of the
prepositional phrases listed at the end of the previous section, using the
match that occurs earliest in the command. For example, in the very odd
command foo as bar to baz, the server would take as as the
preposition, not to.

If the server succeeds in finding a preposition, it considers the words
between the verb and the preposition to be the direct object and those
after the preposition to be the indirect object. In both cases, the
sequence of words is turned into a string by putting one space between
each pair of words. Thus, in the odd command from the previous
paragraph, there are no words in the direct object (i.e., it is
considered to be the empty string, "") and the indirect object is
"bar to baz".

If there was no preposition, then the direct object is taken to be all
of the words after the verb and the indirect object is the empty string.

The next step is to try to find MOO objects that are named by the direct
and indirect object strings.

First, if an object string is empty, then the corresponding object is the
special object \#-1 (aka $nothing in LambdaCore). If an object
string has the form of an object number (i.e., a hash mark (\#) followed
by digits), and the object with that number exists, then that is the named
object. If the object string is either "me" or "here", then the
player object itself or its location is used, respectively.

Note: $nothing is considered a corified object. 
This means that a _property_ has been created on \#0 named nothing
with the value of \#-1. For example (after creating the property): ;\#0.nothing = \#-1

This allows you to reference the \#-1 object via it's corified reference of 
$nothing. In practice this can be very useful as you can use corified references in your code (and should!) 
instead of object numbers.

Among other benefits this allows you to write your code (which references other objects) once and then swap out the 
corified reference, pointing to a different object.

For instance if you have a new errror logging system and you 
want to replace the old $error\_logger reference with your new one, you wont have to find all the references to the old 
error logger object number in your code. You can just change the property on \#0 to reference the new object.

Otherwise, the server considers all of the objects whose location is either
the player (i.e., the objects the player is "holding", so to speak) or the
room the player is in (i.e., the objects in the same room as the player); it
will try to match the object string against the various names for these
objects.

The matching done by the server uses the aliases property of each of the
objects it considers. The value of this property should be a list of strings,
the various alternatives for naming the object. If it is not a list, or the
object does not have an aliases property, then the empty list is used.
In any case, the value of the name property is added to the list for the
purposes of matching.

The server checks to see if the object string in the command is either exactly
equal to or a prefix of any alias; if there are any exact matches, the prefix
matches are ignored. If exactly one of the objects being considered has a
matching alias, that object is used. If more than one has a match, then the
special object \#-2 (aka $ambiguous\_match in LambdaCore) is used.
If there are no matches, then the special object \#-3 (aka
$failed\_match in LambdaCore) is used.

So, now the server has identified a verb string, a preposition string,
and direct- and indirect-object strings and objects. It then looks at
each of the verbs defined on each of the following four objects, in
order:

1. the player who typed the command
2. the room the player is in
3. the direct object, if any
4. the indirect object, if any.

For each of these verbs in turn, it tests if all of the the following
are true:

* the verb string in the command matches one of the names for the verb
* the direct- and indirect-object values found by matching are allowed by the corresponding _argument specifiers_
for the verb
* the preposition string in the command is matched by the _preposition specifier_ for the verb.

I'll explain each of these criteria in turn.

Every verb has one or more names; all of the names are kept in a single
string, separated by spaces. In the simplest case, a verb-name is just
a word made up of any characters other than spaces and stars (i.e., \` '
and \*). In this case, the verb-name matches only itself; that
is, the name must be matched exactly.

If the name contains a single star, however, then the name matches any prefix
of itself that is at least as long as the part before the star. For example,
the verb-name foo\*bar matches any of the strings foo,
foob, fooba, or foobar; note that the star itself is not
considered part of the name.

If the verb name _ends_ in a star, then it matches any string that begins
with the part before the star. For example, the verb-name foo\* matches
any of the strings foo, foobar, food, or foogleman,
among many others. As a special case, if the verb-name is \* (i.e., a
single star all by itself), then it matches anything at all.

Recall that the argument specifiers for the direct and indirect objects are
drawn from the set none, any, and this. If the specifier
is none, then the corresponding object value must be \#-1 (aka
$nothing in LambdaCore); that is, it must not have been specified. If
the specifier is any, then the corresponding object value may be
anything at all. Finally, if the specifier is this, then the
corresponding object value must be the same as the object on which we found
this verb; for example, if we are considering verbs on the player, then the
object value must be the player object.

Finally, recall that the argument specifier for the preposition is
either none, any, or one of several sets of prepositional
phrases, given above. A specifier of none matches only if there
was no preposition found in the command. A specifier of any
always matches, regardless of what preposition was found, if any. If
the specifier is a set of prepositional phrases, then the one found must
be in that set for the specifier to match.

So, the server considers several objects in turn, checking each of their
verbs in turn, looking for the first one that meets all of the criteria
just explained. If it finds one, then that is the verb whose program
will be executed for this command. If not, then it looks for a verb
named huh on the room that the player is in; if one is found,
then that verb will be called. This feature is useful for implementing
room-specific command parsing or error recovery. If the server can't
even find a huh verb to run, it prints an error message like
I couldn't understand that. and the command is considered complete.

At long last, we have a program to run in response to the command typed by the
player. When the code for the program begins execution, the following
built-in variables will have the indicated values:

**player**

> an object, the player who typed the command

**this**

> an object, the object on which this verb was found

**caller**

> an object, the same as player

**verb**

> a string, the first word of the command

**argstr**

> a string, everything after the first word of the command

**args**

> a list of strings, the words in argstr

**dobjstr**

> a string, the direct object string found during parsing

**dobj**

> an object, the direct object value found during matching

**prepstr**

> a string, the prepositional phrase found during parsing

**iobjstr**

> a string, the indirect object string

**iobj**

> an object, the indirect object value

The value returned by the program, if any, is ignored by the server.

## The MOO Programming Language

MOO stands for "MUD, Object Oriented." MUD, in turn, has been said to stand
for many different things, but I tend to think of it as "Multi-User Dungeon"
in the spirit of those ancient precursors to MUDs, Adventure and Zork.

MOO, the programming language, is a relatively small and simple
object-oriented language designed to be easy to learn for most
non-programmers; most complex systems still require some significant
programming ability to accomplish, however.

Having given you enough context to allow you to understand exactly what MOO
code is doing, I now explain what MOO code looks like and what it means. I
begin with the syntax and semantics of expressions, those pieces of code that
have values. After that, I cover statements, the next level of structure up
from expressions. Next, I discuss the concept of a task, the kind of running
process initiated by players entering commands, among other causes. Finally,
I list all of the built-in functions available to MOO code and describe what
they do.

First, though, let me mention comments. You can include bits of text in your
MOO program that are ignored by the server. The idea is to allow you to put
in notes to yourself and others about what the code is doing. To do this,
begin the text of the comment with the two characters /\* and end it
with the two characters \*/; this is just like comments in the C
programming language. Note that the server will completely ignore that text;
it will _not_ be saved in the database. Thus, such comments are only
useful in files of code that you maintain outside the database.

To include a more persistent comment in your code, try using a character
string literal as a statement. For example, the sentence about peanut butter
in the following code is essentially ignored during execution but will be
maintained in the database:

    
    for x in (players())
      "Grendel eats peanut butter!";
      player:tell(x.name, " (", x, ")");
    endfor
    

Note: In practice, the only style of comments you will use is quoted strings of text. Get used
to it.

### MOO Language Expressions

Expressions are those pieces of MOO code that generate values; for
example, the MOO code

    
    3 + 4
    

is an expression that generates (or "has" or "returns") the value 7\.
There are many kinds of expressions in MOO, all of them discussed below.

#### Errors While Evaluating Expressions

Most kinds of expressions can, under some circumstances, cause an error to be
generated. For example, the expression x / y will generate the error
E\_DIV if y is equal to zero. When an expression generates an
error, the behavior of the server is controlled by setting of the d
(debug) bit on the verb containing that expression. If the d bit is not
set, then the error is effectively squelched immediately upon generation; the
error value is simply returned as the value of the expression that generated it.

Note: This error-squelching behavior is very error prone, since it
affects _all_ errors, including ones the programmer may not have
anticipated. The d bit exists only for historical reasons; it was once
the only way for MOO programmers to catch and handle errors. The
error-catching expression and the try-except statement, both
described below, are far better ways of accomplishing the same thing.

If the d bit is set, as it usually is, then the error is _raised_
and can be caught and handled either by code surrounding the expression in
question or by verbs higher up on the chain of calls leading to the current
verb. If the error is not caught, then the server aborts the entire task and,
by default, prints a message to the current player. See the descriptions of
the error-catching expression and the try-except statement for
the details of how errors can be caught, and the chapter on server assumptions
about the database for details on the handling of uncaught errors.

#### Writing Values Directly in Verbs

The simplest kind of expression is a literal MOO value, just as
described in the section on values at the beginning of this document.
For example, the following are all expressions:

* 17
* \#893
* "This is a character string."
* E\_TYPE
* {"This", "is", "a", "list", "of", "words"}

In the case of lists, like the last example above, note that the list
expression contains other expressions, several character strings in this
case. In general, those expressions can be of any kind at all, not
necessarily literal values. For example,

    
    {3 + 4, 3 - 4, 3 * 4}
    

is an expression whose value is the list {7, -1, 12}.

#### Naming Values Within a Verb

As discussed earlier, it is possible to store values in properties on
objects; the properties will keep those values forever, or until another
value is explicitly put there. Quite often, though, it is useful to
have a place to put a value for just a little while. MOO provides local
variables for this purpose.

Variables are named places to hold values; you can get and set the value
in a given variable as many times as you like. Variables are temporary,
though; they only last while a particular verb is running; after it
finishes, all of the variables given values there cease to exist and the
values are forgotten.

Variables are also "local" to a particular verb; every verb has its own
set of them. Thus, the variables set in one verb are not visible to the
code of other verbs.

The name for a variable is made up entirely of letters, digits, and the
underscore character (\_) and does not begin with a digit. The
following are all valid variable names:

* foo
* \_foo
* this2that
* M68000
* two\_words
* This\_is\_a\_very\_long\_multiword\_variable\_name

Note that, along with almost everything else in MOO, the case of the
letters in variable names is insignificant. For example, these are all
names for the same variable:

* fubar
* Fubar
* FUBAR
* fUbAr

A variable name is itself an expression; its value is the value of the named
variable. When a verb begins, almost no variables have values yet; if you try
to use the value of a variable that doesn't have one, the error value
E\_VARNF is raised. (MOO is unlike many other programming languages in
which one must _declare_ each variable before using it; MOO has no such
declarations.) The following variables always have values:

* INT
* FLOAT
* OBJ
* STR
* LIST
* ERR
* player
* this
* caller
* verb
* args
* argstr
* dobj
* dobjstr
* prepstr
* iobj
* iobjstr
* NUM

The values of some of these variables always start out the same:

**INT**

> an integer, the type code for integers (see the description of the function typeof(), below)

**NUM**

> the same as INT (for historical reasons)

**FLOAT**

> an integer, the type code for floating-point numbers

**LIST**

> an integer, the type code for lists

**STR**

> an integer, the type code for strings

**OBJ

> an integer, the type code for objects

**ERR**

> an integer, the type code for error values

**

For others, the general meaning of the value is consistent, though the
value itself is different for different situations:

**player**

> an object, the player who typed the command that started the task that
> involved running this piece of code.

**this**

> an object, the object on which the currently-running verb was found.

**caller**

> an object, the object on which the verb that called the
> currently-running verb was found. For the first verb called for a given
> command, caller has the same value as player.

**verb**

> a string, the name by which the currently-running verb was identified.

**args**

> a list, the arguments given to this verb. For the first verb called for
> a given command, this is a list of strings, the words on the command
> line.

The rest of the so-called "built-in" variables are only really
meaningful for the first verb called for a given command. Their
semantics is given in the discussion of command parsing, above.

To change what value is stored in a variable, use an _assignment_
expression:

    
    _variable_ = _expression_
    

For example, to change the variable named x to have the value 17,
you would write x = 17 as an expression. An assignment
expression does two things:

* it changes the value of of the named variable
* it returns the new value of that variable

Thus, the expression

    
    13 + (x = 17)
    

changes the value of x to be 17 and returns 30\.

#### Arithmetic Operators

All of the usual simple operations on numbers are available to MOO programs:

* +
* -
* \*
* /
* %

These are, in order, addition, subtraction, multiplication, division, and
remainder. In the following table, the expressions on the left have the
corresponding values on the right:

    
    5 + 2       =>   7
    5 - 2       =>   3
    5 * 2       =>   10
    5 / 2       =>   2
    5.0 / 2.0   =>   2.5
    5 % 2       =>   1
    5.0 % 2.0   =>   1.0
    5 % -2      =>   1
    -5 % 2      =>   -1
    -5 % -2     =>   -1
    -(5 + 2)    =>   -7
    

Note that integer division in MOO throws away the remainder and that the result
of the remainder operator (%) has the same sign as the left-hand
operand. Also, note that - can be used without a left-hand operand to
negate a numeric expression.

Fine point: Integers and floating-point numbers cannot be mixed in any
particular use of these arithmetic operators; unlike some other programming
languages, MOO does not automatically coerce integers into floating-point
numbers. You can use the tofloat() function to perform an explicit
conversion.

The + operator can also be used to append two strings. The expression "foo" + "bar"
has the value "foobar"

Unless both operands to an arithmetic operator are numbers of the same kind
(or, for +, both strings), the error value E\_TYPE is raised. If
the right-hand operand for the division or remainder operators (/ or
%) is zero, the error value E\_DIV is raised.

MOO also supports the exponentiation operation, also known as "raising to a
power," using the ^ operator:

    
    3 ^ 4       =>   81
    3 ^ 4.5     error-->   E_TYPE
    3.5 ^ 4     =>   150.0625
    3.5 ^ 4.5   =>   280.741230801382
    

Note that if the first operand is an integer, then the second operand must also
be an integer. If the first operand is a floating-point number, then the
second operand can be either kind of number. Although it is legal to raise an
integer to a negative power, it is unlikely to be terribly useful.

#### Comparing Values

Any two values can be compared for equality using == and
!=. The first of these returns 1 if the two values are equal and
0 otherwise; the second does the reverse:

    
    3 == 4                              =>  0
    3 != 4                              =>  1
    3 == 3.0                            =>  0
    "foo" == "Foo"                      =>  1
    #34 != #34                          =>  0
    {1, #34, "foo"} == {1, #34, "FoO"}  =>  1
    E_DIV == E_TYPE                     =>  0
    3 != "foo"                          =>  1
    

Note that integers and floating-point numbers are never equal to one another,
even in the _obvious_ cases. Also note that comparison of strings (and list
values containing strings) is case-insensitive; that is, it does not
distinguish between the upper- and lower-case version of letters. To test two
values for case-sensitive equality, use the equal function described
later.

Warning: It is easy (and very annoying) to confuse the
equality-testing operator (==) with the assignment operator (=),
leading to nasty, hard-to-find bugs. Don't do this.

Numbers, object numbers, strings, and error values can also be compared
for ordering purposes using the following operators:

**<**

> meaning "less than"

**<=**

> "less than or equal"

**\>=**

> "greater than or equal"

**\>**

> "greater than"

As with the equality
operators, these return 1 when their operands are in the appropriate relation and 0 otherwise:

    
    3 < 4           =>  1
    3 < 4.0         =>  E_TYPE (an error)
    #34 >= #32      =>  1
    "foo" <= "Boo"  =>  0
    E_DIV > E_TYPE  =>  1
    

Note that, as with the equality operators, strings are compared
case-insensitively. To perform a case-sensitive string comparison, use the
strcmp function described later. Also note that the error values are
ordered as given in the table in the section on values. If the operands to
these four comparison operators are of different types (even integers and
floating-point numbers are considered different types), or if they are lists,
then E\_TYPE is raised.

#### Values as True and False

There is a notion in MOO of _true_ and _false_ values; every value
is one or the other. The true values are as follows:

* all integers other than zero
* all floating-point numbers not equal to 0.0
* all non-empty strings (i.e., other than "")
* all non-empty lists (i.e., other than {})

Warning: Negative numbers are considered true.

All other values are false:

* the integer zero
* the floating-point numbers 0.0 and -0.0
* the empty string ("")
* the empty list ({})
* all object numbers
* all error values

Note: Objects are considered false. If you need to evaluate if a value
is of the type object, you can use typeof(potential\_object) == OBJ however, keep in mind
that this does not mean that the object referenced actually exists. IE: \#100000000 will return true, but that does not
mean that object exists in your MOO.

There are four kinds of expressions and two kinds of statements that depend
upon this classification of MOO values. In describing them, I sometimes refer
to the _truth value_ of a MOO value; this is just _true_ or
_false_, the category into which that MOO value is classified.

The conditional expression in MOO has the following form:

    
    _expression-1_ ? _expression-2_ | _expression-3_
    

Note: This is commonly refered to as a ternary statement in most programming languages.

First, _expression-1_ is evaluated. If it returns a true value, then
_expression-2_ is evaluated and whatever it returns is returned as the
value of the conditional expression as a whole. If _expression-1_ returns
a false value, then _expression-3_ is evaluated instead and its value is
used as that of the conditional expression.

    
    1 ? 2 | 3           =>  2
    0 ? 2 | 3           =>  3
    "foo" ? 17 | {#34}  =>  17
    

Note that only one of _expression-2_ and _expression-3_ is evaluated,
never both.

To negate the truth value of a MOO value, use the ! operator:

    
    ! _expression_
    

If the value of _expression_ is true, ! returns 0; otherwise, it
returns 1:

    
    ! "foo"     =>  0
    ! (3 >= 4)  =>  1
    

The negation operator is usually read as "not."

Note: The "negation" or "not" operator is commonly referred to as "bang" by many
programmers.

It is frequently useful to test more than one condition to see if some or all
of them are true. MOO provides two operators for this:

    
    _expression-1_ && _expression-2_
    _expression-1_ || _expression-2_
    

These operators are usually read as "and" and "or," respectively.

The && operator first evaluates _expression-1_. If it returns a
true value, then _expression-2_ is evaluated and its value becomes the
value of the && expression as a whole; otherwise, the value of
_expression-1_ is used as the value of the && expression. 

Note: _expression-2_ is only evaluated if _expression-1_ returns a true
value.

The && expression is equivalent to the conditional expression:

    
    _expression-1_ ? _expression-2_ | _expression-1_
    

except that _expression-1_ is only evaluated once.

The || operator works similarly, except that _expression-2_ is
evaluated only if _expression-1_ returns a false value. It is equivalent
to the conditional expression:

    
    _expression-1_ ? _expression-1_ | _expression-2_
    

except that, as with &&, _expression-1_ is only evaluated once.

These two operators behave very much like "and" and "or" in English:

    
    1 && 1                  =>  1
    0 && 1                  =>  0
    0 && 0                  =>  0
    1 || 1                  =>  1
    0 || 1                  =>  1
    0 || 0                  =>  0
    17 <= 23  &&  23 <= 27  =>  1
    

#### Indexing into Lists and Strings

Both strings and lists can be seen as ordered sequences of MOO values. In the
case of strings, each is a sequence of single-character strings; that is, one
can view the string "bar" as a sequence of the strings "b",
"a", and "r". MOO allows you to refer to the elements of lists
and strings by number, by the _index_ of that element in the list or
string. The first element in a list or string has index 1, the second has
index 2, and so on.

Warning: It is very important to note that unlike many programming
languages (which use 0 as the starting index), MOO uses 1\.

##### Extracting an Element from a List or String

The indexing expression in MOO extracts a specified element from a list or string:

    
    _expression-1_[_expression-2_]
    

First, _expression-1_ is evaluated; it must return a list or a string (the
_sequence_). Then, _expression-2_ is evaluated and must return an
integer (the _index_). If either of the expressions returns some other type
of value, E\_TYPE is returned. The index must be between 1 and the
length of the sequence, inclusive; if it is not, then E\_RANGE is raised.
The value of the indexing expression is the index'th element in the sequence.
Anywhere within _expression-2_, you can use the symbol $ as an
expression returning the length of the value of _expression-1_.

    
    "fob"[2]                =>  "o"
    "fob"[1]                =>  "f"
    {#12, #23, #34}[$ - 1]  =>  #23
    

Note that there are no legal indices for the empty string or list, since
there are no integers between 1 and 0 (the length of the empty string or
list).

Fine point: The $ expression actually returns the length of the
value of the expression just before the nearest enclosing \[...\]
indexing or subranging brackets. For example:

    
    "frob"[{3, 2, 4}[$]]     =>  "b"
    

is possible because $ in this case represents the 3rd index of the list next to it, which evaluates to the value 4, 
which in turn is applied as the index to the string, which evaluates to the b.

##### Replacing an Element of a List or String

It often happens that one wants to change just one particular slot of a list or
string, which is stored in a variable or a property. This can be done
conveniently using an _indexed assignment_ having one of the following
forms:

    
    _variable_[_index-expr_] = _result-expr_
    _object-expr_._name_[_index-expr_] = _result-expr_
    _object-expr_.(_name-expr_)[_index-expr_] = _result-expr_
    $_name_[_index-expr_] = _result-expr_
    

The first form writes into a variable, and the last three forms write into a
property. The usual errors (E\_TYPE, E\_INVIND, E\_PROPNF
and E\_PERM for lack of read/write permission on the property) may be
raised, just as in reading and writing any object property; see the
discussion of object property expressions below for details.

Correspondingly, if _variable_ does not yet have a value (i.e., it has never been assigned
to), E\_VARNF will be raised.

If _index-expr_ is not an integer, or if the value of _variable_ or the
property is not a list or string, E\_TYPE is raised. If
_result-expr_ is a string, but not of length 1, E\_INVARG is
raised. Now suppose _index-expr_ evaluates to an integer _n_. If
_n_ is outside the range of the list or string (i.e. smaller than 1 or
greater than the length of the list or string), E\_RANGE is raised.
Otherwise, the actual assignment takes place.

For lists, the variable or the
property is assigned a new list that is identical to the original one except at
the _n_-th position, where the new list contains the result of
_result-expr_ instead. For strings, the variable or the property is
assigned a new string that is identical to the original one, except the
_n_-th character is changed to be _result-expr_.

The assignment expression itself returns the value of _result-expr_. For
the following examples, assume that l initially contains the list
{1, 2, 3} and that s initially contains the string "foobar":

    
    l[5] = 3          =>   E_RANGE (error)
    l["first"] = 4    =>   E_TYPE  (error)
    s[3] = "baz"      =>   E_INVARG (error)
    l[2] = l[2] + 3   =>   5
    l                 =>   {1, 5, 3}
    l[2] = "foo"      =>   "foo"
    l                 =>   {1, "foo", 3}
    s[2] = "u"        =>   "u"
    s                 =>   "fuobar"
    s[$] = "z"        =>   "z"
    s                 =>   "fuobaz"
    

Note: (error) is only used for formatting and identification purposes in these examples and is not present
in an actual raised error on the MOO.

Note: The $ expression may also be used in indexed assignments with
the same meaning as before.

Fine point: After an indexed assignment, the variable or property
contains a _new_ list or string, a copy of the original list in all but
the _n_-th place, where it contains a new value. In programming-language
jargon, the original list is not mutated, and there is no aliasing. (Indeed,
no MOO value is mutable and no aliasing ever occurs.)

In the list case, indexed assignment can be nested to many levels, to work on
nested lists. Assume that l initially contains the list:

    
    {{1, 2, 3}, {4, 5, 6}, "foo"}
    

in the following examples:

    
    l[7] = 4             =>   E_RANGE (error)
    l[1][8] = 35         =>   E_RANGE (error)
    l[3][2] = 7          =>   E_TYPE (error)
    l[1][1][1] = 3       =>   E_TYPE (error)
    l[2][2] = -l[2][2]   =>   -5
    l                    =>   {{1, 2, 3}, {4, -5, 6}, "foo"}
    l[2] = "bar"         =>   "bar"
    l                    =>   {{1, 2, 3}, "bar", "foo"}
    l[2][$] = "z"        =>   "z"
    l                    =>   {{1, 2, 3}, "baz", "foo"}
    

The first two examples raise E\_RANGE because 7 is out of the range of
l and 8 is out of the range of l\[1\]. The next two examples
raise E\_TYPE because l\[3\] and l\[1\]\[1\] are not lists.

##### Extracting a Subsequence of a List or String

The range expression extracts a specified subsequence from a list or string:

    
    _expression-1_[_expression-2_.._expression-3_]
    

The three expressions are evaluated in order. _Expression-1_ must return a
list or string (the _sequence_) and the other two expressions must return
integers (the _low_ and _high_ indices, respectively); otherwise,
E\_TYPE is raised. The $ expression can be used in either or both
of _expression-2_ and _expression-3_ just as before, meaning the length
of the value of _expression-1_.

If the low index is greater than the high index, then the empty string or list
is returned, depending on whether the sequence is a string or a list.
Otherwise, both indices must be between 1 and the length of the sequence;
E\_RANGE is raised if they are not. A new list or string is returned
that contains just the elements of the sequence with indices between the low
and high bounds.

    
    "foobar"[2..$]                       =>  "oobar"
    "foobar"[3..3]                       =>  "o"
    "foobar"[17..12]                     =>  ""
    {"one", "two", "three"}[$ - 1..$]    =>  {"two", "three"}
    {"one", "two", "three"}[3..3]        =>  {"three"}
    {"one", "two", "three"}[17..12]      =>  {}
    

##### Replacing a Subsequence of a List or String

The subrange assigment replaces a specified subsequence of a list or string
with a supplied subsequence. The allowed forms are:

    
    _variable_[_start-index-expr_.._end-index-expr_] = _result-expr_
    _object-expr_._name_[_start-index-expr_.._end-index-expr_] = _result-expr_
    _object-expr_.(_name-expr_)[_start-index-expr_.._end-index-expr_] = _result-expr_
    $_name_[_start-index-expr_.._end-index-expr_] = _result-expr_
    

As with indexed assigments, the first form writes into a variable, and the last
three forms write into a property. The same errors (E\_TYPE,
E\_INVIND, E\_PROPNF and E\_PERM for lack of read/write
permission on the property) may be raised. If _variable_ does not yet have
a value (i.e., it has never been assigned to), E\_VARNF will be raised.

As before, the $ expression can be used in either _start-index-expr_
or _end-index-expr_, meaning the length of the original value of the
expression just before the \[...\] part.

If _start-index-expr_ or _end-index-expr_ is not an integer, if the value
of _variable_ or the property is not a list or string, or _result-expr_
is not the same type as _variable_ or the property, E\_TYPE is
raised. E\_RANGE is raised if _end-index-expr_ is less than zero
or if _start-index-expr_ is greater than the length of the list or string
plus one. Note: the length of _result-expr_ does not need to be the same
as the length of the specified range.

In precise terms, the subrange assigment

    
    _v_[_start_.._end_] = _value_
    

is equivalent to

    
    _v_ = {@_v_[1.._start_ - 1], @_value_, @_v_[_end_ + 1..$]}
    

if _v_ is a list and to

    
    _v_ = _v_[1.._start_ - 1] + _value_ + _v_[_end_ + 1..$]
    

if _v_ is a string. The assigment expression itself returns the value of _result-expr_.

Note: The use of preceeding a list with the @ symbol is covered in just a bit.

For the following examples, assume that l initially contains the list
{1, 2, 3} and that s initially contains the string "foobar":

    
    l[5..6] = {7, 8}       =>   E_RANGE (error)
    l[2..3] = 4            =>   E_TYPE (error)
    l[#2..3] = {7}         =>   E_TYPE (error)
    s[2..3] = {6}          =>   E_TYPE (error)
    l[2..3] = {6, 7, 8, 9} =>   {6, 7, 8, 9}
    l                      =>   {1, 6, 7, 8, 9}
    l[2..1] = {10, "foo"}  =>   {10, "foo"}
    l                      =>   {1, 10, "foo", 6, 7, 8, 9}
    l[3][2..$] = "u"       =>   "u"
    l                      =>   {1, 10, "fu", 6, 7, 8, 9}
    s[7..12] = "baz"       =>   "baz"
    s                      =>   "foobarbaz"
    s[1..3] = "fu"         =>   "fu"
    s                      =>   "fubarbaz"
    s[1..0] = "test"       =>   "test"
    s                      =>   "testfubarbaz"
    

#### Other Operations on Lists

As was mentioned earlier, lists can be constructed by writing a
comma-separated sequence of expressions inside curly braces:

    
    {_expression-1_, _expression-2_, ..., _expression-N_}
    

The resulting list has the value of _expression-1_ as its first element,
that of _expression-2_ as the second, etc.

    
    {3 < 4, 3 <= 4, 3 >= 4, 3 > 4}  =>  {1, 1, 0, 0}
    

Additionally, one may precede any of these expressions by the splicing
operator, @. Such an expression must return a list; rather than the
old list itself becoming an element of the new list, all of the elements of
the old list are included in the new list. This concept is easy to
understand, but hard to explain in words, so here are some examples. For
these examples, assume that the variable a has the value {2, 3,
4} and that b has the value {"Foo", "Bar"}:

    
    {1, a, 5}   =>  {1, {2, 3, 4}, 5}
    {1, @a, 5}  =>  {1, 2, 3, 4, 5}
    {a, @a}     =>  {{2, 3, 4}, 2, 3, 4}
    {@a, @b}    =>  {2, 3, 4, "Foo", "Bar"}
    

If the splicing operator (@) precedes an expression whose value
is not a list, then E\_TYPE is raised as the value of the list
construction as a whole.

The list membership expression tests whether or not a given MOO value is an
element of a given list and, if so, with what index:

    
    _expression-1_ in _expression-2_
    

_Expression-2_ must return a list; otherwise, E\_TYPE is raised.
If the value of _expression-1_ is in that list, then the index of its first
occurrence in the list is returned; otherwise, the in expression returns
0\.

    
    2 in {5, 8, 2, 3}               =>  3
    7 in {5, 8, 2, 3}               =>  0
    "bar" in {"Foo", "Bar", "Baz"}  =>  2
    

Note that the list membership operator is case-insensitive in comparing
strings, just like the comparison operators. To perform a case-sensitive list
membership test, use the is\_member function described later. Note also
that since it returns zero only if the given value is not in the given list,
the in expression can be used either as a membership test or as an
element locator.

#### Spreading List Elements Among Variables

It is often the case in MOO programming that you will want to access the
elements of a list individually, with each element stored in a separate
variables. This desire arises, for example, at the beginning of almost every
MOO verb, since the arguments to all verbs are delivered all bunched together
in a single list. In such circumstances, you _could_ write statements
like these:

    
    first = args[1];
    second = args[2];
    if (length(args) > 2)
      third = args[3];
    else
      third = 0;
    endif
    

This approach gets pretty tedious, both to read and to write, and it's prone to
errors if you mistype one of the indices. Also, you often want to check
whether or not any _extra_ list elements were present, adding to the
tedium.

MOO provides a special kind of assignment expression, called _scattering
assignment_ made just for cases such as these. A scattering assignment
expression looks like this:

    
    {_target_, ...} = _expr_
    

where each _target_ describes a place to store elements of the list that
results from evaluating _expr_. A _target_ has one of the following
forms:

**_variable_**

> This is the simplest target, just a simple variable; the list element in the
> corresponding position is assigned to the variable. This is called a
> _required_ target, since the assignment is required to put one of the list
> elements into the variable.

**?_variable_**

> This is called an _optional_ target, since it doesn't always get assigned
> an element. If there are any list elements left over after all of the required
> targets have been accounted for (along with all of the other optionals to the
> left of this one), then this variable is treated like a required one and the
> list element in the corresponding position is assigned to the variable. If
> there aren't enough elements to assign one to this target, then no assignment
> is made to this variable, leaving it with whatever its previous value was.

**?_variable_ = _default-expr_**

> This is also an optional target, but if there aren't enough list elements
> available to assign one to this target, the result of evaluating
> _default-expr_ is assigned to it instead. Thus, _default-expr_
> provides a _default value_ for the variable. The default value expressions
> are evaluated and assigned working from left to right _after_ all of the
> other assignments have been performed.

**@_variable_**

> By analogy with the @ syntax in list construction, this variable is
> assigned a list of all of the \`leftover' list elements in this part of the list
> after all of the other targets have been filled in. It is assigned the empty
> list if there aren't any elements left over. This is called a _rest_
> target, since it gets the rest of the elements. There may be at most one rest
> target in each scattering assignment expression.

If there aren't enough list elements to fill all of the required targets, or if
there are more than enough to fill all of the required and optional targets but
there isn't a rest target to take the leftover ones, then E\_ARGS is
raised.

Here are some examples of how this works. Assume first that the verb
me:foo() contains the following code:

    
    b = c = e = 17;
    {a, ?b, ?c = 8, @d, ?e = 9, f} = args;
    return {a, b, c, d, e, f};
    

Then the following calls return the given values:

    
    me:foo(1)                        =>   E_ARGS (error)
    me:foo(1, 2)                     =>   {1, 17, 8, {}, 9, 2}
    me:foo(1, 2, 3)                  =>   {1, 2, 8, {}, 9, 3}
    me:foo(1, 2, 3, 4)               =>   {1, 2, 3, {}, 9, 4}
    me:foo(1, 2, 3, 4, 5)            =>   {1, 2, 3, {}, 4, 5}
    me:foo(1, 2, 3, 4, 5, 6)         =>   {1, 2, 3, {4}, 5, 6}
    me:foo(1, 2, 3, 4, 5, 6, 7)      =>   {1, 2, 3, {4, 5}, 6, 7}
    me:foo(1, 2, 3, 4, 5, 6, 7, 8)   =>   {1, 2, 3, {4, 5, 6}, 7, 8}
    

Using scattering assignment, the example at the begining of this section could
be rewritten more simply, reliably, and readably:

    
    {first, second, ?third = 0} = args;
    

Fine point: It is good MOO programming style to use a scattering assignment at the top of
nearly every verb, since it shows so clearly just what kinds of arguments the verb expects.

#### Getting and Setting the Values of Properties

Usually, one can read the value of a property on an object with a simple
expression:

    
    _expression_._name_
    

_Expression_ must return an object number; if not, E\_TYPE is
raised. If the object with that number does not exist, E\_INVIND is
raised. Otherwise, if the object does not have a property with that name,
then E\_PROPNF is raised. Otherwise, if the named property is not
readable by the owner of the current verb, then E\_PERM is raised.
Finally, assuming that none of these terrible things happens, the value of the
named property on the given object is returned.

I said "usually" in the paragraph above because that simple expression only
works if the name of the property obeys the same rules as for the names of
variables (i.e., consists entirely of letters, digits, and underscores, and
doesn't begin with a digit). Property names are not restricted to this set,
though. Also, it is sometimes useful to be able to figure out what property
to read by some computation. For these more general uses, the following
syntax is also allowed:

    
    _expression-1_.(_expression-2_)
    

As before, _expression-1_ must return an object number. _Expression-2_
must return a string, the name of the property to be read; E\_TYPE
is raised otherwise. Using this syntax, any property can be read,
regardless of its name.

Note that, as with almost everything in MOO, case is not significant in the
names of properties. Thus, the following expressions are all equivalent:

    
    foo.bar
    foo.Bar
    foo.("bAr")
    

The LambdaCore database uses several properties on \#0, the _system
object_, for various special purposes. For example, the value of
\#0.room is the "generic room" object, \#0.exit is the "generic
exit" object, etc. This allows MOO programs to refer to these useful objects
more easily (and more readably) than using their object numbers directly. To
make this usage even easier and more readable, the expression

    
    $_name_
    

(where _name_ obeys the rules for variable names) is an abbreviation for

    
    #0._name_
    

Thus, for example, the value $nothing mentioned earlier is really
\#-1, the value of \#0.nothing.

As with variables, one uses the assignment operator (=) to change the
value of a property. For example, the expression

    
    14 + (#27.foo = 17)
    

changes the value of the foo property of the object numbered 27 to be
17 and then returns 31\. Assignments to properties check that the owner of the
current verb has write permission on the given property, raising
E\_PERM otherwise. Read permission is not required.

#### Calling Built-in Functions and Other Verbs

MOO provides a large number of useful functions for performing a wide
variety of operations; a complete list, giving their names, arguments,
and semantics, appears in a separate section later. As an example to
give you the idea, there is a function named length that returns
the length of a given string or list.

The syntax of a call to a function is as follows:

    
    _name_(_expr-1_, _expr-2_, ..., _expr-N_)
    

where _name_ is the name of one of the built-in functions. The
expressions between the parentheses, called _arguments_, are each
evaluated in turn and then given to the named function to use in its
appropriate way. Most functions require that a specific number of arguments
be given; otherwise, E\_ARGS is raised. Most also require that
certain of the arguments have certain specified types (e.g., the
length() function requires a list or a string as its argument);
E\_TYPE is raised if any argument has the wrong type.

As with list construction, the splicing operator @ can precede
any argument expression. The value of such an expression must be a
list; E\_TYPE is raised otherwise. The elements of this list
are passed as individual arguments, in place of the list as a whole.

Verbs can also call other verbs, usually using this syntax:

    
    _expr-0_:_name_(_expr-1_, _expr-2_, ..., _expr-N_)
    

_Expr-0_ must return an object number; E\_TYPE is raised otherwise.
If the object with that number does not exist, E\_INVIND is raised. If
this task is too deeply nested in verbs calling verbs calling verbs, then
E\_MAXREC is raised; the default limit is 50 levels, but this can be
changed from within the database; see the chapter on server assumptions about
the database for details. If neither the object nor any of its ancestors
defines a verb matching the given name, E\_VERBNF is raised.
Otherwise, if none of these nasty things happens, the named verb on the given
object is called; the various built-in variables have the following initial
values in the called verb:

**this**

> an object, the value of _expr-0_

**verb**

> a string, the _name_ used in calling this verb

**args**

> a list, the values of _expr-1_, _expr-2_, etc.

**caller**

> an object, the value of this in the calling verb

**player**

> an object, the same value as it had initially in the calling verb or, if the
> calling verb is running with wizard permissions, the same as the current value
> in the calling verb.

All other built-in variables (argstr, dobj, etc.) are initialized
with the same values they have in the calling verb.

As with the discussion of property references above, I said "usually" at the
beginning of the previous paragraph because that syntax is only allowed when
the _name_ follows the rules for allowed variable names. Also as with
property reference, there is a syntax allowing you to compute the name of the
verb:

    
    _expr-0_:(_expr-00_)(_expr-1_, _expr-2_, ..., _expr-N_)
    

The expression _expr-00_ must return a string; E\_TYPE is raised
otherwise.

The splicing operator (@) can be used with verb-call arguments,
too, just as with the arguments to built-in functions.

In many databases, a number of important verbs are defined on \#0, the
_system object_. As with the $foo notation for properties on
\#0, the server defines a special syntax for calling verbs on \#0:

    
    $_name_(_expr-1_, _expr-2_, ..., _expr-N_)
    

(where _name_ obeys the rules for variable names) is an abbreviation for

    
    #0:_name_(_expr-1_, _expr-2_, ..., _expr-N_)
    

#### Catching Errors in Expressions

It is often useful to be able to _catch_ an error that an expression
raises, to keep the error from aborting the whole task, and to keep on running
as if the expression had returned some other value normally. The following
expression accomplishes this:

    
    ` _expr-1_ ! _codes_ => _expr-2_ '
    

Note: The open- and close-quotation marks in the previous line are
really part of the syntax; you must actually type them as part of your MOO program for this kind of expression.

The _codes_ part is either the keyword ANY or else a
comma-separated list of expressions, just like an argument list. As in an
argument list, the splicing operator (@) can be used here. The
=\> _expr-2_ part of the error-catching expression is optional.

First, the _codes_ part is evaluated, yielding a list of error codes that
should be caught if they're raised; if _codes_ is ANY, then it is
equivalent to the list of all possible MOO values.

Next, _expr-1_ is evaluated. If it evaluates normally, without raising an
error, then its value becomes the value of the entire error-catching
expression. If evaluating _expr-1_ results in an error being raised, then
call that error _E_. If _E_ is in the list resulting from evaluating
_codes_, then _E_ is considered _caught_ by this error-catching
expression. In such a case, if _expr-2_ was given, it is evaluated to get
the outcome of the entire error-catching expression; if _expr-2_ was
omitted, then _E_ becomes the value of the entire expression. If _E_
is _not_ in the list resulting from _codes_, then this expression does
not catch the error at all and it continues to be raised, possibly to be caught
by some piece of code either surrounding this expression or higher up on the
verb-call stack.

Here are some examples of the use of this kind of expression:

    
    `x + 1 ! E_TYPE => 0'
    

Returns x + 1 if x is an integer, returns 0 if x is
not an integer, and raises E\_VARNF if x doesn't have a value.

    
    `x.y ! E_PROPNF, E_PERM => 17'
    

Returns x.y if that doesn't cause an error, 17 if x
doesn't have a y property or that property isn't readable, and raises
some other kind of error (like E\_INVIND) if x.y does.

    
    `1 / 0 ! ANY'
    

Returns E\_DIV.

Note: It's important to mention how powerful this compact syntax for writing error catching code can be.
When used properly you can write very complex and elegant code. For example imagine that you have a set of objects from different parents,
some of which define a specific verb, and some of which do not. If for instance, your code wants to perform some function _if_ the
verb exists, you can write \`obj:verbname() ! ANY =\> 0' to allow the MOO to attempt to execute that verb and then if it fails, catch the error
and continue operations normally. 

#### Parentheses and Operator Precedence

As shown in a few examples above, MOO allows you to use parentheses to make it
clear how you intend for complex expressions to be grouped. For example, the
expression

    
    3 * (4 + 5)
    

performs the addition of 4 and 5 before multiplying the result by 3\.

If you leave out the parentheses, MOO will figure out how to group the
expression according to certain rules. The first of these is that some
operators have higher _precedence_ than others; operators with higher
precedence will more tightly bind to their operands than those with lower
precedence. For example, multiplication has higher precedence than addition;
thus, if the parentheses had been left out of the expression in the previous
paragraph, MOO would have grouped it as follows:

    
    (3 * 4) + 5
    

The table below gives the relative precedence of all of the MOO
operators; operators on higher lines in the table have higher precedence
and those on the same line have identical precedence:

    
    !       - (without a left operand)
    ^
    *       /       %
    +       -
    ==      !=      <       <=      >       >=      in
    &&      ||
    ... ? ... | ... (the conditional expression)
    =
    

Thus, the horrendous expression

    
    x = a < b && c > d + e * f ? w in y | - q - r
    

would be grouped as follows:

    
    x = (((a < b) && (c > (d + (e * f)))) ? (w in y) | ((- q) - r))
    

It is best to keep expressions simpler than this and to use parentheses
liberally to make your meaning clear to other humans.

### MOO Language Statements

Statements are MOO constructs that, in contrast to expressions, perform some
useful, non-value-producing operation. For example, there are several kinds of
statements, called _looping constructs_, that repeatedly perform some set of
operations. Fortunately, there are many fewer kinds of statements in MOO than
there are kinds of expressions.

#### Errors While Executing Statements

Statements do not return values, but some kinds of statements can, under
certain circumstances described below, generate errors. If such an error is
generated in a verb whose d (debug) bit is not set, then the error is
ignored and the statement that generated it is simply skipped; execution
proceeds with the next statement.

Note: This error-ignoring behavior is very error prone, since it
affects _all_ errors, including ones the programmer may not have
anticipated. The d bit exists only for historical reasons; it was once
the only way for MOO programmers to catch and handle errors. The
error-catching expression and the try-except statement are far
better ways of accomplishing the same thing.

If the d bit is set, as it usually is, then the error is _raised_
and can be caught and handled either by code surrounding the expression in
question or by verbs higher up on the chain of calls leading to the current
verb. If the error is not caught, then the server aborts the entire task and,
by default, prints a message to the current player. See the descriptions of
the error-catching expression and the try-except statement for
the details of how errors can be caught, and the chapter on server assumptions
about the database for details on the handling of uncaught errors.

#### Simple Statements

The simplest kind of statement is the _null_ statement, consisting of just
a semicolon:

    
    ;
    

It doesn't do anything at all, but it does it very quickly.

The next simplest statement is also one of the most common, the expression
statement, consisting of any expression followed by a semicolon:

    
    _expression_;
    

The given expression is evaluated and the resulting value is ignored.
Commonly-used kinds of expressions for such statements include
assignments and verb calls. Of course, there's no use for such a
statement unless the evaluation of _expression_ has some side-effect,
such as changing the value of some variable or property, printing some
text on someone's screen, etc.

    
    #42.weight = 40;
    #42.weight;
    2 + 5;
    obj:verbname();
    1 > 2;
    2 < 1;
    

#### Statements for Testing Conditions

The if statement allows you to decide whether or not to perform some
statements based on the value of an arbitrary expression:

    
    if (_expression_)
      _statements_
    endif
    

_Expression_ is evaluated and, if it returns a true value, the statements
are executed in order; otherwise, nothing more is done.

One frequently wants to perform one set of statements if some condition is
true and some other set of statements otherwise. The optional else
phrase in an if statement allows you to do this:

    
    if (_expression_)
      _statements-1_
    else
      _statements-2_
    endif
    

This statement is executed just like the previous one, except that
_statements-1_ are executed if _expression_ returns a true value and
_statements-2_ are executed otherwise.

Sometimes, one needs to test several conditions in a kind of nested
fashion:

    
    if (_expression-1_)
      _statements-1_
    else
      if (_expression-2_)
        _statements-2_
      else
        if (_expression-3_)
          _statements-3_
        else
          _statements-4_
        endif
      endif
    endif
    

Such code can easily become tedious to write and difficult to read. MOO
provides a somewhat simpler notation for such cases:

    
    if (_expression-1_)
      _statements-1_
    elseif (_expression-2_)
      _statements-2_
    elseif (_expression-3_)
      _statements-3_
    else
      _statements-4_
    endif
    

Note that elseif is written as a single word, without any spaces. This
simpler version has the very same meaning as the original: evaluate
_expression-i_ for _i_ equal to 1, 2, and 3, in turn, until one of
them returns a true value; then execute the _statements-i_ associated with
that expression. If none of the _expression-i_ return a true value, then
execute _statements-4_.

Any number of elseif phrases can appear, each having this form:

    
    elseif (_expression_) 
        _statements_
    

The complete syntax of the if statement, therefore, is as follows:

    
    if (_expression_)
      _statements_
    _zero-or-more-elseif-phrases_
    _an-optional-else-phrase_
    endif
    

#### Statements for Looping

MOO provides three different kinds of looping statements, allowing you to have
a set of statements executed (1) once for each element of a given list, (2)
once for each integer or object number in a given range, and (3) over and over
until a given condition stops being true.

##### The for-in loop

Note: In some programming languages this is
referred to as a foreach loop. The syntax and usage is roughly the same.

To perform some statements once for each element of a given list, use this
syntax:

    
    for _variable_ in (_expression_)
      _statements_
    endfor
    

The expression is evaluated and should return a list; if it does not,
E\_TYPE is raised. The _statements_ are then executed once for
each element of that list in turn; each time, the given _variable_ is
assigned the value of the element in question. For example, consider
the following statements:

    
    odds = {1, 3, 5, 7, 9};
    evens = {};
    for n in (odds)
      evens = {@evens, n + 1};
    endfor
    

The value of the variable evens after executing these statements
is the list

    
    {2, 4, 6, 8, 10}
    

Another example of this, looping over all the children of an object:

    
    for child in (children(obj))
        notify(player, tostr(o.name, " is located in ", o.location));
    endfor
    

Another exmaple of this, looping over a list of strings:

    
    strings = {"foo", "bar", "baz"};
    for string in (strings)
        notify(player, string);
    endfor
    

##### The For-Range Loop

To perform a set of statements once for each integer or object number in a given
range, use this syntax:

    
    for _variable_ in [_expression-1_.._expression-2_]
      _statements_
    endfor
    

The two expressions are evaluated in turn and should either both return integers
or both return object numbers; E\_TYPE is raised otherwise. The
_statements_ are then executed once for each integer (or object number, as
appropriate) greater than or equal to the value of _expression-1_ and less
than or equal to the result of _expression-2_, in increasing order. Each
time, the given variable is assigned the integer or object number in question.
For example, consider the following statements:

    
    evens = {};
    for n in [1..5]
      evens = {@evens, 2 * n};
    endfor
    

The value of the variable evens after executing these statements
is just as in the previous example: the list

    
    {2, 4, 6, 8, 10}
    

The following loop over object numbers prints out the number and name of every
valid object in the database:

    
    for o in [#0..max_object()]
      if (valid(o))
        notify(player, tostr(o, ": ", o.name));
      endif
    endfor
    

##### The While Loop

The final kind of loop in MOO executes a set of statements repeatedly as long
as a given condition remains true:

    
    while (_expression_)
      _statements_
    endwhile
    

The expression is evaluated and, if it returns a true value, the
_statements_ are executed; then, execution of the while statement
begins all over again with the evaluation of the expression. That is,
execution alternates between evaluating the expression and executing the
statements until the expression returns a false value. The following
example code has precisely the same effect as the loop just shown above:

    
    evens = {};
    n = 1;
    while (n <= 5)
      evens = {@evens, 2 * n};
      n = n + 1;
    endwhile
    

Fine point: It is also possible to give a _name_ to a while loop.

    
    while _name_ (_expression_)
      _statements_
    endwhile
    

which has precisely the same effect as

    
    while (_name_ = _expression_)
      _statements_
    endwhile
    

This naming facility is only really useful in conjunction with the break
and continue statements, described in the next section.

With each kind of loop, it is possible that the statements in the body of the
loop will never be executed at all. For iteration over lists, this happens
when the list returned by the expression is empty. For iteration on integers,
it happens when _expression-1_ returns a larger integer than
_expression-2_. Finally, for the while loop, it happens if the
expression returns a false value the very first time it is evaluated.

Warning: With while loops it is especially
important to make sure you do not create an infinite loop. That is, a loop that will never terminate because
it's expression will never become false.

#### Terminating One or All Iterations of a Loop

Sometimes, it is useful to exit a loop before it finishes all of its
iterations. For example, if the loop is used to search for a particular kind
of element of a list, then it might make sense to stop looping as soon as the
right kind of element is found, even if there are more elements yet to see.
The break statement is used for this purpose; it has the form

    
    break;
    

or

    
    break _name_;
    

Each break statement indicates a specific surrounding loop; if
_name_ is not given, the statement refers to the innermost one. If it is
given, _name_ must be the name appearing right after the for or
while keyword of the desired enclosing loop. When the break
statement is executed, the indicated loop is immediately terminated and
executing continues just as if the loop had completed its iterations normally.

MOO also allows you to terminate just the current iteration of a loop, making
it immediately go on to the next one, if any. The continue statement
does this; it has precisely the same forms as the break statement:

    
    continue;
    

or

    
    continue _name_;
    

An example that sums up a list of integers, excluding any integer equal to four:

    
    my_list = {1, 2, 3, 4, 5, 6, 7};
    sum = 0;
    for element in (my_list)
        if (element == 4)
            continue;
        endif
        sum = sum + element;
    endfor
    

An example that 
    
    
    my_list = {#13633, #98, #15840, #18657, #22664};
    i = 0;
    found = 0;
    for obj in (my_list)
        i = i + 1;
        if (obj == #18657)
            found = 1;
            break;
        endif
    endfor
    if (found)
        notify(player, tostr("found #18657 at ", i, " index"));
    else
        notify(player, "couldn't find #18657 in the list!");
    endif
    

#### Returning a Value from a Verb

The MOO program in a verb is just a sequence of statements. Normally, when
the verb is called, those statements are simply executed in order and then the
integer 0 is returned as the value of the verb-call expression. Using the
return statement, one can change this behavior. The return
statement has one of the following two forms:

    
    return;
    

or

    
    return _expression_;
    

When it is executed, execution of the current verb is terminated immediately
after evaluating the given _expression_, if any. The verb-call expression
that started the execution of this verb then returns either the value of
_expression_ or the integer 0, if no _expression_ was provided.

We could modify the example given above. Imagine a verb called _has\_object_ which takes an object (that we want to find) as it's
first argument and a list of objects (to search) as it's second argument:

    
    {seek_obj, list_of_objects} = args;
    for obj in (list_of_objects)
        if (obj == seek_obj)
            return 1;
        endif
    endfor
    

The verb above could be called with _obj\_with\_verb_:_has\_object_(_\#18657_, 
_{\#1, \#3, \#4, \#3000}_) 
and it would return false (0) if the object was 
not found in the list. It would return true (1) if the object was found in the list.

Of course we could write this much simplier (and get the index of the object in the list at the same time):

    
    {seek_obj, list_of_objects} = args;
    return seek_obj in list_of_objects;
    

#### Handling Errors in Statements

Normally, whenever a piece of MOO code raises an error, the entire task is
aborted and a message printed to the user. Often, such errors can be
anticipated in advance by the programmer and code written to deal with them in
a more graceful manner. The try-except statement allows you to
do this; the syntax is as follows:

    
    try
      _statements-0_
    except _variable-1_ (_codes-1_)
      _statements-1_
    except _variable-2_ (_codes-2_)
      _statements-2_
    ...
    endtry
    

where the _variable_s may be omitted and each _codes_ part is either
the keyword ANY or else a comma-separated list of expressions, just like
an argument list. As in an argument list, the splicing operator (@)
can be used here. There can be anywhere from 1 to 255 except clauses.

First, each _codes_ part is evaluated, yielding a list of error codes that
should be caught if they're raised; if a _codes_ is ANY, then it is
equivalent to the list of all possible MOO values.

Next, _statements-0_ is executed; if it doesn't raise an error, then that's
all that happens for the entire try-except statement. Otherwise,
let _E_ be the error it raises. From top to bottom, _E_ is searched
for in the lists resulting from the various _codes_ parts; if it isn't
found in any of them, then it continues to be raised, possibly to be caught by
some piece of code either surrounding this try-except statement
or higher up on the verb-call stack.

If _E_ is found first in _codes-i_, then _variable-i_ (if provided)
is assigned a value containing information about the error being raised and
_statements-i_ is executed. The value assigned to _variable-i_ is a
list of four elements:

    
    {_code_, _message_, _value_, _traceback_}
    

where _code_ is _E_, the error being raised, _message_ and
_value_ are as provided by the code that raised the error, and
_traceback_ is a list like that returned by the callers() function,
including line numbers. The _traceback_ list contains entries for every
verb from the one that raised the error through the one containing this
try-except statement.

Unless otherwise mentioned, all of the built-in errors raised by expressions,
statements, and functions provide tostr(_code_) as _message_ and
zero as _value_.

Here's an example of the use of this kind of statement:

    
    try
      result = object:(command)(@arguments);
      player:tell("=> ", toliteral(result));
    except v (ANY)
      tb = v[4];
      if (length(tb) == 1)
        player:tell("** Illegal command: ", v[2]);
      else
        top = tb[1];
        tb[1..1] = {};
        player:tell(top[1], ":", top[2], ", line ", top[6], ":", v[2]);
        for fr in (tb)
          player:tell("... called from ", fr[1], ":", fr[2], ", line ", fr[6]);
        endfor
        player:tell("(End of traceback)");
      endif
    endtry
    

#### Cleaning Up After Errors

Whenever an error is raised, it is usually the case that at least some MOO code
gets skipped over and never executed. Sometimes, it's important that a piece
of code _always_ be executed, whether or not an error is raised. Use the
try-finally statement for these cases; it has the following
syntax:

    
    try
      _statements-1_
    finally
      _statements-2_
    endtry
    

First, _statements-1_ is executed; if it completes without raising an
error, returning from this verb, or terminating the current iteration of a
surrounding loop (we call these possibilities _transferring control_), then
_statements-2_ is executed and that's all that happens for the entire
try-finally statement.

Otherwise, the process of transferring control is interrupted and
_statments-2_ is executed. If _statements-2_ itself completes without
transferring control, then the interrupted control transfer is resumed just
where it left off. If _statements-2_ does transfer control, then the
interrupted transfer is simply forgotten in favor of the new one.

In short, this statement ensures that _statements-2_ is executed after
control leaves _statements-1_ for whatever reason; it can thus be used to
make sure that some piece of cleanup code is run even if _statements-1_
doesn't simply run normally to completion.

Here's an example:

    
    try
      start = time();
      object:(command)(@arguments);
    finally
      end = time();
      this:charge_user_for_seconds(player, end - start);
    endtry
    

#### Executing Statements at a Later Time

It is sometimes useful to have some sequence of statements execute at a later
time, without human intervention. For example, one might implement an object
that, when thrown into the air, eventually falls back to the ground; the
throw verb on that object should arrange to print a message about the
object landing on the ground, but the message shouldn't be printed until some
number of seconds have passed.

The fork statement is intended for just such situations and has the
following syntax:

    
    fork (_expression_)
      _statements_
    endfork
    

The fork statement first executes the expression, which must return a
integer; call that integer _n_. It then creates a new MOO _task_ that
will, after at least _n_ seconds, execute the statements. When the new
task begins, all variables will have the values they had at the time the
fork statement was executed. The task executing the fork
statement immediately continues execution. The concept of tasks is discussed
in detail in the next section.

By default, there is no limit to the number of tasks any player may fork, but
such a limit can be imposed from within the database. See the chapter on
server assumptions about the database for details.

Occasionally, one would like to be able to kill a forked task before it even
starts; for example, some player might have caught the object that was thrown
into the air, so no message should be printed about it hitting the ground. If
a variable name is given after the fork keyword, like this:

    
    fork _name_ (_expression_)
      _statements_
    endfork
    

then that variable is assigned the _task ID_ of the newly-created task.
The value of this variable is visible both to the task executing the fork
statement and to the statements in the newly-created task. This ID can be
passed to the kill\_task() function to keep the task from running and
will be the value of task\_id() once the task begins execution.

Note: This feature has other uses as well. The MOO
is single threaded, which means that complex logic (verbs that call verbs that call verbs ...) can
cause the MOO to _lag_. For instance, let's say when your user tosses their ball up, you want
to calculate a complex trejectory involve the ball and other objects in the room. These calculations are 
costly and done in another verb, they take time to be performed. However, you want some actions to happen both before the calculations
(everyone in the room seeing the ball is thrown into the air) and after the ball has left the players hands (the player
reaches into their pocket and pulls out a new ball). If there is no fork() then the calculations need to complete before
the verb can continue execution, which means the player won't pull out a fresh ball until after the calculations are complete. A
fork() allows the player to throw the ball, the MOO to fork() the task, which allows execution of the verb to continue right away
and the user to pull out a new ball, without experiencing the delay that the calculations being returned (without a fork()) would have inccured.

An example of this:

    
    {ball} = args;
    player:tell("You throw the ball!");
    ball:calculate_trajectory();
    player:tell("You get out another ball!");
    

In the above example, player:tell("You get out another ball!"); will not be executed until
after ball:calculate\_trajectory(); is completed.

    
    {ball} = args;
    player:tell("You throw the ball!");
    fork (1)
        ball:calculate_trajectory();
    endfor
    player:tell("You get out another ball!");
    

In this forked example, the ball will be thrown, the task forked for 1 second later and the the final line telling the player they
got out another ball will be followed up right after, without having to wait for the trajectory verb to finish running.

This type of fork cannot be used if the trajectory is required by the code that runs after it. For instance:

    
    {ball} = args;
    player:tell("You throw the ball!");
    direction = ball:calculate_trajectory();
    player:tell("You get out another ball!");
    player:tell("Your ball arcs to the " + direction);
    

If the above task was forked as it is below:

    
    {ball} = args;
    player:tell("You throw the ball!");
    fork (1)
        direction = ball:calculate_trajectory();
    endfork
    player:tell("You get out another ball!");
    player:tell("Your ball arcs to the " + direction);
    

The verb would raise E\_VARNF due to direction not being defined.

### MOO Tasks

A _task_ is an execution of a MOO program. There are five kinds of tasks
in LambdaMOO:

* Every time a player types a command, a task is created to execute that
command; we call these _command tasks_.
* Whenever a player connects or disconnects from the MOO, the server starts a
task to do whatever processing is necessary, such as printing out
Munchkin has connected to all of the players in the same room; these
are called _server tasks_.
* The fork statement in the programming language creates a task whose
execution is delayed for at least some given number of seconds; these are
_forked tasks_.
* The suspend() function suspends the execution of the current task. A
snapshot is taken of whole state of the execution, and the execution will be
resumed later. These are called _suspended tasks_.
* The read() function also suspends the execution of the current task, in
this case waiting for the player to type a line of input. When the line is
received, the task resumes with the read() function returning the input
line as result. These are called _reading tasks_.

The last three kinds of tasks above are collectively known as _queued
tasks_ or _background tasks_, since they may not run immediately.

To prevent a maliciously- or incorrectly-written MOO program from running
forever and monopolizing the server, limits are placed on the running time of
every task. One limit is that no task is allowed to run longer than a certain
number of seconds; command and server tasks get five seconds each while other
tasks get only three seconds. This limit is, in practice, rarely reached. The
reason is that there is also a limit on the number of operations a task may
execute.

The server counts down _ticks_ as any task executes. Roughly speaking, it
counts one tick for every expression evaluation (other than variables and
literals), one for every if, fork or return statement, and
one for every iteration of a loop. If the count gets all the way down to zero,
the task is immediately and unceremoniously aborted. By default, command and
server tasks begin with an store of 30,000 ticks; this is enough for almost all
normal uses. Forked, suspended, and reading tasks are allotted 15,000 ticks
each.

These limits on seconds and ticks may be changed from within the database, as
can the behavior of the server after it aborts a task for running out; see the
chapter on server assumptions about the database for details.

Because queued tasks may exist for long periods of time before they begin
execution, there are functions to list the ones that you own and to kill them
before they execute. These functions, among others, are discussed in the
following section.

### Built-in Functions

There are a large number of built-in functions available for use by MOO
programmers. Each one is discussed in detail in this section. The
presentation is broken up into subsections by grouping together functions with
similar or related uses.

For most functions, the expected types of the arguments are given; if the
actual arguments are not of these types, E\_TYPE is raised. Some
arguments can be of any type at all; in such cases, no type specification is
given for the argument. Also, for most functions, the type of the result of
the function is given. Some functions do not return a useful result; in such
cases, the specification none is used. A few functions can potentially
return any type of value at all; in such cases, the specification value
is used.

Most functions take a certain fixed number of required arguments and, in some
cases, one or two optional arguments. If a function is called with too many or
too few arguments, E\_ARGS is raised.

Functions are always called by the program for some verb; that program is
running with the permissions of some player, usually the owner of the verb in
question (it is not always the owner, though; wizards can use
set\_task\_perms() to change the permissions _on the fly_). In the
function descriptions below, we refer to the player whose permissions are being
used as the _programmer_.

Many built-in functions are described below as raising E\_PERM unless
the programmer meets certain specified criteria. It is possible to restrict
use of any function, however, so that only wizards can use it; see the chapter
on server assumptions about the database for details.

#### Object-Oriented Programming

One of the most important facilities in an object-oriented programming language
is ability for a child object to make use of a parent's implementation of some
operation, even when the child provides its own definition for that operation.
The pass() function provides this facility in MOO.

**_function:_ pass**

pass -- calls the verb with the same name as the current verb but as defined on the parent of the object 
that defines the current verb.

_value_ pass (_arg_, ...)

Often, it is useful for a child object to define a verb that _augments_
the behavior of a verb on its parent object. For example, in the LambdaCore
database, the root object (which is an ancestor of every other object) defines
a verb called description that simply returns the value of
this.description; this verb is used by the implementation of the
look command. In many cases, a programmer would like the description of
some object to include some non-constant part; for example, a sentence about
whether or not the object was \`awake' or \`sleeping'. This sentence should be
added onto the end of the normal description. The programmer would like to
have a means of calling the normal description verb and then appending
the sentence onto the end of that description. The function pass() is
for exactly such situations.

pass calls the verb with the same name as the current verb but as
defined on the parent of the object that defines the current verb. The
arguments given to pass are the ones given to the called verb and the
returned value of the called verb is returned from the call to pass.
The initial value of this in the called verb is the same as in the
calling verb.

Thus, in the example above, the child-object's description verb might
have the following implementation:

    
    return pass() + "  It is " + (this.awake ? "awake." | "sleeping.");
    

That is, it calls its parent's description verb and then appends to the
result a sentence whose content is computed based on the value of a property on
the object.

In almost all cases, you will want to call pass() with the same
arguments as were given to the current verb. This is easy to write in MOO;
just call pass(@args).

#### Manipulating MOO Values

There are several functions for performing primitive operations on MOO values,
and they can be cleanly split into two kinds: those that do various very
general operations that apply to all types of values, and those that are
specific to one particular type. There are so many operations concerned with
objects that we do not list them in this section but rather give them their own
section following this one.

##### General Operations Applicable to all Values

**_Function:_ typeof**

typeof -- Takes any MOO value and returns an integer representing the type of _value_.

_int_ typeof (_value_)

The result is the same as the initial value of one of these built-in variables:
INT, FLOAT, STR, LIST, OBJ, or ERR.
Thus, one usually writes code like this:

    
    if (typeof(x) == LIST) ...
    

and not like this:

    
    if (typeof(x) == 3) ...
    

because the former is much more readable than the latter.

**_Function:_ tostr**

tostr -- Converts all of the given MOO values into strings and returns the concatenation
of the results.

_str_ tostr (_value_, ...)

    
    tostr(17)                  =>   "17"
    tostr(1.0/3.0)             =>   "0.333333333333333"
    tostr(#17)                 =>   "#17"
    tostr("foo")               =>   "foo"
    tostr({1, 2})              =>   "{list}"
    tostr(E_PERM)              =>   "Permission denied"
    tostr("3 + 4 = ", 3 + 4)   =>   "3 + 4 = 7"
    

Warning tostr() does not do a good job of converting lists into
strings; all lists, including the empty list, are converted into the string
"{list}". The function toliteral(), below, is better for this
purpose.

**_Function:_ toliteral**

Returns a string containing a MOO literal expression that, when evaluated,
would be equal to _value_.

_str_ toliteral (_value_)

    
    toliteral(17)         =>   "17"
    toliteral(1.0/3.0)    =>   "0.333333333333333"
    toliteral(#17)        =>   "#17"
    toliteral("foo")      =>   "\"foo\""
    toliteral({1, 2})     =>   "{1, 2}"
    toliteral(E_PERM)     =>   "E_PERM"
    

**_Function:_ toint**
  
**_Function:_ tonum**

toint -- Converts the given MOO value into an integer and returns that integer.

_int_ toint (_value_)

Floating-point numbers are rounded toward zero, truncating their fractional
parts. Object numbers are converted into the equivalent integers. Strings are
parsed as the decimal encoding of a real number which is then converted to an
integer. Errors are converted into integers obeying the same ordering (with
respect to <= as the errors themselves. toint() raises
E\_TYPE if _value_ is a list. If _value_ is a string but the
string does not contain a syntactically-correct number, then toint()
returns 0\.

    
    toint(34.7)        =>   34
    toint(-34.7)       =>   -34
    toint(#34)         =>   34
    toint("34")        =>   34
    toint("34.7")      =>   34
    toint(" - 34  ")   =>   -34
    toint(E_TYPE)      =>   1
    

**_Function:_ toobj**

toobj -- Converts the given MOO value into an object number and returns that object number.

_obj_ toobj (_value_)

The conversions are very similar to those for toint() except
that for strings, the number _may_ be preceded by \#.

    
    toobj("34")       =>   #34
    toobj("#34")      =>   #34
    toobj("foo")      =>   #0
    toobj({1, 2})     => E_TYPE (error)
    

**_Function:_ tofloat**

tofloat -- Converts the given MOO value into a floating-point number and returns that number.

_float_ tofloat (_value_)

Integers and object numbers are converted into the corresponding
integral floating-point numbers. Strings are parsed as the decimal encoding of
a real number which is then represented as closely as possible as a
floating-point number. Errors are first converted to integers as in
toint() and then converted as integers are. tofloat() raises
E\_TYPE if _value_ is a list. If _value_ is a string but the
string does not contain a syntactically-correct number, then tofloat()
returns 0\.

    
    tofloat(34)          =>   34.0
    tofloat(#34)         =>   34.0
    tofloat("34")        =>   34.0
    tofloat("34.7")      =>   34.7
    tofloat(E_TYPE)      =>   1.0
    

**_Function:_ equal**

equal -- Returns true if _value1_ is completely indistinguishable from _value2_.

_int_ equal (_value_, _value2_)

This is much the same operation as _value1_ == _value2_
except that, unlike ==, the equal() function does not treat
upper- and lower-case characters in strings as equal and thus, is case-sensitive.

    
    "Foo" == "foo"         =>   1
    equal("Foo", "foo")    =>   0
    equal("Foo", "Foo")    =>   1
    

**_Function:_ value\_bytes**

value\_bytes -- Returns the number of bytes of the server's memory required to store the given _value_.

_int_ value\_bytes (_value_)

**_Function:_ value\_hash**

value\_hash -- Returns the same string as string\_hash(toliteral(_value_)).

_str_ value\_hash (_value_)

See the description of string\_hash() for details.

##### Operations on Numbers

**_Function:_ random**

random -- An integer is chosen randomly from the range \[1.._mod_\] and returned.

_int_ random (\[int _mod_\])

_mod_ must be a positive integer; otherwise, E\_INVARG is raised.
If _mod_ is not provided, it defaults to the largest MOO integer, 2147483647\.

Warning: The random() function is not very random. 
You should augment it's randomness with something like this: random() % 100 + 1 for better
randomness.

**_Function:_ min**

min -- Return the smallest of it's arguments.

_int_ min (int _x_, ...)

All of the arguments must be numbers of the same kind (i.e., either integer or floating-point); 
otherwise E\_TYPE is raised.

**_Function:_ max**

max -- Return the largest of it's arguments.

_int_ max (int _x_, ...)

All of the arguments must be numbers of the same kind (i.e., either integer or floating-point); 
otherwise E\_TYPE is raised.

**_Function:_ abs**

abs -- Returns the absolute value of _x_.

_int_ abs (int _x_)

If _x_ is negative, then the result is -_x_; otherwise, 
the result is _x_. The number _x_ can be either integer or floating-point; 
the result is of the same kind.

**_Function:_ floatstr**

floatstr -- Converts _x_ into a string with more control than provided by either 
tostr() or toliteral().

_str_ floatstr (float _x_, int _precision_ \[, _scientific_\])

_Precision_ is the number of digits
to appear to the right of the decimal point, capped at 4 more than the maximum
available precision, a total of 19 on most machines; this makes it possible to
avoid rounding errors if the resulting string is subsequently read back as a
floating-point value. If _scientific_ is false or not provided, the result
is a string in the form "MMMMMMM.DDDDDD", preceded by a minus sign if
and only if _x_ is negative. If _scientific_ is provided and true, the
result is a string in the form "M.DDDDDDe+EEE", again preceded by a
minus sign if and only if _x_ is negative.

**_Function:_ sqrt**

sqrt -- Returns the square root of _x_.

_float_ sqrt (float _x_)

Raises E\_INVARG if _x_ is negative.

**_Function:_ sin**

sin -- Returns the sine of _x_.

_float_ sin (float _x_)

**_Function:_ cos**

cos -- Returns the cosine of _x_.

_float_ cos (float _x_)

**_Function:_ tangent**

tangent -- Returns the tangent of _x_.

_float_ tangent (float _x_)

**_Function:_ asin**

asin -- Returns the arc-sine (inverse sine) of _x_, in the
range \[-pi/2..pi/2\]

_float_ asin (float _x_)

Raises E\_INVARG if _x_ is outside the range \[-1.0..1.0\].

**_Function:_ acos**

acos -- Returns the arc-cosine (inverse cosine) of _x_, in the range \[0..pi\]

_float_ acos (float _x_)

Raises E\_INVARG if _x_ is outside the range \[-1.0..1.0\].

**_Function:_ atan**

atan -- Returns the arc-tangent (inverse tangent) of _y_ in the range \[-pi/2..pi/2\].

_float_ atan (float _y_ \[, float _x_\])

if _x_ is not provided, or of _y_/_x_ in the range 
\[-pi..pi\] if _x_ is provided.

**_Function:_ sinh**

sinh -- Returns the hyperbolic sine of _x_.

_float_ sinh (float _x_)

**_Function:_ cosh**

cosh -- Returns the hyperbolic cosine of _x_.

_float_ cosh (float _x_)

**_Function:_ tanh**

tanh -- Returns the hyperbolic tangent of _x_.

_float_ tanh (float _x_)

**_Function:_ exp**

exp -- Returns _e_ raised to the power of _x_.

_float_ exp (float _x_)

**_Function:_ log**

log -- Returns the natural logarithm of _x_.

_float_ log (float _x_)

Raises E\_INVARG if _x_ is not positive.

**_Function:_ log10**

log10 -- Returns the base 10 logarithm of _x_.

_float_ log10 (float _x_)

Raises E\_INVARG if _x_ is not positive.

**_Function:_ ceil**

ceil -- Returns the smallest integer not less than _x_, as a floating-point number.

_float_ ceil (float _x_)

**_Function:_ floor**

floor -- Returns the largest integer not greater than _x_, as a floating-point number.

_float_ floor (float _x_)

**_Function:_ trunc**

trunc -- Returns the integer obtained by truncating _x_ at the decimal point, as a
floating-point number.

_float_ trunc (float _x_)

For negative _x_, this is equivalent to ceil(); otherwise it is 
equivalent to floor().

##### Regular Expressions

##### Operations on Properties

**_Function:_ list **properties** (obj _object_)
Returns a list of the names of the properties defined directly on the given
_object_, not inherited from its parent. If _object_ is not valid,
then E\_INVARG is raised. If the programmer does not have read
permission on _object_, then E\_PERM is raised.

**_Function:_ list **property\_info** (obj _object_, str _prop-name_)

**_Function:_ none **set\_property\_info** (obj _object_, str _prop-name_, list _info_)
These two functions get and set (respectively) the owner and permission bits
for the property named _prop-name_ on the given _object_. If
_object_ is not valid, then E\_INVARG is raised. If _object_
has no non-built-in property named _prop-name_, then E\_PROPNF is
raised. If the programmer does not have read (write) permission on the
property in question, then property\_info() (set\_property\_info())
raises E\_PERM. Property info has the following form:****

    
    {_owner_, _perms_ [, _new-name_]}
    

where _owner_ is an object, _perms_ is a string containing only
characters from the set r, w, and c, and _new-name_ is
a string; _new-name_ is never part of the value returned by
property\_info(), but it may optionally be given as part of the value
provided to set\_property\_info(). This list is the kind of value
returned by property\_info() and expected as the third argument to
set\_property\_info(); the latter function raises E\_INVARG if
_owner_ is not valid, if _perms_ contains any illegal characters, or,
when _new-name_ is given, if _prop-name_ is not defined directly on
_object_ or _new-name_ names an existing property defined on
_object_ or any of its ancestors or descendants.

**_Function:_ none **add\_property** (obj _object_, str _prop-name_, _value_, list _info_)
Defines a new property on the given _object_, inherited by all of its
descendants; the property is named _prop-name_, its initial value is
_value_, and its owner and initial permission bits are given by _info_
in the same format as is returned by property\_info(), described above.
If _object_ is not valid or _info_ does not specify a valid owner and
well-formed permission bits or _object_ or its ancestors or descendants
already defines a property named _prop-name_, then E\_INVARG is
raised. If the programmer does not have write permission on _object_ or
if the owner specified by _info_ is not the programmer and the programmer
is not a wizard, then E\_PERM is raised.
**

**_Function:_ none **delete\_property** (obj _object_, str _prop-name_)
Removes the property named _prop-name_ from the given _object_ and all
of its descendants. If _object_ is not valid, then E\_INVARG is
raised. If the programmer does not have write permission on _object_,
then E\_PERM is raised. If _object_ does not directly define a
property named _prop-name_ (as opposed to inheriting one from its parent),
then E\_PROPNF is raised.
**

**_Function:_ int **is\_clear\_property** (obj _object_, str _prop-name_)

**_Function:_ none **clear\_property** (obj _object_, str _prop-name_)
These two functions test for clear and set to clear, respectively, the property
named _prop-name_ on the given _object_. If _object_ is not valid,
then E\_INVARG is raised. If _object_ has no non-built-in property
named _prop-name_, then E\_PROPNF is raised. If the programmer
does not have read (write) permission on the property in question, then
is\_clear\_property() (clear\_property()) raises E\_PERM.
If a property is clear, then when the value of that property is queried the
value of the parent's property of the same name is returned. If the parent's
property is clear, then the parent's parent's value is examined, and so on.
If _object_ is the definer of the property _prop-name_, as opposed to
an inheritor of the property, then clear\_property() raises
E\_INVARG.
****

##### Operations on Verbs

**_Function:_ list **verbs** (obj _object_)
Returns a list of the names of the verbs defined directly on the given
_object_, not inherited from its parent. If _object_ is not valid,
then E\_INVARG is raised. If the programmer does not have read
permission on _object_, then E\_PERM is raised.

Most of the remaining operations on verbs accept a string containing the verb's
name to identify the verb in question. Because verbs can have multiple names
and because an object can have multiple verbs with the same name, this practice
can lead to difficulties. To most unambiguously refer to a particular verb,
one can instead use a positive integer, the index of the verb in the list
returned by verbs(), described above.

For example, suppose that verbs(\#34) returns this list:

    
    {"foo", "bar", "baz", "foo"}
    

Object \#34 has two verbs named foo defined on it (this may not be
an error, if the two verbs have different command syntaxes). To refer
unambiguously to the first one in the list, one uses the integer 1; to refer to
the other one, one uses 4\.

In the function descriptions below, an argument named _verb-desc_ is either
a string containing the name of a verb or else a positive integer giving the
index of that verb in its defining object's verbs() list.

> 

For historical reasons, there is also a second, inferior mechanism for
referring to verbs with numbers, but its use is strongly discouraged. If the
property $server\_options.support\_numeric\_verbname\_strings exists with a
true value, then functions on verbs will also accept a numeric string (e.g.,
"4") as a verb descriptor. The decimal integer in the string works
more-or-less like the positive integers described above, but with two
significant differences:

1. The numeric string is a _zero-based_ index into verbs(); that is,
in the string case, you would use the number one less than what you would use
in the positive integer case.
2. When there exists a verb whose actual name looks like a decimal integer, this
numeric-string notation is ambiguous; the server will in all cases assume that
the reference is to the first verb in the list for which the given string could
be a name, either in the normal sense or as a numeric index.

Clearly, this older mechanism is more difficult and risky to use; new code
should only be written to use the current mechanism, and old code using numeric
strings should be modified not to do so.

**_Function:_ list **verb\_info** (obj _object_, str _verb-desc_)

**_Function:_ none **set\_verb\_info** (obj _object_, str _verb-desc_, list _info_)
These two functions get and set (respectively) the owner, permission bits, and
name(s) for the verb as specified by _verb-desc_ on the given _object_.
If _object_ is not valid, then E\_INVARG is raised. If _object_
does not define a verb as specified by _verb-desc_, then E\_VERBNF is
raised. If the programmer does not have read (write) permission on the verb in
question, then verb\_info() (set\_verb\_info()) raises
E\_PERM. Verb info has the following form:****

    
    {_owner_, _perms_, _names_}
    

where _owner_ is an object, _perms_ is a string containing only
characters from the set r, w, x, and d, and
_names_ is a string. This is the kind of value returned by
verb\_info() and expected as the third argument to
set\_verb\_info(). set\_verb\_info() raises E\_INVARG if
_owner_ is not valid, if _perms_ contains any illegal characters, or if
_names_ is the empty string or consists entirely of spaces; it raises
E\_PERM if _owner_ is not the programmer and the programmer is not a
wizard.

**_Function:_ list **verb\_args** (obj _object_, str _verb-desc_)

**_Function:_ none **set\_verb\_args** (obj _object_, str _verb-desc_, list _args_)
These two functions get and set (respectively) the direct-object, preposition,
and indirect-object specifications for the verb as specified by _verb-desc_
on the given _object_. If _object_ is not valid, then E\_INVARG
is raised. If _object_ does not define a verb as specified by
_verb-desc_, then E\_VERBNF is raised. If the programmer does not
have read (write) permission on the verb in question, then verb\_args()
(set\_verb\_args()) raises E\_PERM. Verb args specifications have
the following form:****

    
    {_dobj_, _prep_, _iobj_}
    

where _dobj_ and _iobj_ are strings drawn from the set "this",
"none", and "any", and _prep_ is a string that is either
"none", "any", or one of the prepositional phrases listed much
earlier in the description of verbs in the first chapter. This is the kind of
value returned by verb\_args() and expected as the third argument to
set\_verb\_args(). Note that for set\_verb\_args(), _prep_ must
be only one of the prepositional phrases, not (as is shown in that table) a set
of such phrases separated by / characters. set\_verb\_args raises
E\_INVARG if any of the _dobj_, _prep_, or _iobj_ strings is
illegal.

    
    verb_args($container, "take")
                        =>   {"any", "out of/from inside/from", "this"}
    set_verb_args($container, "take", {"any", "from", "this"})
    

**_Function:_ none **add\_verb** (obj _object_, list _info_, list _args_)
Defines a new verb on the given _object_. The new verb's owner, permission
bits and name(s) are given by _info_ in the same format as is returned by
verb\_info(), described above. The new verb's direct-object,
preposition, and indirect-object specifications are given by _args_ in the
same format as is returned by verb\_args, described above. The new verb
initially has the empty program associated with it; this program does nothing
but return an unspecified value.**

If _object_ is not valid, or _info_ does not specify a valid owner and
well-formed permission bits and verb names, or _args_ is not a legitimate
syntax specification, then E\_INVARG is raised. If the programmer does
not have write permission on _object_ or if the owner specified by
_info_ is not the programmer and the programmer is not a wizard, then
E\_PERM is raised.

**_Function:_ none **delete\_verb** (obj _object_, str _verb-desc_)
Removes the verb as specified by _verb-desc_ from the given _object_.
If _object_ is not valid, then E\_INVARG is raised. If the
programmer does not have write permission on _object_, then E\_PERM
is raised. If _object_ does not define a verb as specified by
_verb-desc_, then E\_VERBNF is raised.
**

**_Function:_ list **verb\_code** (obj _object_, str _verb-desc_ \[, _fully-paren_ \[, _indent_\]\])

**_Function:_ list **set\_verb\_code** (obj _object_, str _verb-desc_, list _code_)
These functions get and set (respectively) the MOO-code program associated with
the verb as specified by _verb-desc_ on _object_. The program is
represented as a list of strings, one for each line of the program; this is the
kind of value returned by verb\_code() and expected as the third argument
to set\_verb\_code(). For verb\_code(), the expressions in the
returned code are usually written with the minimum-necessary parenthesization;
if _full-paren_ is true, then all expressions are fully parenthesized.
Also for verb\_code(), the lines in the returned code are usually not
indented at all; if _indent_ is true, each line is indented to better show
the nesting of statements.****

If _object_ is not valid, then E\_INVARG is raised. If _object_
does not define a verb as specified by _verb-desc_, then E\_VERBNF is
raised. If the programmer does not have read (write) permission on the verb in
question, then verb\_code() (set\_verb\_code()) raises
E\_PERM. If the programmer is not, in fact. a programmer, then
E\_PERM is raised.

For set\_verb\_code(), the result is a list of strings, the error messages
generated by the MOO-code compiler during processing of _code_. If the
list is non-empty, then set\_verb\_code() did not install _code_; the
program associated with the verb in question is unchanged.

**_Function:_ list **disassemble** (obj _object_, str _verb-desc_)
Returns a (longish) list of strings giving a listing of the server's internal
"compiled" form of the verb as specified by _verb-desc_ on _object_.
This format is not documented and may indeed change from release to release,
but some programmers may nonetheless find the output of disassemble()
interesting to peruse as a way to gain a deeper appreciation of how the server
works.**

If _object_ is not valid, then E\_INVARG is raised. If _object_
does not define a verb as specified by _verb-desc_, then E\_VERBNF is
raised. If the programmer does not have read permission on the verb in
question, then disassemble() raises E\_PERM.

#### [Operations on Player Objects][10]

**_Function:_ list **players** ()
Returns a list of the object numbers of all player objects in the database.
**

**_Function:_ int **is\_player** (obj _object_)
Returns a true value if the given _object_ is a player object and a false
value otherwise. If _object_ is not valid, E\_INVARG is raised.
**

**_Function:_ none **set\_player\_flag** (obj _object_, _value_)
Confers or removes the "player object" status of the given _object_,
depending upon the truth value of _value_. If _object_ is not valid,
E\_INVARG is raised. If the programmer is not a wizard, then
E\_PERM is raised.**

If _value_ is true, then _object_ gains (or keeps) "player object"
status: it will be an element of the list returned by players(), the
expression is\_player(_object_) will return true, and the server will
treat a call to $do\_login\_command() that returns _object_ as
logging in the current connection.

If _value_ is false, the _object_ loses (or continues to lack) "player
object" status: it will not be an element of the list returned by
players(), the expression is\_player(_object_) will return
false, and users cannot connect to _object_ by name when they log into the
server. In addition, if a user is connected to _object_ at the time that
it loses "player object" status, then that connection is immediately broken,
just as if boot\_player(_object_) had been called (see the
description of boot\_player() below).

### [Operations on Network Connections][11]

**_Function:_ list **connected\_players** (\[_include-all_\])
Returns a list of the object numbers of those player objects with
currently-active connections. If _include-all_ is provided and true, then
the list includes the object numbers associated with _all_ current
connections, including ones that are outbound and/or not yet logged-in.
**

**_Function:_ int **connected\_seconds** (obj _player_)

**_Function:_ int **idle\_seconds** (obj _player_)
These functions return the number of seconds that the currently-active
connection to _player_ has existed and been idle, respectively. If
_player_ is not the object number of a player object with a
currently-active connection, then E\_INVARG is raised.
****

**_Function:_ none **notify** (obj _conn_, str _string_ \[, _no-flush_\])
Enqueues _string_ for output (on a line by itself) on the connection
_conn_. If the programmer is not _conn_ or a wizard, then
E\_PERM is raised. If _conn_ is not a currently-active connection,
then this function does nothing. Output is normally written to connections
only between tasks, not during execution.**

The server will not queue an arbitrary amount of output for a connection; the
MAX\_QUEUED\_OUTPUT compilation option (in options.h) controls the
limit. When an attempt is made to enqueue output that would take the server
over its limit, it first tries to write as much output as possible to the
connection without having to wait for the other end. If that doesn't result in
the new output being able to fit in the queue, the server starts throwing away
the oldest lines in the queue until the new ouput will fit. The server
remembers how many lines of output it has \`flushed' in this way and, when next
it can succeed in writing anything to the connection, it first writes a line
like \>\> Network buffer overflow: _X_ lines of output to you have been
lost << where _X_ is the number of flushed lines.

If _no-flush_ is provided and true, then notify() never flushes any
output from the queue; instead it immediately returns false. Notify()
otherwise always returns true.

**_Function:_ int **buffered\_output\_length** (\[obj _conn_\])
Returns the number of bytes currently buffered for output to the connection
_conn_. If _conn_ is not provided, returns the maximum number of bytes
that will be buffered up for output on any connection.
**

**_Function:_ str **read** (\[obj _conn_ \[, _non-blocking_\]\])
Reads and returns a line of input from the connection _conn_ (or, if not
provided, from the player that typed the command that initiated the current
task). If _non-blocking_ is false or not provided, this function suspends
the current task, resuming it when there is input available to be read. If
_non-blocking_ is provided and true, this function never suspends the
calling task; if there is no input currently available for input, read()
simply returns 0 immediately.**

If _player_ is provided, then the programmer must either be a wizard or the
owner of player; if player is not provided, then read()
may only be called by a wizard and only in the task that was last spawned by a
command from the connection in question. Otherwise, E\_PERM is raised.
If the given player is not currently connected and has no pending lines
of input, or if the connection is closed while a task is waiting for input but
before any lines of input are received, then read() raises
E\_INVARG.

The restriction on the use of read() without any arguments preserves the
following simple invariant: if input is being read from a player, it is for the
task started by the last command that player typed. This invariant adds
responsibility to the programmer, however. If your program calls another verb
before doing a read(), then either that verb must not suspend or else
you must arrange that no commands will be read from the connection in the
meantime. The most straightforward way to do this is to call

    
    set_connection_option(player, "hold-input", 1)
    

before any task suspension could happen, then make all of your calls to
read() and other code that might suspend, and finally call

    
    set_connection_option(player, "hold-input", 0)
    

to allow commands once again to be read and interpreted normally.

**_Function:_ none **force\_input** (obj _conn_, str _line_ \[, _at-front_\])
Inserts the string _line_ as an input task in the queue for the connection
_conn_, just as if it had arrived as input over the network. If
_at\_front_ is provided and true, then the new line of input is put at the
front of _conn_'s queue, so that it will be the very next line of input
processed even if there is already some other input in that queue. Raises
E\_INVARG if _conn_ does not specify a current connection and
E\_PERM if the programmer is neither _conn_ nor a wizard.
**

**_Function:_ none **flush\_input** (obj _conn_ \[_show-messages_\])
Performs the same actions as if the connection _conn_'s defined flush
command had been received on that connection, i.e., removes all pending lines
of input from _conn_'s queue and, if _show-messages_ is provided and
true, prints a message to _conn_ listing the flushed lines, if any. See
the chapter on server assumptions about the database for more information about
a connection's defined flush command.
**

**_Function:_ list **output\_delimiters** (obj _player_)
Returns a list of two strings, the current _output prefix_ and _output
suffix_ for _player_. If _player_ does not have an active network
connection, then E\_INVARG is raised. If either string is currently
undefined, the value "" is used instead. See the discussion of the
PREFIX and SUFFIX commands in the next chapter for more
information about the output prefix and suffix.
**

**_Function:_ none **boot\_player** (obj _player_)
Marks for disconnection any currently-active connection to the given
_player_. The connection will not actually be closed until the
currently-running task returns or suspends, but all MOO functions (such as
notify(), connected\_players(), and the like) immediately behave
as if the connection no longer exists. If the programmer is not either a
wizard or the same as _player_, then E\_PERM is raised. If there
is no currently-active connection to _player_, then this function does
nothing.**

If there was a currently-active connection, then the following verb call is
made when the connection is actually closed:

    
    $user_disconnected(_player_)
    

It is not an error if this verb does not exist; the call is simply skipped.

**_Function:_ str **connection\_name** (obj _player_)
Returns a network-specific string identifying the connection being used by the
given player. If the programmer is not a wizard and not _player_, then
E\_PERM is raised. If _player_ is not currently connected, then
E\_INVARG is raised.**

For the TCP/IP networking configurations, for in-bound connections, the string
has the form

    
    "port _lport_ from _host_, port _port_"
    

where _lport_ is the decimal TCP listening port on which the connection
arrived, _host_ is either the name or decimal TCP address of the host from
which the player is connected, and _port_ is the decimal TCP port of the
connection on that host.

For outbound TCP/IP connections, the string has the form

    
    "port _lport_ to _host_, port _port_"
    

where _lport_ is the decimal local TCP port number from which the
connection originated, _host_ is either the name or decimal TCP address of
the host to which the connection was opened, and _port_ is the decimal TCP
port of the connection on that host.

For the System V \`local' networking configuration, the string is the UNIX login
name of the connecting user or, if no such name can be found, something of the
form

    
    "User #_number_"
    

where _number_ is a UNIX numeric user ID.

For the other networking configurations, the string is the same for all
connections and, thus, useless.

**_Function:_ none **set\_connection\_option** (obj _conn_, str _option_, _value_)
Controls a number of optional behaviors associated the connection _conn_.
Raises E\_INVARG if _conn_ does not specify a current connection and
E\_PERM if the programmer is neither _conn_ nor a wizard. The
following values for _option_ are currently supported:

**"hold-input"

> If _value_ is true, then input received on _conn_ will never be treated
> as a command; instead, it will remain in the queue until retrieved by a call to
> read().
> 
> **"client-echo"
> 
> > Send the Telnet Protocol WONT ECHO or WILL ECHO command,
> > depending on whether _value_ is true or false, respectively. For clients
> > that support the Telnet Protocol, this should toggle whether or not the client
> > echoes locally the characters typed by the user. Note that the server itself
> > never echoes input characters under any circumstances. (This option is only
> > available under the TCP/IP networking configurations.)
> > 
> > **"binary"
> > 
> > > If _value_ is true, then both input from and output to _conn_ can
> > > contain arbitrary bytes. Input from a connection in binary mode is not broken
> > > into lines at all; it is delivered to either the read() function or the
> > > built-in command parser as _binary strings_, in whatever size chunks come
> > > back from the operating system. (See the early section on MOO value types for
> > > a description of the binary string representation.) For output to a connection
> > > in binary mode, the second argument to \`notify()' must be a binary string; if
> > > it is malformed, E\_INVARG is raised.
> > > 
> > > **"flush-command"
> > > 
> > > > If _value_ is a non-empty string, then it becomes the new _flush_
> > > > command for this connection, by which the player can flush all queued input
> > > > that has not yet been processed by the server. If _value_ is not a
> > > > non-empty string, then _conn_ is set to have no flush command at all. The
> > > > default value of this option can be set via the property
> > > > $server\_options.default\_flush\_command; see the chapter on server
> > > > assumptions about the database for details.
> > > > 
> > > 
> > > **
> > 
> > **
> 
> **

**
**

**_Function:_ list **connection\_options** (obj _conn_)
Returns a list of {_name_, _value_} pairs describing the
current settings of all of the allowed options for the connection _conn_.
Raises E\_INVARG if _conn_ does not specify a current connection and
E\_PERM if the programmer is neither _conn_ nor a wizard.
**

**_Function:_ value **connection\_option** (obj _conn_, str _name_)
Returns the current setting of the option _name_ for the connection
_conn_. Raises E\_INVARG if _conn_ does not specify a current
connection and E\_PERM if the programmer is neither _conn_ nor a
wizard.
**

**_Function:_ obj **open\_network\_connection** (_value_, ...)
Establishes a network connection to the place specified by the arguments and
more-or-less pretends that a new, normal player connection has been established
from there. The new connection, as usual, will not be logged in initially and
will have a negative object number associated with it for use with
read(), notify(), and boot\_player(). This object number
is the value returned by this function.**

If the programmer is not a wizard or if the OUTBOUND\_NETWORK compilation
option was not used in building the server, then E\_PERM is raised. If
the network connection cannot be made for some reason, then other errors will
be returned, depending upon the particular network implementation in use.

For the TCP/IP network implementations (the only ones as of this writing that
support outbound connections), there must be two arguments, a string naming a
host (possibly using the numeric Internet syntax) and an integer specifying a TCP
port. If a connection cannot be made because the host does not exist, the port
does not exist, the host is not reachable or refused the connection,
E\_INVARG is raised. If the connection cannot be made for other
reasons, including resource limitations, then E\_QUOTA is raised.

The outbound connection process involves certain steps that can take quite a
long time, during which the server is not doing anything else, including
responding to user commands and executing MOO tasks. See the chapter on
server assumptions about the database for details about how the server limits
the amount of time it will wait for these steps to successfully complete.

It is worth mentioning one tricky point concerning the use of this function.
Since the server treats the new connection pretty much like any normal player
connection, it will naturally try to parse any input from that connection as
commands in the usual way. To prevent this treatment, you should use
set\_connection\_option() to set the "hold-input" option true on
the connection.

**_Function:_ value **listen** (obj _object_, _point_ \[, _print-messages_\])
Create a new point at which the server will listen for network connections,
just as it does normally. _Object_ is the object whose verbs
do\_login\_command, do\_command, do\_out\_of\_band\_command,
user\_connected, user\_created, user\_reconnected,
user\_disconnected, and user\_client\_disconnected will be called at
appropriate points, just as these verbs are called on \#0 for normal
connections. (See the chapter on server assumptions about the database for the
complete story on when these functions are called.) _Point_ is a
network-configuration-specific parameter describing the listening point. If
_print-messages_ is provided and true, then the various
database-configurable messages (also detailed in the chapter on server
assumptions) will be printed on connections received at the new listening
point. Listen() returns _canon_, a \`canonicalized' version of
_point_, with any configuration-specific defaulting or aliasing accounted
for.**

This raises E\_PERM if the programmer is not a wizard, E\_INVARG if
_object_ is invalid or there is already a listening point described by
_point_, and E\_QUOTA if some network-configuration-specific error
occurred.

For the TCP/IP configurations, _point_ is a TCP port number on which to
listen and _canon_ is equal to _point_ unless _point_ is zero, in
which case _canon_ is a port number assigned by the operating system.

For the local multi-user configurations, _point_ is the UNIX file name to
be used as the connection point and _canon_ is always equal to _point_.

In the single-user configuration, the can be only one listening point at a
time; _point_ can be any value at all and _canon_ is always zero.

**_Function:_ none **unlisten** (_canon_)
Stop listening for connections on the point described by _canon_, which
should be the second element of some element of the list returned by
listeners(). Raises E\_PERM if the programmer is not a wizard and
E\_INVARG if there does not exist a listener with that description.
**

**_Function:_ list **listeners** ()

> Returns a list describing all existing listening points, including the default
> one set up automatically by the server when it was started (unless that one has
> since been destroyed by a call to unlisten()). Each element of the list
> has the following form:

**

    
    {_object_, _canon_, _print-messages_}
    

where _object_ is the first argument given in the call to listen()
to create this listening point, _print-messages_ is true if the third
argument in that call was provided and true, and _canon_ was the value
returned by that call. (For the initial listening point, _object_ is
\#0, _canon_ is determined by the command-line arguments or a
network-configuration-specific default, and _print-messages_ is true.)

Please note that there is nothing special about the initial listening point
created by the server when it starts; you can use unlisten() on it just
as if it had been created by listen(). This can be useful; for example,
under one of the TCP/IP configurations, you might start up your server on some
obscure port, say 12345, connect to it by yourself for a while, and then open
it up to normal users by evaluating the statments

    
    unlisten(12345); listen(#0, 7777, 1)
    

### [Operations Involving Times and Dates][12]

**_Function:_ int **time** ()

> Returns the current time, represented as the number of seconds that have
> elapsed since midnight on 1 January 1970, Greenwich Mean Time.
> 

**

**_Function:_ str **ctime** (\[int _time_\])

> Interprets _time_ as a time, using the same representation as given in the
> description of time(), above, and converts it into a 28-character,
> human-readable string in the following format:

**

    
    Mon Aug 13 19:13:20 1990 PDT
    

If the current day of the month is less than 10, then an extra blank appears
between the month and the day:

    
    Mon Apr  1 14:10:43 1991 PST
    

If _time_ is not provided, then the current time is used.

Note that ctime() interprets _time_ for the local time zone of the
computer on which the MOO server is running.

### [MOO-Code Evaluation and Task Manipulation][13]

**_Function:_ none **raise** (_code_ \[, str _message_ \[, _value_\]\])

> Raises _code_ as an error in the same way as other MOO expressions,
> statements, and functions do. _Message_, which defaults to the value of
> tostr(_code_), and _value_, which defaults to zero, are made
> available to any try-except statements that catch the error. If
> the error is not caught, then _message_ will appear on the first line of
> the traceback printed to the user.
> 

**

**_Function:_ value **call\_function** (str _func-name_, _arg_, ...)

> Calls the built-in function named _func-name_, passing the given arguments,
> and returns whatever that function returns. Raises E\_INVARG if
> _func-name_ is not recognized as the name of a known built-in function.
> This allows you to compute the name of the function to call and, in particular,
> allows you to write a call to a built-in function that may or may not exist in
> the particular version of the server you're using.
> 

**

**_Function:_ list **function\_info** (\[str _name_\])

> Returns descriptions of the built-in functions available on the server. If
> _name_ is provided, only the description of the function with that name is
> returned. If _name_ is omitted, a list of descriptions is returned, one
> for each function available on the server. Raised E\_INVARG if
> _name_ is provided but no function with that name is available on the
> server.

**

Each function description is a list of the following form:

    
    {_name_, _min-args_, _max-args_, _types_
    

where _name_ is the name of the built-in function, _min-args_ is the
minimum number of arguments that must be provided to the function,
_max-args_ is the maximum number of arguments that can be provided to the
function or -1 if there is no maximum, and _types_ is a list of
_max-args_ integers (or _min-args_ if _max-args_ is -1),
each of which represents the type of argument required in the corresponding
position. Each type number is as would be returned from the typeof()
built-in function except that -1 indicates that any type of value is
acceptable and -2 indicates that either integers or floating-point
numbers may be given. For example, here are several entries from the list:

    
    {"listdelete", 2, 2, {4, 0}}
    {"suspend", 0, 1, {0}}
    {"server_log", 1, 2, {2, -1}}
    {"max", 1, -1, {-2}}
    {"tostr", 0, -1, {}}
    

Listdelete() takes exactly 2 arguments, of which the first must be a
list (LIST == 4) and the second must be an integer (INT == 0).
Suspend() has one optional argument that, if provided, must be an
integer. Server\_log() has one required argument that must be a string
(STR == 2) and one optional argument that, if provided, may be of any
type. Max() requires at least one argument but can take any number
above that, and the first argument must be either an integer or a
floating-point number; the type(s) required for any other arguments can't be
determined from this description. Finally, tostr() takes any number of
arguments at all, but it can't be determined from this description which
argument types would be acceptable in which positions.

**_Function:_ list **eval** (str _string_)

> The MOO-code compiler processes _string_ as if it were to be the program
> associated with some verb and, if no errors are found, that fictional verb is
> invoked. If the programmer is not, in fact, a programmer, then E\_PERM
> is raised. The normal result of calling eval() is a two element list.
> The first element is true if there were no compilation errors and false
> otherwise. The second element is either the result returned from the fictional
> verb (if there were no compilation errors) or a list of the compiler's error
> messages (otherwise).

**

When the fictional verb is invoked, the various built-in variables have values
as shown below:

    
    player    the same as in the calling verb
    this      #-1
    caller    the same as the initial value of this in the calling verb
    
    args      {}
    argstr    ""
    
    verb      ""
    dobjstr   ""
    dobj      #-1
    prepstr   ""
    iobjstr   ""
    iobj      #-1
    

The fictional verb runs with the permissions of the programmer and as if its
d permissions bit were on.

    
    eval("return 3 + 4;")   =>   {1, 7}
    

**_Function:_ none **set\_task\_perms** (obj _who_)

> Changes the permissions with which the currently-executing verb is running to
> be those of _who_. If the programmer is neither _who_ nor a wizard,
> then E\_PERM is raised.
> 
> > 
> 
> 

**

**Note**: This does not change the owner of the currently-running verb,
only the permissions of this particular invocation. It is used in verbs owned
by wizards to make themselves run with lesser (usually non-wizard) permissions.

**_Function:_ obj **caller\_perms** ()

> Returns the permissions in use by the verb that called the currently-executing
> verb. If the currently-executing verb was not called by another verb (i.e., it
> is the first verb called in a command or server task), then
> caller\_perms() returns \#-1.
> 

**

**_Function:_ int **ticks\_left** ()

> **_Function:_ int **seconds\_left** ()
> 
> > These two functions return the number of ticks or seconds (respectively) left
> > to the current task before it will be forcibly terminated. These are useful,
> > for example, in deciding when to call suspend() to continue a long-lived
> > computation.
> > 
> 
> **

**

**_Function:_ int **task\_id** ()

> Returns the non-zero, non-negative integer identifier for the
> currently-executing task. Such integers are randomly selected for each task and
> can therefore safely be used in circumstances where unpredictability is
> required.
> 

**

**_Function:_ value **suspend** (\[int _seconds_\])

> Suspends the current task, and resumes it after at least _seconds_ seconds.
> (If _seconds_ is not provided, the task is suspended indefinitely; such a
> task can only be resumed by use of the resume() function.) When the
> task is resumed, it will have a full quota of ticks and seconds. This function
> is useful for programs that run for a long time or require a lot of ticks. If
> _seconds_ is negative, then E\_INVARG is raised. Suspend()
> returns zero unless it was resumed via resume(), in which case it
> returns the second argument given to that function.

**

In some sense, this function forks the \`rest' of the executing task. However,
there is a major difference between the use of suspend(_seconds_)
and the use of the fork (_seconds_). The fork statement
creates a new task (a _forked task_) while the currently-running task still
goes on to completion, but a suspend() suspends the currently-running
task (thus making it into a _suspended task_). This difference may be best
explained by the following examples, in which one verb calls another:

    
    .program   #0:caller_A
    #0.prop = 1;
    #0:callee_A();
    #0.prop = 2;
    .
    
    .program   #0:callee_A
    fork(5)
      #0.prop = 3;
    endfork
    .
    
    .program   #0:caller_B
    #0.prop = 1;
    #0:callee_B();
    #0.prop = 2;
    .
    
    .program   #0:callee_B
    suspend(5);
    #0.prop = 3;
    .
    

Consider \#0:caller\_A, which calls \#0:callee\_A. Such a task would
assign 1 to \#0.prop, call \#0:callee\_A, fork a new task, return to
\#0:caller\_A, and assign 2 to \#0.prop, ending this task. Five
seconds later, if the forked task had not been killed, then it would begin to
run; it would assign 3 to \#0.prop and then stop. So, the final value of
\#0.prop (i.e., the value after more than 5 seconds) would be 3\.

Now consider \#0:caller\_B, which calls \#0:callee\_B instead of
\#0:callee\_A. This task would assign 1 to \#0.prop, call
\#0:callee\_B, and suspend. Five seconds later, if the suspended task had
not been killed, then it would resume; it would assign 3 to \#0.prop,
return to \#0:caller\_B, and assign 2 to \#0.prop, ending the task.
So, the final value of \#0.prop (i.e., the value after more than 5
seconds) would be 2\.

A suspended task, like a forked task, can be described by the
queued\_tasks() function and killed by the kill\_task() function.
Suspending a task does not change its task id. A task can be suspended again
and again by successive calls to suspend().

By default, there is no limit to the number of tasks any player may suspend,
but such a limit can be imposed from within the database. See the chapter on
server assumptions about the database for details.

**_Function:_ none **resume** (int _task-id_ \[, _value_\])

> Immediately ends the suspension of the suspended task with the given
> _task-id_; that task's call to suspend() will return _value_,
> which defaults to zero. If _value_ is of type ERR, it will be
> raised, rather than returned, in the suspended task. Resume() raises
> E\_INVARG if _task-id_ does not specify an existing suspended task
> and E\_PERM if the programmer is neither a wizard nor the owner of the
> specified task.
> 

**

**_Function:_ list **queue\_info** (\[obj _player_\])

> If _player_ is omitted, returns a list of object numbers naming all players
> that currently have active task queues inside the server. If _player_ is
> provided, returns the number of background tasks currently queued for that
> user. It is guaranteed that queue\_info(_X_) will return zero for
> any _X_ not in the result of queue\_info().
> 

**

**_Function:_ list **queued\_tasks** ()

> Returns information on each of the background tasks (i.e., forked, suspended or
> reading) owned by the programmer (or, if the programmer is a wizard, all queued
> tasks). The returned value is a list of lists, each of which encodes certain
> information about a particular queued task in the following format:

**

    
    {_task-id_, _start-time_, _x_, _y_,
     _programmer_, _verb-loc_, _verb-name_, _line_, _this_}
    

where _task-id_ is an integer identifier for this queued task,
_start-time_ is the time after which this task will begin execution (in
time() format), _x_ and _y_ are obsolete values that are no
longer interesting, _programmer_ is the permissions with which this task
will begin execution (and also the player who _owns_ this task),
_verb-loc_ is the object on which the verb that forked this task was
defined at the time, _verb-name_ is that name of that verb, _line_ is
the number of the first line of the code in that verb that this task will
execute, and _this_ is the value of the variable this in that verb.
For reading tasks, _start-time_ is -1.

The _x_ and _y_ fields are now obsolete and are retained only for
backward-compatibility reasons. They may be reused for new purposes in some
future version of the server.

**_Function:_ none **kill\_task** (int _task-id_)

> Removes the task with the given _task-id_ from the queue of waiting tasks.
> If the programmer is not the owner of that task and not a wizard, then
> E\_PERM is raised. If there is no task on the queue with the given
> _task-id_, then E\_INVARG is raised.
> 

**

**_Function:_ list **callers** (\[_include-line-numbers_\])

> Returns information on each of the verbs and built-in functions currently
> waiting to resume execution in the current task. When one verb or function
> calls another verb or function, execution of the caller is temporarily
> suspended, pending the called verb or function returning a value. At any given
> time, there could be several such pending verbs and functions: the one that
> called the currently executing verb, the verb or function that called that one,
> and so on. The result of callers() is a list, each element of which
> gives information about one pending verb or function in the following format:

**

    
    {_this_, _verb-name_, _programmer_, _verb-loc_, _player_, _line-number_}
    

For verbs, _this_ is the initial value of the variable this in that
verb, _verb-name_ is the name used to invoke that verb, _programmer_ is
the player with whose permissions that verb is running, _verb-loc_ is the
object on which that verb is defined, _player_ is the initial value of the
variable player in that verb, and _line-number_ indicates which line
of the verb's code is executing. The _line-number_ element is included
only if the _include-line-numbers_ argument was provided and true.

For functions, _this_, _programmer_, and _verb-loc_ are all
\#-1, _verb-name_ is the name of the function, and _line-number_
is an index used internally to determine the current state of the built-in
function. The simplest correct test for a built-in function entry is

    
    (VERB-LOC == #-1  &&  PROGRAMMER == #-1  &&  VERB-name != "")
    

The first element of the list returned by callers() gives information on
the verb that called the currently-executing verb, the second element describes
the verb that called that one, and so on. The last element of the list
describes the first verb called in this task.

**_Function:_ list **task\_stack** (int _task-id_ \[, _include-line-numbers_\])

> Returns information like that returned by the callers() function, but
> for the suspended task with the given _task-id_; the
> _include-line-numbers_ argument has the same meaning as in
> callers(). Raises E\_INVARG if _task-id_ does not specify an
> existing suspended task and E\_PERM if the programmer is neither a wizard
> nor the owner of the specified task.
> 

**

### [Administrative Operations][14]

**_Function:_ str **server\_version** ()

> Returns a string giving the version number of the running MOO server.
> 

**

**_Function:_ none **server\_log** (str _message_ \[, _is-error_\])

> The text in _message_ is sent to the server log with a distinctive prefix
> (so that it can be distinguished from server-generated messages). If the
> programmer is not a wizard, then E\_PERM is raised. If _is-error_
> is provided and true, then _message_ is marked in the server log as an
> error.
> 

**

**_Function:_ obj **renumber** (obj _object_)

> The object number of the object currently numbered _object_ is changed to
> be the least nonnegative object number not currently in use and the new object
> number is returned. If _object_ is not valid, then E\_INVARG is
> raised. If the programmer is not a wizard, then E\_PERM is raised.
> If there are no unused nonnegative object numbers less than _object_, then
> _object_ is returned and no changes take place.

**

The references to _object_ in the parent/children and location/contents
hierarchies are updated to use the new object number, and any verbs, properties
and/or objects owned by _object_ are also changed to be owned by the new
object number. The latter operation can be quite time consuming if the
database is large. No other changes to the database are performed; in
particular, no object references in property values or verb code are updated.

This operation is intended for use in making new versions of the LambdaCore
database from the then-current LambdaMOO database, and other similar
situations. Its use requires great care.

**_Function:_ none **reset\_max\_object** ()

> The server's idea of the highest object number ever used is changed to be the
> highest object number of a currently-existing object, thus allowing reuse of
> any higher numbers that refer to now-recycled objects. If the programmer is
> not a wizard, then E\_PERM is raised.

**

This operation is intended for use in making new versions of the LambdaCore
database from the then-current LambdaMOO database, and other similar
situations. Its use requires great care.

**_Function:_ list **memory\_usage** ()

> On some versions of the server, this returns statistics concerning the server
> consumption of system memory. The result is a list of lists, each in the
> following format:

**

    
    {_block-size_, _nused_, _nfree_}
    

where _block-size_ is the size in bytes of a particular class of memory
fragments, _nused_ is the number of such fragments currently in use in the
server, and _nfree_ is the number of such fragments that have been reserved
for use but are currently free.

On servers for which such statistics are not available, memory\_usage()
returns {}. The compilation option USE\_GNU\_MALLOC controls
whether or not statistics are available; if the option is not provided,
statistics are not available.

**_Function:_ none **dump\_database** ()

> Requests that the server checkpoint the database at its next opportunity. It
> is not normally necessary to call this function; the server automatically
> checkpoints the database at regular intervals; see the chapter on server
> assumptions about the database for details. If the programmer is not a wizard,
> then E\_PERM is raised.
> 

**

**_Function:_ int **db\_disk\_size** ()

> Returns the total size, in bytes, of the most recent full representation of the
> database as one or more disk files. Raises E\_QUOTA if, for some reason,
> no such on-disk representation is currently available.
> 

**

**_Function:_ none **shutdown** (\[str _message_\])

> Requests that the server shut itself down at its next opportunity. Before
> doing so, a notice (incorporating _message_, if provided) is printed to all
> connected players. If the programmer is not a wizard, then E\_PERM is
> raised.
> 

**

# [Server Commands and Database Assumptions][15]

This chapter describes all of the commands that are built into the server and
every property and verb in the database specifically accessed by the server.
Aside from what is listed here, no assumptions are made by the server
concerning the contents of the database.

## [Built-in Commands][16]

As was mentioned in the chapter on command parsing, there are five commands
whose interpretation is fixed by the server: PREFIX,
OUTPUTPREFIX, SUFFIX, OUTPUTSUFFIX, and .program.
The first four of these are intended for use by programs that connect to the
MOO, so-called \`client' programs. The .program command is used by
programmers to associate a MOO program with a particular verb. The server can,
in addition, recognize a sixth special command on any or all connections, the
_flush_ command.

The server also performs special processing on command lines that begin with
certain punctuation characters.

This section discusses these built-in pieces of the command-interpretation
process.

### [Command-Output Delimiters][17]

Every MOO network connection has associated with it two strings, the
_output prefix_ and the _output suffix_. Just before executing a
command typed on that connection, the server prints the output prefix, if any,
to the player. Similarly, just after finishing the command, the output suffix,
if any, is printed to the player. Initially, these strings are not defined, so
no extra printing takes place.

The PREFIX and SUFFIX commands are used to set and clear these
strings. They have the following simple syntax:

    
    PREFIX  _output-prefix_
    SUFFIX  _output-suffix_
    

That is, all text after the command name and any following spaces is used as
the new value of the appropriate string. If there is no non-blank text after
the command string, then the corresponding string is cleared. For
compatibility with some general MUD client programs, the server also recognizes
OUTPUTPREFIX as a synonym for PREFIX and OUTPUTSUFFIX as a
synonym for SUFFIX.

These commands are intended for use by programs connected to the MOO, so that
they can issue MOO commands and reliably determine the beginning and end of the
resulting output. For example, one editor-based client program sends this
sequence of commands on occasion:

    
    PREFIX >>MOO-Prefix<<
    SUFFIX >>MOO-Suffix<<
    @list _object_:_verb_ without numbers
    PREFIX
    SUFFIX
    

The effect of which, in a LambdaCore-derived database, is to print out the code
for the named verb preceded by a line containing only \>\>MOO-Prefix<< and
followed by a line containing only \>\>MOO-Suffix<<. This enables the
editor to reliably extract the program text from the MOO output and show it to
the user in a separate editor window. There are many other possible uses.

The built-in function output\_delimiters() can be used by MOO code to
find out the output prefix and suffix currently in effect on a particular
network connection.

### [Programming][18]

The .program command is a common way for programmers to associate a
particular MOO-code program with a particular verb. It has the following
syntax:

    
    .program _object_:_verb_
    ..._several lines of MOO code_...
    .
    

That is, after typing the .program command, then all lines of input from
the player are considered to be a part of the MOO program being defined. This
ends as soon as the player types a line containing only a dot (.). When
that line is received, the accumulated MOO program is checked for proper MOO
syntax and, if correct, associated with the named verb.

If, at the time the line containing only a dot is processed, (a) the player is
not a programmer, (b) the player does not have write permission on the named
verb, or (c) the property $server\_options.protect\_set\_verb\_code exists
and has a true value and the player is not a wizard, then an error message is
printed and the named verb's program is not changed.

In the .program command, _object_ may have one of three forms:

* The name of some object visible to the player. This is exactly like the kind
of matching done by the server for the direct and indirect objects of ordinary
commands. See the chapter on command parsing for details. Note that the
special names me and here may be used.
* An object number, in the form \#_number_.
* A _system property_ (that is, a property on \#0), in the form
$_name_. In this case, the current value of \#0\._name_
must be a valid object.

### [Flushing Unprocessed Input][19]

It sometimes happens that a user changes their mind about having typed one or
more lines of input and would like to \`untype' them before the server actually
gets around to processing them. If they react quickly enough, they can type
their connection's defined _flush_ command; when the server first reads
that command from the network, it immediately and completely flushes any as-yet
unprocessed input from that user, printing a message to the user describing
just which lines of input were discarded, if any.

> 

_Fine point:_ The flush command is handled very early in the server's
processing of a line of input, before the line is entered into the task queue
for the connection and well before it is parsed into words like other commands.
For this reason, it must be typed exactly as it was defined, alone on the line,
without quotation marks, and without any spaces before or after it.

When a connection is first accepted by the server, it is given an initial flush
command setting taken from the current default. This initial setting can be
changed later using the set\_connection\_option() command.

By default, each connection is initially given .flush as its flush
command. If the property $server\_options.default\_flush\_command exists,
then its value overrides this default. If
$server\_options.default\_flush\_command is a non-empty string, then that
string is the flush command for all new connections; otherwise, new connections
are initially given no flush command at all.

### [Initial Punctuation in Commands][20]

The server interprets command lines that begin with any of the following
characters specially:

    
    "        :        ;
    

Before processing the command, the initial punctuation character is replaced by
the corresponding word below, followed by a space:

    
    say      emote    eval
    

For example, the command line

    
    "Hello, there.
    

is transformed into

    
    say Hello, there.
    

before parsing.

## [Server Assumptions About the Database][21]

There are a small number of circumstances under which the server directly and
specifically accesses a particular verb or property in the database. This
section gives a complete list of such circumstances.

### [Server Options Set in the Database][22]

Many optional behaviors of the server can be controlled from within the
database by creating the property \#0.server\_options (also known as
$server\_options), assigning as its value a valid object number, and then
defining various properties on that object. At a number of times, the server
checks for whether the property $server\_options exists and has an object
number as its value. If so, then the server looks for a variety of other
properties on that $server\_options object and, if they exist, uses their
values to control how the server operates.

The specific properties searched for are each described in the appropriate
section below, but here is a brief list of all of the relevant properties for
ease of reference:

**bg\_seconds

> The number of seconds allotted to background tasks.
> 
> **bg\_ticks
> 
> > The number of ticks allotted to background tasks.
> > 
> > **connect\_timeout
> > 
> > > The maximum number of seconds to allow an un-logged-in in-bound connection to
> > > remain open.
> > > 
> > > **default\_flush\_command
> > > 
> > > > The initial setting of each new connection's flush command.
> > > > 
> > > > **fg\_seconds
> > > > 
> > > > > The number of seconds allotted to foreground tasks.
> > > > > 
> > > > > **fg\_ticks
> > > > > 
> > > > > > The number of ticks allotted to foreground tasks.
> > > > > > 
> > > > > > **max\_stack\_depth
> > > > > > 
> > > > > > > The maximum number of levels of nested verb calls.
> > > > > > > 
> > > > > > > **name\_lookup\_timeout
> > > > > > > 
> > > > > > > > The maximum number of seconds to wait for a network hostname/address lookup.
> > > > > > > > 
> > > > > > > > **outbound\_connect\_timeout
> > > > > > > > 
> > > > > > > > > The maximum number of seconds to wait for an outbound network connection to
> > > > > > > > > successfully open.
> > > > > > > > > 
> > > > > > > > > **protect\__property_
> > > > > > > > > 
> > > > > > > > > > Restrict reading of built-in _property_ to wizards.
> > > > > > > > > > 
> > > > > > > > > > **protect\__function_
> > > > > > > > > > 
> > > > > > > > > > > Restrict use of built-in _function_ to wizards.
> > > > > > > > > > > 
> > > > > > > > > > > **support\_numeric\_verbname\_strings
> > > > > > > > > > > 
> > > > > > > > > > > > Enables use of an obsolete verb-naming mechanism.
> > > > > > > > > > > > 
> > > > > > > > > > > 
> > > > > > > > > > > **
> > > > > > > > > > 
> > > > > > > > > > **
> > > > > > > > > 
> > > > > > > > > **
> > > > > > > > 
> > > > > > > > **
> > > > > > > 
> > > > > > > **
> > > > > > 
> > > > > > **
> > > > > 
> > > > > **
> > > > 
> > > > **
> > > 
> > > **
> > 
> > **
> 
> **

**

### [Server Messages Set in the Database][23]

There are a number of circumstances under which the server itself generates
messages on network connections. Most of these can be customized or even
eliminated from within the database. In each such case, a property on
$server\_options is checked at the time the message would be printed. If
the property does not exist, a default message is printed. If the property
exists and its value is not a string or a list containing strings, then no
message is printed at all. Otherwise, the string(s) are printed in place of
the default message, one string per line. None of these messages are ever
printed on an outbound network connection created by the function
open\_network\_connection().

The following list covers all of the customizable messages, showing for each
the name of the relevant property on $server\_options, the default
message, and the circumstances under which the message is printed:

**boot\_msg = "\*\*\* Disconnected \*\*\*"

> The function boot\_player() was called on this connection.
> 
> **connect\_msg = "\*\*\* Connected \*\*\*"
> 
> > The user object that just logged in on this connection existed before
> > $do\_login\_command() was called.
> > 
> > **create\_msg = "\*\*\* Created \*\*\*"
> > 
> > > The user object that just logged in on this connection did not exist before
> > > $do\_login\_command() was called.
> > > 
> > > **recycle\_msg = "\*\*\* Recycled \*\*\*"
> > > 
> > > > The logged-in user of this connection has been recycled or renumbered (via the
> > > > renumber() function).
> > > > 
> > > > **redirect\_from\_msg = "\*\*\* Redirecting connection to new port \*\*\*"
> > > > 
> > > > > The logged-in user of this connection has just logged in on some other
> > > > > connection.
> > > > > 
> > > > > **redirect\_to\_msg = "\*\*\* Redirecting old connection to this port \*\*\*"
> > > > > 
> > > > > > The user who just logged in on this connection was already logged in on some
> > > > > > other connection.
> > > > > > 
> > > > > > **server\_full\_msg
> > > > > > 
> > > > > > > Default:
> > > > > > > 
> > > > > > 
> > > > > > **
> > > > > 
> > > > > **
> > > > 
> > > > **
> > > 
> > > **
> > 
> > **
> 
> **

**

    
    *** Sorry, but the server cannot accept any more connections right now.
    *** Please try again later.
    

This connection arrived when the server really couldn't accept any more
connections, due to running out of a critical operating system resource.

**timeout\_msg = "\*\*\* Timed-out waiting for login. \*\*\*"

> This in-bound network connection was idle and un-logged-in for at least
> CONNECT\_TIMEOUT seconds (as defined in the file options.h when
> the server was compiled).
> 
> > _Fine point:_ If the network connection in question was received at a
> > listening point (established by the listen() function) handled by an
> > object _obj_ other than \#0, then system messages for that connection
> > are looked for on _obj_.server\_options; if that property does not
> > exist, then $server\_options is used instead.
> > 
> 
> ### [Checkpointing the Database][24]
> 
> The server maintains the entire MOO database in main memory, not on disk. It
> is therefore necessary for it to dump the database to disk if it is to persist
> beyond the lifetime of any particular server execution. The server is careful
> to dump the database just before shutting down, of course, but it is also
> prudent for it to do so at regular intervals, just in case something untoward
> happens.
> 
> To determine how often to make these _checkpoints_ of the database, the
> server consults the value of \#0.dump\_interval. If it exists and its
> value is an integer greater than or equal to 60, then it is taken as the number
> of seconds to wait between checkpoints; otherwise, the server makes a new
> checkpoint every 3600 seconds (one hour). If the value of
> \#0.dump\_interval implies that the next checkpoint should be scheduled at
> a time after 3:14:07 a.m. on Tuesday, January 19, 2038, then the server instead
> uses the default value of 3600 seconds in the future.
> 
> The decision about how long to wait between checkpoints is made again
> immediately after each one begins. Thus, changes to \#0.dump\_interval
> will take effect after the next checkpoint happens.
> 
> Whenever the server begins to make a checkpoint, it makes the following verb
> call:
> 
>     
>     $checkpoint_started()
>     
> 
> When the checkpointing process is complete, the server makes the following verb
> call:
> 
>     
>     $checkpoint_finished(_success_)
>     
> 
> where _success_ is true if and only if the checkpoint was successfully
> written on the disk. Checkpointing can fail for a number of reasons, usually
> due to exhaustion of various operating system resources such as virtual memory
> or disk space. It is not an error if either of these verbs does not exist; the
> corresponding call is simply skipped.
> 
> ### [Accepting and Initiating Network Connections][25]
> 
> When the server first accepts a new, incoming network connection, it is given
> the low-level network address of computer on the other end. It immediately
> attempts to convert this address into the human-readable host name that will be
> entered in the server log and returned by the connection\_name()
> function. This conversion can, for the TCP/IP networking configurations,
> involve a certain amount of communication with remote name servers, which can
> take quite a long time and/or fail entirely. While the server is doing this
> conversion, it is not doing anything else at all; in particular, it it not
> responding to user commands or executing MOO tasks.
> 
> By default, the server will wait no more than 5 seconds for such a name lookup
> to succeed; after that, it behaves as if the conversion had failed, using
> instead a printable representation of the low-level address. If the property
> name\_lookup\_timeout exists on $server\_options and has an integer
> as its value, that integer is used instead as the timeout interval.
> 
> When the open\_network\_connection() function is used, the server must
> again do a conversion, this time from the host name given as an argument into
> the low-level address necessary for actually opening the connection. This
> conversion is subject to the same timeout as in the in-bound case; if the
> conversion does not succeed before the timeout expires, the connection attempt
> is aborted and open\_network\_connection() raises E\_QUOTA.
> 
> After a successful conversion, though, the server must still wait for the
> actual connection to be accepted by the remote computer. As before, this can
> take a long time during which the server is again doing nothing else. Also as
> before, the server will by default wait no more than 5 seconds for the
> connection attempt to succeed; if the timeout expires,
> open\_network\_connection() again raises E\_QUOTA. This default
> timeout interval can also be overridden from within the database, by defining
> the property outbound\_connect\_timeout on $server\_options with an
> integer as its value.
> 
> ### [Associating Network Connections with Players][26]
> 
> When a network connection is first made to the MOO, it is identified by a
> unique, negative object number. Such a connection is said to be
> _un-logged-in_ and is not yet associated with any MOO player object.
> 
> Each line of input on an un-logged-in connection is first parsed into words in
> the usual way (see the chapter on command parsing for details) and then these
> words are passed as the arguments in a call to the verb
> $do\_login\_command(). For example, the input line
> 
>     
>     connect Munchkin frebblebit
>     
> 
> would result in the following call being made:
> 
>     
>     $do_login_command("connect", "Munchkin", "frebblebit")
>     
> 
> In that call, the variable player will have as its value the negative
> object number associated with the appropriate network connection. The
> functions notify() and boot\_player() can be used with such object
> numbers to send output to and disconnect un-logged-in connections. Also, the
> variable argstr will have as its value the unparsed command line as
> received on the network connection.
> 
> If $do\_login\_command() returns a valid player object and the connection
> is still open, then the connection is considered to have _logged into_ that
> player. The server then makes one of the following verbs calls, depending on
> the player object that was returned:
> 
>     
>     $user_created(_player_)
>     $user_connected(_player_)
>     $user_reconnected(_player_)
>     
> 
> The first of these is used if the returned object number is greater than the
> value returned by the max\_object() function before
> $do\_login\_command() was invoked, that is, it is called if the returned
> object appears to have been freshly created. If this is not the case, then one
> of the other two verb calls is used. The $user\_connected() call is used
> if there was no existing active connection for the returned player object.
> Otherwise, the $user\_reconnected() call is used instead.
> 
> > 
> 
> _Fine point:_ If a user reconnects and the user's old and new connections
> are on two different listening points being handled by different objects (see
> the description of the listen() function for more details), then
> user\_client\_disconnected is called for the old connection and
> user\_connected for the new one.
> 
> If an in-bound network connection does not successfully log in within a certain
> period of time, the server will automatically shut down the connection, thereby
> freeing up the resources associated with maintaining it. Let _L_ be the
> object handling the listening point on which the connection was received (or
> \#0 if the connection came in on the initial listening point). To
> discover the timeout period, the server checks on
> _L_.server\_options or, if it doesn't exist, on
> $server\_options for a connect\_timeout property. If one is found
> and its value is a positive integer, then that's the number of seconds the
> server will use for the timeout period. If the connect\_timeout property
> exists but its value isn't a positive integer, then there is no timeout at
> all. If the property doesn't exist, then the default timeout is 300 seconds.
> 
> When any network connection (even an un-logged-in or outbound one) is
> terminated, by either the server or the client, then one of the following two
> verb calls is made:
> 
>     
>     $user_disconnected(_player_)
>     $user_client_disconnected(_player_)
>     
> 
> The first is used if the disconnection is due to actions taken by the server
> (e.g., a use of the boot\_player() function or the un-logged-in timeout
> described above) and the second if the disconnection was initiated by the
> client side.
> 
> It is not an error if any of these five verbs do not exist; the corresponding
> call is simply skipped.
> 
> > 
> 
> **Note**: Only one network connection can be controlling a given player
> object at a given time; should a second connection attempt to log in as that
> player, the first connection is unceremoniously closed (and
> $user\_reconnected() called, as described above). This makes it easy to
> recover from various kinds of network problems that leave connections open but
> inaccessible.
> 
> When the network connection is first established, the null command is
> automatically entered by the server, resulting in an initial call to
> $do\_login\_command() with no arguments. This signal can be used by the
> verb to print out a welcome message, for example.
> 
> > 
> 
> **Warning**: If there is no $do\_login\_command() verb defined, then
> lines of input from un-logged-in connections are simply discarded. Thus, it is
> _necessary_ for any database to include a suitable definition for this
> verb.
> 
> ### [Out-of-Band Commands][27]
> 
> It is possible to compile the server with an option defining an
> _out-of-band prefix_ for commands. This is a string that the server will
> check for at the beginning of every line of input from players, regardless of
> whether or not those players are logged in and regardless of whether or not
> reading tasks are waiting for input from those players. If a given line of
> input begins with the defined out-of-band prefix (leading spaces, if any, are
> _not_ stripped before testing), then it is not treated as a normal command
> or as input to any reading task. Instead, the line is parsed into a list of
> words in the usual way and those words are given as the arguments in a call to
> $do\_out\_of\_band\_command(). For example, if the out-of-band prefix were
> defined to be \#$\#, then the line of input
> 
>     
>     #$# client-type fancy
>     
> 
> would result in the following call being made in a new server task:
> 
>     
>     $do_out_of_band_command("#$#", "client-type", "fancy")
>     
> 
> During the call to $do\_out\_of\_band\_command(), the variable player
> is set to the object number representing the player associated with the
> connection from which the input line came. Of course, if that connection has
> not yet logged in, the object number will be negative. Also, the variable
> argstr will have as its value the unparsed input line as received on the
> network connection.
> 
> Out-of-band commands are intended for use by fancy client programs that may
> generate asynchronous _events_ of which the server must be notified. Since
> the client cannot, in general, know the state of the player's connection
> (logged-in or not, reading task or not), out-of-band commands provide the only
> reliable client-to-server communications channel.
> 
> ### [The First Tasks Run By the Server][28]
> 
> Whenever the server is booted, there are a few tasks it runs right at the
> beginning, before accepting connections or getting the value of
> \#0.dump\_interval to schedule the first checkpoint (see below for more
> information on checkpoint scheduling).
> 
> First, the server calls $user\_disconnected() once for each user who
> was connected at the time the database file was written; this allows for any
> cleaning up that's usually done when users disconnect (e.g., moving their
> player objects back to some \`home' location, etc.).
> 
> Next, it checks for the existence of the verb $server\_started(). If
> there is such a verb, then the server runs a task invoking that verb with no
> arguments and with player equal to \#-1. This is useful for
> carefully scheduling checkpoints and for re-initializing any state that is not
> properly represented in the database file (e.g., re-opening certain outbound
> network connections, clearing out certain tables, etc.).
> 
> ### [Controlling the Execution of Tasks][29]
> 
> As described earlier, in the section describing MOO tasks, the server places
> limits on the number of seconds for which any task may run continuously and the
> number of "ticks," or low-level operations, any task may execute in one
> unbroken period. By default, foreground tasks may use 30,000 ticks and five
> seconds, and background tasks may use 15,000 ticks and three seconds. These
> defaults can be overridden from within the database by defining any or all of
> the following properties on $server\_options and giving them integer
> values:
> 
> **bg\_seconds
> 
> > The number of seconds allotted to background tasks.
> > 
> > **bg\_ticks
> > 
> > > The number of ticks allotted to background tasks.
> > > 
> > > **fg\_seconds
> > > 
> > > > The number of seconds allotted to foreground tasks.
> > > > 
> > > > **fg\_ticks
> > > > 
> > > > > The number of ticks allotted to foreground tasks.
> > > > > 
> > > > 
> > > > **
> > > 
> > > **
> > 
> > **
> 
> **
> 
> The server ignores the values of fg\_ticks and bg\_ticks if they
> are less than 100 and similarly ignores fg\_seconds and bg\_seconds
> if their values are less than 1\. This may help prevent utter disaster should
> you accidentally give them uselessly-small values.
> 
> Recall that command tasks and server tasks are deemed _foreground_ tasks,
> while forked, suspended, and reading tasks are defined as _background_
> tasks. The settings of these variables take effect only at the beginning of
> execution or upon resumption of execution after suspending or reading.
> 
> The server also places a limit on the number of levels of nested verb calls,
> raising E\_MAXREC from a verb-call expression if the limit is exceeded.
> The limit is 50 levels by default, but this can be increased from within the
> database by defining the max\_stack\_depth property on
> $server\_options and giving it an integer value greater than 50\. The
> maximum stack depth for any task is set at the time that task is created and
> cannot be changed thereafter. This implies that suspended tasks, even after
> being saved in and restored from the DB, are not affected by later changes to
> $server\_options.max\_stack\_depth.
> 
> Finally, the server can place a limit on the number of forked or suspended
> tasks any player can have queued at a given time. Each time a fork
> statement or a call to suspend() is executed in some verb, the server
> checks for a property named queued\_task\_limit on the programmer. If
> that property exists and its value is a non-negative integer, then that integer
> is the limit. Otherwise, if $server\_options.queued\_task\_limit exists
> and its value is a non-negative integer, then that's the limit. Otherwise,
> there is no limit. If the programmer already has a number of queued tasks that
> is greater than or equal to the limit, E\_QUOTA is raised instead of
> either forking or suspending. Reading tasks are affected by the queued-task
> limit.
> 
> ### [Controlling the Handling of Aborted Tasks][30]
> 
> The server will abort the execution of tasks for either of two reasons:
> 
> 1. an error was raised within the task but not caught, or
> 2. the task exceeded the limits on ticks and/or seconds.
> 
> In each case, after aborting the task, the server attempts to call a particular
> _handler verb_ within the database to allow code there to handle this
> mishap in some appropriate way. If this verb call suspends or returns a true
> value, then it is considered to have handled the situation completely and no
> further processing will be done by the server. On the other hand, if the
> handler verb does not exist, or if the call either returns a false value
> without suspending or itself is aborted, the server takes matters into its own
> hands.
> 
> First, an error message and a MOO verb-call stack _traceback_ are
> printed to the player who typed the command that created the original aborted
> task, explaining why the task was aborted and where in the task the problem
> occurred. Then, if the call to the handler verb was itself aborted, a second
> error message and traceback are printed, describing that problem as well. Note
> that if the handler-verb call itself is aborted, no further \`nested' handler
> calls are made; this policy prevents what might otherwise be quite a vicious
> little cycle.
> 
> The specific handler verb, and the set of arguments it is passed, differs for
> the two causes of aborted tasks.
> 
> If an error is raised and not caught, then the verb-call
> 
>     
>     $handle_uncaught_error(_code_, _msg_, _value_, _traceback_, _formatted_)
>     
> 
> is made, where _code_, _msg_, _value_, and _traceback_ are the
> values that would have been passed to a handler in a try-except
> statement and _formatted_ is a list of strings being the lines of error and
> traceback output that will be printed to the player if
> $handle\_uncaught\_error returns false without suspending.
> 
> If a task runs out of ticks or seconds, then the verb-call
> 
>     
>     $handle_task_timeout(_resource_, _traceback_, _formatted_)
>     
> 
> is made, where _resource_ is the appropriate one of the strings
> "ticks" or "seconds", and _traceback_ and _formatted_ are
> as above.
> 
> ### [Matching in Command Parsing][31]
> 
> In the process of matching the direct and indirect object strings in a command
> to actual objects, the server uses the value of the aliases property, if
> any, on each object in the contents of the player and the player's location.
> For complete details, see the chapter on command parsing.
> 
> ### [Restricting Access to Built-in Properties and Functions][32]
> 
> Whenever verb code attempts to read the value of a built-in property _prop_
> on any object, the server checks to see if the property
> $server\_options.protect\__prop_ exists and has a true value. If so,
> then E\_PERM is raised if the programmer is not a wizard.
> 
> Whenever verb code calls a built-in function _func_() and the caller
> is not the object \#0, the server checks to see if the property
> $server\_options.protect\__func_ exists and has a true value. If so,
> then the server next checks to see if the verb $bf\__func_() exists;
> if that verb exists, then the server calls it _instead_ of the built-in
> function, returning or raising whatever that verb returns or raises. If the
> $bf\__func_() does not exist and the programmer is not a wizard, then
> the server immediately raises E\_PERM, _without_ actually calling
> the function. Otherwise (if the caller is \#0, if
> $server\_options.protect\__func_ either doesn't exist or has a false
> value, or if $bf\__func_() exists but the programmer is a wizard),
> then the built-in function is called normally.
> 
> ### [Creating and Recycling Objects][33]
> 
> Whenever the create() function is used to create a new object, that
> object's initialize verb, if any, is called with no arguments. The call
> is simply skipped if no such verb is defined on the object.
> 
> Symmetrically, just before the recycle() function actually destroys an
> object, the object's recycle verb, if any, is called with no arguments.
> Again, the call is simply skipped if no such verb is defined on the object.
> 
> Both create() and recycle() check for the existence of an
> ownership\_quota property on the owner of the newly-created or -destroyed
> object. If such a property exists and its value is an integer, then it is
> treated as a _quota_ on object ownership. Otherwise, the following two
> paragraphs do not apply.
> 
> The create() function checks whether or not the quota is positive; if
> so, it is reduced by one and stored back into the ownership\_quota
> property on the owner. If the quota is zero or negative, the quota is
> considered to be exhausted and create() raises E\_QUOTA.
> 
> The recycle() function increases the quota by one and stores it back
> into the ownership\_quota property on the owner.
> 
> ### [Object Movement][34]
> 
> During evaluation of a call to the move() function, the server can make
> calls on the accept and enterfunc verbs defined on the
> destination of the move and on the exitfunc verb defined on the source.
> The rules and circumstances are somewhat complicated and are given in detail in
> the description of the move() function.
> 
> ### [Temporarily Enabling Obsolete Server Features][35]
> 
> If the property $server\_options.support\_numeric\_verbname\_strings exists
> and has a true value, then the server supports a obsolete mechanism for less
> ambiguously referring to specific verbs in various built-in functions. For
> more details, see the discussion given just following the description of the
> verbs() function.
> 

******


[0]: CHANGELOG.md
[1]: http://github.com/sevenecks
[2]: http://www.lisdude.com/moo/
[3]: http://www.hayseed.net/MOO/manuals/ProgrammersManual.html
[4]: http://www2.iath.virginia.edu/courses/moo/ProgrammersManual.texinfo_toc.html
[5]: https://github.com/SevenEcks/LambdaMOO
[6]: http://lambda.moo.mud.org/pub/MOO/
[7]: https://groups.google.com/forum/#!forum/MOO-talk
[8]: https://github.com/JavaChilly/dome-client.js
[9]: #introduction
[10]: ProgrammersManual_toc.html#TOC51
[11]: ProgrammersManual_toc.html#TOC52
[12]: ProgrammersManual_toc.html#TOC53
[13]: ProgrammersManual_toc.html#TOC54
[14]: ProgrammersManual_toc.html#TOC55
[15]: ProgrammersManual_toc.html#TOC56
[16]: ProgrammersManual_toc.html#TOC57
[17]: ProgrammersManual_toc.html#TOC58
[18]: ProgrammersManual_toc.html#TOC59
[19]: ProgrammersManual_toc.html#TOC60
[20]: ProgrammersManual_toc.html#TOC61
[21]: ProgrammersManual_toc.html#TOC62
[22]: ProgrammersManual_toc.html#TOC63
[23]: ProgrammersManual_toc.html#TOC64
[24]: ProgrammersManual_toc.html#TOC65
[25]: ProgrammersManual_toc.html#TOC66
[26]: ProgrammersManual_toc.html#TOC67
[27]: ProgrammersManual_toc.html#TOC68
[28]: ProgrammersManual_toc.html#TOC69
[29]: ProgrammersManual_toc.html#TOC70
[30]: ProgrammersManual_toc.html#TOC71
[31]: ProgrammersManual_toc.html#TOC72
[32]: ProgrammersManual_toc.html#TOC73
[33]: ProgrammersManual_toc.html#TOC74
[34]: ProgrammersManual_toc.html#TOC75
[35]: ProgrammersManual_toc.html#TOC76