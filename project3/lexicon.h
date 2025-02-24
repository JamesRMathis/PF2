#ifndef lexicon_h
#define lexicon_h

#include <iostream>
#include <string>

// A data structure that holds words
class Lexicon
{
protected:
    std::string* words;
    int word_count;
    int capacity;

public:
    Lexicon();
    virtual ~Lexicon();
    void push_back(std::string& s);
    void pop_back();
    std::string &get(int index);
    std::string &back();
    int size();
};

#endif