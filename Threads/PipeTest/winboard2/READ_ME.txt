READ_ME.txt

WinBoard is a graphical chessboard for Windows 95
and Windows NT.  It can serve as a user interface for
GNU Chess or for the Internet Chess Server, or can be
used to play out games manually or from game files.
For full details, see the Help file (WinBoard.hlp).
For answers for frequently asked questions, see the
FAQ file (FAQ.html).

WinBoard is free software.  You can redistribute it
and/or modify it under the terms described in the
files COPYRIGHT and COPYING.  If your distribution
does not include the source code, you can get it from
the author's Web page:

 http://www.research.digital.com/SRC/personal/Tim_Mann/

GNU CHESS

A version of GNU Chess is included in the WinBoard
distribution.  WinBoard starts up GNU Chess
automatically when you run it in GNU Chess mode. You
can also run GNU Chess manually from the command line
if you like. This gives you access to some features
of GNU Chess that WinBoard does not provide a
graphical interface for.  See the file gnuchess.txt
for documentation.  You can use either gnuchesx.exe
or gnuchesr.exe for this purpose, but gnuchesr.exe
has a more verbose, slightly friendlier command-line
interface.  Source code for GNU Chess is available
from the Free Software Foundation:

 http://www.gnu.ai.mit.edu/software/software.html

BUGS

Report bugs in WinBoard, XBoard, or GNU Chess to
<bug-gnu-chess@prep.ai.mit.edu>.  Give the program's
version number (WinBoard: Help About; GNU Chess:
gnuchesx -v).  If you improve WinBoard or XBoard,
please send a message about your changes to
bug-gnu-chess, and someone will get in touch with you
about merging them in to the main line of development.

HISTORY

WinBoard is a port of the Unix program XBoard to
Win32, the 32-bit API of Windows NT and Windows 95.
The graphical front end (WinBoard.c, etc.) is all
new, but the back end that understands chess rules,
chess notation, GNU Chess, the ICS, etc., is shared
with XBoard.  See WinBoard.hlp for a list of
contributors to WinBoard and XBoard.

CAVEATS

WinBoard does *not* run on Windows 3.1 or 3.11.  It
works only on Windows NT and Windows 95/98.  Porting
it to 3.1/3.11 would be a huge task. The Win32s 
subset interface is not sufficient to run WinBoard.

The WinBoard source distribution contains some source
files with long names (more than 8.3), but the files
that are actually needed at runtime are now 8.3 or
less, so the software should work when run from an
old-style FAT file system.

MANUAL INSTALLATION

For a minimal installation of WinBoard and its
companion version of GNU Chess, create a new
directory and copy the following files to it:

WinBoard.exe  (WinBoard program)
WinBoard.hlp  (WinBoard documentation)
gnuchesx.exe  (GNU Chess engine used by WinBoard)
gnuchesr.exe  (Command-line GNU Chess engine)
gnuchess.lan  (GNU Chess messages file)
gnuchess.dat  (GNU Chess opening book)
gnuchess.txt  (GNU Chess documentation)

Use the Program Manager or the Explorer to create an
icon for WinBoard.exe, and set its working directory
to the directory you created.  Besides looking for
its companion files in this directory, WinBoard also
loads and saves game, position, and setting files
there.
