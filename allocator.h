#define unlikely(x) __builtin_expect(!!(x), 0)
#define BLOCK_SIZE 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ROUND_UP(bytes) (((bytes) + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1))

template <typename T>
class Allocator {
 public:
  Allocator() : size_(0), capacity_(BLOCK_SIZE) {
    buffer_ = reinterpret_cast<T *>(malloc(BLOCK_SIZE * sizeof(T)));
  }

  inline void Reserve(size_t capacity) {
    if (capacity > capacity_) {
      extend(ROUND_UP(capacity));
    }
  }

  T *Header() { return buffer_; }

  T *GetNode() {
    if (size_ == capacity_) {
      extend(capacity_ << 1);
    }
    return &buffer_[size_++];
  }

  ptrdiff_t GetNodeOffset() {
    if (size_ == capacity_) {
      extend(capacity_ << 1);
    }
    return &buffer_[size_++] - &buffer_[0];
  }

  T *OffsetToPtr(ptrdiff_t offset) { return buffer_ + offset; }

  ptrdiff_t PtrToOffset(T *ptr) { return ptr - buffer_; }

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
};
