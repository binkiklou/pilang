#pragma once

#include "../lexer/source.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

class driver
{
    public:
    // Creates the tools and start the compile process
    void start(source*);

    // Options
    bool dump_tokens = false;

    private:
    void _load_file();

    // Init all the phases(lexer,parser,..) at once
    void _init_phases();

    // Deletes all the phases at once
    void _delete_phases();

    void _dump_tokens();

    bool m_ok = false;

    source* m_src;
    lexer* m_lexer;
    parser* m_parser;
};