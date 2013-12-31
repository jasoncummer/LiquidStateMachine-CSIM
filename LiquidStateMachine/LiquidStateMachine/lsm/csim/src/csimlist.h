#ifndef __CSIMLIST_H__
#define __CSIMLIST_H__
//! Template class for storing an array of pointers
template<class T, int inc> class csimList {
public:
  //! The constructor initializes the empty list
  csimList(void) {
    l = n =0;
    elements = 0;
  }

  //! Add a pointer to the list
  void add(T *el) {
    if ( n >= l ) {
      l += inc;
      elements = (T **)realloc(elements,l*sizeof(T *));
    }
    elements[n++] = el;
  } 

  //! Delete all the objects whos pointers are stored in the list and free the memory occupied by the array.
  void destroy(void) {
    if (elements) {
      for(unsigned long i=0;i<n;i++) {
        delete elements[i];
      }
      free(elements); elements = 0;
      l = n =0;
    }
  }

  //! Delete all the objects whos pointers are stored in the list and free the memory occupied by the array.
  void destroy_arrays(void) {
    if (elements) {
      for(unsigned long i=0;i<n;i++)
        delete [] elements[i];
      free(elements); elements = 0;
      l = n =0;
    }
  }
  //! Free the memory occupied by the array. Note: the \b objects whos pointers are stored \b are \b not \b deleted.
  void clear(void) {
    if (elements) {
      free(elements); elements = 0;
      l=n=0; 
    }
  }

  //! Free the memory occupied by the array. Note: the \b objects whos pointers are stored \b are \b not \b deleted.
  ~csimList() {
    if (elements) {
      free(elements); elements=0;
      n = 0;
    }
  }
  unsigned long n;
  T **elements;
private:
  unsigned long l;
};
#endif
