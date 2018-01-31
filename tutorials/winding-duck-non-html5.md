# Lambda MOO Programmer's Tutorial

* [**Chapter 1: A Simple Object**][1]  
  * [Introduction][2]
  * [Simple Wind][3]
  * [Simple Messages][4]
  * [Simple Drop][5]
  
* [**Chapter 2: A More Complex Object**][6]  
  * [Complex Wind][7]
  * [Complex Messages][8]
  * [Complex Drop][9]
  
* [**Chapter 3: Other Programming Issues**][10]  
  * [Description][11]
  * [Chapter 1: A Simple Object][12]

### Introduction
This is a programming example of reasonable complexity. We are going to
demonstrate property use, forking, and generic object use.

Our example is a wind-up toy. The basic things that a wind-up toy does is get
wound, and then slowly wind its way down, hopping or rolling or twirling along.

First, let's create a couple of objects to work from.

    
      @create $thing named Generic Wind-Up Toy,Toy
      You now have Generic Wind-Up Toy (aka Toy) with object number #12221
      and parent generic thing (#5).
    
      @create #12221 named Wind-Up Duck,Duck
      You now have Wind-Up Duck (aka Duck) with object number #12222 and
      parent Generic Wind-Up Toy (#12221).
    

We'll refer to these as Toy and Duck, the aliases we gave in the @create
command.

### Simple Wind
First, we need some way to tell if the toy has been wound up. We'll create a
property called "wound" on the generic toy.

    
      @property toy.wound 0
      Property added with value 0.
    

Before we can write our program, we need a verb... We want to give commands
like "wind duck" so we give it an argument list of "this". The variable
"this", when used inside a verb, refers to the actual object (e.g. the duck) on
which the verb called. Its place in the argument list designates how the verb
will be found by the built-in parser.

    
      @verb toy:wind this
      Verb added.
    

Now we can make a simple program to wind the toy.

    
      @program toy:wind
      this.wound = this.wound + 2;
      player:tell("You wind up the ", this.name,".");
      player.location:announce(player.name, " winds up the ", this.name,".");
      .
    

Remember that "this" takes the place of whatever we will type in place of
"this" in our command line, so when the program runs, it will change the .wound
property of the duck. "Player" is the person who types the command. These are
built in variables available in every program, they just take on different
meanings depending on who types the command and what object is used in the
command. The idea behind adding 2 to this.wound is that you can wind it a
bunch of times to make it go longer.

Let's try it on our duck:

    
      wind duck
      You wind up the Wind-Up Duck.
    

Everyone else sees:

    
      yduJ winds up the Wind-Up Duck.
    

### Simple Messages
Next, we want to make the duck actually move around. Remember that we're doing
all the programming on the generic toy, and using the specific instance of the
duck to test things out. To be properly generic, we need to define different
properties that can hold message strings for the motions of the toy. So let's
define two messages. One for the toy to start moving, and another for it to
print as it continues to wind down.

    
      @property toy.startup_msg ""
      Property added with value "".
    
      @property toy.continue_msg ""
      Property added with value "".
    

We named these with "\_msg" in order for the messages to show up in @messages,
and be settable with the @message\_name object is "text" command. (See help
@messages for more details.) Let's set values of these messages on the duck.
While we're at it, we should give our duck a description!

    
      @startup duck is "waddles about and starts rolling forward."
      You set the "startup" message of Wind-Up Duck (#12222).
      @continue duck is "swivels its neck and emits a >> Quack <


# This tutorial was taken from [http://netlab.gmu.edu/muve/html/Winding-Duck.html][0] and is included in this repository for posterity. It is non-HTML5 for now.

[0]: http://netlab.gmu.edu/muve/html/Winding-Duck.html
[1]: #A1
[2]: #A1.1
[3]: #A1.2
[4]: #A1.3
[5]: #A1.4
[6]: #A2
[7]: #A2.1
[8]: #A2.2
[9]: #A2.3
[10]: #A3
[11]: #A3.1
[12]: #A3.2>Permissions</a>
    </UL>
</UL>

<HR>
<H2><A NAME=
