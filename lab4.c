#include "debug.h"
#include "vruntime.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  // Arg parsing
  char flag;
  unsigned t = 2048, n = 10, l = 100;
  double p = 0.5;

  while ((flag = getopt(argc, argv, "ut:n:l:p:")) != -1) {
    switch (flag) {
    case 'n':
      n = (unsigned) strtoul(optarg, NULL, 10);
      break;
    case 't':
      t = (unsigned) strtoul(optarg, NULL, 10);
      break;
    case 'p':
      p = (double) strtod(optarg, NULL);
      break;
    case 'l':
      l = (unsigned) strtoul(optarg, NULL, 10);
      break;
    case 'u':
    case '?':
    default:
      // Just kick out and let main print usage
      fprintf(stderr, "USAGE: ./lab4 -t [TLB size] -n [page size] -l [process lifetime average]"
              "-p [locality probability]\n");
      exit(0);
    }

    VRuntime *vruntime = VRuntime_new(t, n, p, l);
    if (!vruntime) {
      logd("Invalid vruntime state detected.");
      exit(1);
    }
  }
}
