#pragma once

#include "parser.hpp"

/*
Uses the parser to create the parse tree,
tbh the only reason this class exists is that
I'm forced to use public parser functions so  the
code doesn't become a mess, but it does not need to exist.
*/
class syntax
{
    public:
    syntax(parser*);

    bool get_const_value();
    bool get_const_expr();

    bool get_scalar_arrow_left();
    bool get_scalar_arrow_right();
    bool get_scalar_arrow_expr();

    bool get_datatype();

    bool get_arr_size();
    bool get_arr_copy();
    bool get_arr_lit();

    bool get_arr_init();

    bool get_vardecl();
 
    void get_top_level();
    private:
    parser* _p;
};