#ifndef EDIT_H
#define EDIT_H

/* edit - text line editor */

#include <string>
#include <memory>
#include <vector>

// addr_type type
enum {LINENUM, LINEOFFSET, DOT, DOLLAR, MARK, PATTERN};

// node of address expression vectors
struct addr_type {
    int type;
    int disp;
    std::wstring pattern;
};

// parsed editor command
struct command_type {
    int naddr;
    std::vector<addr_type> addr1;
    int comma;
    std::vector<addr_type> addr2;
    int command;
    std::vector<addr_type> addr3;
    std::wstring pattern;
    std::wstring param;  // aci: doc, s: replacement, efrw: file
    bool gflag;
    bool pflag;
};

// text and undo line range in buffer.data
struct line_type {
    std::size_t dbegin, dend;
    bool mark;
};

// undo information
struct undo_type {
    int command;
    std::size_t seq;
    std::size_t cur;
    std::size_t line1, line2;
    std::vector<line_type> text;
};

// regular expression Rob Pike's virtual machine code
struct recode_type {
    int op, x, y;
    std::wstring s;
};

// regular expression Rob Pike's virtual machine thread
struct rethread_type {
    std::size_t ip;
    std::shared_ptr<std::vector<std::size_t>> cap;
    std::shared_ptr<std::vector<std::size_t>> update (std::size_t i, std::size_t x) const;
};

class buffer_type;
class grammar_type;
class regexp_type;

class editor_type {
public:
    editor_type (grammar_type& sc, buffer_type& b);
    int edit (std::wstring::const_iterator s);
    static void unquote (std::wstring const& src, std::wstring& dst);
    static std::wstring decode (std::string octets, char const* const lname = "");
    static std::string encode (std::wstring str, char const* lname = "");

private:
    grammar_type& scanner;
    buffer_type& buffer;
    std::size_t line1, line2, line3;
    int command (command_type& ct);
    int global (std::wstring::const_iterator& s, command_type& ct);
    int markglobal (command_type& ct);
    int sweepglobal (command_type& subct);
    int cmd_a (command_type& ct);
    int cmd_c (command_type& ct);
    int cmd_d (command_type& ct);
    int cmd_e (command_type& ct);
    int cmd_f (command_type& ct);
    int cmd_i (command_type& ct);
    int cmd_j (command_type& ct);
    int cmd_k (command_type& ct);
    int cmd_m (command_type& ct);
    int cmd_n (command_type& ct);
    int cmd_p (command_type& ct);
    void print_dot (command_type& ct);
    int cmd_r (command_type& ct);
    int cmd_s (command_type& ct);
    int cmd_t (command_type& ct);
    int cmd_w (command_type& ct);
    int cmd_equal (command_type& ct);
    bool read (std::wstring const& file, std::wstring& doc);
    bool write (std::wstring const& file);
    void getdoc (std::wstring& doc);
    bool evaladdr (std::vector<addr_type> const& addr, std::size_t& line);
    int find (int n, int way, std::wstring const& pattern);
    bool match (regexp_type& re,
        std::wstring::const_iterator s, std::wstring::const_iterator const eos);
    void substitute (std::wstring const& pattern,
        std::wstring const& replacement, bool gflag, 
        std::wstring::const_iterator s0, std::wstring::const_iterator e,
        std::wstring& doc);
    void substhere (
        std::wstring::const_iterator const bos,
        std::wstring const& replacement,
        std::vector<std::size_t>& cap, std::wstring& doc);
};

class buffer_type {
public:
    buffer_type ();
    std::size_t dot () const;
    std::size_t dollar () const;
    std::size_t mark () const;
    std::wstring file () const;
    void get (std::size_t const line,
        std::wstring::const_iterator& s, std::wstring::const_iterator& e) const;
    bool gmark (std::size_t const line) const;
    void setgmark (std::size_t const line, bool const x);
    void setdot (std::size_t const line);
    void setmark ();
    void setfile (std::wstring const& s);
    std::size_t append (std::size_t line, std::wstring const& str);
    std::size_t change (std::size_t line1, std::size_t line2, std::wstring const& str);
    std::size_t erase (std::size_t line1, std::size_t line2);
    std::size_t move (std::size_t line1, std::size_t line2, std::size_t line3);
    void touch ();
    std::size_t undo ();

private:
    std::size_t seq;
    std::size_t cur;
    std::size_t markline;
    std::wstring filename;
    std::wstring data;
    std::vector<line_type> text;
    std::vector<undo_type> history;
    void push_data (std::vector<line_type>& txt, std::wstring const& str);
    void undo_step (undo_type& u);
};

// grammar_type
class grammar_type {
public:
    grammar_type ();
    bool get (std::wstring::const_iterator& s, command_type& ct);

private:
    std::wstring lastpattern;
    bool getlist (std::wstring::const_iterator& s, command_type& ct);
    bool setaddrdefault (std::vector<addr_type>& addr, int const kind);
    bool getaddr3 (std::wstring::const_iterator& s, command_type& ct);
    bool getre (std::wstring::const_iterator& s, command_type& ct);
    bool getreplacement (std::wstring::const_iterator& s, command_type& ct);
    bool getparam (std::wstring::const_iterator& s, command_type& ct);
    bool getfile (std::wstring::const_iterator& s, command_type& ct);
    bool getgflag (std::wstring::const_iterator& s, command_type& ct);
    bool getpflag (std::wstring::const_iterator& s, command_type& ct);
    bool getaddr (std::wstring::const_iterator& s, std::vector<addr_type>& addr);
    bool getbase (std::wstring::const_iterator& s, std::vector<addr_type>& addr);
    bool getterm (std::wstring::const_iterator& s, std::vector<addr_type>& addr);
    int scandecimal (std::wstring::const_iterator& s);
    std::wstring::const_iterator skippattern (std::wstring::const_iterator s0);
    std::wstring::const_iterator skipspace (std::wstring::const_iterator s);
    bool isaddrfirst (int c);
    bool istermfirst (int c);
};

class regexp_type {
public:
    regexp_type ();
    bool compile (std::wstring const& pattern);
    bool execute (std::wstring::const_iterator s,
        std::wstring::const_iterator abos, std::wstring::const_iterator aeos,
        std::vector<std::size_t>& capture);

private:
    int group;
    std::wstring::const_iterator re;
    std::wstring::const_iterator eor;
    std::vector<recode_type> program;
    int gen;
    std::vector<int> mark;
    std::wstring::const_iterator bos;
    std::wstring::const_iterator eos;
    bool alt (std::vector<recode_type>& code);
    bool cat (std::vector<recode_type>& code);
    bool term (std::vector<recode_type>& code);
    bool factor (std::vector<recode_type>& code);
    bool clschar (std::wstring& str);
    void addthread (std::vector<rethread_type>& q, rethread_type&& th,
        std::wstring::const_iterator s);
    bool atwordboundary (std::wstring::const_iterator s);
    bool incclass (std::wstring const& span, int const c);
};

#endif
