#include "debug.h"
#include "vruntime.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

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
      n = std::stoul(optarg, NULL, 10);
      break;
    case 't':
      t = std::stoul(optarg, NULL, 10);
      break;
    case 'p':
      p = std::stod(optarg, NULL);
      break;
    case 'l':
      l = std::stoi(optarg, NULL, 10);
      break;
    case 'o':
      test_file.open(optarg);
      break;
    case 'u':
    case '?':
    default:
      // Just kick out and let main print usage
      std::cerr << "USAGE: ./lab4 -t [address space size] -n [page size] -l [process lifetime average]"
              "-p [locality probability]\n";
      exit(0);
    }
  }

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
