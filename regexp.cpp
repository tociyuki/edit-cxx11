#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "edit.hpp"

/* execute () and addthread () based on Russ Cox's Pike VM */

/* recode_type op */
enum {
    MATCH, ANY, CCLASS, NCCLASS, WORDB, NWORDB, BOL, EOL, JMP, SPLIT, SAVE
};

std::shared_ptr<std::vector<std::size_t>>
rethread_type::update (std::size_t i, std::size_t x) const
{
    auto u = std::make_shared<std::vector<std::size_t>> (cap->cbegin (), cap->cend ());
    if (u->size () < i + 1)
        u->resize (i + 1, std::wstring::npos);
    (*u)[i] = x;
    return u;
}

regexp_type::regexp_type ()
: group (0), re (), eor (), program (), gen (1), mark (),
  bos (), eos () {}

bool
regexp_type::execute (std::wstring::const_iterator s,
    std::wstring::const_iterator abos, std::wstring::const_iterator aeos,
    std::vector<std::size_t>& capture)
{
    gen = 1;
    mark.clear ();
    mark.resize (program.size (), 0);
    bos = abos;
    eos = aeos;
    bool match = false;
    std::vector<rethread_type> run, rdy;
    auto cap0 = std::make_shared<std::vector<std::size_t>> ();
    cap0->push_back (s - bos);
    cap0->push_back (s - bos);
    addthread (run, {0, cap0}, s);
    for (; ! run.empty (); ++s) {
        ++gen;
        for (rethread_type const& th : run) {
            recode_type& re = program[th.ip];
            if (ANY == re.op) {
                if (s < eos && '\n' != *s)
                    addthread (rdy, {th.ip + 1, th.cap}, s + 1);
            }
            else if (CCLASS == re.op || NCCLASS == re.op) {
                if (s < eos && (incclass(re.s, *s) ^ (NCCLASS == re.op)))
                    addthread (rdy, {th.ip + 1, th.cap}, s + 1);
            }
            else if (MATCH == re.op) {
                auto cap0 = th.update (1, s - bos);
                capture = *cap0;
                match = true;
                break;
            }
        }
        std::swap (run, rdy);
        rdy.clear ();
        if (s >= eos)
            break;
    }
    return match;
}

void
regexp_type::addthread (std::vector<rethread_type>& q, rethread_type&& th,
    std::wstring::const_iterator s)
{
    if (mark[th.ip] == gen)
        return;
    mark[th.ip] = gen;
    recode_type& re = program[th.ip];
    switch (re.op) {
    default:
        q.push_back (std::move (th));
        break;
    case BOL:
        if (s <= bos || (s - 1 >= bos && '\n' == s[-1]))
            addthread (q, {th.ip + 1, th.cap}, s);
        break;
    case EOL:
        if (s >= eos || '\n' == s[0])
            addthread (q, {th.ip + 1, th.cap}, s);
        break;
    case WORDB:
    case NWORDB:
        if (atwordboundary (s) ^ (NWORDB == re.op))
            addthread (q, {th.ip + 1, th.cap}, s);
        break;
    case JMP:
        addthread (q, {th.ip + 1 + re.x, th.cap}, s);
        break;
    case SPLIT:
        addthread (q, {th.ip + 1 + re.x, th.cap}, s);
        addthread (q, {th.ip + 1 + re.y, th.cap}, s);
        break;
    case SAVE:
        addthread (q, {th.ip + 1, th.update (re.x, s - bos)}, s);
        break;
    }
}

bool
regexp_type::incclass (std::wstring const& span, int const c)
{
    for (auto p = span.cbegin (); p < span.cend (); ++p) {
        if ('\\' == *p) {
            if (c == *++p)
                return true;
        }
        else if ('-' == *p) {
            if (p[-1] <= c && c <= p[2])
                return true;
            p += 2;
        }
    }
    return false;
}

bool
regexp_type::atwordboundary (std::wstring::const_iterator s)
{
    int c0 = bos <= s - 1 ? s[-1] : ' ';
    int c1 = s < eos ? s[0] : ' ';
    bool isword0 = ('A' <= c0 && c0 <= 'Z') || ('a' <= c0 && c0 <= 'z')
        || ('0' <= c0 && c0 <= '9') || '_' == c0;
    bool isword1 = ('A' <= c1 && c1 <= 'Z') || ('a' <= c1 && c1 <= 'z')
        || ('0' <= c1 && c1 <= '9') || '_' == c1;
    return (isword0 ^ isword1) ? true : false;
}

bool
regexp_type::compile (std::wstring const& pattern)
{
    group = 0;
    re = pattern.cbegin ();
    eor = pattern.cend ();
    program.clear ();
    if (! (alt (program) && re == eor))
        return false;
    program.push_back ({MATCH, 0, 0, L""});
    return true;
}

