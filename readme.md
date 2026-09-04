This is the String Cheese language, or the StrC language (clever eh?).
The syntax is pretty simple right now, but there isn't much more going on because I've hit a wall.
Defining multiple variables, or maybe rather multiple strings (not sure which) causes the program
to throw bad_alloc, and I have no idea why.

A few notes:
Most of the important code will be in the Lexer.cpp and Lexer.h files.
The logger class is not developed yet.

Updates on findings:
Strings do not like to be ended without a non letter character, it's so weird.