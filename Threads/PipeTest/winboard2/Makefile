OS=NT
ENV=WIN32
CPU=i386

!include <$(OS)$(ENV).MAK>

# Use up to date help compiler
hc="c:\program files\help workshop\hcrtf.exe" -xn

# Comment out both to turn on debugging symbols #######
#cdebug=
#linkdebug=
#######################################################

proj = winboard
allobj = winboard.obj backend.obj parser.obj moves.obj lists.obj \
	 gamelist.obj pgntags.obj wedittags.obj wgamelist.obj zippy.obj \
         wsockerr.obj

cvars = $(cvars) -I. -DWINVER=0x0400
cflags = $(cflags) /FR

all: $(proj).exe

# Update the help file if necessary
$(proj).hlp : $(proj).rtf
    $(hc) $(proj).hpj
    cat $(proj).err

# Update the resource if necessary
$(proj).rbj: $(proj).rc $(proj).h $(proj).res resource.h
    $(rc) $(rcvars) -r -fo $(proj).res $(cvars) $(proj).rc
    cvtres -$(CPU) $(proj).res -o $(proj).rbj

# Update the object files if necessary
winboard.obj: winboard.c config.h winboard.h common.h frontend.h backend.h \
	moves.h wgamelist.h defaults.h resource.h
    $(cc) $(cflags) $(cvars) $(cdebug) winboard.c

backend.obj: backend.c config.h frontend.h backend.h common.h parser.h
    $(cc) $(cflags) $(cvars) $(cdebug) backend.c

parser.obj: parser.c config.h common.h backend.h parser.h
    $(cc) $(cflags) $(cvars) $(cdebug) parser.c

parser.c: parser.l
    flex -L parser.l
    del parser.c
    rename lex.yy.c parser.c

moves.obj: moves.c config.h backend.h common.h parser.h moves.h
    $(cc) $(cflags) $(cvars) $(cdebug) moves.c

lists.obj: lists.c config.h lists.h common.h
    $(cc) $(cflags) $(cvars) $(cdebug) lists.c

gamelist.obj: gamelist.c config.h lists.h common.h frontend.h backend.h \
	parser.h
    $(cc) $(cflags) $(cvars) $(cdebug) gamelist.c

pgntags.obj: pgntags.c config.h common.h frontend.h backend.h parser.h lists.h
    $(cc) $(cflags) $(cvars) $(cdebug) pgntags.c

wedittags.obj: wedittags.c config.h common.h winboard.h frontend.h backend.h
    $(cc) $(cflags) $(cvars) $(cdebug) wedittags.c

wgamelist.obj: wgamelist.c config.h. common.h winboard.h frontend.h backend.h \
	wgamelist.h
    $(cc) $(cflags) $(cvars) $(cdebug) wgamelist.c

wsockerr.obj: wsockerr.c wsockerr.h
    $(cc) $(cflags) $(cvars) $(cdebug) wsockerr.c

zippy.obj: zippy.c config.h common.h zippy.h frontend.h
    $(cc) $(cflags) $(cvars) $(cdebug) zippy.c

$(proj).exe: $(allobj) $(proj).rbj $(proj).def $(proj).hlp $(proj).rc
    $(link) $(linkdebug) $(guiflags) $(allobj) \
	wsock32.lib comctl32.lib winmm.lib libc.lib oldnames.lib kernel32.lib \
	advapi32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib \
	$(proj).rbj -out:$(proj).exe
	bscmake *.sbr

test.exe: test.c
	$(cc) $(cflags) $(cvars) $(cdebug) test.c
	$(link) $(linkdebug) $(conflags) test.obj $(conlibs) -out:test.exe

