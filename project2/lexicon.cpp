#include "lexicon.h"
#include "config.h"

Lexicon::Lexicon()
{
    this->word_count = 0;
    this->capacity = 8;
    this->words = new std::string[this->capacity];
}

Lexicon::~Lexicon()
{
    delete[] this->words;
}

void Lexicon::push_back(std::string& s)
{
    log(std::string("before the bug:") + " word_count=" + std::to_string(this->word_count) + ", capacity=" + std::to_string(this->capacity) + ", s=" + s);
    this->words[this->word_count] = s;
    this->word_count++;
}

std::string& Lexicon::get(int index)
{
    return this->words[index];
}

int Lexicon::size() {
    return this->word_count;
}