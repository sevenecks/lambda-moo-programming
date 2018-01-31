
## Some tips on SpinnMoo programming

I'm still trying to get the hang of the LambdaMoo programming language. I thought I'd share some of what I've figured out, for both people who can program and for those who can't (yet).

This page isn't by any means complete. For that, see the documents on [Shawn's page][1]: the _LambdaCore User's Manual_, the _LambdaMOO Programmer's Manual, _and the _LambdaCore Database Programing Manual.

--Zompist_

[![Home](home.gif)][2] [\[ Page o' SpinnStuff \]][3]

---

### Basic objects

Most of you have figured out how to make objects; in case you haven't, the basic command looks like this:

> @create $thing named Maxwell's Silver Hammer,hammer 
> 

The new item will have a full name of Maxwell's Silver Hammer, but can be referred to as hammer for short. An item can have multiple **_aliases_**, separated by commas; you can also add aliases later with the @addalias command.

If you want a container instead, say $container instead of $thing. 

You probably want to add a description that the user will see if she uses the look command:

> @describe hammer as "It's silver, and small, and really really dangerous."
> 

Now, you want it to do something! How?

### Basic verbs
The simplest thing you might want to do is nudge the object and have it respond. This is easy enough, though at first the statements will seem cryptic. First, we **define the verb**:

> @verb rabbit:nudge this none none
> 

That tells the moo to create a new verb nudge for the rabbit, and that the usage will be nudge rabbit. (Moo stupidly doesn't understand articles, tho' I suppose you could define an alias the rabbit for your rabbit.) If you're an accurate typist, you can go on to **program the verb** like this:

> @program rabbit:nudge
>   
> player:tell( "You nudge the rabbit. It squeaks in dismay." );
>   
> player.location:announce( player.name, " nudges the rabbit. It squeaks in dismay." );
>   
> .
> 

### Editing
With any luck the moo will tell you that it successfully compiled the verb, and you're in business---you can test your program by typing nudge rabbit. More likely, though, you typed something wrong and the damn moo will bleat unhelpfully. If you've never programmed at all, you'll be amazed at the anality of the computer; it cannot make the simplest corrections (e.g. you typed player:location.announce instead of player.location:announce; or you spelled name as nmae). 

So, I edit my verbs with the **editor**, which you enter like this:

> @edit rabbit:nudge
> 

This will take you into a vintage-1970s line editor. You can type the program in the same way, but preceding each line with a quote mark, and omitting the final period:

> "player:tell( "You nudge the rabbit. It squeaks in dismay." );
>   
> "player.location:announce( player.name, " nudges the rabbit. It squeaks in dismay." );
> 

Type list to list the program so far. Type list 3 to list just line 3; del 3 to delete it; s/fred/joe/3 to replace fred with joe on line 3\. For more commands, see the _Programming Manual_.

You type save to attempt to compile it. The moo may well bleat again; but the big advantage of @edit over @program is that you don't have to retype your program (an amusement which quickly gets old), just correct it.

### What was that masked command?
It's worth understanding those player commands, because you'll be using them over and over. player:tell prints a message to the _player_, which is the character that entered the command. Player.location:announce prints a message to everyone in the player's room _except _the player. 

As in the example, you often want these two messages to differ. The message to others should include the player's name, not 'you'; you get at it with player.name. 

What is printed appears in parentheses. You can print several things at once, including numbers, text, variables, and more exotic things. You just include them all in the parentheses, separated by commas. 

Text appears in quotes: "This object rocks." If you're not a programmer, it's very easy to leave out the quotes, or just one of them. Don't do that; the moo will complain.

Like most statements, the command ends in a semicolon. 

### Remembering values
The first step to making the verb more interesting is usually to make the object remember certain values between iterations. For instance, the Glass Dildo remembers whether it's on or off. Remembered things are called **properties**, and you define them like this:

> @property dildo:on 1
>   
> @property radio:lastuser \#233
>   
> @property topiary:form "a giant fish"
> 

Unlike 'real' programming languages, moo doesn't require variables to be typed or declared; it just figures out what they are from what you put in them: in the example, respectively, a number, an object (object references are always preceded by \# in moo), and a string (that's programmerese for 'text').

Now, **within a verb's code**, you refer to the object's properties with this: e.g.

> this.on = 1;  
> this.lastuser = player;  
> this.form = iobjstr;
> 

From this you can figure out that player.name is a property---all players have a name---and player.location is another property, one that points to the room that currently contains the player. The _Database Manual_ gives a full listing of all the properties that various canned objects (like players, rooms, containers, and exits) have.

### Varying the description
Very possibly your property is so important you want it to be part of the object's description---you want it to be included when the player looks at your object. (Note that this is the only thing you _can_ do with my Slow Glass object!) This is not hard to do, though it will seem cryptic. First, you @describe the object as usual---this should provide the invariant part of the description. Then you tell the moo that the object has a variable description:

> @verb glass:description this none this
> 

Now you edit the :description verb. An example might be a radio:description verb:

> basic = pass(@args);
>   
> if (this.on == 1)
>   
> basic = basic + " The radio is on." ;
>   
> else
>   
> basic = basic + " The radio is off." ;
>   
> endif
>   
> return basic;
> 

We are overriding the default action of the description command, normally done by the parent object (the generic thing, generic container, or whatever). The first line executes the default routine---pass passes the parameters of our call to the parent---in this case returning the default description to us, the one we set using @describe. 

Now we add to this description. The + operator of course adds two strings together into a longer string. Note that we're testing our property, this.on. Don't forget the semicolons.

Finally, we return the completed string, basic, because description expects a string value to be returned (passed back for printing). 

Note that description doesn't use player:tell; this will be done for us somewhere else.
**

Verb parameters
**

If you're used to programming, moo verbs are frustratingly **limited in parameter handling**. (Actually the limitation is on the parser rather than the verbs themselves.) Basically you can only write verbs on the following models:

> Eat pill  
> Turn radio on  
> Give hernia to zompist  
> Poke k-man with sharp stick
> 

In effect all verbs have **three parameters** (called _direct object, preposition, _and _indirect object_). One of the objects must refer to the object, and the preposition is limited to a short list of predefined words. 

When you define a verb, you do it **backwards**. You're going to _say_ "eat pill", but you _define_ it as "pill:eat". 

When you create the verb, you always have to specify all three parameters. An unused parameter is **none**; one that refers to the object itself is **this**; one that refers to something else is **any**. The above verbs would be defined this way:

> @verb pill:eat this none none  
> @verb radio:turn this on none  
> @verb hernia:give this to any  
> @verb stick:poke any with this
> 

Each definition has **one this parameter**. You may be tempted to leave it out (@verb pill:eat none none none), but then the parser will never be able to call your verb! Think about the commands again, e.g. Eat pill. If it's a command for the pill object, the word pill has to occur somewhere in the command-- otherwise, the parser simply does not have any reason to pass the command to the pill at all. 

For the **simplest commands**, then, the parameter list should read this none none.
**

What if I defined it wrong? **You can use @rmverb. I tend to forget the parameters at first, then have to user @rmverb to get rid of the uncallable verb, as in this session:

> @verb pill:eat
>   
> "D'oh!
>   
> @rmverb pill:eat
>   
> @verb pill:eat this none none
> 

Some verbs, like _turn on _or _act up_, sound better with an **objectless preposition**. (Linguistically it's a particle, but moo calls it a preposition, so there.) You want to say Turn radio on, not Turn radio. You do this with the this (preposition) none parameter list. Note that the parser is not smart enough to let you say Turn on radio.

Often you want **two objects**---normally because you want your object to act on some other object. In this case you **must** define a preposition in between (from the list in the _Programming Manual_). You can't have a command that reads Give zompist money; it has to be Give money to zompist. That's why I had to define odd syntax like Play piano with "Greensleeves" or Drive bus to east.

In this case you can either specify the exact preposition in the verb (this on any), or allow any preposition (this any any).

You can put the other object (the any parameter) either as **direct or indirect object**. It's just a matter of what command sounds better. Compare the following verbs for the Sharp Stick:

> @verb stick:poke any with this -\> "Poke zompist with stick."  
> @verb stick:poke this at any -\> "Poke stick at zompist."
> 

When you **edit** a verb, you don't have to repeat the parameters, which is a blessing. You can @define a verb as shed:get any off this (quick quiz! What command will you use with that?), but to edit the code all you need to say is @edit shed:get.
**

Can I have multiple variations of a verb**? Sure---just in case you really really want to be able to say **both** "poke zompist with stick" and "poke stick at zompist". Just @define each variation, with its own parameter list. Now you'll have to specify the parameter list when you edit. A warning, though: I don't think it's worth it. It's no use trying to pretend to the user that she's typing ordinary English; and you're just likely to confuse or frustrate yourself with the extra parameters and verb versions.
**

Can I have more parameters? **Sort of. What you do is parse the third argument (the indirect object, available inside the program as the string **iobjstr**) yourself. For instance, maybe you want to say listplayers game from 1 to 10. You have to define this as @verb game:listplayers this from any. You'll get "1 to 10" as the iobjstr, which you can then parse yourself. There are functions to help you do this. It's a bit of a hassle, though, and your life will be easier if you define separate verbs or something.
**

I want a string as the third parameter. **No problem; the parser takes care of this for you. The piano uses this: I defined 'play' as @verb piano this with any. If the player types Play piano with the music of Burt Bacharach, the parser takes the entire rest of the sentence, "the music of Burt Bacharach", as the indirect object, and places it in iobjstr. Since I simply want to treat it as a string, I don't need to do anything special.

### Error handling
It's not terribly difficult to write a program that handles correct input. The mark of a professional program, however, is how it handles incorrect input. If you don't think about this, your verb will sometimes crash embarrassingly, or produce silly results, or even corrupt your objects.

The parser will handle some errors for you: misspelling required words (such as the names of your objects or verbs), leaving parameters out; referring to objects that aren't there.

Some error conditions to think about:

* The state you're setting is already set. E.g. you turn the dildo on, and it's already on.
* The property you're checking is empty. E.g. the topiary doesn't have a form right now.
* You wanted an object as a parameter but got garbage. You can test this with (say) iobj == \#0 for this.
* You wanted a player and got an object. You can test this with is\_player(iobj).
* The property you're remembering no longer applies. For instance, the piano remembers who's hiding under it. But if that person has left the room, we have to consider the property to be null.
* You want the object (e.g. the sharp stick) to act on another object; what if the object supplied is yourself, or the object itself?

In general these conditions are handled by if statements. For instance, a radio:on verb might look like this:

> if (this.on == 1)
>   
> player:tell( "The radio is already on!" );
>   
> else
>   
> player:tell( "You turn on the radio." );
>   
> this.on = 1;
>   
> endif
> 

As usual, you can't deviate from the exact syntax the moo wants. The condition has to be in parentheses; there's no semicolon after the if, else, or endif; the endif must exist, even for a one-line statement. 

Very confusing if you're not a C/C++ programmer: **testing** a value requires a **double** equals sign (this.on == 1); **setting** one requires a **single** equals (this.on = 1). You can look at a misbehaving verb for minutes on end without noticing an error like this. (Not-equals is !=, by the way.)

The else part is optional. You can also have any number of elseif (another condition) blocks before the else.

### Some notes for C++ programmers
You'll see a strong C influence on the moo language. Compound statements, such as if/endif, however, look more like Basic. 

The language is object-oriented, with inheritance. Note that the @create command specifies the base class of the newly created object. Verbs correspond to methods (but you use object:method, not object::method), and properties to data members (using the same . as in C/C++). 

Can you define a verb that's not called by a player, but only by other verbs? Sure. It uses a special parameter list: this none this, which the player cannot enter (if there's no preposition the parser just quits). The dartboard's hit command works like this. It's defined by @verb dartboard:hit this none this and called by the dart:toss routine with the line

> \#278:hit();
> 

\#278 is the object number of the dartboard (object numbers are a special data type; think of them as pointers to objects). You can use this anywhere, to make sure the moo understands you whether or not the dartboard is around. It's better programming practice, of course, to avoid using the same number all over---it means you couldn't use the object in another moo, for instance. If we needed to refer to it often, it'd be better to put the dartboard number in a property of the darts, for instance.

There's fairly nice list (array) support, and lists can include any other type, including other lists, as members.

One little gotcha: the C construction cond ? texpr : fexpr morphs in moo into cond ? texpr | fexpr.

Most things the player can do on the command line can also be done by the programmer within a verb-- sometimes with a slightly different verb or syntax. For a full exploration of this, see the _Database Manual_.

Don't worry about prettyprinting your program. Moo stores it in an interpreted form, and supplies its own indents and spacing when you next edit it.

### A few examples
You may be interested in how I got some of the effects in the existing SpinnMoo toys. You can inspect the program code with @edit to see exactly how something was done.
**

Dildo**. The dildo remembers its state in an on property. It also remembers the last user; I defined a lastuser property pointing to an object, and set it to player when appropriate. The delayed action is accomplished using a fork command; see the _Programming Manual_. 
**

Piano**. The piano isn't really a container; it just remembers who is hiding under it (using a hider property). The description is modified to make this seem more real. Note the error handling in the hide command: there can only be one hider at a time, so the previous hider (if it's not the player!) must be ejected. We cannot prevent the hider from leaving the room, so the description and other commands must check to see that the hider is really still there. We do this by checking if hider.location == this.location-- if the hider is in the same room as the piano. The play command uses random to determine the player's style and the onlookers' reaction. 
**

