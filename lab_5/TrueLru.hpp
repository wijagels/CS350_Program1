#pragma once
#include "Page.hpp"
#include "PageTable.hpp"
#include <vector>

class TrueLru {
    public:
        TrueLru(PageTable<TrueLru>* pt) : pt(pt) {}
        norm_iterator select_victim();
        void access_hook(norm_iterator);
        void add_hook();
        const std::string name = "Least Recently Used";
    private:
        PageTable<TrueLru>* pt;
        std::uint64_t last = 0;
};
