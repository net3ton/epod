#ifndef _COMMON_ARRAY_PTR_H_
#define _COMMON_ARRAY_PTR_H_

#include <assert.h>
#include <stddef.h>
#include <vector>

// pointer to array + array size + assert to size bounds

template<class T>
struct TArrayPtr
{
private:
  int m_size;
  T *m_ptr;

public:
  TArrayPtr(): m_size(0), m_ptr(NULL) {};        
  TArrayPtr(size_t size, T* ptr): m_size(static_cast<int>(size)), m_ptr(ptr) {};

  typedef T* iterator;

  int size() { return m_size; }

  T *begin() const { return m_ptr; }
  T *end() const { return m_ptr + m_size; }

  T &operator[] (int i) const
  { 
    assert(i >= 0 && i < m_size);
    return  m_ptr[i];
  }
};


// syntax candy
// ArrayPtr(size, ptr) instead of TArrayPtr<Bla-Bla-Bla>(size, ptr)
template <class T>
inline TArrayPtr<T> ArrayPtr(size_t size, T *ptr)
{
  return TArrayPtr<T>(size, ptr);
}

template <class T>
inline TArrayPtr<T> ArrayPtr(std::vector<T> &vec)
{
  return TArrayPtr<T>(vec.size(), &(*vec.begin()));
}

#endif // _COMMON_ARRAY_PTR_H_
