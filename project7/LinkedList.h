#ifndef linked_list
#define linked_list

#include <string>

// Represents a single node in a linked list
class LinkedListNode
{
public:
    std::string value;
    LinkedListNode* next;

    LinkedListNode(const std::string& val);
    virtual ~LinkedListNode();

    void print();
    void linkNewNode(LinkedListNode *newNode);
};



// A data structure that holds a list of strings
class LinkedList
{
protected:
    LinkedListNode* first;
    LinkedListNode* last;
    int length;

public:
    LinkedList();
    virtual ~LinkedList();

    void print();
    void push_back(const std::string& s);
    void push_front(const std::string& s);
    std::string pop_front();
    int size();
    void split(int n, LinkedList& other);
    void check();
    void merge(LinkedList& other, int& comparisons);
    void sort(int& comparisons);
};

#endif