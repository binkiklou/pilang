#include "driver/driver.hpp"
#include "shared/envargs.hpp"
#include "shared/print.hpp"

#include <memory>

#include <string.h>

// There is probably a betetr way then this
bool envargs::is_verbose;

int main(int argc, char* argv[])
{
    driver d;

    // Default values
    envargs::is_verbose = false;

    // Parses command line arguments
    source* s = nullptr;

    for(int i = 1; i < argc; i++)
    {
        if(strcmp("-v",argv[i]) == 0)
        {
            envargs::is_verbose = true;
            print_verbose("Verbose mode activated");
        }
        else if(strcmp("-dump-tokens",argv[i]) == 0)
        {
            d.dump_tokens = true;
        }
        else if(s == nullptr)
        {
            s = new source;
            s->filename = std::string(argv[i]);
            print_verbose("Set filename to " + s->filename);
        }
        else
        {
            print_verbose("Tried to set another file, ignored.");
        }
    }

    if(s == nullptr)
    {
        print_error("Please specify a filename");
        return -1;
    }

    d.start(s);

    delete s;
    return 0;
}