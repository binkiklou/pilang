#pragma once

#include <vector>
#include <string>

// Manages information about a file
class source
{
    public:
    source();
    ~source();

    std::string filename;

    void set_lines(std::vector<std::string>*);
    std::vector<std::string>* get_lines();
    private:
    std::vector<std::string>* m_lines;
};

class location
{
    public:
    unsigned int col = 0;
    unsigned int line = 0;
    source* src_ptr = nullptr;

    std::string as_string();
};


// Location inside a source file
class word
{
    public:
    std::string data;
    location loc;
};