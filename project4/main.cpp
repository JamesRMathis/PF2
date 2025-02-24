#include <iostream>
#include "config.h"
#include <vector>
#include <unistd.h>
#include "charmatrix.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::runtime_error;

vector<string> lex;
CharMatrix g_grid;

// Count from 0 to n-1
void count(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << i << " ah-ah-ah!" << endl;
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
                cout << endl;
            }
        }
        if (!config.quiet)
        {
            cout << "Please enter another word" << endl;
            cout << "> ";
        }
        string word;
        getline(cin, word);
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
            cout << "So far, the words you have entered are:" << endl;
            for (unsigned i = 0; i < lex.size(); i++)
            {
                cout << i << ". " << lex[i] << endl;
            }
        }
    }
}

void load_char_matrix()
{
    if (!config.quiet)
    {
        cout << "Please enter a grid of characters." << endl;
        cout << "All rows should have the same length." << endl;
        cout << "When you are done, just press Enter." << endl;
    }
    vector<string> grid;
    while(true)
    {
        string row;
        getline(cin, row);
        if (row.compare("") == 0)
            break;
        if (grid.size() > 0 && row.size() != grid[0].size())
            throw runtime_error("Rows in a CharMatrix must all have the same size!");
        grid.push_back(row);
    }
    int height = grid.size();
    int width = 0;
    if (height > 0)
        width = grid[0].size();
    g_grid.resize(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            g_grid.put(x, y, grid[y][x]);
        }
    }
}

void print_char_matrix()
{
    for (int y = 0; y < g_grid.height(); y++)
    {
        for (int x = 0; x < g_grid.width(); x++)
        {
            cout << g_grid.get(x, y);
        }
        cout << endl;
    }
}

void fill (int x, int y, char c, int max_depth) {
    if (config.debug)
    {
        usleep(750000);
        for (int i = 0; i < 20; i++)
            cout << endl;
        print_char_matrix();
        cout.flush();
    }

    char charToReplace = g_grid.get(x, y);
    g_grid.put(x, y, c);

    for (int dy = -1; dy < 2; dy++) {
        for (int dx = -1; dx < 2; dx++) {
            if (max_depth == 0) return;
            if (dy == dx || dy == -dx) continue;
            if (!(0 <= x + dx && x + dx < g_grid.width()) || !(0 <= y + dy && y + dy < g_grid.height())) continue;
            if (g_grid.get(x + dx, y + dy) == c || g_grid.get(x + dx, y + dy) != charToReplace) continue;
            fill(x + dx, y + dy, c, max_depth - 1);
        }
    }
    return;
}

void flood_fill()
{
    string word;
    cout << "Please enter a starting column:" << endl;
    cout << "> ";
    getline(cin, word);
    int x = stoi(word);
    cout << "Please enter a starting row:" << endl;
    cout << "> ";
    getline(cin, word);
    int y = stoi(word);
    cout << "Please enter a fill character:" << endl;
    cout << "> ";
    getline(cin, word);
    int c = word[0];
    cout << "Please enter the max fill depth:" << endl;
    cout << "> ";
    getline(cin, word);
    int max_depth = stoi(word);
    fill(x, y, c, max_depth);
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
            cout << endl;
        }
        cout << "James Mathis's PF2 projects" << endl << endl;
        cout << "Lexicon size: " << lex.size() << endl;
        cout << "0. Quit" << endl;
        cout << "1. Fill lexicon" << endl;
        cout << "2. Tear down lexicon" << endl;
        cout << "3. Load char matrix" << endl;
        cout << "4. Print char matrix" << endl;
        cout << "5. Flood fill" << endl;
        cout << "> ";
        string option;
        getline(cin, option);
        if (option.compare("0") == 0) {
            break;
        } else if (option.compare("1") == 0) {
            fill_lexicon();
        } else if (option.compare("2") == 0) {
            while (lex.size() > 0)
            {
                cout << lex.back() << endl;
                lex.pop_back();
            }
        } else if (option.compare("3") == 0) {
            load_char_matrix();
        } else if (option.compare("4") == 0) {
            print_char_matrix();
        } else if (option.compare("5") == 0) {
            flood_fill();
        } else {
            cout << option << " was not one of the options. Quitting." << endl;
            break;
        }
    }
}