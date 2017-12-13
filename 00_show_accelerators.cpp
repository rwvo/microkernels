#include <hc.hpp>
#include <vector>
#include <iostream>
#include "utils.h"

// Not an actual benchmark; shows the available GPU devices

int main(){
  auto accelerators = hc::accelerator::get_all();
  auto gpu_indices = get_gpu_indices(accelerators);
  for(auto i: gpu_indices){
    const auto& acc = accelerators[i];
    std::wcerr << acc.get_device_path() << '\n';
    std::wcerr << acc.get_description() << '\n';
    std::wcerr << "device memory " << acc.get_dedicated_memory() << " bytes\n\n";
  }
}
