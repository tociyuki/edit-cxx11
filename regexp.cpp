#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "edit.hpp"

// execute () and addthread () based on Russ Cox's Pike VM

// recode_type op
enum {
    MATCH, ANY, CCLASS, NCCLASS, WORDB, NWORDB, BOL, EOL, JMP, SPLIT, SAVE,
    PRIMITIVE, GROUP, LPAREN, MANY, ALT, RPAREN    
};

void recode_type::assign (int a, int b, int c, std::wstring const& d)
{
    op = a; x = b; y = c;
    s = d;
}

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
: group (0), pattern (), pos (), token (), program (), gen (1), mark (),
  bos (), eos ()
{
    modifier_extend = false;
}

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
regexp_type::compile (std::wstring const& s)
{
    group = 0;
    pattern = s;
    pos = pattern.cbegin ();
    program.clear ();
    if (! next_token ())
        return false;
    if (! (alt (program) && MATCH == token.op))
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
    while (ALT == token.op) {
        if (! next_token ())
            return false;
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
    while (ALT != token.op && RPAREN != token.op && MATCH != token.op) {
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
    if (MANY == token.op) {
        k1 = token.x;
        k2 = token.y;
        if (! next_token ())
            return false;
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
    if (PRIMITIVE == token.op) {
        code.push_back ({token.x, 0, 0, L""});
        std::swap (code.back ().s, token.s);
        if (! next_token ())
            return false;
    }
    else if (LPAREN == token.op || GROUP == token.op) {
        bool isgroup = GROUP == token.op;
        int n1 = token.x, n2 = token.x + 1;
        if (isgroup)
            code.push_back ({SAVE, n1, 0, L""});
        if (! next_token ())
            return false;
        if (! (alt (code) && RPAREN == token.op))
            return false;
        if (! next_token ())
            return false;
        if (isgroup)
            code.push_back ({SAVE, n2, 0, L""});
    }
    else
        return false;
    return true;
}

static inline bool
isrspace (wchar_t const c)
{
    return L' ' == c || L'\t' == c || L'\n' == c || L'\r' == c;
}

static inline bool
isrprint (wchar_t const c)
{
    return L' ' <= c && 0x7f != c;
}

static inline std::wstring
rangechar (wchar_t const c)
{
    std::wstring s;
    s.push_back (L'\\');
    s.push_back (c);
    return s;
}

static inline bool
isrdws (wchar_t const e)
{
    return L'd' == e || L'D' == e || L'w' == e || L'W' == e || L's' == e || L'S' == e;
}

static inline std::wstring
rangeesc (wchar_t const e)
{
    std::wstring s;
    if (L'd' == e)
        s.append (L"\\0-\\9");
    else if (L'D' == e)
        s.append (L"\\\t\\ -\\/\\:-\\~");
    else if (L'w' == e)
        s.append (L"\\0-\\9\\a-\\z\\A-\\Z\\_");
    else if (L'W' == e)
        s.append (L"\\\t\\ -\\/\\:-\\@\\[-\\^\\`\\{-\\~");
    else if (L's' == e)
        s.append (L"\\ \\\t\\\n\\\r");
    else if (L'S' == e)
        s.append (L"\\!-\\~");
    else {
        wchar_t const c = L't' == e ? L'\t' : L'r' == e ? L'\r' : L'n' == e ? L'\n' : e;
        s.push_back (L'\\');
        s.push_back (c);
    }
    if (L'D' == e || L'w' == e || L'S' == e) {
        s.push_back (L'\\');
        s.push_back (0x80);
        s.push_back (L'-');
        s.push_back (L'\\');
        s.push_back (0x10ffff);
    }
    return std::move (s);
}

bool
regexp_type::next_token (void)
{
    enum {ERROR, OK, SSTART, SCOMMENT, SBSLASH, SGROUP, SLPQUES,
        SLBRACKET, SNLBRACKET, SRANGELHS, SRANGEAFTER, SRANGERHS,
        SRANGELHB, SRANGERHB, SCCLASSEND};
    int next_state = SSTART;
    for (; next_state >= SSTART && pos <= pattern.cend (); ++pos) {
        int const state = next_state;
        next_state = ERROR;
        if (pos == pattern.cend ()) {
            if (SSTART == state || SCOMMENT == state) {
                token.assign (MATCH, 0, 0, L"");
                return true;
            }
            return false;
        }
        wchar_t c = *pos;
        if (SSTART == state) {
            next_state = OK;
            if (L'\\' == c || L'[' == c || L']' == c || L'(' == c)
                next_state = L'\\' == c ? SBSLASH : L'[' == c ? SLBRACKET
                    : L'(' == c ? SGROUP : ERROR;
            else if (L'|' == c || L')' == c)
                token.assign (L'|' == c ? ALT : RPAREN, 0, 0, L"");
            else if (L'?' == c || L'*' == c || L'+' == c)
                token.assign (MANY, L'+' == c ? 1 : 0, L'?' == c ? 1 : -1, L"");
            else if (L'.' == c || L'^' == c || L'$' == c)
                token.assign (PRIMITIVE, L'.' == c ? ANY : L'^' == c ? BOL :EOL, 0, L"");
            else if (modifier_extend && isrspace (c))
                next_state = SSTART;
            else if (modifier_extend && L'#' == c)
                next_state = SCOMMENT;
            else if (isrprint (c))
                token.assign (PRIMITIVE, CCLASS, 0, rangechar (c));
            else
                next_state = ERROR;
        }
        else if (SCOMMENT == state) {
            next_state = L'\n' == c ? SSTART : SCOMMENT;
        }
        else if (SBSLASH == state) {
            next_state = OK;
            if (L'b' == c || L'B' == c)
                token.assign (PRIMITIVE, L'b' == c ? WORDB : NWORDB, 0, L"");
            else if (isrprint (c))
                token.assign (PRIMITIVE, CCLASS, 0, rangeesc (c));
            else
                next_state = ERROR;
        }
        else if (SGROUP == state) {
            if (L'?' == c)
                next_state = SLPQUES;
            else {
                group += 2;
                token.assign (GROUP, group, 0, L"");
                --pos;
                next_state = OK;
            }
        }
        else if (SLPQUES == state) {
            token.assign (LPAREN, 0, 0, L"");
            next_state = L':' == c ? OK : ERROR;
        }
        else if (SLBRACKET == state || SNLBRACKET == state) {
            int const op = SLBRACKET == state ? CCLASS : NCCLASS;
            if (SLBRACKET == state && L'^' == c)
                next_state = SNLBRACKET;
            else if (SNLBRACKET == state && L']' == c) {
                token.assign (PRIMITIVE, CCLASS, 0, rangechar (L'^'));
                next_state = OK;
            }
            else if (L'\\' == c) {
                token.assign (PRIMITIVE, op, 0, L"");
                next_state = SRANGELHB;
            }
            else if (isrprint (c)) {
                token.assign (PRIMITIVE, op, 0, rangechar (c));
                next_state = SRANGELHS;
            }
        }
        else if (SRANGELHS == state || SRANGERHS == state || SRANGEAFTER == state) {
            if (L']' == c) {
                if (SRANGERHS == state)
                    token.s.append (rangechar (L'-'));
                next_state = OK;
            }
            else if (L'\\' == c)
                next_state = SRANGERHS == state ? SRANGERHB : SRANGELHB;
            else if (L'-' == c)
                next_state = SRANGELHS == state ? SRANGERHS
                    : SRANGEAFTER == state ? SCCLASSEND : ERROR;
            else if (isrprint (c)) {
                if (SRANGERHS == state)
                    token.s.push_back (L'-');
                token.s.append (rangechar (c));
                next_state = SRANGERHS == state ? SRANGEAFTER : SRANGELHS;
            }
        }
        else if (SRANGELHB == state) {
            if (isrprint (c)) {
                token.s.append (rangeesc (c));
                next_state = isrdws (c) ? SRANGEAFTER : SRANGELHS;
            }
        }
        else if (SRANGERHB == state) {
            if (! isrdws (c) && isrprint (c)) {
                token.s.push_back (L'-');
                token.s.append (rangeesc (c));
                next_state = SRANGEAFTER;
            }
        }
        else if (SCCLASSEND == state) {
            if (L']' == c) {
                token.s.append (rangechar (L'-'));
                next_state = OK;
            }
        }
    }
    if (OK != next_state)
        --pos;
    return OK == next_state;
}
