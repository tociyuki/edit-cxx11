#include "taptests.hpp"
#include "edit.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>

std::wstring inspect (
    std::wstring::const_iterator s, std::wstring::const_iterator const e)
{
    std::wstring t;
    t.push_back ('"');
    for (; s < e; ++s)
        switch (*s) {
        default: t.push_back (*s); break;
        case '\t': t.append (L"\\t"); break;
        case '\n': t.append (L"\\n"); break;
        case '\\': t.append (L"\\\\"); break;
        case '"': t.append (L"\\\""); break;
        }
    t.push_back ('"');
    return t;
}

std::wstring inspect (std::wstring const& str)
{
    return inspect (str.cbegin (), str.cend ());
}

struct buffer_test_type {
    test::simple& ts;
    buffer_type& buffer;
    buffer_test_type (test::simple& t, buffer_type& b) : ts (t), buffer (b) {}

    void is (int line, std::wstring const& expected)
    {
        bool ok = false;
        if (1 <= line && line <= buffer.dollar ()) {
            std::wstring::const_iterator s, e;
            buffer.get (line, s, e);
            ok = (std::wstring (s, e) == expected);
        }
        std::wstring name = std::to_wstring (line) + L" " + inspect (expected);
        ts.ok (ok, name);
    }
};

bool
readfile (std::wstring const& file, std::wstring& doc)
{
    if (file.empty ())
        return false;
    std::ifstream fs;
    fs.open (editor_type::encode (file));
    if (! fs.is_open ())
        return false;
    std::string s;
    char b[2048];
    int n;
    while ((n = fs.read (b, sizeof (b)).gcount ()) > 0) {
        s.append (b, n);
    }
    fs.close ();
    doc = editor_type::decode (s);
    return true;
}

