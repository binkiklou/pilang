#include "driver/ast_dumper.hpp"

#include "shared/print.hpp"

// --- DUMP NODE ---

dump_node::dump_node(){
    text = "(empty dump_node)";
}

dump_node::dump_node(const std::string& txt)
{
    text = txt;
}

void dump_node::dump(unsigned int depth)
{
    std::string ident = std::string(depth*3, ' ');
    print(ident + text);
   
    for(std::shared_ptr<dump_node> child : children)
    {
        child.get()->dump(depth+1);
    }
}

void dump_node::add_child(const dump_node& n)
{
    children.push_back(std::make_shared<dump_node>(n));
}

// --- DUMPS ---

void ast_dumper::dump_ast()
{
    dump_node root_dump("[AST_ROOT]");
    for(ast_top_level_stmt* node : root->nodes)
    {
        root_dump.add_child(_dump_top_level_stmt(node));
    }
    root_dump.dump(0);
}

std::string ast_dumper::_dump_loc(ast_loc* loc)
{
    if(loc == nullptr || loc->m_type == LOC_INVALID){
        return colored("INVALID LOCATION", RED);
    }

    // Temporary
    if(loc->m_type == LOC_SINGLE){
        ast_loc_single* single_loc = static_cast<ast_loc_single*>(loc);
        std::string result;
        result = "(";
        result += single_loc->m_loc.as_string();
        result += ")";
        return "LOC_SINGLE";
    }
    else if(loc->m_type == LOC_LARGE){
        ast_loc_large* large_loc = static_cast<ast_loc_large*>(loc);
        std::string result;
        result = "(";
        result += large_loc->m_start.as_string();
        result += ":";
        result += large_loc->m_end.as_string();
        result += ")";
        return result;
    }

    return colored("(STRANGE LOCATION)", RED);
}

std::string ast_dumper::_get_node_str(const std::string& name, ast_node* n)
{
    if(n == nullptr){
        return colored("[NULLPTR: " + name + "]",RED);
    }
    std::string result;
    result = "[" + name + "] ";
    result += _dump_loc(n->m_loc);
    return result;
}

dump_node ast_dumper::_dump_stmt(ast_stmt* node)
{
    return dump_node("STMT");
}

dump_node ast_dumper::_dump_block_stmt(ast_block_stmt* node)
{
    if(node == nullptr || node->m_state == AST_NODE_UNFOUND){
        return dump_node();
    }

    dump_node dnode(_get_node_str("BLOCK_STMT", node));

    for(ast_stmt* stmt : node->m_stmts)
    {
        dnode.add_child(_dump_stmt(stmt));
    }
    
    return dnode;
}

dump_node ast_dumper::_dump_top_level_stmt(ast_top_level_stmt* node)
{
    if(node == nullptr || node->m_state == AST_NODE_UNFOUND){
        return dump_node();
    }
    switch(node->m_type)
    {
        case INVALID_TOP_STMT:
        return dump_node(colored("[INVALID_TOP_STMT]",RED));
        break;

        case ENTRY_STMT:
        {
            dump_node dnode(_get_node_str("ENTRY_STMT", node));
            return dnode;
        }

        default:
        return dump_node(colored("[WIP NODE DUMP]",YELLOW));
        break;
    }
}