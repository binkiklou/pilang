#include "driver/ast_dumper.hpp"

#include "shared/print.hpp"

// --- DUMP NODE ---

dump_node::dump_node(){
    text = "(empty dump_node)";
}

dump_node::dump_node(bool p){
    text = "(empty dump_node)";
    is_property = p;
}

dump_node::dump_node(const std::string& txt)
{
    text = txt;
}

dump_node::dump_node(const std::string& txt, bool p)
{
    text = txt;
    is_property = p;
}

void dump_node::dump(unsigned int depth)
{
    std::string indent = std::string(depth*3, ' ');

    if(depth>0 && !is_property){
        indent[(depth-1)*3] = '|';
        
        for(int i = ((depth-1)*3)+1; i < indent.length(); i++){
            indent[i] = '-';
            if(i == indent.length() - 1){
                indent[i] = '>';
            }
        }
    }
    print(indent + text);
   
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
        result += colored(single_loc->m_loc.as_string(),GREEN);
        result += ")";
        return result;
    }
    else if(loc->m_type == LOC_LARGE){
        ast_loc_large* large_loc = static_cast<ast_loc_large*>(loc);
        std::string result;
        result = "(";
        result += colored(large_loc->m_start.as_string(),GREEN);
        result += ":";
        result += colored(large_loc->m_end.as_string(),GREEN);
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

dump_node ast_dumper::_dump_ident(ast_identifier node)
{
    return dump_node(_get_node_str("IDENTIFIER", &node) + ": \"" + node.m_ident + "\"");
}

dump_node ast_dumper::_dump_typespec(ast_typespec node)
{
    dump_node dnode(_get_node_str("TYPESPEC", &node));

    std::string info;

    info += "FORM:(";

    switch(node.m_form){
        case FORM_SCALAR:
        info += colored("FORM_SCALAR",BLUE);
        break;
        case FORM_ARR:
        info += colored("FORM_ARR",BLUE);
        break;
        default:
        info += colored("FORM_ERR",BLUE);
        break;
    }

    info += "); TYPE:(";

    switch(node.m_type){
        case TYPE_BOOL:
        info += colored("TYPE_BOOL",BLUE);
        break;
        case TYPE_CHAR:
        info += colored("TYPE_CHAR",BLUE);
        break;
        case TYPE_INT:
        info += colored("TYPE_INT",BLUE);
        break;
        case TYPE_FLOAT:
        info += colored("TYPE_FLOAT",BLUE);
        break;
        case TYPE_DOUBLE:
        info += colored("TYPE_DOUBLE",BLUE);
        break;
        default:
        info += colored("TYPE_UNDEF",BLUE);
        break;
    }
    info += ")";
    dnode.add_child(dump_node(info, true));
    return dnode;
}

dump_node ast_dumper::_dump_vdecl(ast_vdecl* node)
{
    dump_node dnode(_get_node_str("VDECL", node));

    dnode.add_child(_dump_typespec(node->m_typespec));

    dnode.add_child(_dump_ident(node->m_ident));

    return dnode;
}

dump_node ast_dumper::_dump_stmt(ast_stmt* node)
{
    switch(node->m_type)
    {
        case VAR_DECL_STMT:{
            return _dump_vdecl(static_cast<ast_vdecl*>(node));
        }
        /*case VAR_DECLDEF_STMT:{
            break;
        }
        case VAR_DEF_STMT:{
            return dump_node();
        }*/
        default:{
            return dump_node("BASE_STMT");
        }
    }
}

dump_node ast_dumper::_dump_block_stmt(ast_block_stmt node)
{
    if(node.m_state == AST_NODE_INVALID){
        return dump_node();
    }

    dump_node dnode(_get_node_str("BLOCK_STMT", &node));

    for(ast_stmt* stmt : node.m_stmts)
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
            dnode.add_child(_dump_block_stmt(static_cast<ast_entry_stmt*>(node)->m_block));
            return dnode;
        }

        default:
        return dump_node(colored("[WIP NODE DUMP]",YELLOW));
        break;
    }
}