#include "Mfu.hpp"
#include <algorithm>
#include <iostream>

norm_iterator Mfu::select_victim() {
    norm_iterator e = std::max_element(pt->table.begin(), pt->table.end(),
            [](Page one, Page two) {
            return one.attrs.front() < two.attrs.front();
            });
    return e;
}

void Mfu::access_hook(norm_iterator e) {
    ++e->attrs.front();
}

void Mfu::add_hook() {
    // std::cout << "Add hook caught " << last << std::endl;
    std::list<std::uint64_t>& attrs = pt->table.back().attrs;
    attrs.emplace(attrs.begin(), 0);
}
