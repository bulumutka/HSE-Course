#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
private:
    struct SharedState;

public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using pointer_difference = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    CustomAllocator();
    CustomAllocator(const CustomAllocator& other) noexcept;
    ~CustomAllocator() = default;

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    T* allocate(size_t n) {  // NOLINT
        if (state_->size + n > kMaxSize) {
            throw std::runtime_error("Not enough memory");
        }
        auto result = static_cast<pointer>(state_->data) + state_->size;
        state_->size += n;
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

    std::shared_ptr<SharedState> State() const {
        return state_;
    }

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;
    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const size_type kMaxSize{100000};

    std::shared_ptr<SharedState> state_;

    struct SharedState {
        void* data;
        size_type size;

        SharedState() : data(operator new(kMaxSize * sizeof(value_type))), size(0) {
        }

        SharedState(const SharedState& other) = delete;
        SharedState& operator=(const SharedState& other) = delete;

        ~SharedState() {
            operator delete(data);
        }

        bool operator==(const SharedState& other) const {
            return data == other.data;
        }

        bool operator!=(const SharedState& other) const {
            return !(*this == other);
        }
    };
};

template <typename T>
CustomAllocator<T>::CustomAllocator() : state_(std::make_shared<SharedState>()) {
}

template <typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept : state_(other.State()) {
}

template <typename T>
template <typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept
    : state_(other.State()) {
}

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.State() == rhs.State();
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}
