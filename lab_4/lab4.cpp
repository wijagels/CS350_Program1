#include "debug.h"
#include "vruntime.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

void print_usage() {
	std::cerr << "USAGE: ./lab4 -t [address space size] -n [page size] -l [process lifetime average] "
		"-p [locality probability] -o [output file]\n";
}

int main(int argc, char **argv) {

  // Arg parsing
  char flag;
  unsigned t = 32, n = 10;
  int l = 100;
  double p = 0.5;
  std::ofstream test_file;


  while ((flag = getopt(argc, argv, "ut:n:l:p:o:w:")) != -1) {
    switch (flag) {
    case 'n':
      // Page size
      n = std::stoul(optarg, NULL, 10);
      if (sandwich(PAGE_MIN, n, PAGE_MAX)) {
				loge("Invalid page size: 2^%u", n);
				exit(1);
			}
      break;
    case 't':
      // Address space size
      t = std::stoul(optarg, NULL, 10);
			if (sandwich(ADDR_MIN, t, ADDR_MAX)) {
				loge("Invalid address space size: 2^%u", t);
				exit(1);
			}
      break;
    case 'p':
			// Locality control
      p = std::stod(optarg, NULL);
			if (0.0 < p && p < 1.0) {
				loge("Locality probability not valid: %f. Must be between 0 and 1", p);
				exit(1);
			}
      break;
    case 'l':
			// Lifetime average
			l = std::stoi(optarg, NULL, 10);
      break;
    case 'o':
      test_file.open(optarg);
      break;
    case 'u':
    case '?':
    default:
			// Just kick out and let main print usage
			print_usage();
      exit(0);
    }
  }

	check(n < t,);
	if (n < t) {
		loge("Address space not divisible by address size");
		exit(1);
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
