#pragma once
#include <utility>
#include <list>
#include <iostream>

class Page {
    public:
        // Constructors
        Page() {}
        Page(unsigned int owner, unsigned int start, unsigned int size) : owner(owner), start(start), size(size) {}
        // Methods
        bool contains(unsigned int);
        // Public data members
        bool in_memory = false;
        unsigned int owner = 0;
        unsigned int frame = 0;
        unsigned int start = 0;
        unsigned int size = 0;
        std::list<int> attrs;
        friend std::ostream& operator<<(std::ostream&, const Page&);
};
