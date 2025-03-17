#include <iostream>
#include <set>
#include <cstring>
#include <vector>
#include "LinkedList.h"

using std::cout;
using std::endl;

int smart_compare(const std::string& str_a, const std::string& str_b, int& comparisons) {
    comparisons++;

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

void LinkedList::merge(LinkedList& other, int& comparisons) {
    LinkedList mergedList;

    while (this->first && other.first) {
        std::string thisItem = this->pop_front();
        std:: string otherItem = other.pop_front();

        int result = smart_compare(thisItem, otherItem, comparisons);

        if (result <= 0) {
            mergedList.push_back(thisItem);
            other.push_front(otherItem); // Put back the item not used
        } else {
            mergedList.push_back(otherItem);
            this->push_front(thisItem); // Put back the item not used
        }
    }

    while (this->first) {
        mergedList.push_back(this->pop_front());
    }

    while (other.first) {
        mergedList.push_back(other.pop_front());
    }

    this->first = mergedList.first;
    this->last = mergedList.last;
    this->length = mergedList.length;

    mergedList.first = nullptr;
    mergedList.last = nullptr;
}

void LinkedList::sort(int& comparisons) {
    if (this->length == 1) return;

    LinkedList right;
    this->split(this->length / 2, right);
    this->sort(comparisons);
    right.sort(comparisons);
    this->merge(right, comparisons);

    right.first = nullptr;
    right.last = nullptr;
}