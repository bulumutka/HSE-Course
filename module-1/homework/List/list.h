#ifndef LIST_H_
#define LIST_H_

#include <cstddef>
#include <stdexcept>
#include <unordered_set>
#include <map>

namespace task {

class list {
  public:
    list();
    list(size_t count, const int& value = int());
    list(const list& other);

    ~list();
    list& operator=(const list& other);

    int& front();
    const int& front() const;

    int& back();
    const int& back() const;

    bool empty() const;
    size_t size() const;
    void clear();

    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);

    void remove(const int& value);
    void unique();
    void sort();

    struct node {
        node(const int value, node* next = nullptr);

        int value;
        node* next;
    };

  private:
    node* head_;
    size_t size_;

    void assertNotEmpty_() const;
    bool equals_(const list& other) const;
    void init_(const list& other);
};


}  // namespace task

#endif
