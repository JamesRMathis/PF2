#include "config.h"
#include <iostream>
#include <cstring>

// Instantiate a global config object
Config config;

// Print a message iff we are in debug mode
void log(const std::string &s)
{
    if (config.debug)
    {
        std::cout << s << std::endl;
    }
}

Config::Config()
{
    this->debug = false;
}

Config::~Config()
{
}

void Config::parse_flags(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        if (strcmp(arg, "debug") == 0)
        {
            this->debug = true;
        } else if (strcmp(arg, "failure_line") == 0) {
            std::cout << "this->words[this->word_count] = s;" << std::endl;
        }
        else
        {
            std::string s = "Error: unrecognized argument: ";
            s += arg;
            throw std::runtime_error(s);
        }
    }
}