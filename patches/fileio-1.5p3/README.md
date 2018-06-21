# Info
This adds file input / output to the MOO. The typical patch is 1.5p1 but this is an updated version which makes it return an actual error E_FILE instead of "E_FILE" among other fixes.

## Disclaimer
I pulled this from a Github Gist by Todd Sundsted, the defacto maintainer of the FileIO extension. This version fixes some stuff. I had to hack this into my dev MOO manually because I already had fileio installed and so the patch wouldn't work. It's definitely possible to get it working with some manual patch work if you already have the existing patch. 

Some of it is just formatting changes but there is some stuff in versions.h or versions.c that I missed at first because I thought it was just a comment. Don't make that mistake.

Original: https://gist.githubusercontent.com/toddsundsted/1126787/raw/99676d66a1ceacf4fade389d5c10c1f52393f44d/fileio-1.5p3.patch
