#pragma once
#include "Page.hpp"
#include "PageTable.hpp"
#include <vector>

class Mfu {
    public:
        Mfu(PageTable<Mfu>* pt) : pt(pt) {}
        norm_iterator select_victim();
        void access_hook(norm_iterator);
        void add_hook();
        const std::string name = "Most Frequently Used";
    private:
        PageTable<Mfu>* pt;
};
