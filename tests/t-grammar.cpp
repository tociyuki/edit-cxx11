#include "taptests.hpp"
#include "edit.hpp"

void
test_1 (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"1\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"1");
    ts.ok (ct.naddr == 1, L"1 => naddr==1");
    ts.ok (ct.addr1.size () == 1, L"1 => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"1 => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"1 => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"a => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"1 => addr2.size()==0");
    ts.ok (ct.command == '\n', L"1 => command=='\\n'");
    ts.ok (! ct.pflag, L"1 => pflag==false");
}

void
test_a (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"a\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"a");
    ts.ok (ct.naddr == 0, L"a => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"a => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"a => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"a => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"a => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"a => addr2[0].type==DOT");
    ts.ok (ct.command == 'a', L"a => command=='a'");
    ts.ok (! ct.pflag, L"a => pflag==false");
}

void
test_c (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"c\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"c");
    ts.ok (ct.naddr == 0, L"c => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"c => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"c => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"c => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"c => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"c => addr2[0].type==DOT");
    ts.ok (ct.command == 'c', L"c => command=='c'");
    ts.ok (! ct.pflag, L"c => pflag==false");
}

void
test_d (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"d\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"d");
    ts.ok (ct.naddr == 0, L"d => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"d => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"d => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"d => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"d => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"d => addr2[0].type==DOT");
    ts.ok (ct.command == 'd', L"d => command=='d'");
    ts.ok (! ct.pflag, L"d => pflag==false");
}

void
test_dp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"dp\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"dp");
    ts.ok (ct.command == 'd', L"dp => command=='d'");
    ts.ok (ct.pflag, L"dp => pflag==true");
}

void
test_e_file (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"e foo\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"e foo");
    ts.ok (ct.naddr == 0, L"e foo => naddr==0");
    ts.ok (ct.command == 'e', L"e foo => command=='e'");
    ts.ok (ct.param == L"foo", L"e foo => patam==\"foo\"");
    ts.ok (! ct.pflag, L"e foo => pflag==false");
}

void
test_e_empty (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"e\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"e");
    ts.ok (ct.command == 'e', L"e => command=='e'");
    ts.ok (ct.param == L"", L"e => patam==\"\"");
    ts.ok (! ct.pflag, L"e => pflag==false");
}

void
test_f_file (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"f foo\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"f foo");
    ts.ok (ct.naddr == 0, L"f foo => naddr==0");
    ts.ok (ct.command == 'f', L"f foo => command=='f'");
    ts.ok (ct.param == L"foo", L"f foo => patam==\"foo\"");
    ts.ok (! ct.pflag, L"f foo => pflag==false");
}

void
test_f_empty (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"f\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"f");
    ts.ok (ct.command == 'f', L"f => command=='f'");
    ts.ok (ct.param == L"", L"f => patam==\"\"");
    ts.ok (! ct.pflag, L"f => pflag==false");
}

void
test_g (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"g/pat/\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"g/pat/");
    ts.ok (ct.naddr == 0, L"g/pat/ => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"g/pat/ => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"g/pat/ => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"g/pat/ => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"g/pat/ => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"g/pat/ => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L"g/pat/ => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'g', L"g/pat/ => command=='g'");
    ts.ok (ct.pattern == L"pat", L"g/pat/ => pattern==\"pat\"");
    ts.ok (! ct.pflag, L"g/pat/ => pflag==false");
}

void
test_i (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"i\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"i");
    ts.ok (ct.naddr == 0, L"i => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"i => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"i => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"i => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"i => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"i => addr2[0].type==DOT");
    ts.ok (ct.command == 'i', L"i => command=='i'");
    ts.ok (! ct.pflag, L"i => pflag==false");
}

void
test_j (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"j\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"j");
    ts.ok (ct.naddr == 0, L"j => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"j => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"j => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"j => comma!=';'");
    ts.ok (ct.addr2.size () == 2, L"j => addr2.size()==2");
    ts.ok (ct.addr2[0].type==DOT, L"j => addr2[0].type==DOT");
    ts.ok (ct.addr2[1].type==LINEOFFSET, L"+5p => addr2[1].type==LINEOFFSET");
    ts.ok (ct.addr2[1].disp==+1, L"j => addr2[1].disp==+1");
    ts.ok (ct.command == 'j', L"j => command=='j'");
    ts.ok (! ct.pflag, L"j => pflag==false");
}

void
test_k (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"k\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"k");
    ts.ok (ct.naddr == 0, L"k => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"k => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"k => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"k => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"k => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"k => addr2[0].type==DOT");
    ts.ok (ct.command == 'k', L"k => command=='k'");
    ts.ok (! ct.pflag, L"k => pflag==false");
}

void
test_m (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"m0\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"m0");
    ts.ok (ct.naddr == 0, L"m0 => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"m0 => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"m0 => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"m0 => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"m0 => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"m0 => addr2[0].type==DOT");
    ts.ok (ct.command == 'm', L"m0 => command=='m'");
    ts.ok (ct.addr3.size () == 1, L"m0 => addr3.size()==1");
    ts.ok (ct.addr3[0].type==LINENUM, L"m0 => addr3[0].type==LINENUM");
    ts.ok (ct.addr3[0].disp==0, L"m0 => addr3[0].disp==0");
    ts.ok (! ct.pflag, L"m0 => pflag==false");
}

void
test_mp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"m0p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"m0p");
    ts.ok (ct.command == 'm', L"m0p => command=='m'");
    ts.ok (ct.addr3.size () == 1, L"m0p => addr3.size()==1");
    ts.ok (ct.addr3[0].type==LINENUM, L"m0p => addr3[0].type==LINENUM");
    ts.ok (ct.addr3[0].disp==0, L"m0p => addr3[0].disp==0");
    ts.ok (ct.pflag, L"m0p => pflag==true");
}

void
test_n (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"n\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"n");
    ts.ok (ct.naddr == 0, L"n => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"n => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"n => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"n => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"n => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"n => addr2[0].type==DOT");
    ts.ok (ct.command == 'n', L"n => command=='n'");
    ts.ok (! ct.pflag, L"n => pflag==false");
}

void
test_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"p");
    ts.ok (ct.naddr == 0, L"p => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"p => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"p => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"p => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"p => addr2[0].type==DOT");
    ts.ok (ct.command == 'p', L"p => command=='p'");
    ts.ok (! ct.pflag, L"p => pflag==false");
}

void
test_q (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"q\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"q");
    ts.ok (ct.naddr == 0, L"q => naddr==0");
    ts.ok (ct.command == 'q', L"q => command=='q'");
    ts.ok (! ct.pflag, L"q => pflag==false");
}

void
test_r_file (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"r foo\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"r foo");
    ts.ok (ct.naddr == 0, L"r foo => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"r foo => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"r foo => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"r foo => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"r foo => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"r foo => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L"r foo => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'r', L"r foo => command=='r'");
    ts.ok (ct.param == L"foo", L"r foo => param==\"foo\"");
    ts.ok (! ct.pflag, L"r foo => pflag==false");
}

void
test_r_empty (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"r\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"r");
    ts.ok (ct.command == 'r', L"r => command=='r'");
    ts.ok (ct.param == L"", L"r => param==\"\"");
    ts.ok (! ct.pflag, L"r => pflag==false");
}

void
test_s (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"s/pat/new/\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"s/pat/new/");
    ts.ok (ct.naddr == 0, L"s/pat/new/ => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"s/pat/new/ => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"s/pat/new/ => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"s/pat/new/ => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"s/pat/new/ => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"s/pat/new/ => addr2[0].type==DOT");
    ts.ok (ct.command == 's', L"s/pat/new/ => command=='s'");
    ts.ok (ct.pattern == L"pat", L"s/pat/new/ => pattern=\"pat\"");
    ts.ok (ct.param == L"new", L"s/pat/new/ => param=\"new\"");
    ts.ok (! ct.gflag, L"s/pat/new/ => gflag==false");
    ts.ok (! ct.pflag, L"s/pat/new/ => pflag==false");
}

void
test_sg (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"s/pat/new/g\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"s/pat/new/g");
    ts.ok (ct.command == 's', L"s/pat/new/g => command=='s'");
    ts.ok (ct.pattern == L"pat", L"s/pat/new/g => pattern=\"pat\"");
    ts.ok (ct.param == L"new", L"s/pat/new/g => param=\"new\"");
    ts.ok (ct.gflag, L"s/pat/new/g => gflag==true");
    ts.ok (! ct.pflag, L"s/pat/new/g => pflag==false");
}

void
test_sp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"s/pat/new/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"s/pat/new/p");
    ts.ok (ct.command == 's', L"s/pat/new/p => command=='s'");
    ts.ok (ct.pattern == L"pat", L"s/pat/new/p => pattern=\"pat\"");
    ts.ok (ct.param == L"new", L"s/pat/new/p => param=\"new\"");
    ts.ok (! ct.gflag, L"s/pat/new/p => gflag==false");
    ts.ok (ct.pflag, L"s/pat/new/p => pflag==true");
}

void
test_sgp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"s/pat/new/gp\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"s/pat/new/gp");
    ts.ok (ct.command == 's', L"s/pat/new/gp => command=='s'");
    ts.ok (ct.pattern == L"pat", L"s/pat/new/gp => pattern=\"pat\"");
    ts.ok (ct.param == L"new", L"s/pat/new/gp => param=\"new\"");
    ts.ok (ct.gflag, L"s/pat/new/gp => gflag==true");
    ts.ok (ct.pflag, L"s/pat/new/gp => pflag==true");
}

void
test_t (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"t0\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"t0");
    ts.ok (ct.naddr == 0, L"t0 => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"t0 => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L"t0 => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L"t0 => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"t0 => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOT, L"t0 => addr2[0].type==DOT");
    ts.ok (ct.command == 't', L"t0 => command=='t'");
    ts.ok (ct.addr3.size () == 1, L"t0 => addr3.size()==1");
    ts.ok (ct.addr3[0].type==LINENUM, L"t0 => addr3[0].type==LINENUM");
    ts.ok (ct.addr3[0].disp==0, L"t0 => addr3[0].disp==0");
    ts.ok (! ct.pflag, L"t0 => pflag==false");
}

void
test_tp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"t0p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"t0p");
    ts.ok (ct.command == 't', L"t0p => command=='t'");
    ts.ok (ct.addr3.size () == 1, L"t0p => addr3.size()==1");
    ts.ok (ct.addr3[0].type==LINENUM, L"t0p => addr3[0].type==LINENUM");
    ts.ok (ct.addr3[0].disp==0, L"t0p => addr3[0].disp==0");
    ts.ok (ct.pflag, L"t0p => pflag==true");
}

void
test_u (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"u\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"u");
    ts.ok (ct.naddr == 0, L"u => naddr==0");
    ts.ok (ct.command == 'u', L"u => command=='u'");
    ts.ok (! ct.pflag, L"u => pflag==false");
}

void
test_v (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"v/pat/\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"v/pat/");
    ts.ok (ct.naddr == 0, L"v/pat/ => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"v/pat/ => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"v/pat/ => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"v/pat/ => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"v/pat/ => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"v/pat/ => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L"v/pat/ => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'v', L"v/pat/ => command=='v'");
    ts.ok (ct.pattern == L"pat", L"v/pat/ => pattern==\"pat\"");
    ts.ok (! ct.pflag, L"v/pat/ => pflag==false");
}

void
test_w_file (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"w foo\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"w foo");
    ts.ok (ct.naddr == 0, L"w foo => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"w foo => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"w foo => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"w foo => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"w foo => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"w foo => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L"w foo => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'w', L"w foo => command=='w'");
    ts.ok (ct.param == L"foo", L"w foo => param==\"foo\"");
    ts.ok (! ct.pflag, L"w foo => pflag==false");
}

void
test_w_empty (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"w\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"w");
    ts.ok (ct.command == 'w', L"w => command=='w'");
    ts.ok (ct.param == L"", L"w => param==\"\"");
    ts.ok (! ct.pflag, L"w => pflag==false");
}

void
test_equal (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"=\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"=");
    ts.ok (ct.naddr == 0, L"= => naddr==0");
    ts.ok (ct.addr1.size () == 1, L"= => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"= => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"= => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"= => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"= => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L"= => addr1[0].type==DOLLAR");
    ts.ok (ct.command == '=', L"= => command=='='");
    ts.ok (! ct.pflag, L"= => pflag==false");
}

void
test_equalp (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"=p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"=p");
    ts.ok (ct.naddr == 0, L"= => naddr==0");
    ts.ok (ct.command == '=', L"=p => command=='='");
    ts.ok (ct.pflag, L"=p => pflag==true");
}

void
test_newline (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"newline");
    ts.ok (ct.naddr == 0, L"newline => naddr==0");
    ts.ok (ct.addr1.size () == 2, L"newline => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"newline => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L"newline => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==1, L"newline => addr1[1].disp==1");
    ts.ok (ct.comma != ';', L"newline => comma!=';'");
    ts.ok (ct.addr2.size () == 2, L"newline => addr2.size()==2");
    ts.ok (ct.addr2[0].type==DOT, L"newline => addr2[0].type==DOT");
    ts.ok (ct.addr2[1].type==LINEOFFSET, L"newline => addr2[1].type==LINEOFFSET");
    ts.ok (ct.addr2[1].disp==1, L"newline => addr2[1].disp==1");
    ts.ok (ct.command == '\n', L"newline => command=='\\n'");
    ts.ok (! ct.pflag, L"newline => pflag==false");
}

void
test_dot_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L".p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L".p");
    ts.ok (ct.naddr == 1, L".p => naddr==1");
    ts.ok (ct.addr1.size () == 1, L".p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L".p => addr1[0].type==DOT");
    ts.ok (ct.comma != ';', L".p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L".p => addr2.size()==0");
    ts.ok (ct.command == 'p', L".p => command=='p'");
    ts.ok (! ct.pflag, L".p => pflag==false");
}

void
test_dollar_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"$p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"$p");
    ts.ok (ct.naddr == 1, L"$p => naddr==1");
    ts.ok (ct.addr1.size () == 1, L"$p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOLLAR, L"$p => addr1[0].type==DOLLAR");
    ts.ok (ct.comma != ';', L"$p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"$p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"$p => command=='p'");
    ts.ok (! ct.pflag, L"$p => pflag==false");
}

void
test_mark_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"'p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"'p");
    ts.ok (ct.naddr == 1, L"'p => naddr==1");
    ts.ok (ct.addr1.size () == 1, L"'p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==MARK, L"'p => addr1[0].type==MARK");
    ts.ok (ct.comma != ';', L"'p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"'p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"'p => command=='p'");
    ts.ok (! ct.pflag, L"'p => pflag==false");
}

void
test_1p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"1p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"1p");
    ts.ok (ct.naddr == 1, L"1p => naddr==1");
    ts.ok (ct.addr1.size () == 1, L"1p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"1p => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"1p => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"1p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"1p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"1p => command=='p'");
    ts.ok (! ct.pflag, L"1p => pflag==false");
}

void
test_dot_plus5_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L".+5p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L".+5p");
    ts.ok (ct.naddr == 1, L".+5p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L".+5p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L".+5p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L".+5p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==5, L".+5p => addr1[1].disp==5");
    ts.ok (ct.comma != ';', L".+5p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L".+5p => addr2.size()==0");
    ts.ok (ct.command == 'p', L".+5p => command=='p'");
    ts.ok (! ct.pflag, L".+5p => pflag==false");
}

void
test_plus5_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"+5p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"+5p");
    ts.ok (ct.naddr == 1, L"+5p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"+5p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"+5p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L"+5p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==5, L"+5p => addr1[1].disp==5");
    ts.ok (ct.comma != ';', L"+5p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"+5p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"+5p => command=='p'");
    ts.ok (! ct.pflag, L"+5p => pflag==false");
}

void
test_plus_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"+p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"+p");
    ts.ok (ct.naddr == 1, L"+p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"+p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"+p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L"+p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==1, L"+p => addr1[1].disp==1");
    ts.ok (ct.comma != ';', L"+p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"+p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"+p => command=='p'");
    ts.ok (! ct.pflag, L"+p => pflag==false");
}

void
test_dot_minus5_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L".-5p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L".-5p");
    ts.ok (ct.naddr == 1, L".-5p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L".-5p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L".-5p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L".-5p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==-5, L".-5p => addr1[1].disp==-5");
    ts.ok (ct.comma != ';', L".-5p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L".-5p => addr2.size()==0");
    ts.ok (ct.command == 'p', L".-5p => command=='p'");
    ts.ok (! ct.pflag, L".-5p => pflag==false");
}

void
test_minus5_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"-5p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"-5p");
    ts.ok (ct.naddr == 1, L"-5p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"-5p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"-5p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L"-5p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==-5, L"-5p => addr1[1].disp==-5");
    ts.ok (ct.comma != ';', L"-5p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"-5p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"-5p => command=='p'");
    ts.ok (! ct.pflag, L"-5p => pflag==false");
}

void
test_minus_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"-p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"-p");
    ts.ok (ct.naddr == 1, L"-p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"-p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"-p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==LINEOFFSET, L"-p => addr1[1].type==LINEOFFSET");
    ts.ok (ct.addr1[1].disp==-1, L"-p => addr1[1].disp==-1");
    ts.ok (ct.comma != ';', L"-p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"-p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"-p => command=='p'");
    ts.ok (! ct.pflag, L"-p => pflag==false");
}

void
test_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"/pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"/pat/p");
    ts.ok (ct.naddr == 1, L"/pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"/pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"/pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L"/pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L"/pat/p => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"pat", L"/pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L"/pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"/pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"/pat/p => command=='p'");
    ts.ok (! ct.pflag, L"/pat/p => pflag==false");
}

void
test_dot_plus_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L".+/pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L".+/pat/p");
    ts.ok (ct.naddr == 1, L".+/pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L".+/pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L".+/pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L".+/pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L".+/pat/p => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"pat", L".+/pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L".+/pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L".+/pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L".+/pat/p => command=='p'");
    ts.ok (! ct.pflag, L".+/pat/p => pflag==false");
}

void
test_plus_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"+/pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"+/pat/p");
    ts.ok (ct.naddr == 1, L"+/pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"+/pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"+/pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L"+/pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L"+/pat/p => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"pat", L"+/pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L"+/pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"+/pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"+/pat/p => command=='p'");
    ts.ok (! ct.pflag, L"+/pat/p => pflag==false");
}

void
test_dot_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L". /pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L". /pat/p");
    ts.ok (ct.naddr == 1, L". /pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L". /pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L". /pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L". /pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L". /pat/p => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"pat", L". /pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L". /pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L". /pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L". /pat/p => command=='p'");
    ts.ok (! ct.pflag, L". /pat/p => pflag==false");
}

void
test_dot_minus_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L".-/pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L".-/pat/p");
    ts.ok (ct.naddr == 1, L".-/pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L".-/pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L".-/pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L".-/pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==-1, L".-/pat/p => addr1[1].disp==-1");
    ts.ok (ct.addr1[1].pattern==L"pat", L".-/pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L".-/pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L".-/pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L".-/pat/p => command=='p'");
    ts.ok (! ct.pflag, L".-/pat/p => pflag==false");
}

void
test_minus_pattern_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"-/pat/p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"-/pat/p");
    ts.ok (ct.naddr == 1, L"-/pat/p => naddr==1");
    ts.ok (ct.addr1.size () == 2, L"-/pat/p => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"-/pat/p => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L"-/pat/p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==-1, L"-/pat/p => addr1[1].disp==-1");
    ts.ok (ct.addr1[1].pattern==L"pat", L"-/pat/p => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L"-/pat/p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"-/pat/p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"-/pat/p => command=='p'");
    ts.ok (! ct.pflag, L"-/pat/p => pflag==false");
}

void
test_exp_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"$ -/f/ +5 // --p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"$ -/f/ +5 // --p");
    ts.ok (ct.naddr == 1, L"$ -/f/ +5 // --p => naddr==1");
    ts.ok (ct.addr1.size () == 6, L"$ -/f/ +5 // --p => addr1.size()==6");
    ts.ok (ct.addr1[0].type==DOLLAR, L"$ -/f/ +5 // --p => addr1[0].type==DOLLAR");
    ts.ok (ct.addr1[1].type==PATTERN, L"$ -/f/ +5 // --p => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==-1, L"$ -/f/ +5 // --p => addr1[1].disp==-1");
    ts.ok (ct.addr1[1].pattern==L"f", L"$ -/f/ +5 // --p => addr1[1].pattern==\"f\"");
    ts.ok (ct.addr1[2].type==LINEOFFSET, L"$ -/f/ +5 // --p => addr1[2].type==LINEOFFSET");
    ts.ok (ct.addr1[2].disp==+5, L"$ -/f/ +5 // --p => addr1[2].disp==+5");
    ts.ok (ct.addr1[3].type==PATTERN, L"$ -/f/ +5 // --p => addr1[3].type==PATTERN");
    ts.ok (ct.addr1[3].disp==+1, L"$ -/f/ +5 // --p => addr1[3].disp==+1");
    ts.ok (ct.addr1[3].pattern==L"f", L"$ -/f/ +5 // --p => addr1[3].pattern==\"f\"");
    ts.ok (ct.addr1[4].type==LINEOFFSET, L"$ -/f/ +5 // --p => addr1[4].type==LINEOFFSET");
    ts.ok (ct.addr1[4].disp==-1, L"$ -/f/ +5 // --p => addr1[4].disp==-1");
    ts.ok (ct.addr1[5].type==LINEOFFSET, L"$ -/f/ +5 // --p => addr1[5].type==LINEOFFSET");
    ts.ok (ct.addr1[5].disp==-1, L"$ -/f/ +5 // --p => addr1[5].disp==-1");
    ts.ok (ct.comma != ';', L"$ -/f/ +5 // --p => comma!=';'");
    ts.ok (ct.addr2.size () == 0, L"$ -/f/ +5 // --p => addr2.size()==0");
    ts.ok (ct.command == 'p', L"$ -/f/ +5 // --p => command=='p'");
    ts.ok (! ct.pflag, L"$ -/f/ +5 // --p => pflag==false");
}

void
test_1_c_3p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"1,3p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"1,3p");
    ts.ok (ct.naddr == 2, L"1,3p => naddr==2");
    ts.ok (ct.addr1.size () == 1, L"1,3p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L"1,3p => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L"1,3p => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L"1,3p => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L"1,3p => addr2.size()==1");
    ts.ok (ct.addr2[0].type==LINENUM, L"1,3p => addr1[0].type==LINENUM");
    ts.ok (ct.addr2[0].disp==3, L"1,3p => addr1[0].disp==3");
    ts.ok (ct.command == 'p', L"1,3p => command=='p'");
    ts.ok (! ct.pflag, L"1,3p => pflag==false");
}

void
test_c_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L",p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L",p");
    ts.ok (ct.naddr == 2, L",p => naddr==2");
    ts.ok (ct.addr1.size () == 1, L",p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==LINENUM, L",p => addr1[0].type==LINENUM");
    ts.ok (ct.addr1[0].disp==1, L",p => addr1[0].disp==1");
    ts.ok (ct.comma != ';', L",p => comma!=';'");
    ts.ok (ct.addr2.size () == 1, L",p => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L",p => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'p', L",p => command=='p'");
    ts.ok (! ct.pflag, L",p => pflag==false");
}

void
test_s_p (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L";p\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L";p");
    ts.ok (ct.naddr == 2, L";p => naddr==2");
    ts.ok (ct.addr1.size () == 1, L";p => addr1.size()==1");
    ts.ok (ct.addr1[0].type==DOT, L";p => addr1[0].type==DOT");
    ts.ok (ct.comma == ';', L";p => comma==';'");
    ts.ok (ct.addr2.size () == 1, L";p => addr2.size()==1");
    ts.ok (ct.addr2[0].type==DOLLAR, L";p => addr1[0].type==DOLLAR");
    ts.ok (ct.command == 'p', L";p => command=='p'");
    ts.ok (! ct.pflag, L";p => pflag==false");
}

void
test_s_pattern (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"s{pat}{new}\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"s{pat}{new}");
    ts.ok (ct.command == 's', L"s{pat}{new} => command=='s'");
    ts.ok (ct.pattern == L"pat", L"s{pat}{new} => pattern=\"pat\"");
    ts.ok (ct.param == L"new", L"s{pat}{new} => param=\"new\"");
}

void
test_pattern_epattern_s_epattern (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"/pat/,//s//new/\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"/pat/,//s//new/");
    ts.ok (ct.naddr == 2, L"/pat/,//s//new/ => naddr==2");
    ts.ok (ct.addr1.size () == 2, L"/pat/,//s//new/ => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"/pat/,//s//new/ => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L"/pat/,//s//new/ => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L"/pat/,//s//new/ => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"pat", L"/pat/,//s//new/ => addr1[1].pattern==\"pat\"");
    ts.ok (ct.comma != ';', L"/pat/,//s//new/ => comma!=';'");
    ts.ok (ct.addr2.size () == 2, L"/pat/,//s//new/ => addr2.size()==2");
    ts.ok (ct.addr2[0].type==DOT, L"/pat/,//s//new/ => addr2[0].type==DOT");
    ts.ok (ct.addr2[1].type==PATTERN, L"/pat/,//s//new/ => addr2[1].type==PATTERN");
    ts.ok (ct.addr2[1].disp==+1, L"/pat/,//s//new/ => addr2[1].disp==+1");
    ts.ok (ct.addr2[1].pattern==L"pat", L"/pat/,//s//new/ => addr2[1].pattern==\"pat\"");
    ts.ok (ct.command == 's', L"/pat/,//s//new/ => command=='s'");
    ts.ok (ct.pattern == L"pat", L"/pat/,//s//new/ => pattern==\"pat\"");
    ts.ok (ct.param == L"new", L"/pat/,//s//new/ => param==\"new\"");
    ts.ok (! ct.pflag, L"/pat/,//s//new/ => pflag==false");
}

void
test_esc_esc_s_esc (test::simple& ts)
{
    grammar_type scanner;
    command_type ct;
    std::wstring input (L"/\\w+/,/\\w+/s/\\w+/new/\n");
    std::wstring::const_iterator s = input.cbegin ();
    bool r = scanner.get (s, ct);
    ts.ok (r, L"/\\w+/,/\\w+/s/\\w+/new/");
    ts.ok (ct.naddr == 2, L"/\\w+/,/\\w+/s/\\w+/new/ => naddr==2");
    ts.ok (ct.addr1.size () == 2, L"/\\w+/,/\\w+/s/\\w+/new/ => addr1.size()==2");
    ts.ok (ct.addr1[0].type==DOT, L"/\\w+/,/\\w+/s/\\w+/new/ => addr1[0].type==DOT");
    ts.ok (ct.addr1[1].type==PATTERN, L"/\\w+/,/\\w+/s/\\w+/new/ => addr1[1].type==PATTERN");
    ts.ok (ct.addr1[1].disp==+1, L"/\\w+/,/\\w+/s/\\w+/new/ => addr1[1].disp==+1");
    ts.ok (ct.addr1[1].pattern==L"\\w+", L"/\\w+/,/\\w+/s/\\w+/new/ => addr1[1].pattern==\"\\w+\"");
    ts.ok (ct.comma != ';', L"/\\w+/,/\\w+/s/\\w+/new/ => comma!=';'");
    ts.ok (ct.addr2.size () == 2, L"/\\w+/,/\\w+/s/\\w+/new/ => addr2.size()==2");
    ts.ok (ct.addr2[0].type==DOT, L"/\\w+/,/\\w+/s/\\w+/new/ => addr2[0].type==DOT");
    ts.ok (ct.addr2[1].type==PATTERN, L"/\\w+/,/\\w+/s/\\w+/new/ => addr2[1].type==PATTERN");
    ts.ok (ct.addr2[1].disp==+1, L"/\\w+/,/\\w+/s/\\w+/new/ => addr2[1].disp==+1");
    ts.ok (ct.addr2[1].pattern==L"\\w+", L"/\\w+/,/\\w+/s/\\w+/new/ => addr2[1].pattern==\"\\w+\"");
    ts.ok (ct.command == 's', L"/\\w+/,/\\w+/s/\\w+/new/ => command=='s'");
    ts.ok (ct.pattern == L"\\w+", L"/\\w+/,/\\w+/s/\\w+/new/ => pattern==\"\\w+\"");
    ts.ok (ct.param == L"new", L"/\\w+/,/\\w+/s/\\w+/new/ => param==\"new\"");
    ts.ok (! ct.pflag, L"/\\w+/,/\\w+/s/\\w+/new// => pflag==false");
}

int main (int argc, char* argv[])
{
    std::locale::global (std::locale (""));
    std::wcout.imbue (std::locale (""));

    test::simple ts (504);

    test_1 (ts);

    test_a (ts);
    test_c (ts);
    test_d (ts);
    test_dp (ts);
    test_e_file (ts);
    test_e_empty (ts);
    test_f_file (ts);
    test_f_empty (ts);
    test_g (ts);
    test_i (ts);
    test_j (ts);
    test_k (ts);
    test_m (ts);
    test_mp (ts);
    test_n (ts);
    test_p (ts);
    test_q (ts);
    test_r_file (ts);
    test_r_empty (ts);
    test_s (ts);
    test_sg (ts);
    test_sp (ts);
    test_sgp (ts);
    test_t (ts);
    test_tp (ts);
    test_u (ts);
    test_v (ts);
    test_w_file (ts);
    test_w_empty (ts);
    test_equal (ts);
    test_equalp (ts);
    test_newline (ts);

    test_dot_p (ts);
    test_dollar_p (ts);
    test_mark_p (ts);
    test_1p (ts);
    test_dot_plus5_p (ts);
    test_plus5_p (ts);
    test_plus_p (ts);
    test_dot_minus5_p (ts);
    test_minus5_p (ts);
    test_minus_p (ts);
    test_pattern_p (ts);
    test_dot_plus_pattern_p (ts);
    test_plus_pattern_p (ts);
    test_dot_pattern_p (ts);
    test_dot_minus_pattern_p (ts);
    test_minus_pattern_p (ts);
    test_exp_p (ts);
    test_1_c_3p (ts);
    test_c_p (ts);
    test_s_p (ts);

    test_s_pattern (ts);
    test_pattern_epattern_s_epattern (ts);
    test_esc_esc_s_esc (ts);

    return ts.done_testing ();
}

