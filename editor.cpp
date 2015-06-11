#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <string>
#include <vector>
#include <algorithm>
#include "edit.hpp"

editor_type::editor_type (grammar_type& sc, buffer_type& b)
: scanner (sc), buffer (b), line1 (0), line2 (0), line3 (0) {}

int
editor_type::edit (std::wstring::const_iterator s)
{
    command_type ct;
    if (! scanner.get (s, ct))
        return '?';
    switch (ct.command) {
    default:
        if ('?' == evalrange (ct))
            return '?';
        if ('?' == command (ct))
            return '?';
        buffer.touch ();
        break;
    case 'g':
    case 'v':
        if ('?' == global (s, ct))
            return '?';
        buffer.touch ();
        break;
    case 'q':
        return ct.command;
    case 'u':
        buffer.undo ();
        return ct.command;
    }
    return ct.command;
}

int
editor_type::command (command_type& ct)
{
    std::wstring doc;
    if (1 == ct.naddr)
        line2 = line1;
    switch (ct.command) {
    default: return '?';
    case 'a': cmd_a (ct); break;
    case 'c': cmd_c (ct); break;
    case 'd': cmd_d (ct); break;
    case 'e': cmd_e (ct); break;
    case 'f': cmd_f (ct); break;
    case 'i': cmd_i (ct); break;
    case 'j': cmd_j (ct); break;
    case 'k': cmd_k (ct); break;
    case 'm': cmd_m (ct); break;
    case 'n': cmd_n (ct); break;
    case 'p': cmd_p (ct); break;
    case 'r': cmd_r (ct); break;
    case 's': cmd_s (ct); break;
    case 't': cmd_t (ct); break;
    case 'w': cmd_w (ct); break;
    case '=': cmd_equal (ct); break;
    case '\n': cmd_p (ct); break;
    }
    print_dot (ct);
    return ct.command;
}

int
editor_type::global (std::wstring::const_iterator& s, command_type& ct)
{
    std::wstring illegal (L"gvqu");
    command_type rct;
    if (! scanner.get (s, rct) || illegal.find (rct.command) != std::wstring::npos)
        return '?';
    if ('?' == markglobal (ct) || '?' == sweepglobal (rct))
        return '?';
    return ct.command;
}

int
editor_type::markglobal (command_type& ct)
{
    if (! evaladdr (ct.addr1, line1) || ! evaladdr (ct.addr2, line2))
        return '?';
    regexp_type re;
    re.compile (ct.pattern);
    for (std::size_t line = 1; line <= buffer.dollar (); ++line) {
        buffer.setgmark (line, false);
        if (line1 <= line && line <= line2) {
            std::wstring::const_iterator s, e;
            buffer.get (line, s, e);
            if (match (re, s, e) ^ ('v' == ct.command))
                buffer.setgmark (line, true);
        }
    }
    return ct.command;
}

int
editor_type::sweepglobal (command_type& ct)
{
    for (std::size_t line = 1; line <= buffer.dollar (); ++line)
        if (buffer.gmark (line)) {
            buffer.setgmark (line, false);
            buffer.setdot (line);
            if (ct.naddr == 0)
                line1 = line2 = line;
            else if ('?' == evalrange (ct))
                return '?';
            if ('?' == command (ct))
                return '?';
            line = 0;
        }
    return ct.command;
}

int
editor_type::cmd_a (command_type& ct)
{
    getdoc (ct.param);
    buffer.append (line2, ct.param);
    return ct.command;
}

int
editor_type::cmd_c (command_type& ct)
{
    getdoc (ct.param);
    buffer.change (line1, line2, ct.param);
    return ct.command;
}

int
editor_type::cmd_d (command_type& ct)
{
    buffer.erase (line1, line2);
    return ct.command;
}

int
editor_type::cmd_e (command_type& ct)
{
    std::wstring doc;
    buffer.setfile (ct.param);
    if (read (buffer.file (), doc)) {
        buffer.change (1, buffer.dollar (), doc);
    }
    else {
        buffer.erase (1, buffer.dollar ());
        if (! buffer.file ().empty ()) {
            std::wcout << L"cannot read file '" << buffer.file () << "'\n";
            return '?';
        }
    }
    return ct.command;
}

int
editor_type::cmd_f (command_type& ct)
{
    buffer.setfile (ct.param);
    if (buffer.file ().empty ())
        return '?';
    std::wcout << buffer.file () << std::endl;
    return ct.command;
}

int
editor_type::cmd_i (command_type& ct)
{
    getdoc (ct.param);
    buffer.append (line2 - 1, ct.param);
    return ct.command;
}

