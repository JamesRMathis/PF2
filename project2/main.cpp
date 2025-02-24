#include <iostream>
#include "config.h"
#include "lexicon.h"

// Count from 0 to n-1
void count(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << i << " ah-ah-ah!" << std::endl;
    }
}

// Entry point
int main(int argc, char** argv)
{
    config.parse_flags(argc, argv);
    log("---Running in debug mode---");

    Lexicon lex;
    for (int loop = 0; loop < 8; loop++)
    {
        for (int i = 0; i < 5; i++)
        {
            std::cout << std::endl;
        }
        std::cout << "Please enter another word" << std::endl;
        std::cout << "> ";
        std::string word;
        getline(std::cin, word);
        if (word.size() == 0)
        {
            break;
        }
        lex.push_back(word);

        std::cout << "So far, the words you have entered are:" << std::endl;
        for (int i = 0; i < lex.size(); i++)
        {
            std::cout << i << ". " << lex.get(i) << std::endl;
        }
    }
}