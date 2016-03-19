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
  std::normal_distribution<double> distribution(vr.avg_lifetime, Process::LIFE_VARIANCE);

  // Start the procs
  for (unsigned i = 0; i < vr.max_procs; i++) {
    unsigned life = std::max(static_cast<unsigned>(distribution(gen)), Process::LIFE_MIN);
    procs[i] = life; // Implicit conversion
    logd("Proc %d: lifetime %u", i, life);
    // TODO paramaterize address space size
    os << "START " << i << " 256\n";
  }


  // TODO
  // os << "REFERENCE " << page << " " << vpn << "\n"
  // os << "TERMINATE " << page << "\n"
  return os;
}
