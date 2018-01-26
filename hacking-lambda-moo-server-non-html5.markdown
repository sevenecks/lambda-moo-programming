# The source code for this file was taken from [http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/hacking.html][0] and is stored in this repository for posterity. It is not HTML5\.

Okay, I got annoyed trying to write parts of the automatic core updater
utility, so I started digging through the server code and have discovered
some things:

# What happens when a connection is made to the LambdaMOO server

The another program connects to a port the server is listening on,
is accepts the connection after some minimal amount of error checking
or black list checking, and then creates a task with the following
associates: a unique identifier for that connection (a negative object
reference), and the object responsible for responding to that port.
In the default case, this object will be \#0 (the system object).
To determine what objects are responsible for which ports, the built in
function listeners() can be used. 

After creating the previously mentioned association and task, the server pushes
an empty string into the input queue of that task. The interesting thing
at this point is that this empty string is what causes the welcome message
in the LambdaCore database to be generated. What happens is this

* Server calls the managing objects "do\_login\_command" with the
input string
* * The input string fails to match any known verbs
* do\_login\_command defaults to a verb named "?" to handle the input
* * "?" displays the contents of $login.welcome\_message, along with a command
summary
* If you don't believe this, start up a copy of your favorite MOO core, connect
as a wizard, and @rmprop $login.welcome\_message and then login in again.
You should receive a fairly bizarre trace back error.

In the normal flow of events, one of the verbs called by "do\_login\_command"
will return an object reference, or end the connection by calling boot\_player.
Assuming that an object reference is returned, that task becomes promoted
to command task, and the rules for dealing with user input changes radically.

For input tasks, the flow of execution is as follows:

* parse input into basic tokens delimited by spaces
* * Check for input and output prefixes
* If input or output prefixes aren't detected, try executing
"do\_command" on the object associated with that connection from before
* If "do\_command" doesn't exist, or returns a false value, perform the
normal verb matching proceedure and execute the located verb as an
input task.
* If all else fails, call "huh", or as a last resort report 
"I didn't understand that." to the connection

1\. It is possible to completely redefine how the server deals with
user input by defining a "do\_command" verb on the object the player
has logged into on. do\_command is called before the normal processing
of input lines into psuedo-english phrases. However, doing macro
processing (as Mr. Fish was wondering about) would require doing all of the
normal text processing functions in LambdaMOO code, since it is not possible
since there are no destructive list maninpulation functions.

Each port the server is listening on is associated with an object. The
port bound by the server on start up is associated with object \#0
(The system object). do\_command gets passed the input string after
it has been broken up into white space delimited tokens. So, if for example 
\#0:do\_command looks like

    
    
      player:tell(@args);
      if (length(args)==3)
        return 1;
      else
        return 0;
      endif;
    
    

then a player connected through \#0 who typed

    
    
     l at me
    
    

would see the text

    
    
     lame
    
    

echoed back to them, and that command would never receive normal processing.
Interestingly enough, the command ".pr\*ogram" in the form

    
     .program Object:verb
    

is hard coded into the server for programmer players before the check for
"do\_command". Interestingly enough, do\_command isn't defined for either
the Sell Game, or Planet Oit.

2\. Server options

Yes, the server options to work. Essentially, the documentation is unclear
about where the properties are supposed to be found. The server code
expects them to be on $server\_options, or rather that $server\_options is
expected to be an object reference which defines the properties.

3\. Forking tasks

Granted that it looks like the server code for dealing with queued tasks
is fairly complicated, it doesn't look like the overhead for forking a task
is too much more that the overhead for invoking a verb on an object.

4\. Server startup scripts

The verb \#0:server\_started is run when the server starts up, after the 
database is loaded and the network code initialized. This would be the
place to put things like the http server initializations for the Sell Game
and Planet Oit.

5\. I've been looking through the server code for the MudOS LPC style
mud server, and managed to locate the main loop of the server. My major
reaction to the MudOS code is that it makes the LambdaMOO server code
(even the confusing parts) look blindingly straight forward. Essentially,
LPC muds implement a large amount of swapping objects out of memory,
calling reset functions every so often, and maintaining a circular list of 
objects to call a heart\_beat function periodically, all while trying to take
up more disk space than memory. Of course, LambdaMOO servers try to do
the exact opposite by trying to take up more virtual memory than disk space.
In any event, I honestly don't see why (if periodic resets and heart\_beat
functions were desired) these two functions could be implemented by carefully
written LambdaMOO code.

6\. I still haven't found a good way of introducing a good destructive list
append function into the Lambda server. At the moment, it appears that
this would involve adding a built in function, and as such the naming
convention would be
static package bf\_ItsName(...)
and then call
register\_function("ItsName",?,?,bf\_ItsName,ArgType,ArgType,ArgType,...);
The notation in functions.c says

    
    /*****************************************************************************
     * This is the table of procedures that register MOO built-in functions.  To
     * add new built-in functions to the server, add to the list below the name of
     * a C function that will register your new MOO built-ins; your C function will
     * be called exactly once, during server initialization.  Also add a
     * declaration of that C function to `bf_register.h' and add the necessary .c
     * files to the `CSRCS' line in the Makefile.
     ****************************************************************************/
    

Which is interesting, and I'll have to give it a try soon.

Conceivably, adding file io would not be necessarily very involved, once
the appropriate interfaces were defined. For adding destructive set 
maninpulations, all that would be necessary would be to figure out how to
dissasociate the previous copy of the variable from the reference counting
scheme, modify it, and then reintroduce it to the ref counters.



[0]: http://lions.cs.ndsu.nodak.edu/~vender/LambdaMOO/hacking.html