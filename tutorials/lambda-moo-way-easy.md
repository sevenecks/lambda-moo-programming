
# Colin's Way Easy Intro Guide to MOO Programming

### Version 2.2  
March 1996

_by Colin McCormick (a.k.a. Snowfall)  
colin@tripod.com_  

---

## 0.0) Silly Foreword

This document is for free distribution. If you have a
brilliant idea for how to make it into truly monumental cybernetic
scripture (or just have a comment, question or flame) [let me know][1]. Personal cheques are
also welcome. The _Way Easy Guide_ is intended to teach basic
programming concepts and methods, so if you already understand how to
code a MOO for loop or build a recursive daemon wanderer, allow me
direct you to Pavel Curtis' _LambdaMOO Programmer's Guide_,
available at [parcftp.xerox.com/pub/MOO/ProgrammersManual.\*][2]. There are a number
of elements of MOO programming that I have omitted in the interests of
time and personal sanity; Pavel's guide is significantly more
complete.

You may also be interested, as an intermediary document, in my [_Way
Easy MOO Programming Examples_][3], a series of building and
programming examples spelled out in (I hope) straightforward steps.

### 0.1) Special Note for HereMOO Programmers

[HereMOO][4] is
revolutionary among MOOs in its Web-based implementation. All
programming techniques on standard MOOs will work on HereMOO, but
there are a few non-standard things one can do as well. The most
relevant of these is the fact that HTML tags can be written directly
into descriptions or even into spoken/emoted text. Thus by typing

    	@describe me as "A <B>great</B> guy!" 

other people, when looking at you, will see:

    A **great** guy!
    
    

You could also type

    	say Melanie, you look <I>marvelous</I> tonight!

and, on the MOO, everyone will see:

    Colin says, "Melanie, you look _marvelous_ tonight!

This technique will also work for adding images, links, lists, etc. to
your description and the description of your room and objects.

For help with HTML, check out NCSA's [_Beginner's Guide to HTML_][5].

For more complicated HTML tricks on HereMOO (including [how to use a link to send a command to the
MOO][6] and [how to add images of your own to
your description][7]) see [below][8].

---

## 1.0) Introduction

This book is for aspiring MOO code programmers and other lower
vertebrates, except sheep. MOO code, for all its power and
flexibility, is really quite easy to learn (except for sheep.) To
read this book you will have to know basic elements of the MOO
environment and commands, but not much else, and by the time you
finish you should have a good grasp of the elements of MOO
programming, how to create and edit verbs, and the basics of the MOO
language.

One note before we begin. In order to do most of the examples
in this guide, you need to have been made a programmer on your MOO.
Most MOO wizards will do this upon request, with a smile and free of
charge. If you try some of the following examples and get errors like
"E\_VERBNF (Verb not found)" or "E\_PERM (Permission Denied)" make sure
that you are a programmer before proceeding, unless you're a sheep, in
which case there's just no hope.

Ovine-less ones, read on!

### 1.1) Object Orientation

