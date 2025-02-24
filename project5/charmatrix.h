#ifndef charmatrix_h
#define charmatrix_h

#include <vector>
#include <string>

class CharMatrix
{
    protected:
        std::vector<std::string> chars;

    public:
        CharMatrix();
        virtual ~CharMatrix();

        int width() const;
        int height() const;
        char get(int x, int y);
        void put(int x, int y, char c);
        void resize(int width, int height);

};

#endif