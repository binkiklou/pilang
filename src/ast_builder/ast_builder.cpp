#include "ast_builder/ast_builder.hpp"
#include  "shared/print.hpp"

ast_builder::ast_builder()
{
    parse_tree = nullptr;
}

std::pair<unsigned int, unsigned int> ast_builder::_get_token_pos_minmax(hint_node* p)
{
    std::pair<unsigned int, unsigned int> min_max = std::make_pair<unsigned int, unsigned int>(0,0);

    bool all_set = false;

    for(parser_node* n : p->m_children)
    {
        if(n->m_type == HINT_NODE && n->m_children.size() > 0){
            // why the fuck is this so verbose
            std::pair<unsigned int, unsigned int> result = _get_token_pos_minmax(static_cast<hint_node*>(n));
            if(!all_set){
                min_max = result;
                all_set = true;
            }
            else{
                min_max.first = std::min(result.first, min_max.first);
                min_max.second = std::max(result.second, min_max.second);
            }
        }
        else if(n->m_type == TOKEN_NODE){
            token_node* t = static_cast<token_node*>(n);
            if(!all_set){
                min_max.first = t->m_token.m_id;
                min_max.second = t->m_token.m_id;
                all_set = true;
            }
            else{
                min_max.first = std::min(t->m_token.m_id, min_max.first);
                min_max.second = std::max(t->m_token.m_id, min_max.second);
            }
        }
    }
    return min_max;
}

ast_loc_single* ast_builder::_get_single_loc(token_node* tkn)
{
    ast_loc_single* loc = new ast_loc_single;
    loc->m_loc = tkn->m_token.m_word.loc;
    return loc;
}

ast_loc_large* ast_builder::_get_large_loc(hint_node* h)
{
    ast_loc_large* loc = new ast_loc_large;

    std::pair<unsigned int, unsigned int> mm = _get_token_pos_minmax(h);

    // Should add checks
    loc->m_start = tokens->at(mm.first).m_word.loc;
    loc->m_end = tokens->at(mm.second).m_word.loc;
    loc->m_end.col += tokens->at(mm.second).m_word.data.length();

    return loc;
}

bool ast_builder::_is_valid(ast_node* n)
{
    if(n == nullptr){
        return false;
    }

    if(n->m_state == AST_NODE_OK)
    {
        return true;
    }
    return false;
}

std::vector<token_node*> ast_builder::_get_tokens(parser_node* node)
{
    std::vector<token_node*> tokens;

    if(node == nullptr){
        return tokens;
    }

    for(parser_node* child: node->m_children)
    {
        if(child->m_type == TOKEN_NODE)
        {
            tokens.push_back(static_cast<token_node*>(child));
        }
    }

    return tokens;
}

std::vector<hint_node*> ast_builder::_get_hints(parser_node* node)
{
    std::vector<hint_node*> hints;

    if(node == nullptr){
        return hints;
    }

    for(parser_node* child : node->m_children)
    {
        if(child->m_type == HINT_NODE)
        {
            hints.push_back(static_cast<hint_node*>(child));
        }
    }

    return hints;
}

search_result ast_builder::_find_token(TKN_TYPE type, parser_node* node)
{
    if(node == nullptr){
        print_verbose("Tried to search null node for tokens");
        return search_result{false};
    }
    
    search_result result;

    for(token_node* tkn : _get_tokens(node))
    {
        if(tkn->m_token.m_type == type)
        {
            if(result.found && result.is_unique){
                result.is_unique = false;
            }
            
            if(!result.found){
                result.found = true;
            }

            result.results.push_back(tkn);
        }
    }

    return  result;
}

search_result ast_builder::_find_hint(const std::string& hint, parser_node* node)
{
    if(node  == nullptr){
        print_verbose("Tried to search null node for hints");
        return search_result{false};
    }

    search_result result;

    // very sophisticated search algorithm
    for(parser_node* child : node->m_children)
    {
        if(child->m_type == HINT_NODE)
        {
            if(static_cast<hint_node*>(child)->m_hint == hint)
            {
                if(result.found && result.is_unique){
                    result.is_unique = false;
                }

                if(!result.found){
                    result.found = true;
                }

                result.results.push_back(child);
            }
        }
    }

    return result;
}

bool ast_builder::build()
{
    m_state = AST_BUILDER_OK;

    // Not sure if that error is even possible
    if(parse_tree == nullptr){
        print_error("Cannot build AST: Parse tree is not defined.");
        return false;
    }

    for(hint_node* h : _get_hints(parse_tree))
    {
        if(h->m_hint == "entry"){
            ast_entry_stmt* node = new ast_entry_stmt;
            *node = build_entry_stmt(h);
            ast.nodes.push_back(node);
        }
        else{
            // temporary
            print_error("Unrecognized top-level node");
            return false;
        }
    }
    
    return true;
}