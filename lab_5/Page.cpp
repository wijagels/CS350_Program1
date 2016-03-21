#include "Page.hpp"

std::ostream& operator<<(std::ostream& os, const Page& p) {
    os << "Page Number: ";
    os << p.number << std::endl;
    return os;
}
