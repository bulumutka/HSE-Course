#pragma once

#include <cassert>
#include <list>
#include <memory>
#include <set>
#include <type_traits>

namespace task {

template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
    struct Node;
    class Iterator;

public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const Iterator;
    using reverse_iterator = std::reverse_iterator<Iterator>;
    using const_reverse_iterator = std::reverse_iterator<const Iterator>;

    // Special member functions
    List() = default;

    List(const List& other) : allocator_{other.allocator_} {
        for (auto it = other.Begin(); it != other.End(); ++it) {
            PushBack(*it);
        }
    }

    List(const List& other, const Allocator& alloc);

    List(List&& other);
    List(List&& other, const Allocator& alloc);

    ~List();

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    // Element access
    reference Front();
    const_reference Front() const;
    reference Back();
    const_reference Back() const;

    // Iterators
    iterator Begin() noexcept;
    const_iterator Begin() const noexcept;

    iterator End() noexcept;
    const_iterator End() const noexcept;

    // Capacity
    bool Empty() const noexcept;

    size_type Size() const noexcept;
    size_type MaxSize() const noexcept;

    // Modifiers
    void Clear();
    void Swap(List& other) noexcept;

    void PushBack(const T& value);
    void PushBack(T&& value);

    template <typename... Args>
    void EmplaceBack(Args&&... args);
    void PopBack();
    void PushFront(const T& value);
    void PushFront(T&& value);
    template <typename... Args>
    void EmplaceFront(Args&&... args);
    void PopFront();

    void Resize(size_type count);

    // Operations
    void Remove(const T& value);
    void Unique();
    void Sort();

    allocator_type GetAllocator() const noexcept;

private:
    using node_alocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    node_alocator allocator_;
    Node* head_{nullptr};
    Node* tail_{nullptr};

    struct Node {
        Node* prev;
        Node* next;
        value_type value;

        Node(Node* prev, Node* next, const_reference value) : prev{prev}, next{next}, value{value} {
        }
    };

    class Iterator {
    public:
        using value_type = List::value_type;
        using pointer = List::pointer;
        using reference = List::reference;
        using difference_type = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        Iterator() = delete;
        explicit Iterator(Node* node, bool end = false) : node_{node}, end_{end} {
        }

        Iterator& operator++() {
            if (!node_->next) {
                end_ = true;
            } else {
                node_ = node_->next;
            }
            return *this;
        }

        Iterator& operator--() {
            if (!end_) {
                node_ = node_->prev;
            }
            end_ = false;
            return *this;
        }

        Iterator operator++(int) {
            Iterator it(*this);
            operator++();
            return it;
        }

        Iterator operator--(int) {
            Iterator it(*this);
            operator--();
            return it;
        }

        pointer operator->() const {
            assert(!end_);
            return &node_->value;
        }

        reference operator*() const {
            assert(!end_);
            return node_->value;
        }

        bool operator==(const Iterator& other) {
            return node_ == other.node_ && end_ == other.end_;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }

    private:
        Node* node_;
        bool end_;
    };
};

