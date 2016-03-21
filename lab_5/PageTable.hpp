#pragma once

#include "Page.hpp"
#include <vector>
#include <algorithm>

typedef std::vector<Page>::iterator norm_iterator;

template <class Alg>
class PageTable {
    public:
        PageTable(unsigned int mem_size) : algorithm(this), capacity(mem_size) {
        }

        void add(const Page& p) {
            if (in_mem >= capacity) {
                norm_iterator e = algorithm.select_victim();
                swap.push_back(*e);
                table.erase(e);
                --this->in_mem;
            }
            table.push_back(p);
            ++this->in_mem;
            algorithm.add_hook();
        }

        void access(unsigned int address) {
            norm_iterator e = std::find_if(table.begin(), table.end(), [address](Page p) {
                    return p.contains(address);
                    });
            if(e == table.end()) {
                std::cout << "Page fault!" << std::endl;
                norm_iterator e = std::find_if(swap.begin(), swap.end(), [address](Page p) {
                        return p.contains(address);
                        });
                norm_iterator victim = algorithm.select_victim();
                std::swap(victim, e);
                e = victim;
                algorithm.access_hook(e);
            }
            else {
                std::cout << *e;
                algorithm.access_hook(e);
            }
        }
        std::vector<Page> table;
        std::vector<Page> swap;
    private:
        Alg algorithm;
        unsigned int capacity;
        unsigned int in_mem;
};