int
editor_type::cmd_j (command_type& ct)
{
    std::wstring sep;
    unquote (ct.param, sep);
    std::wstring doc;
    for (std::size_t line = line1; line <= line2; ++line) {
        if (line > line1)
            doc.append (sep);
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        if (s < e && '\n' == e[-1])
            --e;
        doc.append (s, e);
    }
    doc.push_back ('\n');
    buffer.change (line1, line2, doc);
    return ct.command;
}

int
editor_type::cmd_k (command_type& ct)
{
    buffer.setmark ();
    return ct.command;
}

int
editor_type::cmd_m (command_type& ct)
{
    if (! evaladdr (ct.addr3, line3)
            || line1 < 1 || line3 > buffer.dollar ()
            || (line1 <= line3 && line3 <= line2))
        return '?';
    buffer.move (line1, line2, line3);
    if (line3 > line1)
        buffer.setdot (line3);
    else
        buffer.setdot (line3 + (line2 - line1 + 1));
    return ct.command;
}

int
editor_type::cmd_n (command_type& ct)
{
    for (std::size_t line = line1; line <= line2; ++line) {
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        std::wcout << std::setw (7) << line << L" ";
        for (; s < e; ++s)
            std::wcout.put (*s);
    }
    buffer.setdot (line2);
    return ct.command;
}

int
editor_type::cmd_p (command_type& ct)
{
    for (std::size_t line = line1; line <= line2; ++line) {
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        for (; s < e; ++s)
            std::wcout.put (*s);
    }
    buffer.setdot (line2);
    return ct.command;
}

void
editor_type::print_dot (command_type& ct)
{
    if (ct.pflag && buffer.dot () > 0) {
        std::wstring::const_iterator s, e;
        buffer.get (std::max (buffer.dot (), 1U), s, e);
        for (; s < e; ++s)
            std::wcout.put (*s);
    }
}

int
editor_type::cmd_r (command_type& ct)
{
    std::wstring doc;
    if (buffer.file ().empty ())
        buffer.setfile (ct.param);
    if (ct.param.empty ())
        ct.param = buffer.file ();
    if (read (ct.param, doc))
        buffer.append (line2, doc);
    else
        return '?';
    return ct.command;
}

int
editor_type::cmd_s (command_type& ct)
{
    std::wstring doc;
    for (std::size_t line = line1; line <= line2; ++line) {
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        if (s < e && '\n' == e[-1])
            --e;
        substitute (ct.pattern, ct.param, ct.gflag, s, e, doc);
        doc.push_back ('\n');
    }
    buffer.change (line1, line2, doc);
    return ct.command;
}

int
editor_type::cmd_t (command_type& ct)
{
    if (! evaladdr (ct.addr3, line3)
            || line1 < 1 || line3 > buffer.dollar ()
            || (line1 <= line3 && line3 <= line2))
        return '?';
    std::wstring doc;
    for (std::size_t line = line1; line <= line2; ++line) {
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        doc.append (s, e);
    }
    buffer.append (line3, doc);
    return ct.command;
}

int
editor_type::cmd_w (command_type& ct)
{
    if (buffer.file ().empty ())
        buffer.setfile (ct.param);
    if (ct.param.empty ())
        ct.param = buffer.file ();
    if (! write (ct.param))
        return '?';
    return ct.command;
}

int
editor_type::cmd_equal (command_type& ct)
{
    std::wcout << line2 << std::endl;
    return ct.command;
}

bool
editor_type::read (std::wstring const& file, std::wstring& doc)
{
    if (file.empty ())
        return false;
    std::ifstream fs;
    fs.open (encode (file));
    if (! fs.is_open ())
        return false;
    std::string s;
    char b[2048];
    int n;
    while ((n = fs.read (b, sizeof (b)).gcount ()) > 0) {
        s.append (b, n);
    }
    fs.close ();
    doc = decode (s);
    return true;
}

bool
editor_type::write (std::wstring const& file)
{
    std::wofstream fs;
    fs.open (encode (file));
    if (! fs.is_open ())
        return false;
    for (std::size_t line = line1; line <= line2; ++line) {
        std::wstring::const_iterator s, e;
        buffer.get (line, s, e);
        for (; s < e; ++s)
            fs.put (*s);
    }
    fs.close ();
    return true;
}

void
editor_type::getdoc (std::wstring& doc)
{
    std::wstring s;
    while (std::getline (std::wcin, s)) {
        if (s == L".")
            break;
        doc += s + L"\n";
    }
}

bool
editor_type::evalrange (command_type const& ct)
{
    if (! evaladdr (ct.addr1, line1))
        return '?';
    if (';' == ct.comma)
        buffer.setdot (line1);
    if (! evaladdr (ct.addr2, line2))
        return '?';
    return ct.command;
}

