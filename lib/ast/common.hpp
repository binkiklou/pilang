#pragma once

#include "shared/source.hpp"

#include <string>

// --- AST STATE ---
enum ast_node_state {
    AST_NODE_INVALID, // Base nodes shouldn't exist alone
    AST_NODE_OK,
    AST_NODE_UNFOUND,
    AST_NODE_ERRORED
};

// --- AST Location ---

enum ast_loc_type
{
    LOC_INVALID,
    LOC_SINGLE, // Single location
    LOC_LARGE // Start&Finish location
};

class ast_loc
{
    public:
    ast_loc();

    ast_loc_type m_type;
};

class ast_loc_single : public ast_loc
{
    public:
    ast_loc_single();
    location m_loc;
};

class ast_loc_large : public ast_loc
{
    public:
    ast_loc_large();
    location m_start;
    location m_end;
};

// --- AST Node ---

class ast_node
{
    public:
    ast_node();

    ast_loc* m_loc;
    ast_node_state m_state;
};

// --- Identifier ---

class identifier : public ast_node
{
    public:
    identifier();
    ~identifier();
    std::string m_ident;
};

// --- Typespec ---

enum TYPESPEC_FORM{
    FORM_SCALAR,
    FORM_ARR,
};

enum DATATYPES {
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE
};

class typespec
{
    public:
    TYPESPEC_FORM m_form;
    DATATYPES m_type;
};