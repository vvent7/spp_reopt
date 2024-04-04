#ifndef _SPP_QUEUES_H_
#define _SPP_QUEUES_H_

#include "radix_heap.h"
#include "graph.h"
#include "dheap.h"
#include "spp.h"

namespace spp{
  //constructor: max_size, max_value

  using rh = radix_heap::pair_radix_heap_decrease_duplicate<dist_t, node_t, INF>;
  using dh = dheap<dist_t, node_t, 4>;

  class radix_heap_spp : rh{
    public:
      radix_heap_spp(size_t max_size, node_t max_value)
        : rh(max_value){ };
      
      template<typename InputIterator>
      radix_heap_spp(size_t max_size, node_t max_value, InputIterator begin, InputIterator end)
        : rh(max_value, begin, end) { }
  };

  class dheap_spp : dh{
    public:
      dheap_spp(size_t max_size, node_t max_value)
        : dh(max_size, max_value){ };
      
      template<typename InputIterator>
      dheap_spp(size_t max_size, node_t max_value, InputIterator begin, InputIterator end)
        : dh(max_size, max_value, begin, end){ };
  };
}

#endif