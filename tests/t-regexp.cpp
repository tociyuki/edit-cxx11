#include "taptests.hpp"
#include "edit.hpp"

void
test_nchar_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"a");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a/ !~ \"\"");
}

void
test_char_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    regexp_type re;
    re.compile (L"a");
    std::wstring s (L"abcdcecf");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_nchar_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"a");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a/ !~ \"bcdcecf\"");
}

void
test_nalt_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a|b|c/ !~ \"\"");
}

void
test_alt_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a|b|c/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_alt_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a|b|c/ =~ \"bcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"b", L"$& == \"b\"");
}

void
test_alt_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a|b|c/ =~ \"cdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
}

void
test_alt_d (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a|b|c/ =~ \"cdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
}

void
test_nalt_d (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"dcecf");
    regexp_type re;
    re.compile (L"a|b|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a|b|c/ !~ \"dcecf\"");
}

void
test_nseq_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"abc");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/abc/ !~ \"\"");
}

void
test_seq_abc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"abc");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/abc/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abc", L"$& == \"abc\"");
}

void
test_nseq_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"a");
    regexp_type re;
    re.compile (L"abc");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/abc/ !~ \"a\"");
}

void
test_nseq_bcd (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"abc");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/abc/ !~ \"bcdcecf\"");
}

void
test_nalt_seq_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"ab|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/ab|c/ !~ \"\"");
}

void
test_alt_seq_ab (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abdcecf");
    regexp_type re;
    re.compile (L"ab|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/ab|c/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"ab", L"$& == \"ab\"");
}

void
test_alt_seq_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"ab|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/ab|c/ =~ \"cdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
}

void
test_nalt_seq_d (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"dcecf");
    regexp_type re;
    re.compile (L"ab|c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/ab|c/ !~ \"dcecf\"");
}

void
test_alt_seq_e_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"ab|c|");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/ab|c|/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_alt_seq_e_ab (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"ab|c|");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/ab|c|/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"ab", L"$& == \"ab\"");
}

void
test_alt_seq_e_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"ab|c|");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/ab|c|/ =~ \"cdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
}

void
test_ngroup_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"(ab)");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/(ab)/ !~ \"\"");
}

void
test_ngroup_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"a");
    regexp_type re;
    re.compile (L"(ab)");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/(ab)/ !~ \"a\"");
}

void
test_group_ab (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"(ab)");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab)/ =~ \"ab\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"ab", L"$& == \"ab\"");
    std::wstring m1(s.begin () + m[2], s.begin () + m[3]);
    ts.ok (m1 == L"ab", L"$1 == \"ab\"");
}

void
test_group_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"(ab|c)");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab)/ =~ \"ab\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
    std::wstring m1(s.begin () + m[2], s.begin () + m[3]);
    ts.ok (m1 == L"c", L"$1 == \"c\"");
}

void
test_opt_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"a?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a?/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_opt_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a?/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_opt_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"a?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a?/ =~ \"bcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_exp_opt_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"(ab|c)?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)?/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_exp_opt_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"(ab|c)?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)?/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"ab", L"$& == \"ab\"");
}

void
test_exp_opt_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"(ab|c)?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)?/ =~ \"bcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_exp_opt_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"cdcecf");
    regexp_type re;
    re.compile (L"(ab|c)?");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)?/ =~ \"cdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"c", L"$& == \"c\"");
}

void
test_star_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"a*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a*/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_star_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a*/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_star_a3 (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"aaabcdcecf");
    regexp_type re;
    re.compile (L"a*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a*/ =~ \"aaabcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"aaa", L"$& == \"aaa\"");
}

void
test_star_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"a*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a*/ =~ \"bcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_star_2_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"(ab|c)*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)*/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_star_2_abccababccc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abccababcccdcecf");
    regexp_type re;
    re.compile (L"(ab|c)*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)*/ =~ \"abccababcccdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abccababccc", L"$& == \"abccababccc\"");
}

void
test_star_2_d (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"dcecf");
    regexp_type re;
    re.compile (L"(ab|c)*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/(ab|c)*/ =~ \"dcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_nplus_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"a+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a+/ !~ \"\"");
}

void
test_plus_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a+/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_plus_a3 (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"aaabcdcecf");
    regexp_type re;
    re.compile (L"a+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a+/ =~ \"aaabcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"aaa", L"$& == \"aaa\"");
}

void
test_nplus_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"bcdcecf");
    regexp_type re;
    re.compile (L"a+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/a+/ !~ \"bcdcecf\"");
}

