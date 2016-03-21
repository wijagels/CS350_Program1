#pragma once
#include <utility>
#include <list>
#include <iostream>

class Page {
    public:
        // Constructors
        Page() {}
        Page(unsigned int owner, std::uint64_t number) : owner(owner), number(number) {}
        // Public data members
        unsigned int owner = 0;
        std::uint64_t number = 0;
        std::list<std::uint64_t> attrs;
        friend std::ostream& operator<<(std::ostream&, const Page&);
};
