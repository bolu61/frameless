#include <array>

namespace frameless::core {
  template<typename T, size_t capacity>
  class static_buffer_allocator {
  public:
    template<typename S>
    struct rebind {
      using other = static_buffer_allocator<S, capacity>;
    };

    using value_type = T;
    static_buffer_allocator() noexcept;
    template<typename S> explicit static_buffer_allocator(const static_buffer_allocator<S, capacity> & other) noexcept;
    template<typename S> bool operator==(const static_buffer_allocator<S, capacity> & other) const noexcept;
    template<typename S> bool operator!=(const static_buffer_allocator<S, capacity> & other) const noexcept; 
    T * allocate(std::size_t const size) const;
    void deallocate(T * const p, std::size_t const size) const;
    constexpr size_t max_size() const noexcept;
  private:
    std::array<T, capacity> storage;
  };

  template<size_t capacity>
  using string = std::basic_string<char, std::char_traits<char>, static_buffer_allocator<char, capacity + 1>>;


  // implementations

  template<typename T, size_t capacity>
  static_buffer_allocator<T, capacity>::static_buffer_allocator() noexcept = default;

  template<typename T, size_t capacity>
  template<typename S>
  static_buffer_allocator<T, capacity>::static_buffer_allocator(const static_buffer_allocator<S, capacity> & other) noexcept : storage(other.storage) {}

  template<typename T, size_t capacity>
  template<typename S>
  bool static_buffer_allocator<T, capacity>::operator==(const static_buffer_allocator<S, capacity> & other) const noexcept {
    return storage == other.storage;
  }

  template<typename T, size_t capacity>
  template<typename S>
  bool static_buffer_allocator<T, capacity>::operator!=(const static_buffer_allocator<S, capacity> & other) const noexcept {
    return storage != other.storage;
  }

  template<typename T, size_t capacity>
  T * static_buffer_allocator<T, capacity>::allocate(const std::size_t size) const {
    if (size > capacity) {
      throw std::bad_alloc();
    }
    return (T *)storage.data();
  }

  template<typename T, size_t capacity>
  void static_buffer_allocator<T, capacity>::deallocate([[maybe_unused]] T * const p, [[maybe_unused]] const std::size_t size) const {
    // this stack allocator does not deallocate, since the same buffer is reused
  };

  template<typename T, size_t capacity>
  constexpr size_t static_buffer_allocator<T, capacity>::max_size() const noexcept {
    return capacity;
  }
}
