#include "PageTable.hpp"
#include "TrueLru.hpp"
#include <vector>

int main() {
    PageTable<TrueLru> paget(6);
    for (int i = 0; i < 10; i++) {
        paget.add(Page(2, 2048*i, 2048));
    }
    paget.access(2049);
    paget.access(2049);
    paget.access(4098);
}
