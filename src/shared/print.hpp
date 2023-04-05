#pragma once

#include <string>

// Global print args, shouldn't be changed after startup
class print_args
{
    public:
    static bool is_verbose;
    static bool has_color;
};

enum colors
{
    RED,
    GREEN,
    BLUE,
    YELLOW
};

// Gives color visible in the terminal to the string
std::string colored(const std::string&, colors);

void print(std::string);
void print_verbose(std::string);
void print_error(std::string);