void
test_a (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    std::wstring cmd1 (L"a\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    in.str (L"quick brown fox\nthe lazy dog\n.\n");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'a', L"edit a/quick.../");
    ts.ok (buffer.dot () == 2, L".== 2 a/quick.../");
    ts.ok (buffer.dollar () == 2, L"$== 2 a/quick.../");
    ts.ok (out.str () == L"", L"no output");

    std::wstring cmd2 (L"1a\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    in.str (L"jumps over\n.\n");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'a', L"edit 1a/jumps.../");
    ts.ok (buffer.dot () == 2, L".== 2 1a/jumps.../");
    ts.ok (buffer.dollar () == 3, L"$== 3 1a/jumps.../");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick brown fox\n");
    text.is (2, L"jumps over\n");
    text.is (3, L"the lazy dog\n");
}

void
test_c (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick\nthe lazy dog\n");

    std::wstring cmd1 (L"1c\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    in.str (L"quick brown fox\njumps over\n.\n");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'c', L"edit 1c/quick.../");
    ts.ok (buffer.dot () == 2, L".== 2 1c/quick.../");
    ts.ok (buffer.dollar () == 3, L"$== 3 1c/quick.../");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick brown fox\n");
    text.is (2, L"jumps over\n");
    text.is (3, L"the lazy dog\n");
}

void
test_d (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (),
        L"quick brown fox\njumps 1\njumps 2\njumps over\nthe lazy dog\n");

    std::wstring cmd1 (L"2,3d\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'd', L"edit 2,3d");
    ts.ok (buffer.dot () == 2, L".== 2 1c/quick.../");
    ts.ok (buffer.dollar () == 3, L"$== 3 1c/quick.../");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick brown fox\n");
    text.is (2, L"jumps over\n");
    text.is (3, L"the lazy dog\n");
}

void
test_i (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd1 (L"2i\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    in.str (L"jumps over\n.\n");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'i', L"edit 2i/jumps.../");
    ts.ok (buffer.dot () == 2, L".== 2 2i/jumps.../");
    ts.ok (buffer.dollar () == 3, L"$== 3 2i/jumps.../");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick brown fox\n");
    text.is (2, L"jumps over\n");
    text.is (3, L"the lazy dog\n");
}

void
test_j (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (),
        L"quick brown\nfox\njumps\nover\nthe lazy dog\n");

    std::wstring cmd1 (L"1,4j/ /\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'j', L"edit 1,4j/ /");
    ts.ok (buffer.dot () == 1, L".== 1 1,4j/ /");
    ts.ok (buffer.dollar () == 2, L"$== 2 1,4j/ /");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick brown fox jumps over\n");
    text.is (2, L"the lazy dog\n");
}

void
test_m (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L"3,5m6\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'm', L"edit 3,5m6");
    ts.ok (buffer.dot () == 6, L".== 6 3,5m6");
    ts.ok (buffer.dollar () == 7, L"$== 7 3,5m6");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick\n");
    text.is (2, L"brown\n");
    text.is (3, L"lazy\n");
    text.is (4, L"fox\n");
    text.is (5, L"jumps\n");
    text.is (6, L"over\n");
    text.is (7, L"dog\n");

    std::wstring cmd2 (L"3,5m1\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'm', L"edit 3,5m1");
    ts.ok (buffer.dot () == 4, L".== 4 3,5m1");
    ts.ok (buffer.dollar () == 7, L"$== 7 3,5m1");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick\n");
    text.is (2, L"lazy\n");
    text.is (3, L"fox\n");
    text.is (4, L"jumps\n");
    text.is (5, L"brown\n");
    text.is (6, L"over\n");
    text.is (7, L"dog\n");
}

void
test_n (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L",n\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'n', L"edit ,n");
    ts.ok (buffer.dot () == 7, L".== 7 ,n");
    ts.ok (buffer.dollar () == 7, L"$== 7 ,n");
    ts.ok (out.str () == 
        L"      1 quick\n"
        L"      2 brown\n"
        L"      3 fox\n"
        L"      4 jumps\n"
        L"      5 over\n"
        L"      6 lazy\n"
        L"      7 dog\n", L"output ,n");
}

void
test_p (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L",p\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'p', L"edit ,p");
    ts.ok (buffer.dot () == 7, L".== 7 ,p");
    ts.ok (buffer.dollar () == 7, L"$== 7 ,p");
    ts.ok (out.str () == L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n",
        L"output ,p");

    std::wstring cmd2 (L"0 /fox/p\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'p', L"edit /fox/p");
    ts.ok (buffer.dot () == 3, L".== 3 /fox/p");
    ts.ok (buffer.dollar () == 7, L"$== 7 /fox/p");
    ts.ok (out.str () == L"fox\n", L"output /fox/p");

    std::wstring cmd3 (L"$ /fox/,/jumps/p\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'p', L"edit $ /fox/,/jumps/p");
    ts.ok (buffer.dot () == 4, L".== 4 $ /fox/,/jumps/p");
    ts.ok (buffer.dollar () == 7, L"$== 7 $ /fox/,/jumps/p");
    ts.ok (out.str () == L"fox\njumps\n", L"output $ /fox/,/jumps/p");
}

void
test_s (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick brown fox\njumps over\nlazy dog\nfoo foo foo foo\n");

    std::wstring cmd1 (L"/lazy/s//the $&/\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 's', L"edit /lazy/s//the $&/");
    ts.ok (buffer.dot () == 3, L".== 3 /lazy/s//the $&/");
    ts.ok (buffer.dollar () == 4, L"$== 4 /lazy/s//the $&/");
    ts.ok (out.str () == L"", L"output /lazy/s//the $&/");
    text.is (3, L"the lazy dog\n");

    std::wstring cmd2 (L"4s{foo}{Foo}g\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 's', L"edit 4s{foo}{Foo}g");
    ts.ok (buffer.dot () == 4, L".== 4 4s{foo}{Foo}g");
    ts.ok (buffer.dollar () == 4, L"$== 4 4s{foo}{Foo}g");
    ts.ok (out.str () == L"", L"output 4s{foo}{Foo}g");
    text.is (4, L"Foo Foo Foo Foo\n");

    std::wstring cmd3 (L"1s/q/Q/p\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 's', L"edit 1s/q/Q/p");
    ts.ok (buffer.dot () == 1, L".== 1 1s/q/Q/p");
    ts.ok (buffer.dollar () == 4, L"$== 4 1s/q/Q/p");
    ts.ok (out.str () == L"Quick brown fox\n", L"output 1s/q/Q/p");
    text.is (1, L"Quick brown fox\n");

    std::wstring cmd4 (L"4s/^/# /p\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == 's', L"edit 4s/^/# /p");
    ts.ok (buffer.dot () == 4, L".== 4 4s/^/# /p");
    ts.ok (buffer.dollar () == 4, L"$== 4 4s/^/# /p");
    ts.ok (out.str () == L"# Foo Foo Foo Foo\n", L"output 4s/^/# /p");

    std::wstring cmd5 (L"3s/$/./p\n");
    std::wstring::const_iterator s5 = cmd5.cbegin ();
    out.str (L"");
    int r5 = editor.edit (s5);
    ts.ok (r5 == 's', L"edit 3s/$/./p");
    ts.ok (buffer.dot () == 3, L".== 3 3s/$/./p");
    ts.ok (buffer.dollar () == 4, L"$== 4 3s/$/./p");
    ts.ok (out.str () == L"the lazy dog.\n", L"output 3s/$/./p");

    std::wstring cmd6 (L"1s/ /\\n/gp\n");
    std::wstring::const_iterator s6 = cmd6.cbegin ();
    out.str (L"");
    int r6 = editor.edit (s6);
    ts.ok (r6 == 's', L"edit 1s/ /\\n/gp");
    ts.ok (buffer.dot () == 3, L".== 3 1s/ /\\n/gp");
    ts.ok (buffer.dollar () == 6, L"$== 6 1s/ /\\n/gp");
    ts.ok (out.str () == L"fox\n", L"output 1s/ /\\n/gp");
    text.is (1, L"Quick\n");
    text.is (2, L"brown\n");
    text.is (3, L"fox\n");
    text.is (4, L"jumps over\n");
    text.is (5, L"the lazy dog.\n");
    text.is (6, L"# Foo Foo Foo Foo\n");
}

void
test_t (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L"3,5t6\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    int r1 = editor.edit (s1);
    ts.ok (r1 == 't', L"edit 3,5t6");
    ts.ok (buffer.dot () == 9, L".== 9 3,5t6");
    ts.ok (buffer.dollar () == 10, L"$== 10 3,5t6");
    ts.ok (out.str () == L"", L"no output");
    text.is (1, L"quick\n");
    text.is (2, L"brown\n");
    text.is (3, L"fox\n");
    text.is (4, L"jumps\n");
    text.is (5, L"over\n");
    text.is (6, L"lazy\n");
    text.is (7, L"fox\n");
    text.is (8, L"jumps\n");
    text.is (9, L"over\n");
    text.is (10, L"dog\n");
}

void
test_equal (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L"=\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '=', L"edit =");
    ts.ok (buffer.dot () == 7, L".== 7 =");
    ts.ok (buffer.dollar () == 7, L"$== 7 =");
    ts.ok (out.str () == L"7\n", L"output =");

    std::wstring cmd2 (L"0 /fox/=\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == '=', L"edit 0 /fox/=");
    ts.ok (buffer.dot () == 3, L".== 3 0 /fox/=");
    ts.ok (buffer.dollar () == 7, L"$== 7 0 /fox/=");
    ts.ok (out.str () == L"3\n", L"output 0 /fox/=");

    std::wstring cmd3 (L"$ /fox/,/jumps/=\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == '=', L"edit $ /fox/,/jumps/=");
    ts.ok (buffer.dot () == 4, L".== 4 $ /fox/,/jumps/=");
    ts.ok (buffer.dollar () == 7, L"$== 7 $ /fox/,/jumps/=");
    ts.ok (out.str () == L"4\n", L"output $ /fox/,/jumps/=");

    std::wstring cmd4 (L".=p\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == '=', L"edit .=p");
    ts.ok (buffer.dot () == 4, L".== 4 .=p");
    ts.ok (buffer.dollar () == 7, L"$== 7 .=p");
    ts.ok (out.str () == L"4\njumps\n", L"output .=p");
}

void
test_newline (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"quick\nbrown\nfox\njumps\nover\nlazy\ndog\n");

    std::wstring cmd1 (L"1\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '\n', L"edit 1");
    ts.ok (buffer.dot () == 1, L".== 1 1");
    ts.ok (buffer.dollar () == 7, L"$== 7 1");
    ts.ok (out.str () == L"quick\n", L"output 1");

    std::wstring cmd2 (L"\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == '\n', L"edit newline*1");
    ts.ok (buffer.dot () == 2, L".== 2 newline*1");
    ts.ok (buffer.dollar () == 7, L"$== 7 newline*1");
    ts.ok (out.str () == L"brown\n", L"output newline*1");

    std::wstring cmd3 (L"\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == '\n', L"edit newline*2");
    ts.ok (buffer.dot () == 3, L".== 3 newline*2");
    ts.ok (buffer.dollar () == 7, L"$== 7 newline*2");
    ts.ok (out.str () == L"fox\n", L"output newline*2");

    std::wstring cmd4 (L"$\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == '\n', L"edit $");
    ts.ok (buffer.dot () == 7, L".== 7 $");
    ts.ok (buffer.dollar () == 7, L"$== 7 $");
    ts.ok (out.str () == L"dog\n", L"output $");

    std::wstring cmd5 (L"\n");
    std::wstring::const_iterator s5 = cmd5.cbegin ();
    out.str (L"");
    int r5 = editor.edit (s5);
    ts.ok (r5 == '\n', L"edit newline*3");
    ts.ok (buffer.dot () == 1, L".== 1 newline*3");
    ts.ok (buffer.dollar () == 7, L"$== 7 newline*3");
    ts.ok (out.str () == L"quick\n", L"output newline*3");
}

void
test_g (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"foo 1\n\nbar 1\nfoo 2\nfoo 3\n \nfoo 4\nbar 2\n");

    std::wstring cmd1 (L"g/foo/p\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'g', L"edit g/foo/p");
    ts.ok (buffer.dot () == 7, L".== 7 g/foo/p");
    ts.ok (buffer.dollar () == 8, L"$== 8 g/foo/p");
    ts.ok (out.str () == L"foo 1\nfoo 2\nfoo 3\nfoo 4\n", L"output g/foo/p");

    std::wstring cmd2 (L"g/^ *$/+2p\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'g', L"edit g/^ *$/+2p");
    ts.ok (buffer.dot () == 8, L".== 8 g/^ *$/+2p");
    ts.ok (buffer.dollar () == 8, L"$== 8 g/^ *$/+2p");
    ts.ok (out.str () == L"foo 2\nbar 2\n", L"output g/^ *$/+2p");

    std::wstring cmd3 (L"g/foo/s/$/!/\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'g', L"edit g/foo/s/$/!/");
    ts.ok (buffer.dot () == 7, L".== 7 g/foo/s/$/!/");
    ts.ok (buffer.dollar () == 8, L"$== 8 g/foo/s/$/!/");
    ts.ok (out.str () == L"", L"output g/foo/s/$/!/");

    std::wstring cmd4 (L",p\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == 'p', L"edit ,p");
    ts.ok (out.str () == L"foo 1!\n\nbar 1\nfoo 2!\nfoo 3!\n \nfoo 4!\nbar 2\n", L"output ,p");
}

void
test_v (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"foo 1\n\nbar 1\nfoo 2\nfoo 3\n \nfoo 4\nbar 2\n");

    std::wstring cmd1 (L"v/foo/p\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'v', L"edit v/foo/p");
    ts.ok (buffer.dot () == 8, L".== 8 v/foo/p");
    ts.ok (buffer.dollar () == 8, L"$== 8 v/foo/p");
    ts.ok (out.str () == L"\nbar 1\n \nbar 2\n", L"output v/foo/p");

    std::wstring cmd2 (L"v/foo/s/$/!/\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'v', L"edit v/foo/s/$/!/");
    ts.ok (buffer.dot () == 8, L".== 8 v/foo/s/$/!/");
    ts.ok (buffer.dollar () == 8, L"$== 8 v/foo/s/$/!/");
    ts.ok (out.str () == L"", L"output v/foo/s/$/!/");

    std::wstring cmd3 (L",p\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'p', L"edit ,p");
    ts.ok (out.str () == L"foo 1\n!\nbar 1!\nfoo 2\nfoo 3\n !\nfoo 4\nbar 2!\n", L"output ,p");
}

void
test_find (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (), L"foo 1\n  foo 2\n \n\n  \nfoo 3\n");

    std::wstring cmd1 (L"/foo/=p\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '=', L"edit /foo/=p");
    ts.ok (buffer.dot () == 1, L".== 1 /foo/=p");
    ts.ok (out.str () == L"1\nfoo 1\n", L"output /foo/=p");

    std::wstring cmd2 (L"/^foo/=p\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == '=', L"edit /^foo/=p");
    ts.ok (buffer.dot () == 6, L".== 6 /^foo/=p");
    ts.ok (out.str () == L"6\nfoo 3\n", L"output /^foo/=p");

    std::wstring cmd3 (L"/^$/=p\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == '=', L"edit /^$/=p");
    ts.ok (buffer.dot () == 4, L".== 4 /^$/=p");
    ts.ok (out.str () == L"4\n\n", L"output /^$/=p");

    std::wstring cmd4 (L"0+/foo/;+3p\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == 'p', L"edit 0+/foo/;+3p");
    ts.ok (buffer.dot () == 4, L".== 4 0+/foo/;+3p");
    ts.ok (out.str () == L"foo 1\n  foo 2\n \n\n", L"output 0+/foo/;+3p");
}

void
test_substitute (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    buffer_test_type text (ts, buffer);
    editor_type editor (in, out, scanner, buffer);
    buffer.append (buffer.dot (),
        L"brown quick fox\njumps over\ndog\n");

    std::wstring cmd1 (L"1s/(\\w+) (\\w+)/$2 $1/p\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 's', L"edit 1s/(\\w+) (\\w+)/$2 $1/p");
    ts.ok (buffer.dot () == 1, L".== 1 1s/(\\w+) (\\w+)/$2 $1/p");
    ts.ok (out.str () == L"quick brown fox\n", L"output 1s/(\\w+) (\\w+)/$2 $1/p");
    text.is (1, L"quick brown fox\n");

    std::wstring cmd2 (L"3s/dog/the lazy $&/p\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 's', L"edit 3s/dog/the lazy $&/p");
    ts.ok (buffer.dot () == 3, L".== 3 3s/dog/the lazy $&/p");
    ts.ok (out.str () == L"the lazy dog\n", L"output 3s/dog/the lazy $&/p");
    text.is (3, L"the lazy dog\n");

    std::wstring cmd3 (L"3s/$/./gp\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 's', L"edit 3s/$/./gp");
    ts.ok (buffer.dot () == 3, L".== 3 3s/$/./gp");
    ts.ok (out.str () == L"the lazy dog.\n", L"output 3s/$/./gp");
    text.is (3, L"the lazy dog.\n");

    std::wstring cmd4 (L"3s/^/:/gp\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == 's', L"edit 3s/^/:/gp");
    ts.ok (buffer.dot () == 3, L".== 3 3s/^/:/gp");
    ts.ok (out.str () == L":the lazy dog.\n", L"output 3s/^/:/gp");
    text.is (3, L":the lazy dog.\n");

    buffer.change (1, buffer.dollar (), L"we have mispell and misspell.\n");

    std::wstring cmd5 (L"s/mispell/misspell/gp\n");
    std::wstring::const_iterator s5 = cmd5.cbegin ();
    out.str (L"");
    int r5 = editor.edit (s5);
    ts.ok (r5 == 's', L"edit s/mispell/misspell/gp");
    ts.ok (buffer.dot () == 1, L".== 1 s/mispell/misspell/gp");
    ts.ok (out.str () == L"we have misspell and misspell.\n", L"output s/mispell/misspell/gp");

    buffer.append (buffer.dollar (), L"it is very hot day.\n");
    std::wstring cmd6 (L"s/very/$&, $&, $&,/gp\n");
    std::wstring::const_iterator s6 = cmd6.cbegin ();
    out.str (L"");
    int r6 = editor.edit (s6);
    ts.ok (r6 == 's', L"edit s/very/$&, $&, $&,/gp");
    ts.ok (buffer.dot () == 2, L".== 2 s/very/$&, $&, $&,/gp");
    ts.ok (out.str () == L"it is very, very, very, hot day.\n", L"output s/very/$&, $&, $&,/gp");

    std::wstring cmd7 (L"s/very, //gp\n");
    std::wstring::const_iterator s7 = cmd7.cbegin ();
    out.str (L"");
    int r7 = editor.edit (s7);
    ts.ok (r7 == 's', L"edit s/very, //gp");
    ts.ok (buffer.dot () == 2, L".== 2 s/very, //gp");
    ts.ok (out.str () == L"it is hot day.\n", L"output s/very, //gp");

    buffer.append (buffer.dollar (), L"extra spaces      \n");
    std::wstring cmd8 (L"s/ *$//gp\n");
    std::wstring::const_iterator s8 = cmd8.cbegin ();
    out.str (L"");
    int r8 = editor.edit (s8);
    ts.ok (r8 == 's', L"edit s/ *$//gp");
    ts.ok (buffer.dot () == 3, L".== 3 s/ *$//gp");
    ts.ok (out.str () == L"extra spaces\n", L"output s/ *$//gp");

    buffer.append (buffer.dollar (), L"# disable aaaab active.\n");
    std::wstring cmd9 (L"s/active/in$&/gp\n");
    std::wstring::const_iterator s9 = cmd9.cbegin ();
    out.str (L"");
    int r9 = editor.edit (s9);
    ts.ok (r9 == 's', L"edit s/active/in$&/gp");
    ts.ok (buffer.dot () == 4, L".== 4 s/active/in$&/gp");
    ts.ok (out.str () == L"# disable aaaab inactive.\n", L"output s/active/in$&/gp");

    std::wstring cmd10 (L"s/able/$&-bodied/gp\n");
    std::wstring::const_iterator s10 = cmd10.cbegin ();
    out.str (L"");
    int r10 = editor.edit (s10);
    ts.ok (r10 == 's', L"edit s/able/$&-bodied/gp");
    ts.ok (buffer.dot () == 4, L".== 4 s/able/$&-bodied/gp");
    ts.ok (out.str () == L"# disable-bodied aaaab inactive.\n", L"output s/able/$&-bodied/gp");

    std::wstring cmd11 (L"s/a+b/($&)/gp\n");
    std::wstring::const_iterator s11 = cmd11.cbegin ();
    out.str (L"");
    int r11 = editor.edit (s11);
    ts.ok (r11 == 's', L"edit s/a+b/($&)/gp");
    ts.ok (buffer.dot () == 4, L".== 4 s/a+b/($&)/gp");
    ts.ok (out.str () == L"# dis(ab)le-bodied (aaaab) inactive.\n", L"output s/a+b/($&)/gp");

    buffer.append (buffer.dollar (), L"xy\n");
    std::wstring cmd12 (L"s/a*/A/gp\n");
    std::wstring::const_iterator s12 = cmd12.cbegin ();
    out.str (L"");
    int r12 = editor.edit (s12);
    ts.ok (r12 == 's', L"edit s/a*/A/gp");
    ts.ok (buffer.dot () == 5, L".== 5 s/a*/A/gp");
    ts.ok (out.str () == L"AxAyA\n", L"output s/a*/A/gp");

    buffer.append (buffer.dollar (), L"xay\n");
    std::wstring cmd13 (L"s/a*/A/gp\n");
    std::wstring::const_iterator s13 = cmd13.cbegin ();
    out.str (L"");
    int r13 = editor.edit (s13);
    ts.ok (r13 == 's', L"edit s/a*/A/gp");
    ts.ok (buffer.dot () == 6, L".== 6 s/a*/A/gp");
    ts.ok (out.str () == L"AxAyA\n", L"output s/a*/A/gp");

    buffer.append (buffer.dollar (), L"xaaaaaay\n");
    std::wstring cmd14 (L"s/a*/A/gp\n");
    std::wstring::const_iterator s14 = cmd14.cbegin ();
    out.str (L"");
    int r14 = editor.edit (s14);
    ts.ok (r14 == 's', L"edit s/a*/A/gp");
    ts.ok (buffer.dot () == 7, L".== 7 s/a*/A/gp");
    ts.ok (out.str () == L"AxAyA\n", L"output s/a*/A/gp");
}

void
test_e (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd1 (L"e\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == 'e', L"edit e");
    ts.ok (buffer.dot () == 0, L".== 0 e");
    ts.ok (buffer.dollar () == 0, L"$== 0 e");
    ts.ok (out.str () == L"", L"output e");

    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd2 (L"e tests/foo.txt\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'e', L"edit e tests/foo.txt");
    ts.ok (buffer.dot () == 3, L".== 3 e tests/foo.txt");
    ts.ok (out.str () == L"", L"output e tests/foo.txt");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");

    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd3 (L"e\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'e', L"edit e");
    ts.ok (buffer.dot () == 3, L".== 3 e");
    ts.ok (out.str () == L"", L"output e");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");
}

void
test_r (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd1 (L"r\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '?', L"edit r");
    ts.ok (buffer.dot () == 2, L".== 2 r");
    ts.ok (buffer.dollar () == 2, L"$== 2 r");
    ts.ok (out.str () == L"", L"output r");

    std::wstring cmd2 (L"r tests/foo.txt\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'r', L"edit r tests/foo.txt");
    ts.ok (buffer.dot () == 5, L".== 5 r tests/foo.txt");
    ts.ok (out.str () == L"", L"output r tests/foo.txt");
    text.is (1, L"quick brown fox\n");
    text.is (2, L"the lazy dog\n");
    text.is (3, L"foo 1\n");
    text.is (4, L"foo 2\n");
    text.is (5, L"foo 3\n");

    std::wstring cmd3 (L"0r\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'r', L"edit 0r");
    ts.ok (buffer.dot () == 3, L".== 3 0r");
    ts.ok (out.str () == L"", L"output 0r");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");
    text.is (4, L"quick brown fox\n");
    text.is (5, L"the lazy dog\n");
    text.is (6, L"foo 1\n");
    text.is (7, L"foo 2\n");
    text.is (8, L"foo 3\n");
}

void
test_w (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd1 (L"w\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '?', L"edit w");
    ts.ok (buffer.dot () == 2, L".== 2 w");
    ts.ok (buffer.dollar () == 2, L"$== 2 w");
    ts.ok (out.str () == L"", L"output w");

    std::wstring cmd2 (L"w tests/bar.txt\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'w', L"edit w tests/bar.txt");
    ts.ok (buffer.dot () == 2, L".== 2 w tests/bar.txt");
    ts.ok (buffer.dollar () == 2, L"$== 2 w tests/bar.txt");
    ts.ok (out.str () == L"", L"output w tests/bar.txt");
    ts.ok (buffer.file () == L"tests/bar.txt", L"f==tests/bar.txt");
    std::wstring doc2;
    bool r2b = readfile (L"tests/bar.txt", doc2);
    ts.ok (r2b && doc2 == L"quick brown fox\nthe lazy dog\n",
        L"slurp w tests/bar.txt");

    buffer.append (1, L"jumps over\n");
    std::wstring cmd3 (L"w\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'w', L"edit w");
    ts.ok (buffer.dot () == 2, L".== 2 w");
    ts.ok (buffer.dollar () == 3, L"$== 3 w");
    ts.ok (out.str () == L"", L"output w");
    ts.ok (buffer.file () == L"tests/bar.txt", L"f==tests/bar.txt");
    std::wstring doc3;
    bool r3b = readfile (L"tests/bar.txt", doc3);
    ts.ok (r3b && doc3 == L"quick brown fox\njumps over\nthe lazy dog\n",
        L"slurp w");

    std::wstring cmd4d (L",d\n");
    std::wstring::const_iterator s4d = cmd4d.cbegin ();
    editor.edit (s4d);
    std::wstring cmd4 (L"w\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == '?', L"edit w # could not write empty buffer");
    ts.ok (buffer.dot () == 0, L".== 0 w");
    ts.ok (buffer.dollar () == 0, L"$== 0 w");
    ts.ok (out.str () == L"", L"output w");
    std::wstring doc4;
    bool r4b = readfile (L"tests/bar.txt", doc4);
    ts.ok (r4b && doc4 == L"quick brown fox\njumps over\nthe lazy dog\n", L"slurp w");

    std::remove ("tests/bar.txt");
}

void
test_f (test::simple& ts)
{
    std::wistringstream in;
    std::wostringstream out;
    grammar_type scanner;
    buffer_type buffer;
    editor_type editor (in, out, scanner, buffer);
    buffer_test_type text (ts, buffer);
    buffer.append (buffer.dot (), L"quick brown fox\nthe lazy dog\n");

    std::wstring cmd1 (L"f\n");
    std::wstring::const_iterator s1 = cmd1.cbegin ();
    out.str (L"");
    int r1 = editor.edit (s1);
    ts.ok (r1 == '?', L"edit f");
    ts.ok (buffer.dot () == 2, L".== 2 f");
    ts.ok (buffer.dollar () == 2, L"$== 2 f");
    ts.ok (out.str () == L"", L"output f");

    std::wstring cmd2 (L"f tests/foo.txt\n");
    std::wstring::const_iterator s2 = cmd2.cbegin ();
    out.str (L"");
    int r2 = editor.edit (s2);
    ts.ok (r2 == 'f', L"edit f tests/foo.txt");
    ts.ok (buffer.dot () == 2, L".== 2 f tests/foo.txt");
    ts.ok (buffer.dollar () == 2, L"$== 2 f tests/foo.txt");
    ts.ok (out.str () == L"tests/foo.txt\n", L"output f tests/foo.txt");

    std::wstring cmd3 (L"e\n");
    std::wstring::const_iterator s3 = cmd3.cbegin ();
    out.str (L"");
    int r3 = editor.edit (s3);
    ts.ok (r3 == 'e', L"edit e");
    ts.ok (buffer.dot () == 3, L".== 3 e");
    ts.ok (out.str () == L"", L"output e");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");

    std::wstring cmd4 (L"r\n");
    std::wstring::const_iterator s4 = cmd4.cbegin ();
    out.str (L"");
    int r4 = editor.edit (s4);
    ts.ok (r4 == 'r', L"edit r");
    ts.ok (buffer.dot () == 6, L".== 6 r");
    ts.ok (out.str () == L"", L"output r");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");
    text.is (4, L"foo 1\n");
    text.is (5, L"foo 2\n");
    text.is (6, L"foo 3\n");
}

int
main ()
{
    test::simple ts (324);

    test_a (ts);
    test_c (ts);
    test_d (ts);
    test_i (ts);
    test_j (ts);
    test_m (ts);
    test_n (ts);
    test_p (ts);
    test_s (ts);
    test_t (ts);
    test_equal (ts);
    test_newline (ts);

    test_g (ts);
    test_v (ts);

    test_find (ts);
    test_substitute (ts);

    test_e (ts);
    test_f (ts);
    test_r (ts);
    test_w (ts);

    return ts.done_testing ();
}

