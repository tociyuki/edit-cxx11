line-oriented text editor
=========================

This is a kind of the ed (1) line-oriented text editor.

1. commands from traditional ed (1).
1. sam (1) text editor's line number form.
1. perl4 subset regular expression.
1. unlimited undo.

Invokes.

    $ make
    $ ./edit
    a
    Hello world.
    .
    w hello.txt
    q
    $ ./edit hello.txt
    p
    Hello world.
    q
    $
    
Line number form.

    .   line number of current line
    0   sentinel line number of top of buffer
    1   line number of first line
    $   line number of last line
    '   line number of marked line with k-command.

Relative line numbers.

    form +n  forward decimal n lines
    form -n  backword decimal n lines
    form +/pat/  forward search
    form -/pat/  backward search
    form +   same as +1
    form -   same as -1
    form n   same as +n
    form /pat/   same as +/pat/

If the form starts a relative one, it is assumed
that first term is current line

    /pat/   same as .+/pat/
    -/pat/  same as .-/pat/
    -       same as .-1
    +       same as .+1
    /foo/+6-/bar/++//-     same as .+/foo/+6-/bar/+1+/bar/-1

Commands.

    (.)  a          append text after the line
    (.,.)c          change lines
    (.,.)dp         erase lines
         e (file)   erase buffer and read from file
         f (file)   display current file name
    (1,$)g/pat/command  global command
    (.)  i          insert text before the line
    (.,.+1)j/str/   join lines with separator str
    (.)  k          mark line
    (.,.)m(.)p      move lines
    (.,.)n          display lines with line number
    (.,.)p          display lines
         q          quit
    ($)  r (file)   read from file and append it
    (.,.)s/pat/new/gp  substitute
    (.,.)t(.)p      copy lines
         u          undo
    (1,$)v/pat/command  negative global command
    (1,$)w (file)   write to file
    ($)  =p         display line number
    (.+1)\n         display one line

Regular expressions.

    ^               match top of line
    $               match end of line
    .               match any character without newline
    (re)            group
    (?:re)          uncaptured sub-expression
    re|re|re        alternative
    re re re        sequence
    re?             zero or one
    re*             zero or more times
    re+             one or more times
    [a-z]           character class
    [^a-z]          negative character class
    \d              [0-9]
    \D              [^0-9]
    \s              [ \t]
    \S              [^ \t]
    \w              [0-9A-Za-z_] (and U+0080 to U+10FFFF)
    \W              [^0-9A-Za-z_] (7 bit ASCII only)
    \b              word boundary
    \B              negative word boundary
    \t              tab
    \n              newline
    a               character itself

Replacements.

    s/(\w+)(\s+)(\w+)/$3$2$1/   word swap group variables
    1,$s/push/$&_back/g         not & but $& (or $0)
    s{\s+}{\n}g                 split lines

License: The BSD 3-Clause
=========================

Copyright (c) 2016, MIZUTANI Tociyuki  
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
