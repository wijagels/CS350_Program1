#include "vruntime.hpp"
#include "debug.h"
#include <random>
#include <algorithm>
#include <utility>

// Process impl

// TODO Parameterize the variance
const double Process::LIFE_STDEV = 7.5;
// TODO Move lifetime bounds to another place
const int Process::LIFE_MIN = 10u;
unsigned Process::_next_id = 0;

// Records page numbers accessed
bool Process::access(unsigned long long addr) {
  assert(is_alive());
	assert_debug(addr < 1uLL<<_addr_size, "Invalid address number: %llu", addr);
  _age++;
	_refs->push_back(addr >> _page_size);
	return is_alive();
}

int Process::run() {
	// ¯\_(ツ)_/¯
	unsigned long long last_page = this->last_addr();
	unsigned long long next_ref;
	unsigned long long lower =
		std::max(static_cast<unsigned long long>((last_page < LOCAL_REACH ? 0 : last_page-LOCAL_REACH)<<_page_size),
				0uLL);
	unsigned long long upper =
		std::min(static_cast<unsigned long long>(((last_page+LOCAL_REACH+1)<<_page_size)-1), 1uLL<<_addr_size);
	int old_history_sz = _refs->size();
	std::default_random_engine gen;
	std::bernoulli_distribution local_dist(_locality);

	// MAGIC
	for (int i = 0; this->is_alive() && i < 10; i++) {
		switch (_policy) {
			case OPEN:
				if (!local_dist(gen)) {
					// If local, then generate an address within the same page as the last
					lower = 0;
					upper = 1uLL << _addr_size;
				}
			case NORMAL:
				next_ref = rand() % (upper - lower) + lower;
				this->access(next_ref);
				break;
			case HEAVY:
				next_ref = rand() % (upper - lower) + lower;
				// MAGIC
				for (unsigned j = 0; this->access(next_ref+j*4) &&  j < _phase_length*2; j++);
				_policy = static_cast<Policy>(rand() % 3);
				logd("Process %u switching to policy: %u", _id, _policy);
				break;
			case IO:
				next_ref = rand() % (upper - lower) + lower;
				// MAGIC
				if (i % 5 == 0) {
					this->access(next_ref);
				} else {
					// Get older anyway, but don't access anything
					_age++;
				}
				break;
		}
		if (_age % _phase_length == 0) {
			_policy = static_cast<Policy>(rand() % 4);
			logd("Process %u switching to policy: %u", _id, _policy);
		}
	}
	return _refs->size() - old_history_sz;
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
VRuntime::VRuntime(unsigned procs, unsigned addr_size, unsigned page_size,
		double locality, int life, double v, unsigned phase):
  max_procs{procs}, addr_size{addr_size}, page_size{page_size}, locality{locality},
	avg_lifetime{life}, address_stdev{v}, phase_length{phase} {

  // Play within bounds
  assert_debug(sandwich(ADDR_MIN, addr_size, ADDR_MAX), "Invalid address space size: 2^%u.", addr_size);
  assert_debug(sandwich(PAGE_MIN, page_size, PAGE_MAX), "Invalid page size: 2^%u", page_size);
  // Make sure the page size divides the address space size
  assert_debug(page_size < addr_size, "Address space 2^%u not divisible by address size 2^%u", page_size, addr_size);
  assert_debug(0.0 < locality && locality < 1.0, "Locality probability not valid: %f. Must be between 0 and 1", locality);
	assert_debug(address_stdev > 0.0, "Address variance not valid: %f", address_stdev);
}

std::ostream& operator<<(std::ostream& os, const VRuntime& vr) {
	std::default_random_engine gen;
  std::normal_distribution<double> lifetime_dist(vr.avg_lifetime, Process::LIFE_STDEV);
	std::normal_distribution<double> va_dist(vr.addr_size, vr.address_stdev);
  std::vector<std::unique_ptr<Process> > procs(vr.max_procs);
  // Start the procs
  for (unsigned i = 0; i < vr.max_procs; i++) {
    int life = static_cast<int>(std::max(lifetime_dist(gen), static_cast<double>(Process::LIFE_MIN)));
		unsigned addr_sz = static_cast<unsigned>(std::max(va_dist(gen), static_cast<double>(vr.page_size)));
    procs[i] = std::unique_ptr<Process>(new Process{addr_sz, vr.page_size, life, vr.locality, vr.phase_length});
    logd("Proc %d: lifetime %u page size %u address size %u", procs[i]->id(), life, vr.page_size, addr_sz);
    os << "START " << procs[i]->id() << " " << (1<<(addr_sz-vr.page_size)) << "\n";
  }

  int last = procs.size() - 1;
	int target = 0; while (last > 0) {
    if (procs[target]->is_alive()) {
			int refs_made = procs[target]->run();
			auto history = procs[target]->history();
			for (unsigned i = history.size() - refs_made; i < history.size(); i++) {
				os << "REFERENCE " << procs[target]->id() << " " << history[i] << "\n";
			}
			if (!procs[target]->is_alive()) {
				// Get someone else to fill the dead proc's spot
				os << "TERMINATE " << procs[target]->id() << "\n";
				if (target != last) std::swap(procs[target], procs[last]);
				last--;
			}
    }
    // TODO Parameterize next process choice, also make less arbitrary
    target = rand() % (last+1);
  }

  return os;
}
