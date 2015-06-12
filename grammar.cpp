#include <string>
#include <vector>
#include <algorithm>
#include "edit.hpp"

grammar_type::grammar_type () : lastpattern () {}

bool
grammar_type::get (std::wstring::const_iterator& s, command_type& ct)
{
    static const struct {
        int cmd, a1, a2, a3, re, pr, g, p;
    } tbl[21] = {
        {'a',  2, 2, 0, 0, 0, 0, 0}, // meanings of values at a1 a2 a3:
        {'c',  2, 2, 0, 0, 0, 0, 0}, //   see setaddrdefault ()
        {'d',  2, 2, 0, 0, 0, 0, 1},
        {'e',  0, 0, 0, 0, 2, 0, 0},
        {'f',  0, 0, 0, 0, 2, 0, 0},
        {'g',  1, 4, 0, 1, 0, 0, 0},
        {'i',  2, 2, 0, 0, 0, 0, 0},
        {'j',  2, 3, 0, 0, 1, 0, 1},
        {'k',  2, 2, 0, 0, 0, 0, 0},
        {'m',  2, 2, 2, 0, 0, 0, 1},
        {'n',  2, 2, 0, 0, 0, 0, 0},
        {'p',  2, 2, 0, 0, 0, 0, 0},
        {'q',  0, 0, 0, 0, 0, 0, 0},
        {'r',  1, 4, 0, 0, 2, 0, 0},
        {'s',  2, 2, 0, 1, 3, 1, 1},
        {'t',  2, 2, 2, 0, 0, 0, 1},
        {'u',  0, 0, 0, 0, 0, 0, 0},
        {'v',  1, 4, 0, 1, 0, 0, 0},
        {'w',  1, 4, 0, 0, 2, 0, 0},
        {'=',  1, 4, 0, 0, 0, 0, 1},
        {'\n', 3, 3, 0, 0, 0, 0, 0}};

    ct.pflag = false;
    ct.gflag = false;
    ct.param.clear ();
    if (! getlist (s, ct))
        return false;
    s = skipspace (s);
    int idx = -1;
    for (std::size_t i = 0; i < sizeof (tbl) / sizeof (tbl[0]); ++i)
        if (*s == tbl[i].cmd) {
            idx = i;
            break;
        }
    if (idx < 0)
        return false;
    ct.command = *s;
    if (0 == ct.naddr) {
        setaddrdefault (ct.addr1, tbl[idx].a1);
        setaddrdefault (ct.addr2, tbl[idx].a2);
    }
    else if (tbl[idx].a1 == 0)
        return false;
    if ('\n' != *s)
        s = skipspace (s + 1);
    if (tbl[idx].a3 && ! getaddr3 (s, ct))
        return false;
    if (tbl[idx].re && ! getre (s, ct))
        return false;
    if (1 == tbl[idx].pr && ! getparam (s, ct))
        return false;
    if (2 == tbl[idx].pr && ! getfile (s, ct))
        return false;
    if (3 == tbl[idx].pr && ! getreplacement (s, ct))
        return false;
    if (tbl[idx].g  && ! getgflag (s, ct))
        return false;
    if (tbl[idx].p  && ! getpflag (s, ct))
        return false;
    if ('g' == ct.command || 'v' == ct.command)
        return true;
    s = skipspace (s);
    return '\n' == *s;
}

bool
grammar_type::setaddrdefault (std::vector<addr_type>& addr, int const kind)
{
    if (1 == kind)  // 1
        addr.push_back ({LINENUM, 1, L""});
    if (2 == kind || 3 == kind) // . || .+1
        addr.push_back ({DOT, 0, L""});
    if (3 == kind)  // .+1
        addr.push_back ({LINEOFFSET, 1, L""});
    if (4 == kind)  // $
        addr.push_back ({DOLLAR, 0, L""});
    return true;
}

bool
grammar_type::getaddr3 (std::wstring::const_iterator& s, command_type& ct)
{
    s = skipspace (s);
    if (! isaddrfirst (*s)) {
        ct.addr3.push_back ({DOT, 0, L""});
        return true;
    }
    return getaddr (s, ct.addr3);
}

bool
grammar_type::getre (std::wstring::const_iterator& s, command_type& ct)
{
    static const std::wstring refirst (L"%|:{}<>?/");
    s = skipspace (s);
    if (refirst.find (*s) == std::wstring::npos)
        return false;
    std::wstring::const_iterator s1 = s;
    if ((s = skippattern (s1)) == s1)
        return false;
    if (s - s1 > 2)
        lastpattern.assign (s1 + 1, s - 1);
    ct.pattern = lastpattern;
    return true;
}

bool
grammar_type::getreplacement (std::wstring::const_iterator& s, command_type& ct)
{
    static const std::wstring refirst (L"%|:{}<>?/");
    if ('}' == s[-1] || '>' == s[-1])
        s = skipspace (s);
    else
        --s;
    if (refirst.find (*s) == std::wstring::npos)
        return false;
    std::wstring::const_iterator s1 = s;
    if ((s = skippattern (s1)) == s1)
        return false;
    ct.param.assign (s1 + 1, s - 1);
    return true;
}

