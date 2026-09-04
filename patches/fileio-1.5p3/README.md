# FileIO 1.5p3

This patch adds file input/output support to the MOO server. FileIO 1.5p1 is the more
common version; this update returns the actual `E_FILE` error instead of the string
`"E_FILE"`, among other fixes.

## Compatibility Note

This copy came from a [GitHub Gist by Todd Sundsted](https://gist.githubusercontent.com/toddsundsted/1126787/raw/99676d66a1ceacf4fade389d5c10c1f52393f44d/fileio-1.5p3.patch),
the de facto maintainer of the FileIO extension. Applying it to a source tree that
already contains an earlier FileIO patch may require manual conflict resolution.

Some hunks look like formatting or comment changes but include important updates to
`versions.h` or `versions.c`; review the entire patch before adapting it.