To program on a MOO, one must first understand the concept of
an _object-oriented_ data structure. Everything that exists on a
MOO is an _object_ that has a unique _object number_. These
objects are organized into a sort of genealogical hierarchy, with the
Root Object (\#1) at the top and its "children", "grandchildren", etc.
branching down from it. The nifty thing about this is that every
object automatically _inherits_ all of the characteristics
(i.e. properties and verbs) of its parent, and, by extension, of all
of its ancestors back to \#1\.

### 1.2) Inheritance

Inheritance is useful in programming in that it allows for the
creation of _classes_ of objects. If we wished to create (for
instance) a refrigerator, a backpack and a kangaroo pouch, we could
first make a generic "container" object that could hold other objects
and recognize such commands as _open_, _close_, and
_lock_. Then we could create three children of this generic
container (customizing them to look like a refrigerator, a backpack
and a kangaroo pouch) and they would all function as containers that
could be opened, closed and locked, without having to reprogram each
one individually. Most MOOs are organized around this idea, and there
are many generic items available to use as parents. (For a complete
list, ask the resident wizards.) 

To see the children or parent of an object, type 

    	@kids _object name_
    	@parent _object name_ 

### 1.3) Referring to Objects

Objects are refered to in MOO code by their object number,
preceded by the number symbol (**\#**). Hence, the Root Object is
\#1, and the next object made will be \#2, etc. One of the best ways to
find object numbers is the @contents command. To see the
names and object numbers of the objects in your room, for instance,
type @contents here. To see your own contents (the items you
are holding), type @contents me.

### 1.4) In-Database Help

Type help objects for more information about objects.

---

## 2.0) Properties

Now that we've established the idea of objects and their
hierarchy, let's talk about what you can do with them. Objects can
have _properties_ and _verbs_ defined on them.
_Properties_ are chunks of data that have four possible data
types (or "flavours") while _verbs_ are MOO-code programs.
Properties are more straightforward, so we'll start with them.

### 2.1) Properties and their Flavours

Properties can hold any data you wish. Some properties,
defined on \#1 and thus inherited by all objects, characterize an
object in the MOO's virtual reality; among these are the "name",
"location" and "contents" properties. Others may be used by you, the
programmer, to hold the value of (say) the number of sandwiches left
in a sandwich vending machine, or a list of possible phrases your pet
parrot might say. There are four valid data types for properties;
they are:

* _Numbers_ -- any integer value, positive, negative or zero
* _Strings_ -- any group of text characters (can include numbers, but
they will be treated as text)
* _Lists_ -- a list of any number of "elements", which can be numbers,
strings, other lists or object numbers
* _Object numbers_
Strings must be surrounded by quotes (") and lists by curly brackets
({}) with their elements delimited by commas (,). All elements in a
list must be of the same data type. For example, {"this", "is",
"a", "list"} and {{1, "hi"}, {2, "there"}, {3, "bob"}}
are both valid properties.

### 2.2) Referring to Properties

Properties are referred to as 'object number.property
name' -- i.e., the name property of the Root Object is '\#1.name',
and its location property is '\#1.location'. To see the value of any
property, just type its reference (without quotes), preceded by the
_evaluation operator_, ";":

    	;#1.name
    	=> "Root Class"

The evaluation operator (;) is used to find the value of any reference
or MOO code expression. It is a very powerful programming tool, but
has the drawback that it can only recognize objects by number, not by
name or alias.

### 2.3) Creating and Removing Properties

To create a new property, use the @property command:

    	@property me.nickname "Crazy Caboola"
    
    	@property #123.flowers = {"iris", "rose", "daisy"}

(You may abbreviate this as @prop.) The first command will
create a property on you (remember, _everything_ is an object,
including you) called "nickname" and give it the value "Crazy
Caboola", a string. The second will create a property on object \#123
called "flowers" with the value {"iris", "rose", "daisy"}, a list.
Property names must not contain spaces (use the underscore "\_" for
property names like "sandwiches\_left".)

It's worth noting at this point that if you don't _own_ the
object onto which you are attempting to define a property, the MOO
probably won't let you define it. We'll return to the topic of
ownership and permissions [later][9]; for now
try examples on yourself or objects you've created.

If no initial value is specified in @property, it will
default to 0, a number. Be careful about this, because if you create
a property and give it no initial value, and later try to change the
value of that property to a string, a list or an object number, the
MOO will become confused and you will have to remove the property and
start again. If you are creating a blank property that you know will
eventually be a string, list or object number, give it an initial
value of "" (the empty string), {} (the empty list)
or \#-1 (a default "nothing" object.)

To remove a property, use the @rmproperty command:

    	@rmproperty me.flowers

(You may also abbreviate this as @rmprop.)

To list all properties _explicitly_ defined on an
object (the ones implicitly defined on it because of inheritance are
difficult to list -- do the following for each parent on up the object
tree) use:

    	;properties(#1)

### 2.4) Changing and Viewing Property Values

Object inheritance includes properties, and the initial values
of these properties are the same as those of the object's parent,
except for certain key properties such as "name", "location" and
"owner". Both inherited and explicity defined properties you own can
be changed by the @set command:

    	@set me.nickname to "A Dyspeptic Aardvark"

In order to see the value of a given property, you can either use the
evaluation operator (;) if you know the object number of the object on
which the property is defined, or you can use the @display
command:

    @display me.nickname
    .nickname     Colin (#123)       r c    "A Dyspeptic Aardvark"

The name, owner and value of the property are displayed, as well as
its "permission bits", which we will discuss [later][9]. @display may be abbreviated as
@disp.

### 2.5) In-Database Help

Type help properties for more information about properties.

---

## 3.0) Verbs

Verbs are the best part of a MOO. They are what make people
able to _do_ things - hence their name. A verb is a short
program, written in MOO code and "defined on" (assigned to) some
object. (Remember that this means the verb is implicitly defined on
all of this object's children, as well.) Examples of verbs include
_look_, _get_, and _floccinaucinihilipillificate_. The
greater the quantity and quality of verbs that are defined, the more
complex and engaging the MOO's virtual reality becomes. 

### 3.1) Verb Syntax

All MOO verbs not involving the evalulation operator (also
claled "eval") are structured in the following way:

    (verb) (direct object) (preposition) (indirect object)

The three _arguments_ (direct object, preposition and indirect object)
are optional, and can be used in any combination to suit the form of
the verb. For instance:

    	spill sewage on Yog-Shaggoth

uses all three optional arguments, while

    	harass Cobweb

uses only one, the direct object. It is important to figure out how
you want a verb to be typed in by players before you write it; make
sure that you have all the information typed in that the verb will
need (i.e. you need to know on whom to spill the sewage, and also what
to spill in the first place!) Be aware that if an object other than a
player or a room has a verb defined on it, the name of that object
**must** be one of the arguments. Otherwise, the MOO doesn't know
what object to find the verb on!

### 3.2) Creating Verbs

The first step to writing a verb (after you have figured out
its syntax) is to create it on a particular object. Usually this
object will be some nifty toy or tool you've already created using
@create, but you can also define verbs on yourself and any
room you own. Verbs on yourself will always be usable by you, but
never by anyone else (unless they copy the verb to themselves.) Verbs
on a room are usable when standing in the room, but not anywhere else.
Verbs defined on any other object will only be usable if you are
holding that object or if it is in the same room as you.

To create an (unprogrammed) verb on an object, use the
@verb command:

    	@verb object:verb name d.o. prep i.o

You must specify the object on which the verb is to be defined, the
name of the verb, and its arguments (if any.) Objects can be
specified either by their object number or by their name (or alias) if
they are in your possession or in the same room as you.

Argument specifications are of the following form: for the
direct object and indirect object, you may choose _this_ (the
name or number of this object must be typed in with the command);
_any_ (any valid object name or number must be typed in with the
command) or _none_ (no \[in\]direct object name or number is
required.) Valid prepositions are pretty straightforward: (_on_,
_at_, _with_, _over_, etc.) A full list can be seen by
typing help preposition. Examples of two valid verb
creations:

    	@verb #231:chuck this at any
    
    	@verb loins:gird this none none

The first command creates the verb _chuck_ on object \#231\.
_chuck_ expects to see a command like chuck \#231 at
Melia. The second command creates the verb _gird_ on the
object _loins_. _gird_ expects to see a command line like
gird loins. The object number, name and alias are
interchangeable as long as you are either holding the object or it is
in the same room as you.

### 3.3) Correcting Verb Arguments

If you realize after creating a verb that you have specified
the wrong arguments, use the @args command to correct it:

    	@args loins:gird this with any

_loins:gird_ now expects to see something like gird loins
with magnesium.

### 3.4) Removing/Renaming Verbs

To delete a verb that you don't want, the command is
@rmbverb object number:verb name. To rename a verb, the
command is @rename object number:old verb name to new verb
name. To see a list of all the verbs defined on an object, type
@verbs (object).

Finally, to see all the verbs explicitly defined on an object, type

    	;verbs(_object number_)

### 3.5) In-Database Help

Type help verbs or help @verb for more
information on verbs.

---

## 4.0) Permissions

As with most human systems, MOO employs a system of ownership
to keep track of who can do what to what. This system is somewhat
complex, but the basic rule of thumb is: **if you made it, you own
it, and you can do anything you want to it.** If you're in a hurry
to get to programming, [skip on ahead][10], but
it's a good idea to understand permissions if you want your verbs to
interact with other players.

### 4.1) Overview

Each object, property and verb has an owner, specified in its
"owner" property. When a new object, property or verb is created, the
player who creates it is the owner unless otherwise specified. All
players have an "owned\_objects" property that is a list of the objects
they own; unfortunately, no such system exists for keeping track of
the properties or verbs one owns, which can occasionally cause
problems.

In addition to an owner, objects, properties and verbs all
have a set of permission bits that specify what everyone other than
their owner may do to them (owners have no restrictions on what they
may do to their objects. Calm down, Melanie.) These permissions are
similar to those on UNIX filesystems, and are stored as a string of
characters. The possible characters, or "bits" for objects,
properties and verbs are different, and will be discussed in turn.

The best way of viewing a property or verb's owner and
permission bits is the @display command, mentioned above.

    	@display me.nickname
    
    	@display me:run

The best way of viewing an object's owner and permission bits is the
@examine command:

    	@examine me
    
    	@examine #100

We'll start by discussing property permissions.

### 4.2) Property Permissions

The permission bits for properties are drawn from the
following set:

* r -- the property is "readable", meaning others can use
@disply to see its value
* w -- the property is "writable", meaning others can
change its value, using @set
* c -- the inherited version of this property on a child
object will change ownership to the creator of the child
A permission bit string can have zero, one, two or all three of these
characters in it. For example, a permission bit string "rc" means
that the property is readable by everyone, writable (changable) only
by its owner, and the inherited version of the property on any
children of the object will be owned by the child object's creator.
This latter property is particularly important for wizard-owned
objects that become generics. The default permission string for
properties created with @property is "rc".

As a side note, it's a good idea to avoid writable properties where
possible. There's no reason for other people to be able to change
your data, and if you rely on the propety to give information to some
of your verbs, you have no guarantee that those verbs will continue to
work from day to day.

### 4.3) Verb Permissions

Verb permission strings are drawn from the following set:

* r -- the verb's code is readable by everyone, by using
@list
* w -- the verb's code is writable by everyone, by using
@edit
* x -- the verb is executable, and may be called from
within another verb
* d -- the verb is set for debugging; if it encounters an
error in execution it will print a "traceback" description of the
error
Writable verbs are very dangerous, as they allow any programmer to
issue commands as if he/she were you, by editing the verb's code. The
default permission string for verbs is "rd", but if a verb is
specified with the arguments "this none this" then it will have the
permission string "rxd", which is useful when creating verbs that are
to be called from within other verbs.

### 4.4) Object Permissions

Object permission strings are in fact not exactly that -- they
are individual properties called "r", "w", and "f", and are either 1
or 0 (true or false). Still, you can think of them as a string, as
the @chmod command (see below) will accept them as a string.
Their meanings are:

* r -- the object is readable: anyone may list the
properties and verbs defined on the object, using
;properties() and ;verbs()
* w -- the object is writable: anyone may define properties
or verbs on it using @property and @verb
* f -- the object is fertile: anyone may create a child of
it using @create
Again, writable objects are to be avoided unless there's a good reason
for them. Since no concise list of the properties and verbs you own
is kept, it's easy to lose track of verbs and properties stored on
objects you don't own.

### 4.5) Changing Permissions

To change the permission bit string of a property or object,
use the @chmod command:

    	@chmod me.nickname "rwc"
    
    	@chmod me:run "rxd"
    
    	@chmod me "rw"

The first of these commands means that anyone on the MOO (who is a
programmer) can read and change your nickname property; plus, if
anyone creates a child of you, they will own the inherited version of
the nickname property. The second command means that anyone can list
the code of the "run" verb (which we assume for the moment is a verb
defined on you); it can be called from within another verb, and it
will print traceback error messages if something goes wrong during its
execution. Finally, the third command means that anyone can see what
properties and verbs you have defined on you, and anyone can define a
new property or verb on you (which they will own.)

As a final emphasis, **beware of writable propeties, verbs and
objects**. If there's no good reason for them to be writable, don't
make them so.

### 4.6) In-Database Help

Type help property\_info, help verb\_info and help
@chown for more information on object, property and verb
permissions.

---

## 5.0) Programming Verbs

Once a verb has been created and all its arguments are
correct, all that remains is to program it. No sweat! The command to
program a verb is:

    	@edit _object:verb_

This will move you to the Verb Editor, a special room where you can
edit your verb line by line. Since the Verb Editor is every
programmer's best friend, it behooves us to briefly mention the
commands therein.

### 5.1) The Verb Editor

When you arrive in the Verb Editor, you will see a list of
commands. Typing look at any time will also give you this
list. The important ones are:

* _say (line)_: add a line at the current insertion point
* _list_: list the lines of your verb, with line numbers
* _delete \#_ - delete line number \#
* _insert \#_ - make the current insert point before line \# (to insert
at the end, use a dollar sign $ instead of a number)
* _move \#a to \#b_ - move line \#a to before line \#b
Two other non-editing commands are important:

* _compile_ - compile your verb (translate the MOO code lines into a
binary program that the computer can execute)
* _quit_ - quit and return to your previous location
N.B. -- you must compile your verb in order to it to work. Compile
after each version of the verb is completed. If you don't compile,
the next time you use @edit, the Verb Editor will ask if you want to
continue to work on the last (uncompiled) verb or throw it away. 

### 5.2) Programming Verbs (for real)

MOO verb code looks a lot like C (don't panic!) It is easy
and practical to learn, and may even serve as a good introduction to
learning other computer languages. A sample bit of MOO code looks
like:

    	player:tell("You take a cookie from the jar.");
    	player.location:announce(player.name, " takes a cookie from
    the cookie jar and eyes it hungrily.");
    	if (player.name == "Cookie Monster")
    	     player:tell("You wolf down the cookie with gusto.");
    	     player.location:announce(player.name, " gobbles up the
    cookie with no regard for decorum.  You are splattered with crumbs.");
    	else
    	     player:tell("You delicately nibble at the cookie.");
    	     player.location:announce(player.name, " demurely nibbles
    at the cookie and politely cleans up the crumbs.");
    	endif
    

The code is executed line by line, except in the event of [flow control][11] commands, which will be discussed
later. Every line (except the aforementioned flow control commands)
must be followed by a semi-colon (;). While simple verbs are easy to
keep track of, you may want to make flow charts or other sketches of
more complicated verbs before leaping right into writing them.

### 5.3) The First Verb

The first verb to try, of course, is a simple one. Without
barely more knowledge than what we already have we can write a simple
verb, that will say something back to us when we type it in. The two
most important MOO code commands you will ever know are the following:

    	player:tell("Immanuel Kant was a real pissant...");
    	player.location:announce("...who was very rarely stable...");
    

The former tells the player who has called the verb (typed the verb
name as a command) whatever is in the quotes. The latter tells every
other player in the calling player's room whatever is in _its_
quotes. The above two lines, if written and compiled as a verb named
"bruce", would do the following: by typing bruce, the player
would see the words:

    Immanuel Kant was a real pissant...

and the other players in room would see:

    ...who was very rarely stable...

This may not seem like a big deal, but considering that you can make
the words in the parentheses anything you want, it can get fairly
interesting. Also, you needn't have just text in the parentheses: you
may also place property references or variables in the parentheses.
For example:

    	player:tell("Your name is ", player.name, " and the value of
    the variable \"count\" is ", count, ".");

will display to the calling player something like:

    Your name is Griselde, and the value of the variable "count" is
    1293.

Note that the various components of the sentence are separated
by commas. The expression '\\"' prints out a quote sign in the
sentence (a single quote " will be interpreted as the end of the text
string!)

_(Technical Note: the lines _player:tell(...)_and
_player.location:announce(..)_aren't actually MOO
commands. They instruct the MOO to call other verbs, either
_tell_or _announce_, that are defined on the objects
_player_and _player.location_, i.e. a room. These
particular verbs derive from the LambdaCore database distribution, and
a very few MOOs will not have them. If these lines fail to work on
your MOO, contact your wizards for assistance.)_

### 5.4) Congratulations...

...you can now write simple MOO verbs! Okay, so they're not
anything to get hot and bothered about, but they're a start. With
some general refinements, we can start writing interesting and
complicated verbs that will be the envy of other players for miles
around.

---

## 6.0) (Slightly) More Complicated Verb Programming

The most interesting and complicated verbs are ones that
respond differently to different conditions: who is calling it, where
that player is, what time it is, etc. This can be accomplished with
variables and flow control statements. We've already used one
variable unwittingly (_player_, above), so we're halfway there
already.

### 6.1) Variables

Variables are changeable bits of data that are used only while
the verb is being executed. Each variable has a name and a value. All
verbs begin with certain pre-defined variables that can be used
without any initializing statements. The more useful of these are:

* _player_ - the object number of the player who called this verb
* _this_ - the object number of the object on which the verb is
defined
* _verb_ - the name of the verb
* _dobjstr_ - the direct object string
* _iobjstr_ - the indirect object string
* _prepstr_ - the preposition string
* _args_ - list made up of the given arguments, i.e. for the
command "slather honey on Kassandra", args = {"honey", "on", "Kassandra"}
Any variable (including the predefined ones) can be created
and given a value or just given a new value by the equal sign ("="):

    	count = 3;
    	my_object = #143;
    	old_character_name = "Rumplestiltskin";
    

Variables may also be set equal to other variables in this way,
although a variable can never be set to a value of a different data
type than its original value.

### 6.2) Flow Control

Variables are most useful when they are used within a more
complex structure of execution than just one line after the next. The
three basic flow control statements allow for lines to be executed in
a special order, depending on certain tests. These statements are:

    	if (_condition 1_)
    		...
    	elseif (_condition 2_)
    		...
    	else
    		...
    	endif
    
    
    	while (_condition A_)
    		...
    	endwhile
    
    
    	for _variable_ in (_list or numerical interval_)
    		...
    	endfor
    

The ellipses stand for an arbitrary number of lines, including zero.

#### 6.2.1) The If Statement

* The if statement first tests _condition 1_. If
it is true then all the commands between it and the next
elseif, else or endif are executed, and
the whole if statement is over.
* If _condition 1_ is false, _condition 2_ is tested. If
it is true, all the commands between it and the _following_
elseif, else or endif are executed, and
the whole if statement is over.
* If _condition 2_ is false, or there was no elseif,
the statements between else and endif are executed,
and the whole if statement is over. If there's no
else, nothing at all happens.
You can have as many elseifs as you want; they are
tested in order (after the first if) from top to bottom.
Once one is judged true, the commands between it and the next are
executed, and the whole if statement is over. You may have
one or zero elses, and you must have one and only one
endif.

#### 6.2.2) The While Statement

The while statement is simpler. _condition A_
is tested, and if it is true all lines between while and
endwhile are executed. _condition A_ is then tested
again, and the same occurs. Only once _condition A_ is false
will the while statement be over. Note that this can result
in an infinite loop. Make sure that at least one of the lines after
while affects _condition A_ so that it will eventually
be false.

#### 6.2.3) The For Statement

for looks at the list or numerical interval it has
been given, and sets _variable_ equal to the first element or
number in the list or interval. It then executes all lines until it
gets to the endfor, sets _variable_ equal to the next element or
number in the list or interval, and repeats the process until it has
run through the entire list or interval. Note: to specify a numerical
interval, the syntax is:

    	for _variable_ in [_number1_.._number2_]

with square brackets "\[\]" and two periods between the numbers
specifying the range.

#### 6.2.4) Examples

Some examples seem in order:

    	snork = random(10);
    	badsnork = 4;
    	while (snork != badsnork)
    	     player:tell("Snork number is still not ", badsnork, "!");
    	     snork = random(10);
    	endwhile
    
    	for count in [1..10]
    	     player:tell("The count is now ", count, ".");
    	endfor
    
    	if (player.name == "Dirtbag")
    	     player:tell("You are a dirtbag.");
    	else
    	     player:tell("You are not a dirtbag.");
    	endif
    

The command random(x) generates a random integer from 1 to x.

### 6.3) Evaluation

Conditions in flow loops are evaluated in reasonably
straightforward ways. The following operators are important:

* == (tests equality; **NOTE**: do not confuse this with =,
which assigns a value to a variable!!!)
* The following tests if a value is in a given list:

    	_variable name_ in (list)

The list or list name must be surrounded by parentheses. Finally, the
logical operators are as follows:

    	&& - and	|| - or 

Thus (3 

# This source code was taken from [http://snowfall.tripod.com/Way\_Easy\_Guide.latest.html][0], modified to remove the javascript and other such stuff that tripod added to the page, and saved in this repository for posterity. It is not HTML5\.

[0]: http://snowfall.tripod.com/Way_Easy_Guide.latest.html
[1]: mailto:colin@tripod.com
[2]: ftp://parcftp.xerox.com/pub/MOO/
[3]: http://members.tripod.com/~Snowfall/Way_Easy_Examples.latest.html
[4]: http://trickle.tripod.com
[5]: http://www.ncsa.uiuc.edu/demoweb/html-primer.html
[6]: #link_command
[7]: #own_images
[8]: #html_tricks
[9]: #permissions
[10]: #programming
[11]: #flow_control
