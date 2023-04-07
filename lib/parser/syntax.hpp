#pragma once

#include "parser/parser.hpp"

#define HINT_START(c) _p->try_hint(c);

#define CANCEL_EXIT _p->cancel_try(); \
return false;

#define ERROR_EXIT _p->keep_hint(); \
return false;

#define MATCH_EXIT _p->keep_hint(); \
return true;

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

    bool get_datatype();
    bool get_typespec();

    bool get_scalar_value();
    bool get_scalar_expr();
    
    bool get_scalar_operation();

    bool get_scalar_arrow_right();
    bool get_scalar_arrow_left();
    bool get_scalar_arrow_expr();

    bool get_array_arrow_right();
    bool get_array_arrow_left();
    bool get_array_arrow_expr();

    bool get_selector();
    bool get_selection();

    bool get_array_expr_term();
    bool get_array_expr();

    bool get_arr_size();
    bool get_arr_copy();
    bool get_arr_lit();
    bool get_arr_init();

    bool get_assign();
    bool get_vardecl();

    bool get_param();
    bool get_param_list();

    bool get_fndecl();

    bool get_block();

    bool get_call_arg();
    bool get_call_arg_list();
    bool get_call();

    bool get_proc_call();
    bool get_proc();

    bool get_attr();
    bool get_attr_field();

    bool get_impl_fn();
    bool get_impl();
    bool get_impl_field();

    bool get_typedef();

    bool get_entry();

    bool get_import();

    void get_statement();
    void get_top_level();
    private:
    parser* _p;
};