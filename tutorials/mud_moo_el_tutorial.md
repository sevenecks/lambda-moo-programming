# Getting started with moo.el

__by Puff the Fractal Dragon__

This should get you started with the moo.el client, an ELISP module for GNU EMACS.  Moo.el is extremely useful, particularly if you're adept with GNU EMACS, but this help file is aimed at somebody who's never even used GNU EMACS before, so DON'T PANIC (kudos to Douglas Adams).

__NOTE__: Many players have contacted me asking where they can get a copy of mud.el or moo.el.  I never discussed this because the help object is intended to be used by those who already HAVE moo.el but don't know how to use it.  JoeFeedback is the creator and sole distributor of moo.el.  He requests that users do NOT distribute it, as he wishes to track its use and testing for purposes of further enhancements.

## A Note About Terminology

Ctrl-Anything
     ...means hold down the Control key and press another key.  

Esc Anything
     ...means press the Escape key (or meta key if you're not on an MS-DOS compatible keyboard), let it up, THEN press another key.  

Esc x blah-blah  or  Ctrl-X blah-blah
     Often I'll use this where blah-blah is a set of commands.  You do either Esc x or Ctrl-x and wait a moment, and either "M-x" or "C-x" will appear on the bottom line of your screen.  Start to type blah-blah.  

Tab  and  ?
     While typing blah-blah, if you press Tab, EMACS will try to complete the command for you.  If you press Tab twice, or press ?, EMACS will list the possible completions of the command.  You can continue typing and using Tab until you get the entire command, then press [enter].

## Starting up

     Assuming you have GNU EMACS on your system, you start it up by entering "EMACS [enter]" from the prompt.  If you have more experience with EMACS you can set it up so it automatically loads moo.el, but for now, use:

Esc x load-file [enter] moo.el [enter] 

## Connecting

     Connect to a mud with:

Esc x mud [enter] mudname [enter].  

        If you don't know the mud's name, you can use completion, or type ?  to get a list of the muds in your .moo_worlds file.

## The .moo_worlds file

If you want to add new muds that aren't in the list, you have to edit the .moo_worlds file (you should have gotten a basic .moo_worlds file when you got moo.el).  To add new muds to your .moo_worlds file, you can either edit it directly, or use:

```
Esc x mud-add-world [enter]
World name: LambdaMOO [enter]
Login name: player's name &lt;I suggest you leave this blank&gt; [enter]
Password: player's password &lt;I suggest you leave this blank&gt; [enter]
Address: lambda.parc.xerox.com [enter]
Port number: 8888 [enter]
Type: MOO [enter]
```

You may find it handy to make more than one entry for your favorite muds.  You can connect to two different characters at once, but only if you have two different entries for that mud in .moo_worlds.  I use LambdaMOO and lambda, for example.  If I tried to use LambdaMOO twice, moo.el would just switch me back to the LambdaMOO buffer (more about buffers later).

## Fun with moo.el for MOO

 Once you've connected, there are several ways moo.el can make your life easier, particularly if you're connecting to a MOO.  The ones I use most often are multiple buffers, multiple windows, verb and property editing, and general nifty things you can do with your buffer when you can edit the whole thing with EMACS commands.

## Multiple Buffers

Like I said up above about .moo_worlds, you can connect to a mud more than once, using multiple buffers.  With multiple buffers you can hop from one mud connection to another and work on two or more muds, or edit files, and so on.  Buffers are part of EMACS, and are generally nifty things that you can learn more about with the EMACS manual.  One thing to keep in mind is that the buffer sticks around until you kill it - out of sight does NOT mean out of mind.  

## Switching Between Buffers

You switch between buffers using: 

```
Ctrl-x b 
Switch to buffer: (default: blahblah) [enter]
```

If you just hit enter you'll go to the default buffer.  The default buffer is the last buffer you were in, in this window (more about windows next).  For just two buffers, this is fine, you can do Ctrl-x b forever and keep going back and forth.  If you have more than one buffer, you can type part of the buffer name and use command completion or ? to see the other buffer names. 

## Opening New Buffers

You can also open new buffers to edit text or whatever, simply by switching to a buffer that isn't there.

```
Ctrl-x b 
Switch to buffer: newbuffername [enter]
```

## Killing Buffers

Generally I don't close buffers, I just @quit, or save the file I was editing, and ignore it until I'm ready to kill the whole EMACS.  But if you want to or need to make it go away, you can use:

```
Esc x kill-buffer [enter]
Kill buffer: (default blahblah)
```

You can specify which buffer to kill the same way you specify which buffer to switch to.

## Uploading Buffers

You can send the contents of a buffer to the mud with the command:

```
Esc x mud-upload-buffer [enter]
```

EMACS will ask you for the buffer to upload, and the mud to upload it to.  You can also use 

## Quitting from a MUD

If you want to @quit from a mud, then reconnect (to connect to another character, for example), I've found you have to @quit, wait a few seconds until it says:
```
|*** Disconnected ***
```

then press [enter] a couple of times.  It'll say:

```
Process MUD<1> exited abnormally with code 256
```

And now you can do:

```
Esc x mud [enter] mudname [enter] 
```

and reconnect.

## Multiple Windows

Multiple windows are like multiple buffers (in fact, they usually ARE multiple buffers), only they're on the same 24 line screen.  Right now,as I type this, I have about 10 lines for this document on the top half of my screen, and the bottom half contains two windows, one for a LambdaMOO session and one for a GodNet session.

## How Many Windows Is Too Many?

You can keep opening more windows until all you have left on your screen is window bars (solid light bars that separate the windows).  For the most part, two windows is optimal to work with, three if two of them aren't too busy, and four is pushing it.  If I have to keep track of more buffers than this, I generally switch buffers in and out.

## The Windows Commands

The windows commands are generally:

```
Ctrl-x 2  separate the present window into two windows.
Ctrl-x 1  shut down all of the windows except the one the cursor is in.
Ctrl-x o  hop to the next window.
Ctrl-x ^  (^ is shift-6 on most keyboards) expand the window you're in by
          1 line.  This one is tricky, it takes a line away the "closest"
          window, a not-always-predictable choice.
```

## Verb and Property Editing

The MOO verb and property editor functions in moo.el automagically open up windows to edit verbs and properties (aka fields):

```
Esc x moo-get-verb #object:verbname [enter]
    and
Esc x moo-get-field #object.propertyname [enter]
```

These commands also pull a copy of the verb or property contents into the buffer.  Once you've finished editing the verb or property, you can send it back to the MOO with:

```
Esc x mud-macro-send [enter]
```

## Editing Long List Properties

A property is basically one line, often an extremely long line.  If you're editing a property that's a list of elements or strings, separated by commas, there are useful commands for breaking the list up into separate lines for editing and recombining them into a list for sending:

```
Esc x moo-explode-list [enter]
Esc x moo-implode-list [enter]
```

If the list is a list of strings, with quotes (") at either end, use:

```
Esc x moo-explode-message [enter]
Esc x moo-implode-message [enter]
```

This will strip off the quotes until you're ready to implode the message.

## Quoting text 

It's often very useful to clip out text and send it to a mud, to "quote" it to other players who are listening, or to send a long series of commands, or even to send a mail message inside MOO.  Once you've put together the text you're going to send, clip or copy it into the kill ring (see below for details on doing this) then use:

```
Esc x mud-send-kill [enter]
```

This sends the text verbatim.  If you wanted to put a command in front of every line, for example you wanted to "say" or "emote" every line, you could use:

```
Esc x mud-send-kill-prefix [enter]
Prefix: say [enter]
Suffix: [enter]
```

## @quicksending mail

If you're like me, you don't like using Lambda's mail editor (nice though it is, and kudos to Rog for coding it) to edit your mail.  You can edit it in another buffer, place "@quicksend recipient-name" at the beginning and "." at the ending, and use x mud-upload-buffer or mud-send-kill to send it.

## Nifty Things EMACS can do

EMACS is a very powerful and useful editor.  I even use an EMACS clone (JOVE - Jonathan's Own Version of EMACS) on my PC.  To really learn about it you should dig up a GNU EMACS manual or file or try 'learn EMACS' or te EMACS help function (Esc x help-for-help).  GNU EMACS (in my opinion the best version, although it devours  UP CPU and is a real memory hog) is available free on the Internet.  You can FTP the documentation files from numerous archive sites (wuarchive.wustl.edu is my favorite site).

## Quitting EMACS

Methods for quitting from EMACS vary depending on how the version in use is set up.  Ctrl-c c, ctrl-x c, ctrl-c ctrl-c, etc, often do the job.  When I can't do it any other way, this almost always works:

```
Esc x kill-emacs [enter]
```

## Logging 

If you decide you want to save a log of a mud session, do:

```
Ctrl-x Ctrl-w
Write file: ~/filename
```

## Moving Around

Sometimes you can use the keyboard arrow keys to move around, sometimes not.  When you can't, remember these commands:

```
Ctrl-f  forward one space.
Ctrl-b  backwards one space.
Ctrl-p  previous line.
Ctrl-n  next line.
Ctrl-a  beginning of line.
Ctrl-e  end of line.
Ctrl-d  deletes a character the cursor is on.
```

The mud session takes any line you press [enter] on as input.  So if you mistype something, you can easily use Ctrl-p to go back up, Ctrl-b or Ctrl-f to get to your mistake, and Ctrl-d or backspace to fix it, then press [enter] and send it again.  When I want to repeat an action many times however, I kill it and yank it back multiple times (see cutting and pasting, below).

## Macro-movement Commands

There are also some useful commands for moving around the buffer by large steps:

```
Ctrl-v  move down one screenfull.
Esc v   move up one screenfull.
Esc <   move to beginning of file.
Esc >   move to end of file.
```

## Cutting, Pasting, Etc

A couple of basic concepts;  mark, region, kill, kill ring.  

The mark is just that, a mark.  You usually can't see it.  Sometimes it's represented by a highlighted space, sometimes not.  The Ctrl key to set it varies from terminal to terminal, so I just remeber Esc x set-mark and I have no problems.

The region is everything between your cursor and wherever the mark was last set.  Everything.  This is very useful for deleting or changing large areas of text, but can be dangerous if you forget where the mark last was.

Killing a line means deleting from the cursor to the end of the line.  To really delete the whole line you have to do it twice - once to kill the text, another time to kill the empty space.  Ctrl-k usually does kill this.  You can also kill an entire region with Ctrl-w.  This is something you have to be careful about, or you could end up killing large regions you wanted to keep.  Fortunately you have the kill ring.

Killed text all goes into the kill-ring.  You can "yank" back all of the text that you last killed in one "contiguous" batch with Ctrl-y.  This means that if you kill five lines in a row by pressing Ctrl-k ten times, Ctrl-y will yank them all back.  But if you press Ctrl-k ten times, then use Ctrl-n to move somewhere else and do Ctrl-k four more limes, you're only going to be able to yank back the last batch (the four lines) you killed.

If you don't want to kill anything, you can just set the mark, move to the end of the area you want, and do Esc x copy-region to copy those lines into the kill ring just as if you'd killed them.

There are commands for moving around the kill ring;  check EMACS help or your EMACS manual.

```
Esc x set-mark     set the mark.
Ctrl-k             kill to end of line.
Ctrl-w             kill region.
Ctrl-y             yank back last contiguous kill.
Esc x copy-region  copy region into kill-ring.
```

## Mud- commands

```
mud-add-world                      mud-beginning-of-line
mud-cancel-input                   mud-cleanup-extra-processes
mud-get-text                       mud-interactive-mode
mud-load-macro-commands            mud-load-worlds
mud-macro-abort                    mud-macro-command
mud-macro-expansion-mode           mud-macro-label
mud-macro-send                     mud-macro-send-and-destroy
mud-macro-send-slowly              mud-mode
mud-next-command                   mud-output-here
mud-parse-entry-file               mud-previous-command
mud-previous-matching-command      mud-quit
mud-realign-and-send               mud-recover-last-macro
mud-remove-macro                   mud-retarget
mud-send                           mud-send-kill
mud-send-kill-prefix               mud-shell-buffer
mud-store-macro-commands           mud-upload-buffer
mud-upload-file                    mud-upload-file-as-lambda-mail
mud-upload-file-straight
```

## This file was modified from it's original to include updated markdown formatting. The original can be found in the /src directory