void
test_bos_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"^");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/^/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_bos_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"^");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/^/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_nbos_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"^");
    bool r = re.execute (s.cbegin () + 1, s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/^/ !~ \"abcdcecf\"");
}

void
test_eos_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"^");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/$/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_eos_abcdcecf (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"$");
    bool r = re.execute (s.cend (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/$/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_neos_abcdcecf (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"$");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/$/ !~ \"abcdcecf\"");
}

void
test_nboundary_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"\\b");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/\\b/ !~ \"\"");
}

void
test_boundary_abc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abc");
    regexp_type re;
    re.compile (L"\\babc");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\babc/ =~ \"abc\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abc", L"$& == \"abc\"");
}

void
test_nboundary_a_bc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abc");
    regexp_type re;
    re.compile (L"\\bbc");
    bool r = re.execute (s.cbegin () + 1, s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/\\bbc/ !~ \"abc\"");
}

void
test_boundary_abc_b (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abc");
    regexp_type re;
    re.compile (L"abc\\b");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/abc\\b/ =~ \"abc\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abc", L"$& == \"abc\"");
}

void
test_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"^$");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/^$/ =~ \"\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"", L"$& == \"\"");
}

void
test_nempty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"a");
    regexp_type re;
    re.compile (L"^$");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/^$/ !~ \"a\"");
}

void
test_nany_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L".");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/./ !~ \"\"");
}

void
test_any_a (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L".");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/./ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"a", L"$& == \"a\"");
}

void
test_any_ascii (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~\n");
    regexp_type re;
    re.compile (L".*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/.*/ =~ \"0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~", L"$& == \"0-9A-Za-z...\"");
}

void
test_digit (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~\n");
    regexp_type re;
    re.compile (L"\\d*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\d*/ =~ \"0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"0123456789", L"$& == \"0123456789\"");
}

void
test_ndigit (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~0123456789\n");
    regexp_type re;
    re.compile (L"\\D*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\D*/ =~ \"A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~", L"$& == \"A-Za-z...\"");
}

void
test_ndigit_2 (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~\n0123456789");
    regexp_type re;
    re.compile (L"\\D*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\D*/ =~ \"A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~", L"$& == \"A-Za-z...\"");
}

void
test_word (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_ \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~\n");
    regexp_type re;
    re.compile (L"\\w*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\w*/ =~ \"0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_", L"$& == \"0-9A-Za-z_\"");
}

void
test_nword (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L" \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_\n");
    regexp_type re;
    re.compile (L"\\W*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\W*/ =~ \" \\t!\\\"#$%&'()*+,-./:;<=>\x3f@[\\\\]^`{|}~0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L" \t!\"#$%&'()*+,-./:;<=>\x3f@[\\]^`{|}~", L"$& == \" \\t!\\\"#$%&'()*+,-./:;<=>\x3f@[\\\\]^`{|}~\"");
}

void
test_nword_2 (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L" \t!@#$%^&*()-+=|~\n0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_");
    regexp_type re;
    re.compile (L"\\W*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\W*/ =~ \" \\t!@#$%^&*()-+=|~\\n0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L" \t!@#$%^&*()-+=|~", L"$& == \" \\t!@#$%^&*()-+=|~\\n\"");
}

void
test_space (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L" \t\n0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_!@#$%^&*()-+=|~");
    regexp_type re;
    re.compile (L"\\s*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\s*/ =~ \" \\t\\n0-9A-Za-z...\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L" \t\n", L"$& == \" \\t\\n\"");
}

void
test_nspace (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_!@#$%^&*()-+=|~ \t\n");
    regexp_type re;
    re.compile (L"\\S*");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\S*/ =~ \"0-9A-Za-z... \\t\\n\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_!@#$%^&*()-+=|~", L"$& == \"0-9A-Za-z...\"");
}

void
test_escape (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L".+*?()| \t\n");
    regexp_type re;
    re.compile (L"\\.\\+\\*\\?\\(\\)\\|\\ \\t\\n");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/\\.\\+\\*\\?\\(\\)\\|\\ \\t\\n/ =~ \".+*?()| \\t\\n\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L".+*?()| \t\n", L"$& == \".+*?()| \\t\\n\"");
}

void
test_ncclass_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"[abc]");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/[abc]/ !~ \"\"");
}

void
test_cclass_abc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"[abc]+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[abc]+/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abc", L"$& == \"abc\"");
}

void
test_cclass_a_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"[a-c]+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[a-c]+/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abc", L"$& == \"abc\"");
}

void
test_nncclass_empty (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"");
    regexp_type re;
    re.compile (L"[^abc]");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (! r, L"qr/[^abc]/ !~ \"\"");
}

