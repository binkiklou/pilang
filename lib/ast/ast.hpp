#pragma once

#include <string>
#include <vector>

#include "ast/common.hpp"
#include "ast/stmt.hpp"
#include "ast/top_level.hpp"

// Top-Level Statements
class ast_root
{
    public:
    ast_root();
    ~ast_root();
    std::vector<ast_top_level_stmt*> nodes;
};