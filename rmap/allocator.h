#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define unlikely(x) __builtin_expect(!!(x), 0)
#define BLOCK_SIZE 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ROUND_UP(bytes) (((bytes) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

template <typename T>
class Allocator {
 public:
  Allocator() : size_(1), capacity_(BLOCK_SIZE), count_(0) {
    buffer_ = reinterpret_cast<T *>(malloc(BLOCK_SIZE * sizeof(T)));
  }

  inline void Reserve(size_t capacity) {
    if (capacity > capacity_) {
      extend(ROUND_UP(capacity));
    }
  }

  T *HeadPtr() { return buffer_; }

  T &Header() { return buffer_[0]; }

  T &_M_header() { return buffer_[0]; }

  T *GetNode() {
    if (size_ == capacity_) {
      extend(capacity_ << 1);
    }
    return &buffer_[size_++];
  }

  ptrdiff_t Ptr2Offset(T *ptr) { return ptr ? ptr - buffer_ : -1; }

  T *Offset2Ptr(ptrdiff_t offset) {
    return offset == -1 ? NULL : buffer_ + offset;
  }

  inline T &operator[](size_t loc) { return buffer_[loc]; }

  inline size_t Size() const { return size_; }

  inline size_t Capacity() const { return capacity_; }

  inline int Count() const { return count_; }
  inline void Dec() { --count_; }
  inline void Inc() { ++count_; }

 private:
  inline void extend(size_t new_capacity) {
#ifdef USE_REALLOC
    T *new_buf =
        reinterpret_cast<T *>(realloc(buffer_, new_capacity * sizeof(T)));
    if (unlikely(new_buf == NULL)) {
      new_buf = reinterpret_cast<T *>(malloc(new_capacity * sizeof(T)));
      memcpy(new_buf, buffer_, size_ * sizeof(T));
      free(buffer_);
    }
#else
    T *new_buf = reinterpret_cast<T *>(malloc(new_capacity * sizeof(T)));
    memcpy(new_buf, buffer_, size_ * sizeof(T));
    free(buffer_);
#endif
    buffer_ = new_buf;
    capacity_ = new_capacity;
  }

  T *buffer_;
  size_t size_, capacity_;

  int count_;
};

#endif
