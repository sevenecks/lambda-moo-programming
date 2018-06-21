# Lambda MOO Programming

Table of Contents
=================

* [Introduction](#introduction)
* [Local Setup](#local-setup)
* [Contents](#contents) 
    * [Lambda MOO Programmer's Manual \[Updated\]](#lambda-moo-programmers-manual-updated)
    * [Learn MOO Programming Videos](#learn-moo-programming-videos)
    * [Yib's Pet Rock](#yibs-pet-rock)
    * [LambdaMOO Programming Tutorial (Steven Owens)](#lambdamoo-programming-tutorial-steven-owens)
    * [Colin's Way Easy intro Guide to MOO Programming](#colins-way-easy-intro-guide-to-moo-programming)
    * [Winding Duck](#winding-duck)
    * [MOO Progrmaming Tips](#moo-programming-tips)
    * [Hacking LambdaMOO Server Nodak](#hacking-lambdamoo-server-nodak)
    * [Lambda MOO Background Information Nodak](#lambda-moo-background-information-nodak)
    * [Lambda MOO Programming Nodak](#lambda-moo-programming-nodak)
    * [How to set up a LambdaMOO server on GenesisMud](#how-to-set-up-a-lambdamoo-server-on-genesismud)
    * [Getting Started With moo.el](#getting-started-with-moo-el)

* [Server Patches](#server-patches)
* [Code](#code)
* [Web Client](#web-client)
    * [Public Web Client](#public-web-client)
* [Notes](#notes)
* [ToDo](#todo)
* [Contributing](#contributing)
* [Author](#author)

## Introduction
The primary purpose of this repository is to aggregate and update the existing LambdaMOO Programming Resources that exist. The core of the work done here has been to convert the original MOO Programmer's Manual written by Pavel Curtis to HTML5/Bootstrap and to update each section of the manual with additional information.

There are a number of other files included in this repository. Many are scraped from various websites that continue to exist, somehow, to this day. Most are at least 15 years old. I have included them here for posterity, since losing them would be tragic.

## Local Setup
```bash
git clone git@github.com:SevenEcks/the-new-moo-programming-guide.git
```

### Markdown Conversion

We use the [to-markdown-cli](https://github.com/fabianmoronzirfas/to-markdown-cli) library to convert HTML files to markdown on the command line. It can be installed using NPM.

```bash
npm install -g to-markdown-cli
html2md -i ./foo.html -o out.md <= output to out.md
```

## Contents
There are two versions of each document in this repository. One is a markdown copy located in [/tutorials](/tutorials) which is accessible on GitHub and one is an HTML version located in [/tutorials/src](/tutorials/src).


### Lambda MOO Programmer's Manual [Updated]
This LambdaMOO Programmer's Manual is an **updated an expanded version** of the *LambdaMOO Programmer's Manual* originally written by Pavel Curtis. Some pretty extensive work has gone into updating it.

#### On The Web
This file is important, and thus, it is available online: [MOO Programmers Manual](http://www.sindome.org/moo-manual.html)

#### On GitHub
To use this guide you simply need to open [MOO Programmer's Manual](/tutorials/moo-programmers-manual-updated.md). This version of the MOO Programmer's Manual lacks much of the formatting that the HTML5 version below has. It is great for quick perusal, but the HTML version is superior. 

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/moo-programmers-manual-updated.html](/tutorials/src/moo-programmers-manual-updated.html) in your web browser.

### Learn MOO Programming Videos
This is a series of videos on installing and learning MOO programming.

1. [Installing and Compiling LambdaMOO from Source Code](https://www.youtube.com/watch?v=CmwWF6Hm4iE&t=&index=1&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

2. [Using LambdaMOO DB and Applying Server Patches](https://www.youtube.com/watch?v=IyxPQFn0kG4&t=&index=2&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

3. [The Basics of LambdaMOO Programming](https://www.youtube.com/watch?v=kj6ABbvnybI&t=1s&index=3&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

4. [Create your own Debugger in MOO](https://www.youtube.com/watch?v=s0-cBEpW7JM&t=2s&index=4&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

5. [Intro to Properties on Object (1/3)](https://www.youtube.com/watch?v=QBnXvtR1qBw&t=0s&index=5&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

6. [Properties on Objects Continued (2/3)](https://www.youtube.com/watch?v=Lv5ZCjTqrjQ&t=0s&index=6&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

7. [Properties on Object Concluded (3/3)](https://www.youtube.com/watch?v=HYOsog_og-o&t=0s&index=7&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

8. [Extending the MOO with Custom Verbs](https://www.youtube.com/watch?v=yYoGHvmo7RU&t=0s&index=8&list=PLDRWME7vpHrrHmGJ8Va7GAIbkxg3BkT94)

### Yib's Pet Rock
A great first introduction to Lambda MOO programming.

#### On GitHub
To use this guide you simply need to open [Yib's Pet Rock](/tutorials/yibs-pet-rock.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/yibs-pet-rock-non-html5.html](/tutorials/src/yibs-pet-rock-non-html5.html) in your web browser.

### LambdaMOO Programming Tutorial (Steven Owens)
This is an excellent guide, the most complete one I've seen aside from the original Lambda MOO Programmer's Manual. It was written, as the title states, by Steven J. Owens. __This guide was updated 03/07/18__ after Steven sent over a fresh link to an updated version of the guide.

#### On GitHub
To use this guide you simply need to open [LambdaMOO Programming Tutorial by Steven Owens](/tutorials/lambda-moo-steven-owens-guide.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/dark-sleep-lambdamoo-programming-tutorial-non-html5.html](/tutorials/src/dark-sleep-lambdamoo-programming-tutorial-non-html5.html) in your web browser.

### Colin's Way Easy Intro Guide to MOO Programming
A good introduction to MOO programming. This guide gets you through all the basics.

#### On GitHub
To use this guide you simply need to open [Colin's Way Easy Intro Guide to MOO Programming](/tutorials/lambda-moo-way-easy.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/way-easy-moo-programming-guide-non-html5.html](/tutorials/src/way-easy-moo-programming-guide-non-html5.html) in your web browser.

### Winding Duck
Winding Duck MOO Programmer's Tutorial walks you through, step by step, how to create some simple code, then make it progressively more complicated.

#### On GitHub
To use this guide you simply need to open [Winding Duck](/tutorials/winding-duck.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/winding-duck-non-html5.html](/tutorials/src/winding-duck-non-html5.html) in your web browser.

### MOO Programming Tips
This looks like it was a guide created for a specific MOO but it has some tips that those new to MOO might find useful.

#### On GitHub
To use this guide you simply need to open [MOO Programming Tips](/tutorials/zompost-moo-help.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/zompost-moo-help-non-html5.html](/tutorials/src/zompost-moo-help-non-html5.html) in your web browser.

### Hacking LambdaMOO Server Nodak
This document walks you through what happens when a connection is made to the Lambda MOO server.

#### On GitHub
To use this guide you simply need to open [Hacking LambdaMOO Server](/tutorials/hacking-lambda-moo-server.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/hacking-lambda-moo-server-non-html5.html](/tutorials/src/hacking-lambda-moo-server-non-html5.html) in your web browser.

### Lambda MOO Background Information Nodak
This document contains background information on what LambdaMOO is in relation to other servers and languages.

#### On GitHub
To use this guide you simply need to open [Lambda MOO Background](/tutorials/lambda-moo-background.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/lambda-moo-background-non-html5.html](/tutorials/src/lambda-moo-background-non-html5.html) in your web browser.

### Lambda MOO Programming Nodak
This document contains information on Lambda MOO programming. It is a short and somewaht technical document without much background information.

#### On GitHub
To use this guide you simply need to open [Lambda MOO Nodak Edu](/tutorials/lambda-moo-nodak-edu.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/lambda-moo-programming-tutorial-nodak-edu-non-html5.html](/tutorials/src/lambda-moo-programming-tutorial-nodak-edu-non-html5.html) in your web browser.

### How to set up a LambdaMOO server on GenesisMud
This is a step by step guide by Puff the Fractal Dragon on how get LambdaMOO running on GenesisMud. However, for anyone looking to get a general idea of how to get LambdaMOO up and running, this tutorial provides a good reference for the process.

#### On GitHub
To use this guide you simply need to open [LambdaMOO on GenesisMud](/tutorials/genesismud.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/genesismud.txt](/tutorials/src/genesismud.txt) in your web browser.

### Gettings Started With moo.el
This tutorial by Puff the Fractal Dragon walks you through using emacs to edit MOO code. It also includes some generalized emacs information.

#### On GitHub
To use this guide you simply need to open [moo.el](/tutorials/mud_moo_el_tutorial.md).

#### Locally
To access this file locally you simply need clone the repository and open [/tutorials/src/mud_el_tutorial.txt](/tutorials/src/mud_el_tutorial.txt) in your web browser.

### Server Patches
I've been worried that we might lose some server patches to time. Also they aren't really gathered anywhere on GitHub, just on [LisDude MOO Resources](http://www.lisdude.com/moo/) and random source forge unmainted LambdaMOO repos. So I'm starting to collect them here. If you have any, throw them up in a pull request please!

* [FileIO 1.5p3](patches/fileio-1.5p3) (Todd Sundsted version)
* [Goblin List Utils](patches/goblin) (GammaMOO only I think)

### Code
There are some code samples for enterprising developers to check out. It's limited for now, please feel free to put a pull request in for any code you'd like included.

* [Local Editing](code/LocalEditing.md) (Edit in a pop up window with @edit using Dome-Client web client)

### Web Client
I've mentioned [https://github.com/JavaChilly/dome-client.js](Dome Client) in the updated MOO Programming Guide. It's a web based MOO client that uses NodeJS and web sockets. It works great, connecting to most LambdaMOO servers. You can run your own. 

#### Public Web Client
Sindome offers a [public web client](http://pubclient.sindome.org/) that can connect to whatever LambdaMOO server you want: http://pubclient.sindome.org/ 

It works great, and if you use the [Local Editing](code/LocalEditing.md) code in this repo, you can write an @edit verb which will let you edit code in a nice pop up window. It's excellent.

## Changes
See [CHANGELOG.md](CHANGELOG.md)

## Notes
See [NOTES.md](NOTES.md)

## TODO
See [TODO.md](TODO.md)

## Contributing
Please feel free to fork the repo and contribute via pull request. This is a labor of love. Give back to the community.  

## Author
[Sindome](https://www.sindome.org/) has been online since 1997. I've been playing it since 2003.  I've been developing in MOO since 2005, and a developer on Sindome since 2006. Ours is heavily modified, we have roughly 70-80 players online a lot of time time and dozens of people have developed the game over 20+ year run.  We've got a ton of best practices and design patterns that we implement in order to keep the MOO happy and healthy.  I've attempted to distill as much of that into this document while at the same time present the document in a more up to date format.

Text based gaming is an important part of our history and our future.  No game is more accessible to those with handicaps, for example.

Come visit me on Sindome if you dare :)

-- Brendan / Slither / Fengshui
