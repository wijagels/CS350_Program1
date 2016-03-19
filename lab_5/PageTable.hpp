#include "Page.hpp"
#include <vector>

class PageTable {
    PageTable();
    PageTable(unsigned int capacity) : capacity(capacity) {}
    unsigned int capacity;
};
