#include "Dataset.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

using std::strcmp;
using std::sort;
using std::cout;
using std::endl;

int smart_compare(const std::string& str_a, const std::string& str_b) {
    // Strip whitespace from both ends of str_a and str_b
    std::string a = str_a;
    while (a.size() > 0 && std::isspace(a[a.size() - 1]))
        a.erase(a.end() - 1);
    while (a.size() > 0 && std::isspace(a[0]))
        a.erase(a.begin());
    std::string b = str_b;
    while (b.size() > 0 && std::isspace(b[b.size() - 1]))
        b.erase(b.end() - 1);
    while (b.size() > 0 && std::isspace(b[0]))
        b.erase(b.begin());

    // Skip the parts that are the same
    unsigned i = 0;
    while(i < a.size() && i < b.size() && a[i] == b[i]) {
        i++;
    }
    if (i >= a.size()) {
        if (i >= b.size()) {
            return 0;
        } else {
            return -1;
        }
    } else if (i >= b.size()) {
        return 1;
    }

    // Skip zeros
    unsigned int a_start = i;
    unsigned int b_start = i;
    while (a_start < a.size() && a[a_start] == '0')
        a_start++;
    while (b_start < b.size() && b[b_start] == '0')
        b_start++;

    // Count digits
    unsigned int a_digits = a_start;
    while (a_digits < a.size() && a[a_digits] >= '0' && a[a_digits] <= '9')
        a_digits++;
    unsigned int b_digits = b_start;
    while (b_digits < b.size() && b[b_digits] >= '0' && b[b_digits] <= '9')
        b_digits++;
    if (a_digits > 0 && a_digits < b_digits) {
        return -1; // a comes first because its number is shorter
    } else if (b_digits > 0 && b_digits < a_digits) {
        return 1; // b comes first because its number is shorter
    } else {
        // The numbers are the same length, so compare alphabetically
        return strcmp(a.c_str() + a_start, b.c_str() + b_start);
    }
}

Dataset::Dataset() {

}

Dataset:: ~Dataset() {

}

void Dataset::load_csv(string& filename)
{
    // Clear any existing data
    this->data.clear();

    // Open the file
    std::ifstream stream;
    stream.open(filename);
    if (!stream.is_open()) {
        throw std::system_error(errno, std::generic_category(), filename);
    }

    // Read the file
    while (!stream.eof())
    {
        // Read a line
        string s;
        if (!std::getline(stream, s))
            break;
        vector<string> empty;
        this->data.push_back(empty);
        vector<string>& current_row = this->data.back();

        // Break up the line into cells
        while (true)
        {
            // Find the next comma
            size_t pos = s.find(",");
            if (pos == std::string::npos)
            {
                // This is the last cell in the row
                current_row.push_back(s);
                break;
            }
            else
            {
                // This is not the last cell in the row
                current_row.push_back(s.substr(0, pos));
                s.erase(0, pos + 1); // erase the cell and the comma
            }
        }

        // Ensure all rows have the same size
        if (current_row.size() < 1)
        {
            // This row is empty, so just drop it
            this->data.pop_back();
        }
        else if (current_row.size() != this->data[0].size())
        {
            // Uh oh, the row is the wrong size!
            string s = "Error, row " + this->data.size();
            s += " has " + current_row.size();
            s += " elements. Expected " + this->data[0].size();
            throw std::runtime_error(s);
        }
    }

    // Move the first row into this->col_names
    this->col_names = this->data[0];
    this->data.erase(this->data.begin());
}

int Dataset::col_count() const {
    return int(this->col_names.size());
}

int Dataset::row_count() const {
    if (col_count() == 0) {
        return 0;
    }
    return int(this->data.size());
}

vector<string>& Dataset::get_row(int i) {
    return this->data[i];
}

int active_column = 0;

bool custom_less(vector<string>* vector1, vector<string>* vector2) {
    string val1 = (*vector1)[active_column];
    string val2 = (*vector2)[active_column];

    return smart_compare(val1, val2) < 0;
}

void Dataset::index_data()
{
    // Index the data
    this->indices.clear();
    this->indices.resize(this->col_count());
    for (int i = 0; i < this->col_count(); i++)
    {
        // Build an index for column i
        vector< vector<string>* >& index = this->indices[i];
        for (int j = 0; j < this->row_count(); j++)
            index.push_back(&this->data[j]);

        // Sort the index in column i using smart_compare
        active_column = i;
        std::sort(index.begin(), index.end(), custom_less);
    }
}

void Dataset::print_index(int col_num)
{
    cout << "Sorted by column " << col_num << ":" << endl;
    cout << "-------------------" << endl;
    vector< vector<string>*> index = this->indices[col_num];
    for (size_t i = 0; i < index.size(); i++)
    {
        vector<string>& row = *index[i];
        for (size_t j = 0; j < row.size(); j++)
        {
            if (j > 0)
                cout << ", ";
            cout << row[j];
        }
        cout << endl;
    }
}

int Dataset::bSearch(int col, string val) const {
    int floor = 0;
    int ceil = this->row_count() - 1;
    int rowNum;

    while (true) {
        rowNum = (floor + ceil) / 2;
        if (rowNum == floor) break;
        string valAtRow = this->indices[col][rowNum]->at(col);
        int result = smart_compare(valAtRow, val);
        if (result < 0) {
            floor = rowNum;
        } else {
            ceil = rowNum;
        }
    }

    while (rowNum > 0 && smart_compare(this->indices[col][rowNum - 1]->at(col), val) >= 0) rowNum--;
    while (rowNum < this->row_count() && smart_compare(this->indices[col][rowNum]->at(col), val) < 0) rowNum++;

    return rowNum;
}

void Dataset::query(string startVal, string endVal, int col) const {
    int rowInd = this->bSearch(col, startVal);

    while (smart_compare(this->indices[col][rowInd]->at(col), endVal) < 0) {
        vector<string>& row = *(this->indices[col][rowInd]);
        for (size_t j = 0; j < row.size(); j++)
        {
            if (j > 0)
                cout << ", ";
            cout << row[j];
        }
        cout << endl;
        rowInd++;
    }
}