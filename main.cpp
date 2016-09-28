//===----------------------------------------------------------------------===//
// MAIN SOURCE
//===----------------------------------------------------------------------===//

#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

#include "btree.h"
#include "timer.h"

void Insert(BTree *tree){


  for(uint32_t key_itr = 0; key_itr < MAX_KEYS; key_itr++) {
    auto key = key_itr;
    tree->Insert(key);
  }

  //std::cout << "Success: " << success_count << "\n";

}

int main(void){

  uint32_t num_threads = 4;
  uint32_t num_loops = 10000;

  Timer<> timer;
  std::vector<std::thread> thread_group;

  for(uint32_t loop_itr = 0;
      loop_itr < num_loops;
      ++loop_itr) {

    thread_group.clear();

    // Construct btree instance
    BTree tree;

    // Start timer
    timer.Start();

    // Launch a group of threads
    for (uint32_t thread_itr = 0;
        thread_itr < num_threads;
        ++thread_itr) {
      thread_group.push_back(std::thread(Insert, &tree));
    }

    // Join the threads with the main thread
    for (uint64_t thread_itr = 0;
        thread_itr < num_threads;
        ++thread_itr) {
      thread_group[thread_itr].join();
    }

    // Stop timer
    timer.Stop();

    std::sort(tree.current_node_.keys_, tree.current_node_.keys_ + tree.current_node_.offset_);
    bool contains_duplicates = (std::unique(tree.current_node_.keys_,
                                            tree.current_node_.keys_ + tree.current_node_.offset_) !=
                                                tree.current_node_.keys_ + tree.current_node_.offset_);
    assert(contains_duplicates == false);
  }

  auto duration = timer.GetDuration();

  std::cout << "Duration: " << duration << "\n";

  //tree.Dump();

  return 0;
}
