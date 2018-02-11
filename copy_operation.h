#ifndef _COPY_OPERATION_H_
#define _COPY_OPERATION_H_

#include "hc.hpp"
#include <cstdlib>
#include <iostream>
#include "hc_am.hpp"

template<typename T>
class copy_operation {
public:
  copy_operation(size_t source_acc_no, size_t dest_acc_no,
		 size_t size, bool pinned=false);
  ~copy_operation();
private:
  T* m_source;
  T* m_dest;
  T* m_source_accelerator_pointer;
  T* m_dest_accelerator_pointer;
  hc::array<T, 2>* m_source_array;
  hc::array<T, 2>* m_dest_array;
  size_t m_source_acc_no;
  size_t m_dest_acc_no;
  size_t m_size;
  bool m_pinned;
};

//--- implementation of template members

template<typename T>
copy_operation<T>::copy_operation(size_t source_acc_no, size_t dest_acc_no,
				  size_t size, bool pinned)
  : m_source(nullptr),
    m_dest(nullptr),
    m_source_accelerator_pointer(nullptr),
    m_dest_accelerator_pointer(nullptr),
    m_source_array(nullptr),
    m_dest_array(nullptr),
    m_source_acc_no(source_acc_no),
    m_dest_acc_no(dest_acc_no),
    m_size(size),
    m_pinned(pinned)
{
  std::wcerr << "copy " << m_size << " bytes from " << m_source_acc_no
	     << " to " << m_dest_acc_no << " using"
	     << (m_pinned ? " host-pinned" : " regular")
	     << " memory\n";
}
    
template<typename T>
copy_operation<T>::~copy_operation(){
}

#endif // _COPY_OPERATION_H_
