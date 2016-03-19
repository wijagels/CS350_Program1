#include "debug.h"
#include "vruntime.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {

  // Arg parsing
  char flag;
  unsigned t = 2048, n = 10, l = 100;
  double p = 0.5;

  while ((flag = getopt(argc, argv, "ut:n:l:p:")) != -1) {
    switch (flag) {
    case 'n':
      n = static_cast<unsigned>(strtoul(optarg, NULL, 10));
      break;
    case 't':
      t = static_cast<unsigned>(strtoul(optarg, NULL, 10));
      break;
    case 'p':
      p = strtod(optarg, NULL);
      break;
    case 'l':
      l = static_cast<unsigned>(strtoul(optarg, NULL, 10));
      break;
    case 'u':
    case '?':
    default:
      // Just kick out and let main print usage
      std::cerr << "USAGE: ./lab4 -t [TLB size] -n [page size] -l [process lifetime average]"
              "-p [locality probability]\n";
      exit(0);
    }
  }

  VRuntime vruntime{10, t, n, p, l};
  // Output input file here
  std::cout << vruntime;

  // Cleanup
  return 0;
}
