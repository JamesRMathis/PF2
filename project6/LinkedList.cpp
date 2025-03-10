#include <iostream>
#include <set>
#include "LinkedList.h"

using std::cout;
using std::endl;

int allocations = 0;

LinkedListNode::LinkedListNode(const std::string& val) {
    allocations++;
    this->next = nullptr;
    this->value = val;
}

int deallocations = 0;

LinkedListNode::~LinkedListNode()
{
    deallocations++;
    delete(this->next);
}

void LinkedListNode::print()
{
        cout << this->value;
        if (this->next) {
                cout << " -> ";
                this->next->print();
        } 
}

void LinkedListNode::linkNewNode(LinkedListNode *newNode) {
    if (this->next == nullptr) {
        this->next = newNode;
    } else {
        this->next->linkNewNode(newNode);
    }
}

LinkedList::LinkedList() {
    this->first = nullptr;
    this->last = nullptr;
    this->length = 0;
}

LinkedList::~LinkedList() {
    if (this->first) delete(this->first);
}

void LinkedList::print()
{
        if (this->first)
            this->first->print();
        cout << endl;
} 

void LinkedList::push_back(const std::string &s) {
    LinkedListNode *newNode = new LinkedListNode(s);
    this->length++;
    
    if (!this->first) {
        this->first = newNode;
        this->last = newNode;
    } else {
        this->first->linkNewNode(newNode);
        this->last = newNode;
    }
}

void LinkedList::push_front(const std::string &s) {
    LinkedListNode *newNode = new LinkedListNode(s);
    this->length++;

    if (!this->first) {
        this->first = newNode;
        this->last = newNode;
    } else {
        newNode->next = this->first;
        this->first = newNode;
    }
}

std::string LinkedList::pop_front() {
    if (!this->first) throw std::runtime_error("LinkedList is empty");

    std::string returnStr = this->first->value;
    this->length--;

    LinkedListNode *secondNode = this->first->next;
    this->first->next = nullptr;
    delete(this->first);
    this->first = secondNode;

    return returnStr;
}

int LinkedList::size() {
    return this->length;
}

void LinkedList::split(int n, LinkedList& other) {
    if (n <= 0) throw std::runtime_error("n must be > 0");
    if (this->size() < n) throw std::runtime_error("Not enough items in LinkedList");

    for (int i = 0; i < n; i++) {
        std::string val = this->pop_front();
        other.push_back(val);
    }
}

void LinkedList::check()
{
    if (this->first && !this->last)
        throw std::runtime_error("first but no last");
    if (!this->first && this->last)
        throw std::runtime_error("last but no first");
    if (this->first == this->last && this->first && this->first->next)
        throw std::runtime_error("first and last are the same, but first has a next");
    if (this->first != this->last && !this->first->next)
        throw std::runtime_error("first and last are different, but first has no next");
    int count = 0;
    LinkedListNode* pLast = nullptr;
    std::set<LinkedListNode*> seen_before;
    for (LinkedListNode* pNode = this->first; pNode; pNode = pNode->next)
    {
        if (seen_before.find(pNode) != seen_before.end())
            throw std::runtime_error("cycle or duplicate node in the linked list");
        seen_before.insert(pNode);
        count++;
        pLast = pNode;
    }
    if (count != this->length)
        throw std::runtime_error("length does not match the number of nodes in the list");
    if (pLast != this->last)
        throw std::runtime_error("last is not in the list");
    if (pLast && pLast->next)
        throw std::runtime_error("last has a next");
}