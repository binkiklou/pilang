#include "syntax.hpp"

bool syntax::get_value()
{
    _p->try_hint("value");

    if(
        _p->match(INT_LIT) || 
        _p->match(BOOL_LIT) || 
        _p->match(CHAR_LIT) ||
        _p->match(FLOAT_LIT) ||
        _p->match(DOUBLE_LIT))
    {
        _p->keep_hint();
        return true;
    }

    _p->cancel_try();
    return false;
}