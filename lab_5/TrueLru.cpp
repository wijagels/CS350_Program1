#include "TrueLru.hpp"
#include <algorithm>
#include <iostream>

norm_iterator TrueLru::select_victim() {
    // std::cout << "Victim selection" << std::endl;
    norm_iterator e = std::min_element(pt->table.begin(), pt->table.end(),
            [](Page one, Page two) {
            return one.attrs.front() < two.attrs.front();
            });
    std::cout << "Selected page..." << std::endl << *e;
    std::cout << "Last: " << e->attrs.front() << std::endl;
    return e;
}

void TrueLru::access_hook(norm_iterator e) {
    e->attrs.front() = ++last;
}

void TrueLru::add_hook() {
    // std::cout << "Add hook caught " << last << std::endl;
    std::list<int>& attrs = pt->table.back().attrs;
    attrs.emplace(attrs.begin(), ++last);
}
