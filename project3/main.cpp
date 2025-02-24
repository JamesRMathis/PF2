#include <iostream>
#include "config.h"
#include "lexicon.h"

Lexicon lex;

// Count from 0 to n-1
void count(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << i << " ah-ah-ah!" << std::endl;
    }
}

void fill_lexicon()
{
    while (true)
    {
        for (int i = 0; i < 5; i++)
        {
            if (!config.quiet)
            {
                std::cout << std::endl;
            }
        }
        if (!config.quiet)
        {
            std::cout << "Please enter another word" << std::endl;
            std::cout << "> ";
        }
        std::string word;
        getline(std::cin, word);
        if (word.size() == 0)
        {
            break;
        } else if (word.compare("-") == 0)
        {
            lex.pop_back();
        } else
        {
            lex.push_back(word);
        }

        if (!config.quiet)
        {
            std::cout << "So far, the words you have entered are:" << std::endl;
            for (int i = 0; i < lex.size(); i++)
            {
                std::cout << i << ". " << lex.get(i) << std::endl;
            }
        }
    }
}

// Entry point
int main(int argc, char** argv)
{
    config.parse_flags(argc, argv);
    log("---Running in debug mode---");

    while (true)
    {
        for (int i = 0; i < 5; i++)
        {
            std::cout << std::endl;
        }
        std::cout << "James Mathis's PF2 projects" << std::endl << std::endl;
        std::cout << "Lexicon size: " << lex.size() << std::endl;
        std::cout << "0. Quit" << std::endl;
        std::cout << "1. Fill lexicon" << std::endl;
        std::cout << "2. Tear down lexicon" << std::endl;
        std::cout << "> ";
        std::string option;
        getline(std::cin, option);
        if (option.compare("0") == 0) {
            break;
        } else if (option.compare("1") == 0) {
            fill_lexicon();
        } else if (option.compare("2") == 0) {
            while (lex.size() > 0)
            {
                std::cout << lex.back() << std::endl;
                lex.pop_back();
            }
            
        } else {
            std::cout << option << " was not one of the options. Quitting." << std::endl;
            break;
        }
    }
}