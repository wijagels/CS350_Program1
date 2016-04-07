#include "debug.h"
#include "vruntime.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

void check(bool p, const char *message) {
  if (p) {
    loge(message);
    exit(1);
  }
}

int main(int argc, char **argv) {

  // Arg parsing
  char flag;
  unsigned t = 32, n = 10;
  int l = 100;
  double p = 0.5;
  std::ofstream test_file;


  while ((flag = getopt(argc, argv, "ut:n:l:p:o:")) != -1) {
    switch (flag) {
    case 'n':
      // Page size
      n = std::stoul(optarg, NULL, 10);
      check(sandwich(PAGE_MIN, n, PAGE_MAX), "Invalid page size: 2^%u");
      break;
    case 't':
      // Address space size
      t = std::stoul(optarg, NULL, 10);
      check(sandwich(ADDR_MIN, t, ADDR_MAX), "Invalid address space size");
      break;
    case 'p':
      p = std::stod(optarg, NULL);
      break;
    case 'l':
      l = std::stoi(optarg, NULL, 10);
      check(0.0 < l && l < 1.0, "Locality probability not valid: Must be between 0 and 1");
      break;
    case 'o':
      test_file.open(optarg);
      break;
    case 'u':
    case '?':
    default:
      // Just kick out and let main print usage
      std::cerr << "USAGE: ./lab4 -t [address space size] -n [page size] -l [process lifetime average] "
              "-p [locality probability]\n";
      exit(0);
    }
  }

  check(n < t, "Address space not divisible by address size");

  VRuntime vruntime{10, t, n, p, l};
  // Output input file here
  if (test_file.is_open()) {
    test_file << vruntime;
    test_file.close();
  } else {
    std::cout << vruntime;
  }

  // Cleanup
  return 0;
}
