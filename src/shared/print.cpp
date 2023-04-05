#include "print.hpp"

#include <iostream>

std::string colored(const std::string& text, colors c)
{
    if(!print_args::has_color){
        return text;
    }

    std::string prefix = "\u001b[";
    switch(c)
    {
        case RED:
        prefix += "31m";
        break;
        case GREEN:
        prefix += "32m";
        break;
        case BLUE:
        prefix += "34m";
        break;
        case YELLOW:
        prefix += "33m";
        break;
    }
    return prefix + text + "\u001b[0m";
}

void print(std::string str)
{
    std::cout<<str<<"\n";
}

void print_verbose(std::string str)
{
    if(print_args::is_verbose)
    {
        std::cout<<str<<"\n";
    }
}

void print_error(std::string str)
{
    std::cout<<colored("[ERROR] ",RED)<<str<<"\n";
}