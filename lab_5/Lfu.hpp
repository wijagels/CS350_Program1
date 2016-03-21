#pragma once
#include "Page.hpp"
#include "PageTable.hpp"
#include <vector>

class Lfu {
    public:
        Lfu(PageTable<Lfu>* pt) : pt(pt) {}
        norm_iterator select_victim();
        void access_hook(norm_iterator);
        void add_hook();
        const std::string name = "Least Frequently Used";
    private:
        PageTable<Lfu>* pt;
};
