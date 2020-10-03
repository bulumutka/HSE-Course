#include <vector>
#include <list>

#include "gtest/gtest.h"
#include "list.h"

using task::list;

TEST(Creation, Test1) {
    list arr;
    ASSERT_TRUE(arr.empty());

    list brr(10);
    ASSERT_EQ(10, brr.size());
}

TEST(Workflow, Test2) {
    list arr;
    arr.push_back(10);
    arr.push_back(5);
    ASSERT_EQ(5, arr.back());
    arr.push_back(3);
    ASSERT_EQ(10, arr.front());
    ASSERT_EQ(3, arr.back());
    arr.push_front(200);
    ASSERT_EQ(200, arr.front());
    arr.pop_front();
    ASSERT_EQ(3, arr.size());
    ASSERT_EQ(10, arr.front());
    arr.pop_back();
    ASSERT_EQ(2, arr.size());
    ASSERT_EQ(5, arr.back());
}

TEST(Assign, Test3) {
    list a(5);
    list b;
    b = a;
    ASSERT_EQ(a.size(), b.size());
    a = b;
}

TEST(Resize, Test4) {
    list a(4);
    a.resize(4);
    ASSERT_EQ(4, a.size());

    a.resize(6);
    ASSERT_EQ(6, a.size());
    
    a.resize(2);
    ASSERT_EQ(2, a.size());

    list b;
    b.resize(10);
}

TEST(Swap, Test5) {
    list a(3);
    list b(6);
    a.swap(b);
    ASSERT_EQ(6, a.size());
    ASSERT_EQ(3, b.size());
}

TEST(Remove, Test6) {
    std::list<int> data = {
        25, 11, 42, 84, 34, 6, 95,
        2, 66, 6, 25, 75, 63, 57, 49,
        46, 85, 25, 25, 56, 20, 75,
        50, 60, 95, 61, 1, 11, 73, 86,
        42, 15, 64, 10
    };
    list b;
    for (const auto i : data) {
        b.push_back(i);
    }
    data.remove(data.front());
    b.remove(b.front());
    ASSERT_EQ(data.size(), b.size());
}

bool isSorted(list a) {
    if (a.empty()) return true;
    int last = a.front();
    while (!a.empty()) {
        if (last > a.front()) {
            return false;
        }
        last = a.front();
        a.pop_front();
    }
    return true;
}

TEST(Sort, Test7) {
    list a;
    a.sort();
    ASSERT_TRUE(isSorted(a));
    a.push_front(10);
    a.sort();
    ASSERT_TRUE(isSorted(a));
    a.push_front(0);
    a.push_front(1);
    a.sort();
    ASSERT_TRUE(isSorted(a));
}

TEST(Unique, Test8) {
    list a(10);
    a.unique();
    ASSERT_EQ(1, a.size());
    a.push_back(5);
    a.push_back(5);
    a.push_back(5);
    a.push_back(3);
    a.push_back(3);
    a.push_back(3);
    a.push_back(3);
    a.unique();
    ASSERT_EQ(3, a.size());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
