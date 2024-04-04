#ifndef _D_HEAP_H_
#define _D_HEAP_H_

#include <utility> // std::pair
#include <cstddef> // size_t
#include <memory> // std::unique_ptr
#include <algorithm> // std::swap, std::min, std::copy

template<typename KeyType, typename ValueType, size_t d = 2>
class dheap{
  using pkv = std::pair<KeyType, ValueType>;
  public:
    dheap();
    dheap(size_t max_size, ValueType max_value)
      : max_size(max_size), max_value(max_value),
        heap(new pkv[max_size]),
        valueMap(new size_t[max_value + 1]) { }
    
    template<typename InputIterator>
    dheap(size_t max_size, ValueType max_value, InputIterator begin, InputIterator end)
      : dheap(max_size, max_value) {
      heap_size = std::copy(begin, end, heap) - heap;
      for(size_t i=0; i<heap_size; ++i) valueMap[heap[i].second] = i;
      if(heap_size>1){
        for(size_t i=parent(heap_size-1); i>0; --i) restore_down(i);
        restore_down(0);
      }
    }

    ~dheap(){
      delete[] heap;
      delete[] valueMap;
    }

    size_t size() const { return heap_size; };
    bool empty() const { return heap_size==0; }
    
    void insert(KeyType key, ValueType value){
      heap[heap_size] = {key,value};
      valueMap[value] = heap_size;
      restore_up(heap_size++);
    }
    
    pkv extract_min(){
      if(heap_size==0) return {-1,-1};
      if(heap_size==1) return heap[--heap_size];

      pkv root = heap[0];
      
      ValueType value = heap[--heap_size].second;
      heap[0] = heap[heap_size];
      valueMap[value] = 0;
      restore_down(0);

      return root;
    }

    void decrease_key(KeyType newKey, ValueType value){
      size_t index = valueMap[value];
      heap[index].first = newKey;
      restore_up(index);
    }

    void clear(){ heap_size = 0; }
  
  private:
    size_t max_size;
    ValueType max_value;
    
    size_t heap_size;

    pkv *heap;
    // std::vector<pii> heap;
    
    size_t *valueMap;
    // std::vector<int> valueMap; // valueMap[value] = index in heap
    
    void swap_nodes(size_t i, size_t j){
      ValueType valueI = heap[i].second;
      ValueType valueJ = heap[j].second;
      std::swap(valueMap[valueI],valueMap[valueJ]);
      std::swap(heap[i],heap[j]);
    }

    size_t parent(size_t i){ return (i-1)/d; } // i!=0

    void restore_down(size_t index){
      while(true){
        size_t smallest = index;
        size_t first = index*d+1, last = std::min(index*d+d, heap_size-1);

        for(size_t child=first;child<=last;++child){ //find smallest child
          if(heap[smallest].first > heap[child].first) //key comparison
            smallest = child;
        }

        if(smallest!=index){ // if smallest child is smaller than parent (index)
          swap_nodes(index, smallest);
          index = smallest;
        }
        else break;
      }
    }

    void restore_up(size_t index){
      size_t p;
      while(index!=0 && heap[p = parent(index)].first > heap[index].first){
        swap_nodes(p, index);
        index = p;
      }
    }
};

#endif