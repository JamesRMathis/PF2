#ifndef dataset_h
#define dataset_h

#include <vector>
#include <string>

using std::vector;
using std::string;

class Dataset {
    protected:
        vector<string> col_names;
        vector< vector<string> > data;
        vector<vector <vector<string>* > > indices;

    public:
        Dataset();
        virtual ~Dataset();

        void load_csv(string& filename);
        int col_count() const;
        int row_count() const;
        vector<string>& get_row(int i);
        void index_data();
        void print_index(int col_num);
        int bSearch(int col, string val) const;
        void query(string startVal, string endVal, int col) const;
};

#endif