#ifndef _SPP_QUEUES_H_
#define _SPP_QUEUES_H_

#include "radix_heap.h"
#include "graph.h"
#include "dheap.h"
#include "spp.h"

namespace spp{
  using radix_heap_spp = radix_heap::pair_radix_heap_decrease_duplicate<dist_t, node_t, INF>;
  using dheap_spp = dheap<dist_t, node_t, 4>;
}

#endif