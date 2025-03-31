#include <iostream>
#include "config.h"
#include <vector>
#include <unistd.h>
#include "charmatrix.h"
#include "LinkedList.h"
#include "Dataset.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::runtime_error;

vector<string> lex;
CharMatrix g_grid;
Dataset g_dataset;

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

void findWord(string word, string foundChars, int index, vector<int> coords, vector<vector<int> > visited) {
    bool haveVisited = false;
    for (unsigned int coordInd = 0; coordInd < visited.size(); coordInd++) {
        if (visited[coordInd] == coords) {
            haveVisited = true;
            break;
        }
    }

    if (haveVisited) return;
    else visited.push_back(coords);

    if (foundChars == word) {
        cout << word << endl;
        return;
    }

    char charToFind = word[index];
    int x = coords[0];
    int y = coords[1];

    for (int dy = -1; dy < 2; dy++) {
        for (int dx = -1; dx < 2; dx++) {
            if (dy == 0 && dx == 0) continue;
            if (!((0 <= y + dy && y + dy < g_grid.height()) && (0 <= x + dx && x + dx < g_grid.width()))) continue;

            vector<int> newCoords = {x + dx, y + dy};
            if (g_grid.get(x + dx, y + dy) == charToFind) {
                findWord(word, foundChars + g_grid.get(x + dx, y + dy), index + 1, newCoords, visited);
            }
        }
    }

    return;
}

void boggle() {
    for (unsigned int i = 0; i < lex.size(); i++) {
        string word = lex[i];
        for (int y = 0; y < g_grid.height(); y++) {
            for (int x = 0; x < g_grid.width(); x++) {
                if (g_grid.get(x, y) != word[0]) continue;
                string foundChars = "";
                foundChars += word[0];
                vector<vector<int> > visited;
                vector<int> coords = {x, y};
                // visited.push_back(coords);
                findWord(word, foundChars, 1, coords, visited);
            }
        }
    }
}

void load_csv()
{
    cout << "Please enter a filename of a .csv file:" << endl;
    cout << "> ";
    string filename;
    getline(std::cin, filename);
    g_dataset.load_csv(filename);
    g_dataset.index_data();
}

void query()
{
    // Ask for query parameters
    cout << "Please enter a column index:" << endl;
    cout << "> ";
    string column_str;
    getline(std::cin, column_str);
    int column = stoi(column_str);
    cout << "Please enter a starting value:" << endl;
    cout << "> ";
    string start;
    getline(std::cin, start);
    cout << "Please enter an ending value:" << endl;
    cout << "> ";
    string end;
    getline(std::cin, end);

    // Perform the query
    g_dataset.query(start, end, column); // (you will write this method)
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
        cout << "6. Boggle" << endl;
        cout << "7. Linked List unit test" << endl;
        cout << "8. Merge Sort" << endl;
        cout << "9. Load CSV file" << endl;
        cout << "10. Query" << endl;
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
        } else if (option.compare("6") == 0) {
            boggle();
        } else if (option.compare("7") == 0) {
            LinkedList list;
            list.check();
            list.push_back("10");
            list.print();
            list.check();
            list.push_front("hello");
            list.print();
            list.check();
            LinkedList other;
            list.split(1, other);
            list.check();
            other.check();
            list.print();
            other.print();
            list.pop_front();
            cout << "passed" << endl;
        } else if (option.compare("8") == 0) {
            LinkedList wordList;
            for (unsigned int i = 0; i < lex.size(); i++) {
                string elem = lex[i];
                wordList.push_back(elem);
            }

            lex.clear();

            int comparisons = 0;
            wordList.sort(comparisons);
            int size = wordList.size();
            for (int i = 0; i < size; i++) {
                lex.push_back(wordList.pop_front());
            }
            cout << "comparisons: " << comparisons << endl;
        } else if (option.compare("9") == 0) {
            load_csv();
        } else if (option.compare("10") == 0) {
            query();
        } else {
            cout << option << " was not one of the options. Quitting." << endl;
            break;
        }
    }
}