Slow glass**. This is mostly implemented in a huge description command. There are three properties that increment (at different speeds, so they don't overlap) on each call: location, weather, and time of day. There's also a direction, 1 or --1, which determines whether location is incremented or decremented.
**

Readme**. The readme object simply contains couple of list properties, items and signed. The first contains the items added to the bulletin board; the second contains the players' names.
**

Dartboard**. This was the most complicated programming task. The dartboard remembers the current scores (as pair of lists, the players and their scores) and contains verbs to reset the scores, set the target, print out the scores, or retrieve the darts, which may have wandered to another room. There is also a hidden verb, hit, which is called by the darts when you toss them. This verb makes heavy use of the random function to vary the results. It throws three darts, totals them up, adjusts the scores, determines winners, and on the second and higher rounds passes the darts to the next player.
**

VW Microbus**. This required wizard status to fully implement; but the actual coding is straightforward. The bus is actually a separate room which stays in one place; the "bus" object you can pick up and carry around (and which moves around the moo as the bus is driven) is not even a container, and its enter verb just teleports the player to the separate room. The room has a single exit, whose destination point is reset as the player drives. The bus room itself is, of course, not inside another room at all; we just remember where it is as a property. The ways() function tells us what exits a room has; the _Database Manual_ can be ransacked for tidbits like this. The difficulty here was not so much writing the code, as in being careful about what needed to be done. For instance, as the bus moves, the bus object not only has to be moved, but messages have to be printed in the previous and new locations so users see the bus coming through.

---

# The sourcecode for this file was taken from [http://www.zompist.com/moohelp.htm][0] and is included in this repository for posterity. It is non-HTML5\.

[0]: http://www.zompist.com/moohelp.htm
[1]: http://moo.collapsar.cx/
