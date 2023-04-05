#pragma once

#include "parser.hpp"

/*
Uses the parser to create the parse tree
*/
class syntax
{
    public:
    syntax(parser*);
    
    void get_top_level();
    private:
    parser* _p;
};