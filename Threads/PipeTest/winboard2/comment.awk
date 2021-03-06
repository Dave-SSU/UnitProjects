#! /usr/bin/awk -f
#
# Preprocessor for chess games that are formatted as blocks of moves
#  interspersed with blocks of text, separated by blank lines.  Finds
#  the blocks of text and encloses them with "{ }" brackets so that
#  xboard's game parser can identify them as comments.  Many games are
#  posted to rec.games.chess in this format.
#
# Limitation:  Fails if a text block starts with a numbered move.  In
#  that case you'll have to hand-edit the output.
#
BEGIN {
    inmoves = 0;
    blankline = 1;
}

{
    if (NF == 0) {
	blankline = 1;
    } else if (blankline) {
	blankline = 0;
	if ($1 ~ /^[0-9]+/) {
	    if (!inmoves) {
		inmoves = 1;
		print "}";
	    } else {
		print "";
	    }
	} else {
	    if (inmoves) {
		inmoves = 0;
		print "{";
	    } else {
		print "}{";
	    }
	}
	print $0;
    } else {
	print $0;
    }
}

