#pragma once

#include "Page.hpp"
#include <vector>
#include <algorithm>

typedef std::vector<Page>::iterator norm_iterator;

template <class Alg>
class PageTable {
    public:
        PageTable(unsigned int mem_size) : algorithm(this), capacity(mem_size) {}

        void print_stats(bool machine_readable) {
            if(!machine_readable) {
                std::cout << "Algorithm: " << algorithm.name
                    << "\nTotal faults: " << faults
                    << "\nTotal accesses: " << accesses
                    << "\nFault percentage: " << (double) 100*faults/accesses
                    << std::endl;
            } else {
                std::cout << algorithm.name << "," << faults
                    << "," << accesses << "," << (double) 100*faults/accesses
                    << std::endl;
            }
        }

        /*
         * Add a new page into the page table, hopefully into main memory.
         * If main memory is full, then kick something out of main memory
         * and insert the new page.
         */
        void add(const Page& p) {
            ++accesses;
            if (table.size() >= capacity) {
                ++faults;
                // std::cout << "Page fault on add!" << std::endl;
                norm_iterator e = algorithm.select_victim();
                swap.push_back(*e);
                table.erase(e);
            }
            table.push_back(p);
            algorithm.add_hook();
        }

        /*
         * On access, check if the page is in memory
         * If it's not, then check swap and generate a page fault
         * If it's also not in swap, then it's a new page that needs to be
         * created, so send it off to the add function.
         */
        void access(unsigned int owner, std::uint64_t number) {
            ++accesses;
            norm_iterator e = std::find_if(table.begin(), table.end(), [owner, number](Page p) {
                    return p.number == number && owner == p.owner;
                    });
            if(e == table.end()) {
                norm_iterator e = std::find_if(swap.begin(), swap.end(), [owner, number](Page p) {
                        return p.number == number && owner == p.owner;
                        });
                if (e == swap.end()) {
                    // The page wasn't created yet
                    this->add(Page(owner, number));
                    return;
                }
                // std::cout << "Page fault!" << std::endl;
                ++faults;
                if (table.size() >= capacity) {
                    norm_iterator victim = algorithm.select_victim();
                    std::iter_swap(victim, e);
                    algorithm.access_hook(victim);
                } else {
                    // Just dump it on the back of the list
                    table.push_back(*e);
                    swap.erase(e);
                    algorithm.access_hook(table.end()-1);
                }
            }
            else {
                algorithm.access_hook(e);
            }
        }

        /*
         * Clear memory and swap when processes die
         */
        void reap(unsigned int owner) {
            std::remove_if(table.begin(), table.end(),
                    [owner](Page p) {
                    return p.owner == owner;
                    });
            std::remove_if(swap.begin(), swap.end(),
                    [owner](Page p) {
                    return p.owner == owner;
                    });
        }

        std::vector<Page> table;
        std::vector<Page> swap;
    private:
        Alg algorithm;
        unsigned int capacity;
        std::uint64_t faults = 0;
        std::uint64_t accesses = 0;
};
