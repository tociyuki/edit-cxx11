#include <iostream>
#include <string>
#include <utility>
#include <locale>
#include "edit.hpp"

int
main (int argc, char* argv[])
{
    buffer_type buffer;
    grammar_type scanner;

    std::locale::global (std::locale (""));
    std::wcin.imbue (std::locale (""));
    std::wcout.imbue (std::locale (""));
    std::wstring progname = editor_type::decode (argv[0]);
    if (argc == 2) {
        std::wstring file = editor_type::decode (argv[1]);
        std::wstring input = L"e " + file + L"\n";
        editor_type editor (scanner, buffer);
        std::wstring::const_iterator s = input.cbegin ();
        editor.edit (s);
    }
    else if (argc != 1) {
        std::wcout << L"usage: " << progname << L" [file]" << std::endl;
        return EXIT_FAILURE;
    }
    std::wstring input;
    while (std::getline (std::wcin, input)) {
        input.push_back ('\n');
        editor_type editor (scanner, buffer);
        std::wstring::const_iterator s = input.cbegin ();
        int cmd = editor.edit (s);
        if ('q' == cmd)
            break;
        else if ('?' == cmd)
            std::wcout << L"?\n";
    }
    return EXIT_SUCCESS;
}
