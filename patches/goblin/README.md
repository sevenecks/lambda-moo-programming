This extension pack holds several new built-ins as a counterpart of their MOO-verb equivalents.

```
$object_utils:ancestors() => ancestors()

$string_utils:space() => space()
$string_utils:char_list() => char_list()
$string_utils:explode() => explode()
$string_utils:words() => words()
$string_utils:from_list() => from_list()
$string_utils:english_list() => english_list()
$string_utils:from_seconds() => from_seconds()

$set_utils:diff*erence() => set_diff()
set_difference()
$set_utils:union() => set_union()
$set_utils:intersection() => Disabled due to a bug

$list_utils:reverse() => list_reverse()
$list_utils:setremove_all() => setremove_all()

```
Discussion
Goblin

Goblin - 2008-08-19
Logged In: YES 
user_id=1436857
Originator: YES

File Added: extension-goblin.tgz

 
Goblin

Goblin - 2008-08-20
Logged In: YES 
user_id=1436857
Originator: YES

Merged the bf_min/bf_max patch into this one.
Bumped version to 1.2
File Added: extension-goblin.tgz

 
Goblin

Goblin - 2008-09-02
Logged In: YES 
user_id=1436857
Originator: YES

Fixed a bug so that set_intersection() no longer has to be disabled.
File Added: extension-goblin.tgz

 
Goblin

Goblin - 2008-10-08
Fixed a bug in set_intersection().
File Added: extension-goblin.tgz

 
Goblin

Goblin - 2009-04-15
Added range() as it's $list_utils counterpart
Bumped version to 1.5.

 
Goblin

Goblin - 2009-04-15
Added count() as it's $list_utils counterpart
Bumped version to 1.6

 
Goblin

Goblin - 2009-04-16
Fixed a bug in bf_min() where a 'case TYPE_STR:' clause was mysteriously missing.
Rewrote char_list() to use regular substr() function from list.c
Also realligned code.
Bumped to version 1.6.1

 
Goblin

Goblin - 2011-01-04
I found out that the from_list function is broken when you feed it non-strings in args[1].. It causes a server panic.. I will look into this and post an updated version soon, but be advised to disable the builtin meanwhile..

 
Goblin

Goblin - 2011-01-05
Extensions pack v1.6.2

 
Goblin

Goblin - 2011-01-05
Bug has been fixed.. Bumped to version v1.6.2

 
Luke-Jr

Luke-Jr - 2011-11-11
Alternative: https://gitorious.org/moo/gammamoo/commits/core_functions allows wizards to write new functions in-db.

 
Goblin

Goblin - 2013-11-09
Bug fixed in all three set-builtins... Bumped to version v1.6.3

  
Last edit: Goblin 2013-11-10
