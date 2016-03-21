#include "Page.hpp"

bool Page::contains(unsigned int addr) {
    return start < addr && addr < start + size;
}


std::ostream& operator<<(std::ostream& os, const Page& p) {
    os << "Start: ";
    os << p.start << std::endl;
    os << "Size: ";
    os << p.size << std::endl;
    return os;
}
