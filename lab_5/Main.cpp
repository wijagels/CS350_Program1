#include "PageTable.hpp"
#include "TrueLru.hpp"
#include "Lfu.hpp"
#include "Mfu.hpp"
#include <vector>
#include <fstream>

const int table_size = 100;
PageTable<Lfu> lfu(table_size);
PageTable<Mfu> mfu(table_size);
PageTable<TrueLru> lru(table_size);

template <typename T>
void parser(T&);

int main(int argc, char* argv[]) {
    if(argc == 2) {
        std::ifstream in(argv[1]);
        parser<std::ifstream>(in);
    } else {
        parser<std::istream>(std::cin);
    }
}

template <typename T>
void parser(T& stream) {
    std::string cmd;
    unsigned int owner;
    std::uint64_t num;
    while (stream.good()) {
        stream >> cmd;
        stream >> owner;
        stream >> num;
        if (cmd == "START") {
            // K, thx
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
