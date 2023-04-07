#include "driver/driver.hpp"

#include "shared/print.hpp"

#include <fstream>

void driver::start(source* src)
{
    m_src = src;
    m_ok = true;

    this->_load_file();

    if(!m_ok){
        return;
    }
    
    _init_phases();
    
    // Lexer
    if(!this->m_lexer->tokenize())
    {
        for(diagnostic& d : this->m_lexer->diagnostics)
        {
            _write_diagnostic(d);
        }

        print_verbose("Tokenization failed, aborting compilation.");
        _delete_phases();
        return;
    }

    if(this->dump_tokens)
    {
        _dump_tokens();
    }

    // Parser
    this->m_parser->tokens = this->m_lexer->tokens;

    if(!this->m_parser->parse())
    {
        print_verbose("Parser failed, aborting compilation.");
        
        // Should only allow dump for certain errors
        if(dump_syntax_tree)
        {
            print("--- PARSE TREE DUMP ---");
            _dump_tree();
            print("----------");
        }

        for(diagnostic& d : this->m_parser->diagnostics)
        {
            _write_diagnostic(d);
        }

        _delete_phases();
        return;
    }

        // Should only allow dump for certain errors
        if(dump_syntax_tree)
        {
            _dump_tree();
        }

    _delete_phases();
}

void driver::_load_file()
{
    if(m_src == nullptr){
        this->m_ok = false;
        return;
    }

    std::fstream file;
    file.open(m_src->filename, std::ios::in);
    
    if(!file.is_open()){
        print_error("Could not open source file: " + m_src->filename);
        this->m_ok = false;
        return;
    }

    print_verbose("Opened source file: " + m_src->filename);

    std::vector<std::string>* lines = new std::vector<std::string>;
    std::string line;
    while(std::getline(file, line))
    {
        lines->push_back(line);
    }
    file.close();

    m_src->set_lines(lines);
}

void driver::_init_phases()
{
    print_verbose("Init phases");
    
    this->m_lexer = new lexer;
    this->m_lexer->m_src = this->m_src;

    this->m_parser = new parser;
}

void driver::_delete_phases()
{
    print_verbose("Deleting phases");

    if(this->m_lexer != nullptr)
    {
        delete this->m_lexer;
        this->m_lexer = nullptr;
    }

    if(this->m_parser != nullptr)
    {
        delete this->m_parser;
        this->m_parser = nullptr;
    }
}

void driver::_dump_tokens()
{
    for(token& t: *this->m_lexer->tokens)
    {
        print(
            tkn_type_as_string(t.m_type) +
            " '" +
            t.m_word.data +
            "' loc=" +
            t.m_word.loc.as_string()
            );
    }
    print("----------");
}

void driver::_dump_pnode(parser_node* pnode, unsigned int depth)
{
    std::string txt;
    txt = std::string(depth*3, ' ');
    if(depth>0){
        txt[(depth-1)*3] = '|';

        for(int i = ((depth-1)*3)+1; i < txt.length(); i++){
            txt[i] = '-';
            if(i == txt.length() - 1){
                txt[i] = '>';
            }
        }
    }

    txt += pnode->get_node_str();
    print(txt);
    
    for(parser_node* child : pnode->m_children)
    {
        _dump_pnode(child, depth + 1);
    }
}

void driver::_dump_tree()
{
    if(m_parser == nullptr){
        return;
    }

    print_verbose("Dumping syntax tree");
    _dump_pnode(&this->m_parser->m_root, 0);
}

void driver::_write_diagnostic(diagnostic& diag)
{
    switch(diag.m_level)
    {
        case diagnostic_level::error:
        print_error(diag.m_msg);
        break;
        case diagnostic_level::warning:
        print_warning(diag.m_msg);
        break;
        case diagnostic_level::info:
        print(diag.m_msg);
        break;
    }
    //print_error(diag.m_msg);

    if(diag.m_show)
    {
        // line length
        unsigned int cursorl = diag.m_clength;

        if(diag.m_cursor_line)
        {
            if(diag.m_loc.line-1>=0 && diag.m_loc.line-1<diag.m_loc.src_ptr->get_lines()->at(diag.m_loc.line-1).size())
            {
                cursorl = diag.m_loc.src_ptr->get_lines()->at(diag.m_loc.line-1).size() - (diag.m_loc.col-1);
            }
        }

        _write_src_view(
            diag.m_vlcount, 
            diag.m_has_cursor,
            cursorl,
            diag.m_loc
        );
    }
}

void driver::_write_src_view(unsigned int view_padding, bool has_cursor, unsigned int cursor_length, location loc)
{
    if(loc.src_ptr == nullptr)
    {
        print("Could not view source.");
    }

    unsigned int cursor_line = (loc.line-1);
    unsigned int start_line = cursor_line - std::min(cursor_line, view_padding);
    unsigned int end_line = cursor_line + std::min((unsigned int)loc.src_ptr->get_lines()->size() - cursor_line, view_padding);

    print("--> " + loc.as_string());
    for(int i = start_line; i <= end_line; i++)
    {
        std::string buffer = std::to_string(start_line+1);
        buffer.resize(4, ' ');
        print(buffer + "| " + loc.src_ptr->get_lines()->at(start_line));

        if(has_cursor && i == cursor_line)
        {
            std::string txt;
            for(int j = 0; j < (loc.col - 1) + 6; j++){
                txt += " ";
            }
            for(int j = 0; j < cursor_length; j++)
            {
                txt += "^";
            }
            print(txt);
        }
    }
}