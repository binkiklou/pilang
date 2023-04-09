#pragma once

#include "ast/ast.hpp"

#include <memory>

// This class should also be used for the syntax tree printing
// So that node printing looks the same for every tree
class dump_node
{
    public:
    dump_node();
    dump_node(const std::string&);

    void dump(unsigned  int);
    void add_child(const dump_node&);

    std::string text;

    // Litteraly the only place in the code where a smart pointer is used
    std::vector<std::shared_ptr<dump_node>> children;
};

// No reason that this is a class
class ast_dumper
{
    public:

    ast_root* root;

    void dump_ast();

    private:
    std::string _dump_loc(ast_loc*);
    std::string _get_node_str(const std::string&, ast_node*);

    dump_node _dump_stmt(ast_stmt*);
    dump_node _dump_block_stmt(ast_block_stmt*);
    dump_node _dump_top_level_stmt(ast_top_level_stmt*);
};