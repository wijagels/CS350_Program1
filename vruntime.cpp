#include "vruntime.hpp"
#include "debug.h"
#include <cassert>
#include <random>

// Process impl

void Process::access(unsigned addr) {
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
 *  \param tlb Size of the TLB will be tlb
 *  \param page_size Size of each page will be 2^(page_size)
 *  \param locality The probability that the next memory reference will be near the last.
 *  0 <= locality <= 1
 *  \param avg_lifetime The average amount of time a process will be alive
 *  \return return type
 */
VRuntime::VRuntime(unsigned procs, unsigned tlb, unsigned page_size, double locality, unsigned life):
  tlb_size{tlb}, page_size{page_size}, locality{locality}, avg_lifetime{life}, max_procs{procs} {

  assert(sandwich(TLB_MIN, tlb, TLB_MAX) && sandwich(PAGE_MIN, page_size, PAGE_MAX));
}

std::ostream& operator<<(std::ostream& os, const VRuntime& vr) {
  std::vector<Process> procs(vr.max_procs);
  std::default_random_engine gen;
  std::normal_distribution<double> lifetime_dist(vr.avg_lifetime, Process::LIFE_VARIANCE);
  std::bernoulli_distribution local_dist(vr.locality);

  // Start the procs
  for (unsigned i = 0; i < vr.max_procs; i++) {
    unsigned life = std::max(static_cast<unsigned>(lifetime_dist(gen)), Process::LIFE_MIN);
    procs[i] = life; // Implicit conversion
    logd("Proc %d: lifetime %u", i, life);
    // TODO paramaterize address space size
    os << "START " << i << " 256\n";
  }

  int finished = 0;
  int target = 0;
  while (finished < procs.size()) {
    if (procs[target].is_alive()) {
      unsigned next_ref, lower = 0, upper = 256;
      // Do a locality test
      if (local_dist(gen)) {
        // If local, then generate an address within the same page as the last
        // FIXME
        unsigned page_num = procs[target].last_addr / vr.page_size;
        lower = page_num * vr.page_size;
        upper = lower + vr.page_size;
      }
      // Else pick an arbitrary address in the address space
      next_ref = rand() % (upper - lower) + lower;
      procs[target].access(next_ref);
      // TODO Do we want the virtual address or the virtual page number?
      os << "REFERENCE " << target << " " << next_ref << "\n";

      // Kill proc if it's time has come
      if (!procs[target].is_alive()) {
        os << "TERMINATE " << target << "\n";
      }
    }
    // TODO Parameterize next process choice, also make less arbitrary
    target = rand() % procs.size();
  }

  return os;
}
