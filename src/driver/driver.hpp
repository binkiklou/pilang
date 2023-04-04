#pragma once

#include "source.hpp"

#include "../lexer/lexer.hpp"

class driver
{
    public:
    // Creates the tools and start the compile process
    void start(source*);
    
    private:
    void _load_file();

    // Init all the phases(lexer,parser,..) at once
    void _init_phases();

    // Deletes all the phases at once
    void _delete_phases();

    bool m_ok = false;

    source* m_src;
    lexer* m_lexer;
};