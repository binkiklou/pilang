#include "driver.hpp"

#include "../shared/print.hpp"

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
        print_verbose("Tokenization failed, aborting compilation.");
        _delete_phases();
        return;
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
}

void driver::_delete_phases()
{
    print_verbose("Deleting phases");

    if(this->m_lexer != nullptr)
    {
        delete this->m_lexer;
        this->m_lexer = nullptr;
    }
}