bool
regexp_type::alt (std::vector<recode_type>& code)
{
    std::vector<std::size_t> patch;
    std::vector<recode_type> lhs;
    if (! cat (lhs))
        return false;
    while (re < eor && '|' == *re) {
        ++re;
        std::vector<recode_type> rhs;
        if (! cat (rhs))
            return false;
        if (lhs.empty () && rhs.empty ())
            continue;
        int y = lhs.size () + 1;
        code.push_back ({SPLIT, 0, y, L""});
        code.insert (code.end (), lhs.begin (), lhs.end ());
        patch.push_back (code.size ());
        code.push_back ({JMP, 0, 0, L""});
        lhs = std::move (rhs);
    }
    code.insert (code.end (), lhs.begin (), lhs.end ());
    std::size_t const j = code.size ();
    for (auto i : patch)
        code[i].x = j - i - 1;
    return true;
}

bool
regexp_type::cat (std::vector<recode_type>& code)
{
    while (re < eor && '|' != *re && ')' != *re) {
        std::vector<recode_type> code1;
        if (! term (code1))
            return false;
        code.insert (code.end (), code1.begin (), code1.end ());
    }
    return true;
}

bool
regexp_type::term (std::vector<recode_type>& code)
{
    std::vector<recode_type> code1;
    if (! factor (code1))
        return false;
    int k1 = 1, k2 = 1;
    if (re < eor && ('?' == *re || '*' == *re || '+' == *re)) {
        k1 = '+' == *re ? 1 : 0;
        k2 = '?' == *re ? 1 : -1;
        ++re;
    }
    int n1 = code1.size ();
    int x = k1 == 1 && k2 == -1 ? -(n1 + 1) : 0;
    int y = k1 == 0 && k2 == 1 ? n1 : k1 == 1 && k2 == -1 ? 0 : n1 + 1;
    if (k1 == 1 && k2 == 1)
        code.insert (code.end (), code1.begin (), code1.end ());
    else if (k1 == 1 && k2 == -1) {
        code.insert (code.end (), code1.begin (), code1.end ());
        code.push_back ({SPLIT, x, y, L""});
    }
    else {
        code.push_back ({SPLIT, x, y, L""});
        code.insert (code.end (), code1.begin (), code1.end ());
        if (k2 == -1)
            code.push_back ({JMP, -(n1 + 2), 0, L""});
    }
    return true;
}

bool
regexp_type::factor (std::vector<recode_type>& code)
{
    std::wstring str;
    if (re >= eor)
        return false;
    if ('.' == re[0] || '^' == re[0] || '$' == re[0]) {
        int op = '^' == re[0] ? BOL : '$' == re[0] ? EOL : ANY;
        code.push_back ({op, 0, 0, L""});
        ++re;
    }
    else if ('\\' == re[0] && re + 1 < eor && ('b' == re[1] || 'B' == re[1])) {
        int op = 'b' == re[1] ? WORDB : NWORDB;
        code.push_back ({op, 0, 0, L""});
        re += 2;
    }
    else if ('(' == re[0]) {
        ++re;
        ++group;
        int n1 = group * 2, n2 = group * 2 + 1;
        code.push_back ({SAVE, n1, 0, L""});
        if (! (alt (code) && re < eor && ')' == re[0]))
            return false;
        ++re;
        code.push_back ({SAVE, n2, 0, L""});
    }
    else if ('[' == re[0]) {
        ++re;
        int op = re < eor && '^' == re[0] ? NCCLASS : CCLASS;
        if (NCCLASS == op)
            ++re;
        if (! clschar (str))
            return false;
        while (re < eor && ']' != re[0]) {
            if ('-' == re[0] && re + 1 < eor && ']' != re[1])
                str.push_back (*re++);
            if ('-' == re[0] && re + 1 < eor && ']' != re[1])
                return false;
            if (! clschar (str))
                return false;
        }
        if (! (re < eor && ']' == re[0]))
            return false;
        ++re;
        code.push_back ({op, 0, 0, str});
    }
    else if (clschar (str))
        code.push_back ({CCLASS, 0, 0, str});
    else
        return false;
    return true;
}

bool
regexp_type::clschar (std::wstring& str)
{
    if (re >= eor || ! (' ' <= *re && 0x7f != *re))
        return false;
    int c = *re++;
    bool esc = '\\' == c;
    if (esc) {
        if (re >= eor || ! (' ' <= *re && 0x7f != *re))
            return false;
        c = *re++;
    }
    if (esc && 'd' == c)
        str.append (L"\\0-\\9");
    else if (esc && 'D' == c)
        str.append (L"\\\t\\ -\\/\\:-\\~");
    else if (esc && 'w' == c)
        str.append (L"\\0-\\9\\a-\\z\\A-\\Z\\_");
    else if (esc && 'W' == c)
        str.append (L"\\\t\\ -\\/\\:-\\@\\[-\\^\\`\\{-\\~");
    else if (esc && 's' == c)
        str.append (L"\\ \\\t\\\n\\\r");
    else if (esc && 'S' == c)
        str.append (L"\\!-\\~");
    else if (esc && 't' == c)
        str.append (L"\\\t");
    else if (esc && 'n' == c)
        str.append (L"\\\n");
    else {
        str.push_back ('\\');
        str.push_back (c);
    }
    return true;
}
