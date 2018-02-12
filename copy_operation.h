#ifndef _COPY_OPERATION_H_
#define _COPY_OPERATION_H_

#include "hc.hpp"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "hc_am.hpp"
#include "device_info.h"

template<typename T>
class copy_operation {
public:
  copy_operation(size_t source_acc_no, size_t dest_acc_no,
		 size_t size, bool pinned, device_info& dev_info);
  copy_operation(const copy_operation&) = delete;
  copy_operation(copy_operation&&) = delete;
  ~copy_operation();

  void copy_async();
  void wait() const;
  
  void print_info() const;
  
private:
  const device_info& m_dev_info;
  hc::completion_future m_future;
  T* m_source;
  T* m_dest;
  hc::array<T, 1>* m_source_array;
  hc::array<T, 1>* m_dest_array;
  T* m_source_accelerator_pointer;
  T* m_dest_accelerator_pointer;
  size_t m_source_acc_no;
  size_t m_dest_acc_no;
  size_t m_size;
  bool m_pinned;
};

//--- implementation of template members

namespace {

  template<typename T>
  T* allocate(size_t acc_no, size_t size, bool pinned, device_info& dev_info){
    if(acc_no){ // device allocation
      return static_cast<T*>(hc::am_alloc(size, dev_info.m_accelerators[acc_no], 0));
    } else { // host allocation
      if(pinned){ // host-pinned memory
	hc::accelerator default_acc;
	return static_cast<T*>(hc::am_alloc(size, default_acc, amHostPinned));
      } else { // plain vanilla memory
	return static_cast<T*>(malloc(size));
      }
    }
  }

  template<typename T>
  void deallocate(T* ptr, size_t acc_no, bool pinned){
    assert(ptr != nullptr);
    if(acc_no or pinned){ // device memory or host-pinned memory
      hc::am_free(ptr);
    } else { // plain vanilla memory
      free(ptr);
    }
  }

  template<typename T>
  hc::array<T, 1>* create_array(size_t acc_no, size_t size, T* ptr, device_info& dev_info){
    if(acc_no){ // ptr lives on a GPU, and we can create an array that uses
                // the pointer
      assert(ptr != nullptr);
      auto acc_view = hc::accelerator_view(dev_info.m_accelerators[acc_no].create_view());
      return new hc::array<T, 1>(hc::extent<1>(size/sizeof(T)), acc_view, ptr);
    } else {    // ptr lives on the host, and we don't need an array
      return nullptr;
    }
  }

  template<typename T>
  T* get_accelerator_pointer(size_t acc_no, T* ptr, hc::array<T, 1>* arr){
    if(acc_no){ // ptr lives on the device, and is captured ny arr
      assert(arr != nullptr);
      return arr->accelerator_pointer();
    } else { // ptr lives on the host, and can be used in place of accelerator_pointer
      assert(arr == nullptr);
      return ptr;
    }
  }
  
  
} // unnamed namespace

template<typename T>
copy_operation<T>::copy_operation(size_t source_acc_no, size_t dest_acc_no,
				  size_t size, bool pinned, device_info& dev_info)
  : m_dev_info(dev_info),
    m_future(),
    m_source(allocate<T>(source_acc_no, size, pinned, dev_info)),
    m_dest(allocate<T>(dest_acc_no, size, pinned, dev_info)),
    m_source_array(create_array<T>(source_acc_no, size, m_source, dev_info)),
    m_dest_array(create_array<T>(dest_acc_no, size, m_dest, dev_info)),
    m_source_accelerator_pointer
    (get_accelerator_pointer<T>(source_acc_no, m_source, m_source_array)),
    m_dest_accelerator_pointer
    (get_accelerator_pointer<T>(dest_acc_no, m_dest, m_dest_array)),
    m_source_acc_no(source_acc_no),
    m_dest_acc_no(dest_acc_no),
    m_size(size),
    m_pinned(pinned)
{
  assert(m_source_array or m_dest_array); // we need at least one accelerator_view
}

template<typename T>
void copy_operation<T>::copy_async() {
  assert(m_pinned or (m_source_acc_no and m_dest_acc_no));
  auto av = m_source_array ? m_source_array->get_accelerator_view()
    : m_dest_array->get_accelerator_view();
  m_future = av.copy_async(m_source_accelerator_pointer, m_dest_accelerator_pointer,
			   m_size);
}

template<typename T>
void copy_operation<T>::wait() const {
  assert(m_future.valid());
  m_future.wait();
}

template<typename T>
void copy_operation<T>::print_info() const {
  std::wcerr << "copy " << m_size << " bytes from " << m_source_acc_no
	     << " to " << m_dest_acc_no << " using"
	     << (m_pinned ? " host-pinned" : " regular")
	     << " memory\n";
}
    

template<typename T>
copy_operation<T>::~copy_operation(){
  delete m_dest_array;
  delete m_source_array;
  deallocate(m_dest, m_dest_acc_no, m_pinned);
  deallocate(m_source, m_source_acc_no, m_pinned);
}

#endif // _COPY_OPERATION_H_
