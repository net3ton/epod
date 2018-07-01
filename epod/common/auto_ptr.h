#ifndef _COMMON_AUTO_PTR_H_
#define _COMMON_AUTO_PTR_H_

#include <assert.h>

// simple std::auto_ptr
// delete object with ptr deletion

template<class T>
struct TAutoPtr
{
private:
  mutable T *m_ptr;

  template<class U> friend struct TAutoPtr;

public:
  TAutoPtr(): m_ptr(0) {}
  explicit TAutoPtr(T *ptr): m_ptr(ptr) {}

  TAutoPtr(const TAutoPtr &other)
    :m_ptr(other.m_ptr)
  {
    other.m_ptr = 0;
  }

  template<class U>
  TAutoPtr(const TAutoPtr<U> &other)
    :m_ptr(other.m_ptr)
  {
    other.m_ptr = 0;
  }

  ~TAutoPtr()
  {
    (void) sizeof(T); // T should be fully defined
    delete m_ptr;
  }

  void swap(TAutoPtr &other)
  {
    T *ptr = this->m_ptr;
    this->m_ptr = other.m_ptr;
    other.m_ptr = ptr;
  }

  TAutoPtr &operator= (const TAutoPtr &other)
  {
    TAutoPtr(other).swap(*this);
    return *this;
  }

  template<class U>
  TAutoPtr<T> &operator= (const TAutoPtr<U> &other)
  {
    TAutoPtr(other).swap(*this);
    return *this;
  }

  T *get() const { assert(m_ptr); return m_ptr; }
  
  T *operator->() const { assert(m_ptr); return m_ptr; }
  T &operator*() const { assert(m_ptr); return m_ptr; }
};


// syntax candy
// AutoPtr(ptr) instead of TAutoPtr<Bla-Bla-Bla>(ptr)
template <class T>
inline TAutoPtr<T> AutoPtr(T *ptr)
{
  return TAutoPtr<T>(ptr);
}

#endif // _COMMON_AUTO_PTR_H_
