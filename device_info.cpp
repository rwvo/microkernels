#include "device_info.h"
#include <iostream>
#include <algorithm>

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


bool device_info::is_gpu_index(size_t i) const {
  return std::find(m_gpu_indices.cbegin(), m_gpu_indices.cend(), i) != m_gpu_indices.cend();
}

bool device_info::validate_gpu_indices(const std::vector<size_t>& indices) const {
  bool all_valid = true;
  for(auto i: indices){
    auto is_valid = is_gpu_index(i);
    if(not is_valid){
      std::wcerr << "invalid GPU index: " << i << '\n';
    }
    all_valid &= is_valid;
  }
  return all_valid;
}
