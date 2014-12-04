ircombine 1.0
=============
ircombine 1.0 - a log merger for irssi. This program reads in two
irssi logfiles, and sorts them chronologically. This is useful 
when, for example, you have two logfiles for the same person, 
separated by an underscore: User and User_, or just somebody who 
has a variety of names! It requires at least two arguments, the
names of the two logfiles that you want to read, with an optional
command to output the resultant log to a filename of your choosing
(otherwise it'll default to ircombineOutput.log, and that's a really
boring file name, not to mention a pretty useless one, so don't let
this happen to you!

This program is meant to compile with minimum dependencies; everything
needed to run it should be found in the standard libraries and the
included "logfile.h" header. Note that this program does require C++11
to compile, otherwise you'll likely get an error stating that stoi() is
not a member of std. This program has been tested with MinGW 4.8 and
g++ 4.9.1. Let me know if you face any compilation issues, especially
if you know how they might be solved!

Problems with this program are, as mentioned, the fact that it doesn't take
into account the possibility of custom irssi logs that don't use the same
format that I do. I'm planning to fix this in later versions, I promise!
Of course, if you have the inclination to do this yourself (and I don't
doubt that there are many who could do a better job than I have) go ahead!
There may also be issues with very large log files, edge cases definitely 
need to be tested. This will, again, be addressed in future versions, hopefully.

This software doesn't come with any kind of license; do with it what you will.
I would ask that you strive to only use it for the betterment of others, and
without any malice, but you're still free to use it if you can't satisfy those
conditions. If you somehow manage to find a way to profit off of this software,
congratulations! You aren't beholden to share your profits with me, or to even
mention the use of this little piece of code in your project, although it would
be appreciated. You are also free to modify this code however you please to suit
your needs; in fact, I encourage you to do so! If you do make any improvements,
please let me know! I welcome the opportunity to learn!