void
test_ncclass_abc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"defefabcdcecf");
    regexp_type re;
    re.compile (L"[^abc]+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[^abc]+/ =~ \"defefabcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"defef", L"$& == \"defef\"");
}

void
test_ncclass_a_c (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"defefabcdcecf");
    regexp_type re;
    re.compile (L"[^a-c]+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[^a-c]+/ =~ \"defefabcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"defef", L"$& == \"defef\"");
}

void
test_cclass_minus (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"--- abc");
    regexp_type re;
    re.compile (L"[-]+");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[-]+/ =~ \"--- abc\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"---", L"$& == \"---\"");
}

void
test_cclass_bracket (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"[[abc]] def");
    regexp_type re;
    re.compile (L"[[][[]\\w+[]][]]");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/[[][[]\\w+[]][]]/ =~ \"[[abc]] def\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"[[abc]]", L"$& == \"[[abc]]\"");
}

void
test_astarc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a(.*)c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a(.*)c/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abcdcec", L"$& == \"abcdcec\"");
    std::wstring m1(s.begin () + m[2], s.begin () + m[3]);
    ts.ok (m1 == L"bcdce", L"$1 == \"bcdce\"");
}

void
test_aplusc (test::simple& ts)
{
    std::vector<std::size_t> m;
    std::wstring s (L"abcdcecf");
    regexp_type re;
    re.compile (L"a(.+)c");
    bool r = re.execute (s.cbegin (), s.cbegin (), s.cend (), m);
    ts.ok (r, L"qr/a(.+)c/ =~ \"abcdcecf\"");
    std::wstring m0(s.begin () + m[0], s.begin () + m[1]);
    ts.ok (m0 == L"abcdcec", L"$& == \"abcdcec\"");
    std::wstring m1(s.begin () + m[2], s.begin () + m[3]);
    ts.ok (m1 == L"bcdce", L"$1 == \"bcdce\"");
}

int main (int argc, char* argv[])
{
    std::locale::global (std::locale (""));
    std::wcout.imbue (std::locale (""));

    test::simple ts (133);

    test_nchar_empty (ts);
    test_char_a (ts);
    test_nchar_b (ts);

    test_nalt_empty (ts);
    test_alt_a (ts);
    test_alt_b (ts);
    test_alt_c (ts);
    test_nalt_d (ts);

    test_nseq_empty (ts);
    test_seq_abc (ts);
    test_nseq_a (ts);
    test_nseq_bcd (ts);

    test_nalt_seq_empty (ts);
    test_alt_seq_ab (ts);
    test_alt_seq_c (ts);
    test_nalt_seq_d (ts);

    test_alt_seq_e_empty (ts);
    test_alt_seq_e_ab (ts);
    test_alt_seq_e_c (ts);

    test_ngroup_empty (ts);
    test_ngroup_a (ts);
    test_group_ab (ts);
    test_group_c (ts);

    test_opt_empty (ts);
    test_opt_a (ts);
    test_opt_b (ts);

    test_exp_opt_empty (ts);
    test_exp_opt_a (ts);
    test_exp_opt_b (ts);
    test_exp_opt_c (ts);

    test_star_empty (ts);
    test_star_a (ts);
    test_star_a3 (ts);
    test_star_b (ts);

    test_star_2_empty (ts);
    test_star_2_abccababccc (ts);
    test_star_2_d (ts);

    test_nplus_empty (ts);
    test_plus_a (ts);
    test_plus_a3 (ts);
    test_nplus_b (ts);

    test_bos_empty (ts);
    test_bos_a (ts);
    test_nbos_b (ts);

    test_eos_empty (ts);
    test_eos_abcdcecf (ts);
    test_neos_abcdcecf (ts);

    test_nboundary_empty (ts);
    test_boundary_abc (ts);
    test_nboundary_a_bc (ts);
    test_boundary_abc_b (ts);

    test_empty (ts);
    test_nempty (ts);

    test_nany_empty (ts);
    test_any_a (ts);
    test_any_ascii (ts);

    test_digit (ts);
    test_ndigit (ts);
    test_ndigit_2 (ts);

    test_word (ts);
    test_nword (ts);
    test_nword_2 (ts);

    test_space (ts);
    test_nspace (ts);

    test_escape (ts);

    test_ncclass_empty (ts);
    test_cclass_abc (ts);
    test_cclass_a_c (ts);
    test_nncclass_empty (ts);
    test_ncclass_abc (ts);
    test_ncclass_a_c (ts);

    test_cclass_minus (ts);
    test_cclass_bracket (ts);

    test_astarc (ts);
    test_aplusc (ts);

    return ts.done_testing ();
}