bool
editor_type::evaladdr (std::vector<addr_type> const& addr, std::size_t& line)
{
    int n = 0;
    int n1 = n;
    if (0 < buffer.dollar ())
        for (addr_type const& x: addr)
            switch (x.type) {
            case LINENUM:
                n = x.disp < 1 ? 0 : (x.disp - 1) % buffer.dollar () + 1;
                break;
            case LINEOFFSET:
                n = (n + x.disp - 1) % buffer.dollar () + 1;
                break;
            case DOT:
                n = buffer.dot ();
                break;
            case DOLLAR:
                n = buffer.dollar ();
                break;
            case MARK:
                n = buffer.mark ();
                break;
            case PATTERN:
                n1 = n;
                n = find (n1, x.disp, x.pattern);
                if (n == n1)
                    return false;
                break;
            }
    line = n;
    return true;
}

int
editor_type::find (int n, int way, std::wstring const& pattern)
{
    regexp_type re;
    re.compile (pattern);
    int n1 = (n + way - 1) % buffer.dollar () + 1;
    while (n1 != n) {
        std::wstring::const_iterator s, e;
        buffer.get (n1, s, e);
        if (match (re, s, e))
            return n1;
        n1 = (n1 + way - 1) % buffer.dollar () + 1;
    }
    return n;
}

bool
editor_type::match (regexp_type& re,
    std::wstring::const_iterator s, std::wstring::const_iterator const eos)
{
    std::vector<std::size_t> cap;
    std::wstring::const_iterator const bos = s;
    --s;
    do {
        ++s;
        if (re.execute (s, bos, eos, cap))
            return true;
    } while (s < eos && '\n' != *s);
    return false;
}

void
editor_type::substitute (std::wstring const& pattern,
    std::wstring const& replacement, bool gflag, 
    std::wstring::const_iterator s, std::wstring::const_iterator eos,
    std::wstring& doc)
{
    regexp_type re;
    std::vector<std::size_t> cap;
    std::wstring::const_iterator const bos = s;
    if (! re.compile (pattern))
        return;
    std::wstring::const_iterator s3 = s;
    while (s3 <= eos) {
        if (re.execute (s3, bos, eos, cap)) {
            substhere (s3, replacement, cap, doc);
            s3 = bos + cap[1];
            if (! gflag)
                break;
        }
        else if (s3 < eos)
            doc.push_back (*s3++);
    }
    if (s3 < eos)
        doc.append (s3, eos);
}

void
editor_type::substhere (
    std::wstring::const_iterator const bos,
    std::wstring const& replacement,
    std::vector<std::size_t>& cap, std::wstring& doc)
{
    std::wstring::const_iterator i;
    for (i = replacement.cbegin (); i < replacement.cend (); ++i)
        if ('$' == i[0] && i + 1 < replacement.cend ()
                && ('&' == i[1] || ('0' <= i[1] && i[1] <= '9'))) {
            std::size_t n = '&' == i[1] ? 0 : i[1] - '0';
            if (n * 2 + 1 < cap.size ()
                    && cap[n * 2] != std::wstring::npos
                    && cap[n * 2 + 1] != std::wstring::npos)
                doc.append (bos + cap[n * 2], bos + cap[n * 2 + 1]);
            ++i;
        }
        else if ('\\' == *i && i + 1 < replacement.cend ()) {
            int c = *++i;
            c = 'n' == c ? '\n' : 't' == c ? '\t' : c;
            doc.push_back (c);
        }
        else
            doc.push_back (*i);
}

void
editor_type::unquote (std::wstring const& src, std::wstring& dst)
{
    std::wstring::const_iterator i;
    for (i = src.cbegin (); i < src.cend (); ++i)
        if ('\\' == *i && i + 1 < src.cend ()) {
            int c = *++i;
            c = 'n' == c ? '\n' : 't' == c ? '\t' : c;
            dst.push_back (c);
        }
        else
            dst.push_back (*i);
}

std::wstring
editor_type::decode (std::string octets, char const* const lname)
{
    std::locale loc (std::locale::classic (), lname, std::locale::ctype);
    auto& cvt = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>> (loc);
    auto mb = std::mbstate_t ();
    std::wstring str (octets.size (), L'\0');
    char const* octetsnext;
    wchar_t* strnext;
    cvt.in (mb, &octets[0], &octets[octets.size ()], octetsnext,
                &str[0], &str[str.size ()], strnext);
    str.resize (strnext - &str[0]);
    return str;
}

std::string
editor_type::encode (std::wstring str, char const* lname)
{
    std::locale loc (std::locale::classic (), lname, std::locale::ctype);
    auto& cvt = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>> (loc);
    auto mb = std::mbstate_t ();
    std::string octets(str.size () * cvt.max_length (), '\0');
    wchar_t const* strnext;
    char* octetsnext;
    cvt.out (mb, &str[0], &str[str.size ()], strnext,
                 &octets[0], &octets[octets.size ()], octetsnext);
    octets.resize (octetsnext - &octets[0]);
    return octets;
}
