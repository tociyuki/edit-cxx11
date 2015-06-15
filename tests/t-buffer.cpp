#include "taptests.hpp"
#include "edit.hpp"

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

void test_init (test::simple& ts)
{
    buffer_type buffer;
    ts.ok (buffer.dot () == 0,    L".==0 initialized");
    ts.ok (buffer.dollar () == 0, L"$==0 initialized");
    ts.ok (buffer.mark () ==0 ,   L"'==0 initialized");
    ts.ok (buffer.file () == L"", L"file==\"\" initialized");
}

void test_append (test::simple& ts)
{
    buffer_type buffer;
    buffer_test_type text (ts, buffer);

    buffer.append (buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");
    ts.ok (buffer.dot () == 5,    L".==5 .a/quick\\n../");
    ts.ok (buffer.dollar () == 5, L"$==5 .a/quick\\n../");

    buffer.append (buffer.dot (), L"foo\nbar\n");
    ts.ok (buffer.dot () == 7, L".==7 .a/foo\\n../");
    ts.ok (buffer.dollar () == 7, L"$==7 .a/foo\\n../");

    buffer.append (2, L"hoge\nfuga\n");
    ts.ok (buffer.dot () == 4, L".==4 2a/hoge\\n../");
    ts.ok (buffer.dollar () == 9, L"$==9 2a/hoge\\n../");

    buffer.append (0, L"---\n");
    ts.ok (buffer.dot () == 1, L".==1 0a/---\\n/");
    ts.ok (buffer.dollar () == 10, L"$==10 0a/---\\n/");
    text.is (1, L"---\n");
    text.is (2, L"quick\n");
    text.is (3, L"brown\n");
    text.is (4, L"hoge\n");
    text.is (5, L"fuga\n");
    text.is (6, L"fox jumps\n");
    text.is (7, L"over\n");
    text.is (8, L"the lazy dog\n");
    text.is (9, L"foo\n");
    text.is (10, L"bar\n");
}

void test_change (test::simple& ts)
{
    buffer_type buffer;
    buffer_test_type text (ts, buffer);

    buffer.append (buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");

    buffer.change (2, 3, L"brown fox\njumps\n");
    ts.ok (buffer.dot () == 3, L".==3 2,3c/brown fox\\njumps\\n/");
    ts.ok (buffer.dollar () == 5, L"$==5 2,3c/brown fox\\njumps\\n/");

    buffer.change (2, 3, L"brown\nfox\njumps\n");
    ts.ok (buffer.dot () == 4, L".==4 2,3c/brown\\nfox\\njumps\\n/");
    ts.ok (buffer.dollar () == 6, L"$==6 2,3c/brown\\nfox\\njumps\\n/");

    buffer.change (2, 4, L"brown fox jumps\n");
    ts.ok (buffer.dot () == 2, L".==2 2,3c/brown fox jumps\\n/");
    ts.ok (buffer.dollar () == 4, L"$==4 2,3c/brown fox jumps\\n/");
    text.is (1, L"quick\n");
    text.is (2, L"brown fox jumps\n");
    text.is (3, L"over\n");
    text.is (4, L"the lazy dog\n");

    buffer.change (1, buffer.dollar (), L"foo 1\nfoo 2\nfoo 3\n");
    ts.ok (buffer.dot () == 3, L".==3 1,$c/foo 1\\nfoo 2\\nfoo 3\\n/");
    ts.ok (buffer.dollar () == 3, L"$==3 1,$c/foo 1\\nfoo 2\\nfoo 3\\n/");
    text.is (1, L"foo 1\n");
    text.is (2, L"foo 2\n");
    text.is (3, L"foo 3\n");
}

void test_erase (test::simple& ts)
{
    buffer_type buffer;
    buffer_test_type text (ts, buffer);

    buffer.append (buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");
    buffer.erase (1, buffer.dollar ());
    ts.ok (buffer.dot () == 0, L".==0 1,$d");
    ts.ok (buffer.dollar () == 0, L"$==0 1,$d");

    buffer.append (buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");
    buffer.erase (1, 3);
    ts.ok (buffer.dot () == 1, L".==1 1,3d");
    ts.ok (buffer.dollar () == 2, L"$==2 1,3d");
    text.is (1, L"over\n");
    text.is (2, L"the lazy dog\n");

    buffer.change (1, buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");
    buffer.erase (3, buffer.dollar ());
    ts.ok (buffer.dot () == 2, L".==2 3,$d");
    ts.ok (buffer.dollar () == 2, L"$==2 3,$d");
    text.is (1, L"quick\n");
    text.is (2, L"brown\n");

    buffer.change (1, buffer.dot (), L"quick\nbrown\nfox jumps\nover\nthe lazy dog\n");
    buffer.erase (2, 3);
    ts.ok (buffer.dot () == 2, L".==2 2,3d");
    ts.ok (buffer.dollar () == 3, L"$==3 2,3d");
    text.is (1, L"quick\n");
    text.is (2, L"over\n");
    text.is (3, L"the lazy dog\n");
}

void test_move (test::simple& ts)
{
    buffer_type buffer;
    buffer_test_type text (ts, buffer);

    buffer.append (buffer.dot (), L"line 1\nline 2\nline 3\nline 4\nline 5\n"
                                  L"line 6\nline 7\nline 8\nline 9\n");
    buffer.move (7, 8, 3);
    ts.ok (buffer.dot () == 5, L".==5 7,8m3");
    ts.ok (buffer.dollar () == 9, L"$==9 7,8m3");
    text.is (1, L"line 1\n");
    text.is (2, L"line 2\n");
    text.is (3, L"line 3\n");
    text.is (4, L"line 7\n");
    text.is (5, L"line 8\n");
    text.is (6, L"line 4\n");
    text.is (7, L"line 5\n");
    text.is (8, L"line 6\n");
    text.is (9, L"line 9\n");

    buffer.move (7, 8, 0);
    ts.ok (buffer.dot () == 2, L".==2 7,8m0");
    ts.ok (buffer.dollar () == 9, L"$==9 7,8m0");
    text.is (1, L"line 5\n");
    text.is (2, L"line 6\n");
    text.is (3, L"line 1\n");
    text.is (4, L"line 2\n");
    text.is (5, L"line 3\n");
    text.is (6, L"line 7\n");
    text.is (7, L"line 8\n");
    text.is (8, L"line 4\n");
    text.is (9, L"line 9\n");

    buffer.move (2, 4, 8);
    ts.ok (buffer.dot () == 8, L".==8 2,4m8");
    ts.ok (buffer.dollar () == 9, L"$==9 2,4m8");
    text.is (1, L"line 5\n");
    text.is (2, L"line 3\n");
    text.is (3, L"line 7\n");
    text.is (4, L"line 8\n");
    text.is (5, L"line 4\n");
    text.is (6, L"line 6\n");
    text.is (7, L"line 1\n");
    text.is (8, L"line 2\n");
    text.is (9, L"line 9\n");

    buffer.move (2, 4, 9);
    ts.ok (buffer.dot () == 9, L".==8 2,4m9");
    ts.ok (buffer.dollar () == 9, L"$==9 2,4m9");
    text.is (1, L"line 5\n");
    text.is (2, L"line 4\n");
    text.is (3, L"line 6\n");
    text.is (4, L"line 1\n");
    text.is (5, L"line 2\n");
    text.is (6, L"line 9\n");
    text.is (7, L"line 3\n");
    text.is (8, L"line 7\n");
    text.is (9, L"line 8\n");
}

void test_undo (test::simple& ts)
{
    buffer_type buffer;
    buffer_test_type text (ts, buffer);

    buffer.append (buffer.dot (), L"append 1\nappend 2\nappend 3\nappend 4\n"
                                  L"append 5\nappend 6\nappend 7\nappend 8\n");
    buffer.touch ();
    buffer.append (3, L"append A\nappend B\nappend C\n");
    buffer.touch ();
    buffer.change (2, 4, L"change D\nchange E\nchange F\nchange G\n");
    buffer.touch ();
    buffer.erase (3, 7);
    ts.ok (buffer.dot () == 3, L".==3 before undo sigle");
    ts.ok (buffer.dollar () == 7, L"$==7 before undo single");
    buffer.touch ();
    text.is (1, L"append 1\n");
    text.is (2, L"change D\n");
    text.is (3, L"append 4\n");
    text.is (4, L"append 5\n");
    text.is (5, L"append 6\n");
    text.is (6, L"append 7\n");
    text.is (7, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 5, L".==5 u");
    ts.ok (buffer.dollar () == 12, L"$==12 u");
    text.is (1, L"append 1\n");
    text.is (2, L"change D\n");
    text.is (3, L"change E\n");
    text.is (4, L"change F\n");
    text.is (5, L"change G\n");
    text.is (6, L"append B\n");
    text.is (7, L"append C\n");
    text.is (8, L"append 4\n");
    text.is (9, L"append 5\n");
    text.is (10, L"append 6\n");
    text.is (11, L"append 7\n");
    text.is (12, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 6, L".==6 u");
    ts.ok (buffer.dollar () == 11, L"$==11 u");
    text.is (1, L"append 1\n");
    text.is (2, L"append 2\n");
    text.is (3, L"append 3\n");
    text.is (4, L"append A\n");
    text.is (5, L"append B\n");
    text.is (6, L"append C\n");
    text.is (7, L"append 4\n");
    text.is (8, L"append 5\n");
    text.is (9, L"append 6\n");
    text.is (10, L"append 7\n");
    text.is (11, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 8, L".==8 u");
    ts.ok (buffer.dollar () == 8, L"$==8 u");
    text.is (1, L"append 1\n");
    text.is (2, L"append 2\n");
    text.is (3, L"append 3\n");
    text.is (4, L"append 4\n");
    text.is (5, L"append 5\n");
    text.is (6, L"append 6\n");
    text.is (7, L"append 7\n");
    text.is (8, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 0, L".==0 u");
    ts.ok (buffer.dollar () == 0, L"$==0 u");

    buffer.append (buffer.dot (), L"append 1\nappend 2\nappend 3\nappend 4\n"
                                  L"append 5\nappend 6\nappend 7\nappend 8\n");
    buffer.touch ();
    buffer.change (2, 2, L"subst A\n");
    buffer.change (4, 4, L"subst B\n");
    buffer.change (6, 6, L"subst C\n");
    buffer.touch ();
    buffer.move (5, 7, 1);
    buffer.touch ();
    ts.ok (buffer.dot () == 4, L".==4 before undo multi");
    ts.ok (buffer.dollar () == 8, L"$==8 before undo multi");
    text.is (1, L"append 1\n");
    text.is (2, L"append 5\n");
    text.is (3, L"subst C\n");
    text.is (4, L"append 7\n");
    text.is (5, L"subst A\n");
    text.is (6, L"append 3\n");
    text.is (7, L"subst B\n");
    text.is (8, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 6, L".==6 u");
    ts.ok (buffer.dollar () == 8, L"$==8 u");
    text.is (1, L"append 1\n");
    text.is (2, L"subst A\n");
    text.is (3, L"append 3\n");
    text.is (4, L"subst B\n");
    text.is (5, L"append 5\n");
    text.is (6, L"subst C\n");
    text.is (7, L"append 7\n");
    text.is (8, L"append 8\n");

    buffer.undo ();
    ts.ok (buffer.dot () == 8, L".==8 u");
    ts.ok (buffer.dollar () == 8, L"$==8 u");
    text.is (1, L"append 1\n");
    text.is (2, L"append 2\n");
    text.is (3, L"append 3\n");
    text.is (4, L"append 4\n");
    text.is (5, L"append 5\n");
    text.is (6, L"append 6\n");
    text.is (7, L"append 7\n");
    text.is (8, L"append 8\n");
}

int
main ()
{
    test::simple ts (174);

    test_init (ts);
    test_append (ts);
    test_change (ts);
    test_erase (ts);
    test_move (ts);
    test_undo (ts);

    return ts.done_testing ();
}

