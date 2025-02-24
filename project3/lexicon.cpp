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
    if (this->word_count == this->capacity)
    {
        this->capacity *= 2;
        std::string* new_words = new std::string[this->capacity];
        for (int i = 0; i < this->word_count; i++)
        {
            new_words[i] = this->words[i];
        }
        delete[] this->words;
        this->words = new_words;
    } else {
        // log(std::string("before the bug:") + " word_count=" + std::to_string(this->word_count) + ", capacity=" + std::to_string(this->capacity) + ", s=" + s);
        this->words[this->word_count] = s;
        this->word_count++;
    }
}

void Lexicon::pop_back()
{
    this->word_count--;
}

std::string& Lexicon::get(int index)
{
    if (index < 0)
    {
        throw std::runtime_error("Expected a non-negative index. Got " + std::to_string(index));
    }
    if (index >= this->word_count)
    {
        throw std::runtime_error("Expected index (" + std::to_string(index) + ") to be less than word_count (" + std::to_string(this->word_count) + ")");
    }

    return this->words[index];
}

std::string& Lexicon::back() {
    return this->get(this->word_count - 1);
}

int Lexicon::size() {
    return this->word_count;
}