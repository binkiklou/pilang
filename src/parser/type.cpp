#include "parser/syntax.hpp"

// typespec identifier 
bool syntax::get_attr()
{
    HINT_START("attr");

    if(!get_typespec()){
        CANCEL_EXIT;
    }

    if(!_p->match(IDENTIFIER)){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// attr {attr}
bool syntax::get_attr_field()
{
    HINT_START("attr_field");

    if(!_p->match(KW_ATTR)){
        CANCEL_EXIT;
    }

    // expect

    if(!_p->expect(LBRACE)){
        ERROR_EXIT;
    }

    // Does not recover inside typdef
    while(!_p->is_errored() && !_p->match(RBRACE)){
        if(!get_attr()){
            _p->error_line("Attribute is expected on this line.");
            ERROR_EXIT;
        }   
    }

    MATCH_EXIT;
}

// fn type_op identifier 
bool syntax::get_impl_fn()
{
    HINT_START("impl_fn");

    if(!_p->match(KW_FN)){
        CANCEL_EXIT;
    }

    // expect

    if(_p->match(TYPE_OP)){}
    else if(_p->match(MATH_OP)){}
    else{
        _p->error_here("Implementation function must be followed by an operator.");
        ERROR_EXIT;
    }

    if(!_p->match(IDENTIFIER)){
        _p->error_here("Function declaration need an identifier.");
        ERROR_EXIT;
    }

    if(!get_param_list()){
        _p->error_here("Function Declaration needs parameters");
        ERROR_EXIT;
    }

    if(!_p->expect(COLON)){
        ERROR_EXIT
    }
    
    if(!get_array_expr()){
        _p->error_here("Function Declaration needs an expression");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// fn type_op ident
bool syntax::get_impl()
{
    HINT_START("impl");

    if(!get_impl_fn()){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

bool syntax::get_impl_field()
{
    HINT_START("impl_field");

    if(!_p->match(KW_IMPL)){
        CANCEL_EXIT;
    }

    // expect

    if(!_p->expect(LBRACE)){
        ERROR_EXIT;
    }

    // Does not recover inside typdef
    while(!_p->is_errored() && !_p->match(RBRACE)){
        if(!get_impl()){
            _p->error_line("Implementation is expected on this line.");
            ERROR_EXIT;
        }   
    }


    MATCH_EXIT;
}

// typedef ident {}
bool syntax::get_typedef()
{
    HINT_START("typedef");

    if(!_p->match(KW_TYPEDEF))
    {
        CANCEL_EXIT;
    }

    // Expect

    if(!_p->expect(IDENTIFIER)){
        CANCEL_EXIT;
    }

    if(!_p->expect(LBRACE))
    {
        ERROR_EXIT;
    }

    // attr first then impl
    if(!get_attr_field()){
        _p->error_here("Typedef's attr field is expected here.");
        ERROR_EXIT;
    }

    if(!get_impl_field()){
        _p->error_here("Typedef's attr field is expected here.");
        ERROR_EXIT;
    }

    if(!_p->expect(RBRACE))
    {
        ERROR_EXIT;
    }
    
    MATCH_EXIT;
}