bool
grammar_type::getparam (std::wstring::const_iterator& s, command_type& ct)
{
    ct.param.clear ();
    s = skipspace (s);
    static const std::wstring paramfirst (L"%|:{}<>?/\"'");
    if (paramfirst.find (*s) == std::wstring::npos)
        return true;
    std::wstring::const_iterator s1 = s;
    if ((s = skippattern (s1)) != s1)
        ct.param.assign (s1 + 1, s - 1);
    return true;
}

bool
grammar_type::getfile (std::wstring::const_iterator& s, command_type& ct)
{
    s = skipspace (s);
    std::wstring::const_iterator s1 = s;
    while (' ' < *s && 0x7f != *s)
        ++s;
    ct.param.assign (s1, s);
    return true;
}

bool
grammar_type::getgflag (std::wstring::const_iterator& s, command_type& ct)
{
    s = skipspace (s);
    ct.gflag = 'g' == *s;
    if ('g' == *s)
        ++s;
    return true;
}

bool
grammar_type::getpflag (std::wstring::const_iterator& s, command_type& ct)
{
    s = skipspace (s);
    ct.pflag = 'p' == *s;
    if ('p' == *s)
        ++s;
    return true;
}

bool
grammar_type::isaddrfirst (int c)
{
    static const std::wstring s (L"0123456789+-/.$'");
    return s.find (c) != std::wstring::npos;
}

bool
grammar_type::istermfirst (int c)
{
    static const std::wstring s (L"0123456789+-/");
    return s.find (c) != std::wstring::npos;
}

bool
grammar_type::getlist (std::wstring::const_iterator& s, command_type& ct)
{
    ct.naddr = 0;
    ct.addr1.clear ();
    ct.comma = ' ';
    ct.addr2.clear ();
    s = skipspace (s);
    if (',' == *s)
        ct.addr1.push_back ({LINENUM, 1, L""});
    else if (';' == *s)
        ct.addr1.push_back ({DOT, 0, L""});
    else if (! isaddrfirst (*s))
        return true;
    else if (! getaddr (s, ct.addr1))
        return false;
    ++ct.naddr;
    s = skipspace (s);
    if (',' == *s || ';' == *s) {
        ct.comma = *s;
        s = skipspace (s + 1);
        if (! isaddrfirst (*s))
            ct.addr2.push_back ({DOLLAR, 0, L""});
        else if (! getaddr (s, ct.addr2))
            return false;
        ++ct.naddr;
    }
    return true;
}

bool
grammar_type::getaddr (std::wstring::const_iterator& s, std::vector<addr_type>& addr)
{
    s = skipspace (s);
    if (! isaddrfirst (*s))
        return false;
    getbase (s, addr);
    while (istermfirst (*s)) {
        if (! getterm (s, addr))
            return false;
        s = skipspace (s);
    }
    return true;
}

bool
grammar_type::getbase (std::wstring::const_iterator& s, std::vector<addr_type>& addr)
{
    if ('0' <= *s && *s <= '9')
        addr.push_back ({LINENUM, scandecimal (s), L""});
    else if ('$' == *s || '\'' == *s)
        addr.push_back ({('$' == *s++ ? DOLLAR : MARK), 0, L""});
    else {
        addr.push_back ({DOT, 0, L""});
        if ('.' == *s)
            ++s;
    }
    s = skipspace (s);
    return true;
}

bool
grammar_type::getterm (std::wstring::const_iterator& s, std::vector<addr_type>& addr)
{
    int way = *s == '-' ? -1 : +1;
    if ('+' == *s || '-' == *s)
        s = skipspace (s + 1);
    if ('0' <= *s && *s <= '9')
        addr.push_back ({LINEOFFSET, way * scandecimal (s), L""});
    else if ('/' == *s) {
        std::wstring::const_iterator s1 = s;
        if ((s = skippattern (s1)) == s1)
            return false;
        if (s - s1 > 2)
            lastpattern.assign (s1 + 1, s - 1);
        addr.push_back ({PATTERN, way, lastpattern});
    }
    else
        addr.push_back ({LINEOFFSET, way, L""});
    return true;
}

int
grammar_type::scandecimal (std::wstring::const_iterator& s)
{
    int n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

std::wstring::const_iterator
grammar_type::skippattern (std::wstring::const_iterator const s0)
{
    std::wstring::const_iterator s = s0;
    int lquote = *s++;
    int rquote = '{' == lquote ? '}' : '<' == lquote ? '>'
               : '[' == lquote ? ']' : '(' == lquote ? ')' : lquote;
    int level = 1;
    while (level > 0) {
        if ('\n' == s[0])
            return s0;
        else if ('\\' == s[0] && '\n' != s[1])
            ++s;
        else if (rquote == s[0])
            --level;
        else if (lquote == s[0])
            ++level;
        ++s;
    }
    return s;
}

std::wstring::const_iterator
grammar_type::skipspace (std::wstring::const_iterator s)
{
    while (' ' == *s || '\t' == *s)
        ++s;
    return s;
}
