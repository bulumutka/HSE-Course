#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using pointer_difference = ptrdiff_t;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    CustomAllocator();
    CustomAllocator(const CustomAllocator& other) noexcept;
    ~CustomAllocator();

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    T* allocate(size_t n) {  // NOLINT
        if (*size_ + n > kMaxSize) {
            throw std::runtime_error("Not enough memory");
        }
        auto result = static_cast<pointer>(data_) + *size_;
        *size_ += n;
        return result;
    }

    void deallocate(T* p, size_t n) {  // NOLINT
        // Do nothing for linear strategy.
    }

    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        new (static_cast<void*>(p)) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {  // NOLINT
        p->~value_type();
    }

    void* Data() const {
        return data_;
    }

    size_type* Counter() const {
        return counter_;
    }

    size_type* Size() const {
        return size_;
    }

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;
    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const size_type kMaxSize{100000};

    void* data_{nullptr};
    size_type* size_{nullptr};
    size_type* counter_{nullptr};
};

template <typename T>
CustomAllocator<T>::~CustomAllocator() {
    --(*counter_);
    if (*counter_ == 0) {
        operator delete(data_);
        delete size_;
        delete counter_;
    }
}

template <typename T>
CustomAllocator<T>::CustomAllocator()
    : data_{operator new(kMaxSize * sizeof(value_type))},
      size_{new size_t(0)},
      counter_{new size_t(1)} {
}

template <typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept
    : data_{other.data_}, size_{other.size_}, counter_{other.counter_} {
    ++(*counter_);
}

template <typename T>
template <typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept
    : data_{other.Data()}, size_{other.Size()}, counter_{other.Counter()} {
    ++(*counter_);
}

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.data_ == rhs.data_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}
