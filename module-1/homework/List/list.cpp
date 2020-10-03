#include "list.h"

namespace task {

list::list(): head_(nullptr), size_(0) {}

list::list(size_t count, const int& value): list() {
    for (size_t i = 0; i != count; ++i) {
        push_front(value);
    }
}

list::list(const list& other) : list() {
    init_(other);
}

list::~list() {
    clear();
}

list& list::operator=(const list& other) {
    init_(other);
    return *this;
}

int& list::front() {
    assertNotEmpty_();
    return head_->value;
}

const int& list::front() const {
    assertNotEmpty_();
    return head_->value;
}

int& list::back() {
    assertNotEmpty_();
    auto ptr = head_;
    while (ptr->next) {
        ptr = ptr->next;
    }
    return ptr->value;
}

const int& list::back() const {
    assertNotEmpty_();
    auto ptr = head_;
    while (ptr->next) {
        ptr = ptr->next;
    }
    return ptr->value;
}

bool list::empty() const {
    return size_ == 0;
}

size_t list::size() const {
    return size_;
}

void list::clear() {
    while (!empty()) {
        pop_front();
    }
}

void list::push_back(const int& value) {
    if (empty()) {
        head_ = new node(value);
    } else {
        auto ptr = head_;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = new node(value);
    }
    ++size_;
}

void list::pop_back() {
    assertNotEmpty_();
    if (size_ == 1) {
        clear();
        return;
    }

    auto pre_ptr = head_;
    auto ptr = head_->next;
    while (ptr->next) {
        pre_ptr = ptr;
        ptr = ptr->next;
    }

    pre_ptr->next = nullptr;
    delete ptr;
    --size_;
}

void list::push_front(const int& value) {
    head_ = new node(value, head_);
    ++size_;
}

void list::pop_front() {
    assertNotEmpty_();
    auto old_head = head_;
    head_ = head_->next;
    delete old_head; 
    --size_;
}

void list::resize(size_t count) {
    if (count == size_) return;

    if (count < size_) {
        auto ptr = head_;
        for (size_t i = 1; i < count; ++i) {
            ptr = ptr->next;
        }
        auto last = ptr;
        ptr = last->next;
        last->next = nullptr;
        while (ptr) {
            last = ptr;
            ptr = ptr->next;
            delete last;
        }
    } else {
        if (empty()) {
            *this = list(count);
            return;
        }

        auto ptr = head_;
        while (ptr->next) {
            ptr = ptr->next;
        }
        for (size_t i = size_; i != count; ++i) {
            ptr->next = new node(int());
            ptr = ptr->next;
        }
    }

    size_ = count;
}

void list::swap(list& other) {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
}

void list::remove(const int& value) {
    int value_copy = value;
    while (!empty() && head_->value == value) {
        pop_front();
    }
    if (empty()) return;

    auto pre_ptr = head_;
    auto ptr = pre_ptr->next;
    while (ptr) {
        if (ptr->value == value_copy) {
            pre_ptr->next = ptr->next;
            delete ptr;
            ptr = pre_ptr->next;
            --size_;
        } else {
            pre_ptr = ptr;
            ptr = ptr->next;
        }
    }
}

void list::unique() {
    if (empty() || size_ == 1) return;

    std::unordered_set<int> elems;
    elems.insert(head_->value);
    auto pre_ptr = head_;
    auto ptr = head_->next;
    while (ptr) {
        if (elems.find(ptr->value) != elems.end()) {
            pre_ptr->next = ptr->next;
            delete ptr;
            ptr = pre_ptr->next;
            --size_;
        } else {
            elems.insert(ptr->value);
            pre_ptr = ptr;
            ptr = ptr->next;
        }
    }
}

void list::sort() {
    if (empty() || size_ == 1) return;

    std::map<int, int> count;
    while (!empty()) {
        ++count[front()];
        pop_front();
    }

    for (auto it = count.rbegin(); it != count.rend(); ++it) {
        for (int i = 0; i < it->second; ++i) {
            push_front(it->first);
        }
    }
}

void list::assertNotEmpty_() const {
    if (empty()) {
        throw std::logic_error("list is empty");
    }  
}

bool list::equals_(const list& other) const {
    if (size_ != other.size_) {
        return false;
    }

    auto ptr = head_;
    auto other_ptr = other.head_;
    while (ptr) {
        if (ptr->value != other_ptr->value) {
            return false;
        }
        ptr = ptr->next;
        other_ptr = other_ptr->next;
    }
    return false;
}

void list::init_(const list& other) {
    if (equals_(other)) return;
    clear();
    if (other.empty()) return;

    head_ = new node(other.head_->value);
    auto ptr = head_;
    auto other_ptr = other.head_->next;

    while (other_ptr) {
        ptr->next = new node(other_ptr->value);
        ptr = ptr->next;
        other_ptr = other_ptr->next;
    }

    size_ = other.size_;
}

list::node::node(const int value, node* next): value(value), next(next) {}

} // namespace tas
