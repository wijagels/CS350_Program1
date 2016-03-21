#include "PageTable.hpp"
#include "TrueLru.hpp"
#include "Lfu.hpp"
#include "Mfu.hpp"
#include <vector>
#include <fstream>

const int table_size = 100;

template <typename T>
void parser(T&);

int main(int argc, char* argv[]) {
    if(argc == 2) {
        std::ifstream in(argv[1]);
        parser<std::ifstream>(in);
    } else if (argc == 1) {
        std::cerr << "No filename given, assuming stdin" << std::endl;
        parser<std::istream>(std::cin);
    } else {
        std::cerr << "Unsupported number of arguments, please use either a single filename"
            << " or nothing to use stdin." << std::endl;
        return 1;
    }
}

/*
 * TODO: make this run async with <future>
 * My guess is that this would benefit hugely from parallel execution.
 * Also, it would be better to read the entire input from a stream, then pass
 * it around as a constant reference to seperate async workers.
 * This code runs with very high cpu utilization already, so splitting into
 * threads will likely result in a 3-4x performance boost (on a 4 core cpu).
 * Also some more efficient page replacement algorithms would be nice,
 * the current collection is **very** slow.
 */
template <typename T>
void parser(T& stream) {
    PageTable<Lfu> lfu(table_size);
    PageTable<Mfu> mfu(table_size);
    PageTable<TrueLru> lru(table_size);
    std::string cmd;
    unsigned int owner;
    std::uint64_t num;
    while (stream.good()) {
        stream >> cmd;
        stream >> owner;
        stream >> num;
        if (cmd == "START") {
            // K, thx
            // Seriously though, this command is useless to me.
            // Pages are allocated when they're referenced
        } else if (cmd == "REFERENCE") {
            lru.access(owner, num);
            lfu.access(owner, num);
            mfu.access(owner, num);
        } else if (cmd == "TERMINATE") {
            lru.reap(owner);
            lfu.reap(owner);
            mfu.reap(owner);
        } else {
            std::cerr << "Unknown entry" << std::endl;
            exit(1);
        }
    }
}
