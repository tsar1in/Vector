#include <iostream>
#include <memory>
#include <vector>
#include <forward_list>
#pragma once
#define VECTOR_MEMORY_IMPLEMENTED
template <class T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

 private:
  Pointer ptr_;
  size_t size_;
  size_t capacity_;

 public:
  Vector() : ptr_(nullptr), size_(0), capacity_(0) {
  }
  explicit Vector(SizeType size) {  // NOLINT
    ptr_ = (size != 0) ? static_cast<T*>(operator new[](sizeof(T) * size)) : nullptr;
    size_t k = 0;
    try {
      for (size_t i = 0; i < size; i++) {
        k++;
        new (ptr_ + i) T;
      }
    } catch (...) {
      for (size_t i = 0; i < k; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      throw;
    }
    size_ = size;
    capacity_ = size;
  }
  ~Vector() noexcept {
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    operator delete[](ptr_);
  }
  Vector(std::initializer_list<T> args) {
    if (args.size() == 0) {
      ptr_ = nullptr;
    } else {
      ptr_ = static_cast<T*>(operator new[](sizeof(T) * args.size()));
      size_t k = 0;
      try {
        for (size_t i = 0; i < args.size(); i++) {
          k++;
          new (ptr_ + i) T(*(args.begin() + i));
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(ptr_ + i);
        }
        operator delete[](ptr_);
        throw;
      }
    }
    size_ = args.size();
    capacity_ = args.size();
  }
  Vector(int size, int value) {
    if (size == 0) {
      ptr_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * size));
      for (int i = 0; i < size; i++) {
        new (copy + i) T(value);
      }
      ptr_ = copy;
      size_ = size;
      capacity_ = size;
    }
  }
  template <class H>
  Vector(H begin, H end) {
    size_t n = 0;
    auto tmp = begin;
    while (tmp != end) {
      tmp++;
      n++;
    }
    if (n > 0) {
      ptr_ = static_cast<T*>(operator new[](sizeof(T) * n));
      size_t k = 0;
      try {
        for (size_t i = 0; i < n; i++) {
          k++;
          new (ptr_ + i) T;
        }
        for (size_t i = 0; i < n; i++) {
          ptr_[i] = std::move(begin[i]);
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(ptr_ + i);
        }
        operator delete[](ptr_);
        throw;
      }
    } else {
      ptr_ = nullptr;
    }
    size_ = n;
    capacity_ = n;
  }
  template <ConstIterator>
  Vector(ConstIterator begin, ConstIterator end) {
    size_t n = 0;
    auto tmp = begin;
    while (tmp != end) {
      tmp++;
      n++;
    }
    if (n > 0) {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * n));
      size_t k = 0;
      try {
        for (size_t i = 0; i < n; i++) {
          k++;
          new (copy + i) T(begin[i]);
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      ptr_ = copy;
      size_ = n;
      capacity_ = n;
    } else {
      ptr_ = nullptr;
      size_ = n;
      capacity_ = n;
    }
  }
  template <class H>
  Vector(SizeType size, H&& value) {
    if (size == 0) {
      ptr_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * size));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size; i++) {
          k++;
          new (copy + i) T(value);
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      ptr_ = copy;
      size_ = size;
      capacity_ = size;
    }
  }
  template <const char*>
  Vector(SizeType size, const char* value) {
    if (size == 0) {
      ptr_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * size));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size; i++) {
          k++;
          new (copy + i) T(value);
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      ptr_ = copy;
      size_ = size;
      capacity_ = size;
    }
  }
  Vector(const Vector& other) {  // NOLINT
    if (other.ptr_ != nullptr) {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * other.size_));
      size_t k = 0;
      try {
        for (size_t i = 0; i < other.size_; i++) {
          k++;
          new (copy + i) T(other[i]);
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      ptr_ = copy;
      size_ = other.size_;
      capacity_ = other.capacity_;
    } else {
      ptr_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }
  }
  Vector& operator=(const Vector& other) {
    if (this != &other) {
      if (other.ptr_ != nullptr) {
        auto copy = static_cast<T*>(operator new[](sizeof(T) * other.size_));
        size_t k = 0;
        try {
          for (size_t i = 0; i < other.size_; i++) {
            k++;
            new (copy + i) T(other[i]);
          }
        } catch (...) {
          for (size_t i = 0; i < k; i++) {
            std::destroy_at(copy + i);
          }
          operator delete[](copy);
          throw;
        }
        for (size_t i = 0; i < size_; i++) {
          std::destroy_at(ptr_ + i);
        }
        operator delete[](ptr_);
        ptr_ = copy;
        size_ = other.size_;
        capacity_ = other.capacity_;
      } else {
        for (size_t i = 0; i < size_; i++) {
          std::destroy_at(ptr_ + i);
        }
        operator delete[](ptr_);
        ptr_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }
    }
    return *this;
  }
  Vector(Vector<T>&& other) noexcept : ptr_(other.ptr_), size_(other.size_), capacity_(other.capacity_) {
    other.ptr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  Vector& operator=(Vector<T>&& other) noexcept {  // NOLINT
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    operator delete[](ptr_);
    ptr_ = other.ptr_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.ptr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
  }
  SizeType Size() const {
    return size_;
  }
  SizeType Capacity() const {
    return capacity_;
  }
  bool Empty() const {
    return (size_ == 0);
  }
  Reference operator[](size_t i) {
    return ptr_[i];
  }
  ConstReference operator[](size_t i) const {
    return ptr_[i];
  }
  Reference At(size_t i) {
    if (i >= size_) {
      throw std::out_of_range{""};
    }
    return ptr_[i];
  }
  ConstReference At(size_t i) const {
    if (i >= size_) {
      throw std::out_of_range{""};
    }
    return ptr_[i];
  }
  Reference Front() {
    return ptr_[0];
  }
  ConstReference Front() const {
    return ptr_[0];
  }
  Reference Back() {
    return ptr_[size_ - 1];
  }
  ConstReference Back() const {
    return ptr_[size_ - 1];
  }
  Pointer Data() {
    return ptr_;
  }
  ConstPointer Data() const {
    return ptr_;
  }
  void Swap(Vector<T>& b) {
    auto copy = ptr_;
    ptr_ = b.ptr_;
    b.ptr_ = copy;
    size_t cop = size_;
    size_ = b.size_;
    b.size_ = cop;
    size_t co = capacity_;
    capacity_ = b.capacity_;
    b.capacity_ = co;
  }
  void Resize(SizeType new_size) {
    if (new_size <= capacity_) {
      if (new_size <= size_) {
        for (size_t i = new_size; i < size_; i++) {
          std::destroy_at(ptr_ + i);
        }
        size_ = new_size;
        return;
      }
      for (size_t i = size_; i < new_size; i++) {
        new (ptr_ + i) T;
      }
      size_ = new_size;
      return;
    }
    auto copy = static_cast<T*>(operator new[](sizeof(T) * new_size));
    size_t k = 0;
    try {
      for (size_t i = 0; i < size_; i++) {
        k++;
        new (copy + i) T(std::forward<T>(ptr_[i]));
      }
      for (size_t i = size_; i < new_size; i++) {
        k++;
        new (copy + i) T;
      }
    } catch (...) {
      for (size_t i = 0; i < k; i++) {
        std::destroy_at(copy + i);
      }
      operator delete[](copy);
      throw;
    }
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    operator delete[](ptr_);
    ptr_ = copy;
    size_ = new_size;
    capacity_ = new_size;
  }
  void Resize(SizeType new_size, ValueType value) {
    if (new_size <= capacity_) {
      if (new_size <= size_) {
        for (size_t i = new_size; i < size_; i++) {
          std::destroy_at(ptr_ + i);
        }
        size_ = new_size;
        return;
      }
      for (size_t i = size_; i < new_size; i++) {
        ptr_[i] = value;
      }
      size_ = new_size;
      return;
    }
    auto copy = static_cast<T*>(operator new[](sizeof(T) * new_size));
    size_t k = 0;
    try {
      for (size_t i = 0; i < size_; i++) {
        k++;
        new (copy + i) T(ptr_[i]);
      }
      for (size_t i = size_; i < new_size; i++) {
        k++;
        new (copy + i) T(value);
      }
    } catch (...) {
      for (size_t i = 0; i < k; i++) {
        std::destroy_at(copy + i);
      }
      operator delete[](copy);
      throw;
    }
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    operator delete[](ptr_);
    ptr_ = copy;
    size_ = new_size;
    capacity_ = new_size;
  }
  void ShrinkToFit() {
    if (size_ > 0) {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * size_));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size_; i++) {
          k++;
          new (copy + i) T(std::forward<T>(ptr_[i]));
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      for (size_t i = 0; i < size_; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      ptr_ = copy;
      capacity_ = size_;
      return;
    }
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    operator delete[](ptr_);
    ptr_ = nullptr;
    capacity_ = 0;
  }
  void Clear() {
    for (size_t i = 0; i < size_; i++) {
      std::destroy_at(ptr_ + i);
    }
    size_ = 0;
  }
  void PushBack(T&& other) {
    if (capacity_ == size_) {
      capacity_ = capacity_ * 2 + 1;
      auto copy = static_cast<T*>(operator new[](sizeof(T) * capacity_));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size_; i++) {
          k++;
          new (copy + i) T(std::move(ptr_[i]));
        }
        k++;
        new (copy + size_) T(std::move(other));
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      for (size_t i = 0; i < size_; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      size_++;
      ptr_ = copy;
    } else {
      new (ptr_ + size_) T(std::move(other));
      size_++;
    }
  }
  void PushBack(const T& other) {
    if (capacity_ == size_) {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * (capacity_ * 2 + 1)));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size_; i++) {
          k++;
          new (copy + i) T(ptr_[i]);
        }
        k++;
        new (copy + size_) T(other);
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      for (size_t i = 0; i < size_; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      capacity_ = capacity_ * 2 + 1;
      size_++;
      ptr_ = copy;
    } else {
      new (ptr_ + size_) T(other);
      size_++;
    }
  }
  template <class... Args>
  void EmplaceBack(Args&&... arg) {
    if (capacity_ == size_) {
      capacity_ = capacity_ * 2 + 1;
      auto copy = static_cast<T*>(operator new[](sizeof(T) * capacity_));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size_; i++) {
          k++;
          new (copy + i) T(ptr_[i]);
        }
        k++;
        new (copy + size_) T(std::forward<Args>(arg)...);
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      for (size_t i = 0; i < size_; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      size_++;
      ptr_ = copy;
    } else {
      new (ptr_ + size_) T(std::forward<Args>(arg)...);
      size_++;
    }
  }
  void PopBack() {
    if (ptr_ != nullptr) {
      std::destroy_at(ptr_ + size_ - 1);
      size_--;
    }
  }
  Iterator begin() {  // NOLINT
    return ptr_;
  }
  ConstIterator begin() const {  // NOLINT
    return ptr_;
  }
  Iterator end() {  // NOLINT
    return (ptr_ + size_);
  }
  ConstIterator end() const {  // NOLINT
    return (ptr_ + size_);
  }
  ConstIterator cbegin() const {  // NOLINT
    return ptr_;
  }
  ConstIterator cend() const {  // NOLINT
    return (ptr_ + size_);
  }
  ReverseIterator rbegin() {  // NOLINT
    return static_cast<ReverseIterator>(ptr_ + size_);
  }
  ConstReverseIterator rbegin() const {  // NOLINT
    return static_cast<ConstReverseIterator>(ptr_ + size_);
  }
  ReverseIterator rend() {  // NOLINT
    return static_cast<ReverseIterator>(ptr_);
  }
  ConstReverseIterator rend() const {  // NOLINT
    return static_cast<ConstReverseIterator>(ptr_);
  }
  ConstReverseIterator crbegin() const {  // NOLINT
    return static_cast<ConstReverseIterator>(ptr_ + size_);
  }
  ConstReverseIterator crend() const {  // NOLINT
    return static_cast<ConstReverseIterator>(ptr_);
  }
  void Reserve(SizeType new_cap) {
    if (new_cap <= capacity_) {
      return;
    }
    if (new_cap != 0) {
      auto copy = static_cast<T*>(operator new[](sizeof(T) * new_cap));
      size_t k = 0;
      try {
        for (size_t i = 0; i < size_; i++) {
          k++;
          new (copy + i) T(std::forward<T>(ptr_[i]));
        }
      } catch (...) {
        for (size_t i = 0; i < k; i++) {
          std::destroy_at(copy + i);
        }
        operator delete[](copy);
        throw;
      }
      for (size_t i = 0; i < size_; i++) {
        std::destroy_at(ptr_ + i);
      }
      operator delete[](ptr_);
      ptr_ = copy;
      capacity_ = new_cap;
    }
  }
  friend bool operator<(const Vector<T>& d1, const Vector<T>& d2) {
    if (d1.ptr_ == nullptr && d2.ptr_ == nullptr) {
      return false;
    }
    if (d1.ptr_ == nullptr && d2.ptr_ != nullptr) {
      return true;
    }
    if (d1.ptr_ != nullptr && d2.ptr_ == nullptr) {
      return false;
    }
    size_t tmp = (d1.size_ < d2.size_) ? d1.size_ : d2.size_;
    for (size_t i = 0; i < tmp; i++) {
      if (d1.ptr_[i] > d2.ptr_[i]) {
        return false;
      }
      if (d1.ptr_[i] < d2.ptr_[i]) {
        return true;
      }
    }
    return (d2.size_ > d1.size_);
  }
  friend bool operator>=(const Vector<T>& a, const Vector<T>& b) {
    return !(a < b);
  }
  friend bool operator==(const Vector<T>& a, const Vector<T>& b) {
    return ((a >= b) && (b >= a));
  }
  friend bool operator<=(const Vector<T>& a, const Vector<T>& b) {
    return (a < b) || (a == b);
  }
  friend bool operator>(const Vector<T>& a, const Vector<T>& b) {
    return !(a <= b);
  }
  friend bool operator!=(const Vector<T>& a, const Vector<T>& b) {
    return !(a == b);
  }
};