template <typename T, typename Allocator>
List<T, Allocator>::List(const List& other, const Allocator& alloc) {
    allocator_ = alloc;
    for (auto it = other.Begin(); it != other.End(); ++it) {
        PushBack(*it);
    }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List&& other) {
    *this = std::move(other);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List&& other, const Allocator& alloc) {
    allocator_ = alloc;
    for (auto it = other.Begin(); it != other.End(); ++it) {
        PushBack(*it);
    }
    other.Clear();
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    Clear();
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& other) {
    if (this == &other) {
        return *this;
    }

    Clear();

    if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
        allocator_ = other.allocator_;
    }

    for (auto it = other.Begin(); it != other.End(); ++it) {
        PushBack(*it);
    }

    return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    Clear();

    if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
        allocator_ = other.allocator_;
        head_ = other.head_;
        tail_ = other.tail_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
    } else {
        for (auto it = other.Begin(); it != other.End(); ++it) {
            PushBack(*it);
        }
        other.Clear();
    }

    return *this;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Front() {
    return head_->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Front() const {
    return head_->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Back() {
    return tail_->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Back() const {
    return tail_->value;
}

template <typename T, typename Allocator>
bool List<T, Allocator>::Empty() const noexcept {
    return head_ == nullptr;
}

template <typename T, typename Allocator>
size_t List<T, Allocator>::Size() const noexcept {
    size_t count = 0;
    auto node = head_;
    while (node) {
        ++count;
        node = node->next;
    }
    return count;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Clear() {
    while (!Empty()) {
        PopFront();
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(const_reference value) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, tail_, nullptr, value);

    if (!head_) {
        head_ = node;
    }
    if (tail_) {
        tail_->next = node;
    }
    tail_ = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(T&& value) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, tail_, nullptr, std::move(value));

    if (!head_) {
        head_ = node;
    }
    if (tail_) {
        tail_->next = node;
    }
    tail_ = node;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceBack(Args&&... args) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, tail_, nullptr, std::forward<Args>(args)...);

    if (!head_) {
        head_ = node;
    }
    if (tail_) {
        tail_->next = node;
    }
    tail_ = node;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopBack() {
    assert(tail_);

    auto node = tail_;
    tail_ = node->prev;
    if (!tail_) {
        head_ = nullptr;
    } else {
        tail_->next = nullptr;
    }

    allocator_.destroy(node);
    allocator_.deallocate(node, 1);
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(const_reference value) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, nullptr, head_, value);

    if (head_) {
        head_->prev = node;
    }
    head_ = node;
    if (!tail_) {
        tail_ = node;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(T&& value) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, nullptr, head_, std::move(value));

    if (head_) {
        head_->prev = node;
    }
    head_ = node;
    if (!tail_) {
        tail_ = node;
    }
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceFront(Args&&... args) {
    auto node = allocator_.allocate(1);
    allocator_.construct(node, nullptr, head_, std::forward<Args>(args)...);

    if (head_) {
        head_->prev = node;
    }
    head_ = node;
    if (!tail_) {
        tail_ = node;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopFront() {
    if (!head_) {
        return;
    }

    auto node = head_;
    head_ = node->next;
    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }

    allocator_.destroy(node);
    allocator_.deallocate(node, 1);
}

template <typename T, typename Allocator>
void List<T, Allocator>::Resize(size_type count) {
    const size_type size = Size();
    if (size < count) {
        for (size_type i = 0; i + size < count; ++i) {
            PushBack(T());
        }
    } else {
        for (size_type i = 0; i + count < size; ++i) {
            PopBack();
        }
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Swap(List& other) noexcept {
    if (&other == this) {
        return;
    }

    if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
        std::swap(allocator_, other.allocator_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    } else {
        List list = std::move(other);
        other = std::move(*this);
        *this = std::move(list);
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Remove(const_reference value) {
    auto node = head_;
    while (node) {
        Node* next;

        if (node->value != value) {
            next = node->next;
        } else {
            if (node->prev) {
                node->prev->next = node->next;
            }
            if (node->next) {
                node->next->prev = node->prev;
            }
            if (node == head_) {
                head_ = node->next;
            }
            if (node == tail_) {
                tail_ = node->prev;
            }

            next = node->next;

            allocator_.destroy(node);
            allocator_.deallocate(node, 1);
        }

        node = next;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Unique() {
    auto node = head_;
    std::set<T> set;

    while (node) {
        Node* next;

        if (set.find(node->value) == set.end()) {
            set.insert(node->value);
            next = node->next;
        } else {
            node->prev->next = node->next;
            if (node->next) {
                node->next->prev = node->prev;
            }
            if (node == head_) {
                head_ = node->next;
            }
            if (node == tail_) {
                tail_ = node->prev;
            }

            next = node->next;

            allocator_.destroy(node);
            allocator_.deallocate(node, 1);
        }

        node = next;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Sort() {
    const size_type size = Size();
    for (size_type i = 0; i < size; ++i) {
        auto node = head_;
        for (size_type j = 0; j + i + 1 < size; ++j, node = node->next) {
            if (node->value > node->next->value) {
                std::swap(node->value, node->next->value);
            }
        }
    }
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::Begin() noexcept {
    return iterator(head_, head_ == nullptr);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::Begin() const noexcept {
    return const_iterator(head_, head_ == nullptr);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::End() noexcept {
    return iterator(tail_, true);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::End() const noexcept {
    return const_iterator(tail_, true);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::allocator_type List<T, Allocator>::GetAllocator() const noexcept {
    return Allocator(allocator_);
}

}  // namespace task
