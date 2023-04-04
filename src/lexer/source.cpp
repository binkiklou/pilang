#include "source.hpp"

#include "../shared/print.hpp"

source::source()
{
    this->m_lines = nullptr;
}

source::~source()
{
    print_verbose("Deleting source manager");

    if(this->m_lines != nullptr)
    {
        delete this->m_lines;
    }
}

void source::set_lines(std::vector<std::string>* l)
{
    print_verbose("Set lines for file");
    this->m_lines = l;
}

std::vector<std::string>* source::get_lines()
{
    return this->m_lines;
}

std::string location::as_string()
{
    if(src_ptr == nullptr)
    {
        return std::to_string(line) + ":" + std::to_string(col);
    }
    return "<" + src_ptr->filename + ":" +
    std::to_string(line) + ":" +
    std::to_string(col) + ">";
}