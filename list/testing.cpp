//
// Created by Andrew Berlin on 5/7/18.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <gtest/gtest.h>

#include "list.h"

using namespace std;

TEST(correctness, copy_const) {
    list<int> l;
    for (int i = 0; i < 22; i++) {
        l.push_back(i);
    }
    list<int> l2(l);
    while (!l2.empty()) {
        EXPECT_EQ(l2.back(), l.back());
        l2.pop_back();
        l.pop_back();
    }
    EXPECT_TRUE(l.empty());
}

TEST(correctness, assign) {
    list<int> l;
    for (int i = 0; i < 22; i++) {
        l.push_back(i);
    }
    list<int> l2 = l;
    while (!l2.empty()) {
        EXPECT_EQ(l2.back(), l.back());
        l2.pop_back();
        l.pop_back();
    }
    EXPECT_TRUE(l.empty());
}


TEST(correctness, erase_multiple_1) {
    list<int> l;
    for (int i = 0; i < 10; i++) {
        l.push_back(i);
    }

    l.erase(l.begin(), l.end());
    EXPECT_EQ(true, l.empty());
}

TEST(correctness, erase_end) {
    list<int> l;
    auto i = l.end();
    l.push_back(42);
    --i;
    EXPECT_EQ(42, *i);
}

TEST(correctness, splice) {
    list<int> l1, l2;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);

    l2.push_back(5);
    l2.push_back(6);
    l2.push_back(7);
    l2.push_back(8);

    auto pos = l1.begin();
    ++pos;
    ++pos;

    auto first = l2.begin();
    ++first;
    auto second = first;
    ++second;
    ++second;

    vector<int> v1 = {1, 2, 6, 7, 3, 4};
    vector<int> v2 = {5, 8};

    l1.splice(pos, l2, first, second);

    while (!l1.empty()) {
        EXPECT_EQ(v1.back(), l1.back());
        v1.pop_back();
        l1.pop_back();
    }

    while (!l2.empty()) {
        EXPECT_EQ(v2.back(), l2.back());
        v2.pop_back();
        l2.pop_back();
    }
}

TEST(correctness, slice_all) {
    list<int> l1, l2;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);

    l2.push_back(5);
    l2.push_back(6);
    l2.push_back(7);
    l2.push_back(8);
    l1.splice(l1.begin(), l2, l2.begin(), l2.end());
    EXPECT_TRUE(l2.empty());
}

TEST(correctness, pp) {
    list<int> l;
    for (int i = 0; i < 10; i++) {
        l.push_back(i);
    }

    for (int i = 9; i >= 0; i--) {
        EXPECT_EQ(i, l.back());
        l.pop_back();
    }

    EXPECT_EQ(true, l.empty());

    for (int i = 0; i < 10; i++) {
        l.push_back(i);
    }

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(i, l.front());
        l.pop_front();
    }

    EXPECT_EQ(true, l.empty());
}

TEST(correctness, front) {
    list<int> l;
    for (int i = 0; i < 10; i++) {
        l.push_front(i);
    }

    for (int i = 9; i >= 0; i--) {
        EXPECT_EQ(i, l.front());
        l.pop_front();
    }

    EXPECT_EQ(true, l.empty());
}


TEST(basic, push_pop_back) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ASSERT_EQ(1, v.front());
    ASSERT_EQ(3, v.back());
    v.pop_back();
    ASSERT_EQ(2, v.back());
    v.pop_back();
    ASSERT_EQ(1, v.back());
    v.pop_back();
}

TEST(basic, push_pop_front) {
    list<int> v;
    v.push_front(2);
    v.push_front(3);
    v.push_back(1);
    ASSERT_EQ(3, v.front());
    ASSERT_EQ(1, v.back());
    v.pop_front();
    ASSERT_EQ(2, v.front());
}

TEST(basic, push_clear) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ASSERT_FALSE(v.empty());
    v.clear();
    ASSERT_TRUE(v.empty());
}

//TEST(basic, initcopy) {
//    list<int> v;
//    v.push_back(1);
//    v.push_back(2);
//    v.push_back(3);
//
//    list<int> v2(v);
//    ASSERT_EQ(3, v2.back());
//    v.pop_back();
//    ASSERT_EQ(3, v2.back());
//    v.clear();
//    ASSERT_EQ(3, v2.back());
//}

TEST(iterators, begin_end) {
    list<int> v;
    v.push_back(1);
    ASSERT_EQ(1, *v.begin());
    ASSERT_EQ(1, *--v.end());
    v.push_back(2);
    v.push_back(3);
    ASSERT_EQ(1, *v.begin());
    ASSERT_EQ(3, *--v.end());
}

TEST(iterators, for_it) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int i = 1;
    ASSERT_EQ(1, *v.begin());
    ASSERT_EQ(2, *++v.begin());
    ASSERT_EQ(3, *++++v.begin());
    for (auto it = v.begin(); it != v.end(); ++it) { // NOLINT
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST(iterators, for_each) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    int i = 1;
    for (int it : v) {
        ASSERT_EQ(i, it);
        i++;
    }
}

TEST(iterators, rbegin_rend) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ASSERT_EQ(3, *(v.rbegin()));
    ASSERT_EQ(1, *(--v.rend()));
}

TEST(iterators, insert) {
    list<int> v;
    v.push_back(1);
    v.push_back(2);

    v.insert(v.begin(), 0);
    ASSERT_EQ(0, *v.begin());

    v.insert(v.end(), 228);
    ASSERT_EQ(228, *(--v.end()));
}

TEST(iterators, insert_push) {
    list<int> v;
    v.insert(v.end(), 2);
    v.insert(v.begin(), 0);
    v.insert(++v.begin(), 1);

    ASSERT_EQ(0, *v.begin());
    ASSERT_EQ(1, *++v.begin());
    ASSERT_EQ(2, *++++v.begin());
}

TEST(iterators, insert_push_pop) {
    list<int> v;
    v.insert(v.end(), 2);
    v.insert(v.begin(), 0);
    v.insert(++v.begin(), 1);

    auto it = v.end();

    v.pop_back();

    ASSERT_EQ(1, *(--v.end()));
    v.insert(it, 228);
    ASSERT_EQ(228, *(--v.end()));
}
