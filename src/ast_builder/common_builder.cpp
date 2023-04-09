#include "ast_builder/ast_builder.hpp"

ast_identifier ast_builder::build_identifier(token_node* node)
{
    ast_identifier ident;
    ident.m_ident = node->m_token.m_word.data;
    ident.m_loc = _get_single_loc(node);
    return ident;
}

ast_typespec ast_builder::build_typespec(hint_node* hint)
{
    ast_typespec t;

    t.m_loc = _get_large_loc(hint);

    search_result tkn_search = _find_token(TKN_TYPE::IDENTIFIER, hint);

    if(!tkn_search.found)
    {
        return t;
    }

    return t;
}