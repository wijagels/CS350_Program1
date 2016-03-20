#include "vruntime.hpp"
#include "debug.h"
#include <random>
#include <algorithm>

// Process impl

// TODO Parameterize the variance
const double Process::LIFE_VARIANCE = 40.0;
const unsigned Process::LIFE_MIN = 10u;
unsigned Process::_next_id = 0;

void Process::access(unsigned long long addr) {
  assert(is_alive());
  _age++;
  _last_addr = addr;
}

// VRuntime impl

/**
 *  \brief Describes the runtime of a virtual machine
 *
 *  This struct contains information that will be used to simulate
 *  the runtime of several processes on our virtual machine for memory
 *  profiling.
 *
 *  \param procs Number of processes that will be simulated
 *  \param addr_size Size of the address space will be 2^addr_size
 *  \param page_size Size of each page will be 2^(page_size)
 *  \param locality The probability that the next memory reference will be near the last.
 *  0 <= locality <= 1
 *  \param avg_lifetime The average amount of time a process will be alive
 *  \return return type
 */
VRuntime::VRuntime(unsigned procs, unsigned addr_size, unsigned page_size, double locality, unsigned life):
  addr_size{addr_size}, page_size{page_size}, locality{locality}, avg_lifetime{life}, max_procs{procs} {

  // Play within bounds
  assert_debug(sandwich(ADDR_MIN, addr_size, ADDR_MAX), "Invalid address space size: 2^%u.", addr_size);
  assert_debug(sandwich(PAGE_MIN, page_size, PAGE_MAX), "Invalid page size: 2^%u", page_size);
  // Make sure the page size divides the address space size
  assert_debug(page_size < addr_size, "Address space 2^%u not divisible by address size 2^%u", page_size, addr_size);
  assert_debug(0.0 < locality && locality < 1.0, "Locality probability not valid: %f. Must be between 0 and 1", locality);
}

std::ostream& operator<<(std::ostream& os, const VRuntime& vr) {
  std::default_random_engine gen;
  std::normal_distribution<double> lifetime_dist(vr.avg_lifetime, Process::LIFE_VARIANCE);
  std::bernoulli_distribution local_dist(vr.locality);
  std::vector<Process> procs(vr.max_procs);
  // Start the procs
  for (unsigned i = 0; i < vr.max_procs; i++) {
    unsigned life = std::max(static_cast<unsigned>(lifetime_dist(gen)), Process::LIFE_MIN);
    procs[i] = Process{life};
    logd("Proc %d: lifetime %u", procs[i].id(), life);
    os << "START " << procs[i].id() << " " << ~(-1uLL<<vr.addr_size) << "\n";
  }

  int last = procs.size() - 1;
  int target = 0;
  unsigned page_size = (1 << vr.page_size);
  while (last > 0) {
    if (procs[target].is_alive()) {
      unsigned long long next_ref, lower = 0, upper = ~(-1uLL << vr.addr_size);
      // Do a locality test
      if (local_dist(gen)) {
        // If local, then generate an address within the same page as the last
        unsigned last_page = procs[target].last_addr() / page_size;
        lower = last_page * page_size;
        upper = lower + page_size;
      }
      // Else pick an arbitrary address in the address space
      next_ref = rand() % (upper - lower) + lower;
      procs[target].access(next_ref);
      os << "REFERENCE " << procs[target].id() << " " << next_ref / page_size << "\n";

      // Kill proc if it's time has come
      if (!procs[target].is_alive()) {
        // Get someone else to fill the dead proc's spot
        os << "TERMINATE " << procs[target].id() << "\n";
        if (target != last) procs[target] = procs[last];
        last--;
      }
    }
    // TODO Parameterize next process choice, also make less arbitrary
    target = rand() % (last+1);
  }

  return os;
}
