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
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>

#include <type_traits>

#define unlikely(x) __builtin_expect(!!(x), 0)
#define BLOCK_SIZE 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ROUND_UP(bytes) (((bytes) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

template <typename _Val>
class _Rb_tree_impl {
  static_assert(std::is_pod<_Val>::value, "T must be POD");

 private:
  enum _Rb_tree_impl_state {
    RBInited,
    RBLoaded,
  };

 public:
  _Rb_tree_impl() : size_(0), capacity_(BLOCK_SIZE), state_(RBInited) {
    buffer_ = reinterpret_cast<_Val *>(malloc(BLOCK_SIZE * sizeof(_Val)));
  }

  ~_Rb_tree_impl() {
    if (state_ == RBInited) {
      if (buffer_ != NULL) {
        free(buffer_);
      }
    } else {
      munmap(mapped_buffer_, mapped_size_);
    }
  }

  void Reset() {
    if (state_ == RBInited) {
      if (buffer_ != NULL) {
        free(buffer_);
      }
    } else {
      munmap(mapped_buffer_, mapped_size_);
    }
    size_ = 0;
    capacity_ = BLOCK_SIZE;
    buffer_ = reinterpret_cast<_Val *>(malloc(BLOCK_SIZE * sizeof(_Val)));
    state_ = RBInited;
  }

  void Clear() {
    if (state_ == RBInited) {
      if (buffer_ != NULL) {
        free(buffer_);
      }
    } else {
      munmap(mapped_buffer_, mapped_size_);
    }
    size_ = capacity_ = 0;
    buffer_ = NULL;
    state_ = RBInited;
  }

  _Val *GetNode() {
    if (state_ == RBLoaded) {
      return NULL;
    }
    if (size_ == capacity_) {
      extend(capacity_ << 1);
    }
    return &buffer_[size_++];
  }

  ptrdiff_t Ptr2Offset(_Val *ptr) { return ptr ? ptr - buffer_ : -1; }

  _Val *Offset2Ptr(ptrdiff_t offset) {
    return offset == -1 ? NULL : buffer_ + offset;
  }

  inline _Val &operator[](size_t loc) { return buffer_[loc]; }

  inline size_t Size() const { return size_; }

  inline size_t Capacity() const { return capacity_; }

  void Dump(const char *fname) {
    if (state_ == RBLoaded) {
      return;
    }
    FILE *fout = fopen(fname, "wb");
    fwrite(&size_, sizeof(size_t), 1, fout);
    fwrite(buffer_, sizeof(_Val), size_, fout);
    fflush(fout);
    fclose(fout);
  }

  void Load(const char *fname) {
    Clear();
    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
      printf("Failed open file\n");
      Reset();
      return;
    }

    struct stat st;
    fstat(fd, &st);
    mapped_size_ = st.st_size;
    mapped_buffer_ = mmap(NULL, mapped_size_, PROT_READ, MAP_PRIVATE, fd, 0);

    size_ = *reinterpret_cast<size_t *>(mapped_buffer_);

    size_t minimum_size = size_ * sizeof(_Val) + sizeof(size_t);

    if (minimum_size > mapped_size_) {
      printf("Broken file\n");
      munmap(mapped_buffer_, mapped_size_);
      Reset();
      return;
    }

    capacity_ = size_;
    buffer_ = reinterpret_cast<_Val *>(mapped_buffer_ + sizeof(size_t));
    state_ = RBLoaded;
  }

  _Rb_tree_impl &operator=(const _Rb_tree_impl &rhs) {
    capacity_ = size_ = rhs.size_;
    buffer_ = reinterpret_cast<_Val *>(malloc(sizeof(_Val) * size_));
    state_ = RBInited;
    memcpy(buffer_, rhs.buffer_, sizeof(_Val) * size_);
    return *this;
  }

 private:
  inline void extend(size_t new_capacity) {
    if (state_ == RBLoaded) {
      return;
    }
#ifdef USE_REALLOC
    _Val *new_buf =
        reinterpret_cast<_Val *>(realloc(buffer_, new_capacity * sizeof(_Val)));
    if (unlikely(new_buf == NULL)) {
      new_buf = reinterpret_cast<_Val *>(malloc(new_capacity * sizeof(_Val)));
      memcpy(new_buf, buffer_, size_ * sizeof(_Val));
      free(buffer_);
    }
#else
    _Val *new_buf =
        reinterpret_cast<_Val *>(malloc(new_capacity * sizeof(_Val)));
    memcpy(new_buf, buffer_, size_ * sizeof(_Val));
    free(buffer_);
#endif
    buffer_ = new_buf;
    capacity_ = new_capacity;
  }

  _Val *buffer_;
  size_t size_, capacity_;

  _Rb_tree_impl_state state_;

  void *mapped_buffer_;
  size_t mapped_size_;
};

#endif
