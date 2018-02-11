#include "device_info.h"
#include <iostream>

namespace {
  // returns a vector of indices of "accelerators" that are actual GPU devices 
  std::vector<size_t> get_gpu_indices(const std::vector<hc::accelerator>& accelerators) {
    std::vector<size_t> gpu_indices;
    for(std::size_t i=0; i!= accelerators.size(); ++i){
      if(accelerators[i].get_device_path() != L"cpu"){
	gpu_indices.push_back(i);
      }
    }
    return gpu_indices;
  }
}


device_info::device_info()
  : m_accelerators{ hc::accelerator::get_all() },
    m_gpu_indices{ get_gpu_indices(m_accelerators) }
{
  // body intentionally empty
}

void device_info::show() const {
  for(auto i: m_gpu_indices){
    const auto& acc = m_accelerators[i];
    std::wcerr << "accelerator " << i << ": " << acc.get_device_path() << '\n';
    std::wcerr << acc.get_description() << '\n';
    std::wcerr << "device memory " << acc.get_dedicated_memory() << " bytes\n\n";
  }
}
