#include "ast_builder/ast_builder.hpp"

ast_vdecl ast_builder::build_vdecl(hint_node* h)
{
    ast_vdecl vdecl;

    vdecl.m_loc = _get_large_loc(h);

    search_result tsearch = _find_hint("typespec",h);
    
    if(!tsearch.found){
        // No real error mechanism is made yet
        vdecl.m_state = AST_NODE_ERRORED;
        return vdecl;
    }

    vdecl.m_typespec = build_typespec(static_cast<hint_node*>(tsearch.results[0]));

    search_result isearch = _find_token(IDENTIFIER,h);

    if(!isearch.found){
        vdecl.m_state = AST_NODE_ERRORED;
        return vdecl;
    }

    vdecl.m_ident = build_identifier(static_cast<token_node*>(isearch.results[0]));

    return vdecl;
}

ast_block_stmt ast_builder::build_block_stmt(hint_node* pnode)
{
    ast_block_stmt block;

    block.m_loc = _get_large_loc(pnode);

    for(hint_node* h : _get_hints(pnode)) {
        if(h->m_hint == "vardecl"){
            ast_vdecl* node = new ast_vdecl;
            *node = build_vdecl(h);
            block.m_stmts.push_back(node);
        }
    }

    return block;
}

// --- TOP LEVEL ---

ast_entry_stmt ast_builder::build_entry_stmt(hint_node* pnode)
{
    ast_entry_stmt stmt;

    stmt.m_loc = _get_large_loc(pnode);

    search_result block_search = _find_hint("block", pnode);
    
    if(!block_search.found)
    {
        stmt.m_state = AST_NODE_UNFOUND;
        return stmt;
    }

    stmt.m_block = build_block_stmt(static_cast<hint_node*>(block_search.results[0]));

    return stmt;
}