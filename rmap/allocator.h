#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>

#define unlikely(x) __builtin_expect(!!(x), 0)
#define BLOCK_SIZE 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ROUND_UP(bytes) (((bytes) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

enum AllocState {
  ASInited,
  ASLoaded,
};

struct AllocDesc {
  size_t size;
  int count;
};

template <typename T>
class Allocator {
 public:
  Allocator() : size_(1), capacity_(BLOCK_SIZE), count_(0) {
    buffer_ = reinterpret_cast<T *>(malloc(BLOCK_SIZE * sizeof(T)));
    state_ = ASInited;
  }

  ~Allocator() {
    if (state_ == ASInited) {
      if (buffer_ != NULL) {
        free(buffer_);
      }
    } else {
      munmap(mapped_buffer_, mapped_size_);
    }
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
    if (state_ == ASLoaded) {
      return;
    }
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

  void Dump(const char *fname) {
    if (state_ == ASLoaded) {
      return;
    }
    FILE *fout = fopen(fname, "wb");
    AllocDesc desc;
    desc.count = count_;
    desc.size = size_;
    fwrite(&desc, sizeof(AllocDesc), 1, fout);
    fwrite(buffer_, sizeof(T), size_, fout);
    fflush(fout);
    fclose(fout);
  }

  void Load(const char *fname) {
    if (state_ == ASLoaded) {
      munmap(mapped_buffer_, mapped_size_);
    } else if (state_ == ASInited) {
      if (buffer_ != NULL) {
        free(buffer_);
        size_ = 0;
        capacity_ = 0;
        count_ = 0;
      }
    }

    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
      printf("Failed open file\n");
      return;
    }

    struct stat st;
    fstat(fd, &st);
    mapped_size_ = st.st_size;
    mapped_buffer_ = mmap(NULL, mapped_size_, PROT_READ, MAP_PRIVATE, fd, 0);

    AllocDesc *desc_ptr = reinterpret_cast<AllocDesc *>(mapped_buffer_);

    size_t minimum_size = desc_ptr->size * sizeof(T) + sizeof(AllocDesc);

    if (minimum_size > mapped_size_) {
      printf("Broken file\n");
      munmap(mapped_buffer_, mapped_size_);
      return;
    }

    size_ = capacity_ = desc_ptr->size;
    count_ = desc_ptr->count;

    buffer_ = reinterpret_cast<T *>(desc_ptr + 1);

    state_ = ASLoaded;
  }

 private:
  inline void extend(size_t new_capacity) {
    if (state_ == ASLoaded) {
      return;
    }
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
  AllocState state_;

  void *mapped_buffer_;
  size_t mapped_size_;
};

#endif
