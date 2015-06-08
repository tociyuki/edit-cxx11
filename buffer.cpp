#include <string>
#include <vector>
#include <algorithm>
#include "edit.hpp"

// undo_type command
enum {ERASE, CHANGE, APPEND};

buffer_type::buffer_type ()
:   seq (1), cur (0), markline (0), filename (),
    data (), text ({{0,0,false}}), history () {}

std::size_t
buffer_type::dot () const
{
    return cur;
}

std::size_t
buffer_type::dollar () const
{
    return text.size () - 1;
}

std::size_t
buffer_type::mark () const
{
    return markline;
}

std::wstring
buffer_type::file () const
{
    return filename;
}

void
buffer_type::get (std::size_t const line,
    std::wstring::const_iterator& s, std::wstring::const_iterator& e) const
{
    if (1 <= line && line < text.size ()) {
        s = data.cbegin () + text[line].dbegin;
        e = data.cbegin () + text[line].dend;
    }
    else {
        s = data.cend ();
        e = data.cend ();
    }
}

bool
buffer_type::gmark (std::size_t const line) const
{
    if (1 <= line && line < text.size ())
        return text[line].mark;
    else
        return false;
}

void
buffer_type::setgmark (std::size_t const line, bool const x)
{
    if (1 <=line && line < text.size ())
        text[line].mark = x;
}

void
buffer_type::setdot (std::size_t const line)
{
    if (line < text.size ())
        cur = line;
}

void
buffer_type::setmark ()
{
    markline= cur;
}

void
buffer_type::setfile (std::wstring const& s)
{
    if (! s.empty ())
        filename = s;
}

std::size_t
buffer_type::append (std::size_t line, std::wstring const& str)
{
    std::vector<line_type> a;
    push_data (a, str);
    line = std::min (line + 1, text.size ());
    history.push_back ({APPEND, seq, cur, line, line + a.size () - 1, {}});
    text.insert (text.begin () + line, a.begin (), a.end ());
    cur = line + a.size () - 1;
    return cur;
}

std::size_t
buffer_type::change (std::size_t line1, std::size_t line2, std::wstring const& str)
{
    if (1 == text.size ())
        return append (line2, str);
    line1 = std::max (line1, 1U);
    line2 = std::max (line2, 1U);
    std::vector<line_type> a;
    push_data (a, str);
    history.push_back ({CHANGE, seq, cur, line1, line1 + a.size () - 1, {}});
    history.back ().text.insert (history.back ().text.begin (),
        text.begin () + line1, text.begin () + line2 + 1);
    text.insert (text.begin () + line1, a.begin (), a.end ());
    text.erase (text.begin () + line1 + a.size (), text.begin () + line2 + a.size () + 1);
    cur = line1 + a.size () - 1;
    return cur;
}

std::size_t
buffer_type::erase (std::size_t line1, std::size_t line2)
{
    if (1 == text.size ())
        return cur;
    line1 = std::max (line1, 1U);
    line2 = std::max (line2, 1U);
    history.push_back ({ERASE, seq, cur, line1, line1 - 1, {}});
    history.back ().text.insert (history.back ().text.begin (),
        text.begin () + line1, text.begin () + line2 + 1);
    text.erase (text.begin () + line1, text.begin () + line2 + 1);
    cur = line1 <= text.size () - 1 ? line1 : line1 - 1;
    return cur;
}

std::size_t
buffer_type::move (std::size_t line1, std::size_t line2, std::size_t line3)
{
    std::vector<line_type> a (text.begin () + line1, text.begin () + line2 + 1);
    std::size_t n = a.size ();
    if (line3 < line1) {
        history.push_back ({ERASE, seq, cur, line1, line2, a});
        history.push_back ({APPEND, seq, cur, line3 + 1, line3 + n, {}});
        text.erase (text.begin () + line1, text.begin () + line2 + 1);
        text.insert (text.begin () + line3 + 1, a.begin (), a.end ());
    }
    else {
        history.push_back ({APPEND, seq, cur, line3 + 1, line3 + n, {}});
        history.push_back ({ERASE, seq, cur, line1, line2, a});
        text.insert (text.begin () + line3 + 1, a.begin (), a.end ());
        text.erase (text.begin () + line1, text.begin () + line2 + 1);
    }
    return cur;
}

void
buffer_type::touch ()
{
    ++seq;
}

std::size_t
buffer_type::undo ()
{
    if (history.empty ())
        return cur;
    std::size_t latest = history.back ().seq;
    while (! history.empty () && latest == history.back().seq) {
        undo_step (history.back());
        cur = history.back ().cur;
        history.pop_back ();
    }
    return cur;
}

void
buffer_type::push_data (std::vector<line_type>& a, std::wstring const& str)
{
    std::size_t i = data.size ();
    data.append (str);
    while (i < data.size ()) {
        std::size_t top = i;
        while (i < data.size () && '\n' != data[i])
            ++i;
        if (i < data.size () && '\n' == data[i])
            ++i;
        a.push_back ({top, i, false});
    }
}

void
buffer_type::undo_step (undo_type& u)
{
    switch (u.command) {
    case ERASE:
        text.insert (text.begin () + u.line1, u.text.begin (), u.text.end ());
        break;
    case CHANGE:
        text.insert (text.begin () + u.line1, u.text.begin (), u.text.end ());
        text.erase (text.begin () + u.line1 + u.text.size (),
            text.begin () + u.line2 + u.text.size () + 1);
        break;
    case APPEND:
        text.erase (text.begin () + u.line1, text.begin () + u.line2 + 1);
        break;
    }
}
