#pragma once

#include "diagnostic.hpp"
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
    bool dump_syntax_tree = false;

    private:
    void _load_file();

    // Init all the phases(lexer,parser,..) at once
    void _init_phases();

    // Deletes all the phases at once
    void _delete_phases();

    void _dump_tokens();
    
    void _dump_pnode(parser_node*, unsigned int);
    void _dump_tree();

    void _write_diagnostic(diagnostic&);
    void _write_src_view(unsigned int, bool, unsigned int, location);

    bool m_ok = false;

    source* m_src;
    lexer* m_lexer;
    parser* m_parser;
};