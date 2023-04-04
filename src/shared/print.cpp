#include "print.hpp"

#include "envargs.hpp"

#include <iostream>

void print(std::string str)
{
    std::cout<<str<<"\n";
}

void print_verbose(std::string str)
{
    if(envargs::is_verbose)
    {
        std::cout<<str<<"\n";
    }
}

void print_error(std::string str)
{
    std::cout<<"[ERROR]"<<str<<"\n";
}