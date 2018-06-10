//
// Created by Andrew Berlin on 6/10/18.
//

#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <iterator>
template<typename T>
struct list {
private:
    struct base_list_node {
        base_list_node *l, *r;

        base_list_node() {
            l = nullptr;
            r = nullptr;
        }

    };

    struct list_node : base_list_node {
        T data;

        list_node(T const &data) : data(data) {
        }
    };

    base_list_node fake;
    base_list_node *tail;
public:
    template<typename V>
    class list_iterator : public std::iterator<std::forward_iterator_tag, V> {
    public:
        template<typename W>
        list_iterator(const list_iterator<W> &other) {
            p = other.p;
        };

        V &operator*() {
            return (static_cast<list_node *>(p))->data;
        }

        list_iterator &operator++() {
            p = p->r;
            return *this;
        }

        list_iterator &operator--() {
            p = p->l;
            return *this;
        }

        bool operator!=(const list_iterator &other) const {
            return p != other.p;
        }

        bool operator==(const list_iterator &other) const {
            return p == other.p;
        }

        list_iterator(base_list_node *p) : p(p) {}

        base_list_node *get() {
            return p;
        }
        base_list_node *p;
    };

    typedef list_iterator<T> iterator;
    typedef list_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list() noexcept {
        tail = &fake;
        fake.l = tail;
        fake.r = tail;
    }

    ~list() {
        clear();
    }

    list &operator=(const list &l) {
        list tmp(l);
        swap(tmp);
        return *this;
    }

    list(const list &l) : list() {
        for (auto const &it : l) {
            push_back(it);
        }
    }

    bool empty() const {
        return tail->r == tail;
    }

    void clear() {
        while (!empty()) {
            pop_back();
        }
    }

    void push_back(T const &value) {
        auto *node = new list_node(value);
        node->r = tail;
        node->l = tail->l;
        tail->l->r = node;
        tail->l = node;
    }

    void pop_back() {
        auto rem = tail->l;
        tail->l->l->r = tail;
        tail->l = tail->l->l;
        delete rem;
    }

    void push_front(T const &value) {
        auto *node = new list_node(value);
        node->l = tail;
        node->r = tail->r;
        tail->r->l = node;
        tail->r = node;
    }

    void pop_front() {
        auto rem = tail->r;
        tail->r->r->l = tail;
        tail->r = tail->r->r;
        delete rem;
    }

    T &back() {
        return static_cast<list_node *>(tail->l)->data;
    }

    T &front() {
        return static_cast<list_node *>(tail->r)->data;
    }

    T const &back() const {
        return static_cast<list_node *>(tail->l)->data;
    }

    T const &front() const {
        return static_cast<list_node *>(tail->r)->data;
    }

    iterator insert(const_iterator pos, T const &value) {
        list_node *node = new list_node(value);
        node->l = pos.get()->l;
        node->r = pos.get();
        pos.get()->l->r = node;
        pos.get()->l = node;
        return iterator(node);
    }

    iterator erase(const_iterator pos) {
        iterator ans(pos.p->r);
        pos.p->l->r = pos.p->r;
        pos.p->r->l = pos.p->l;
        delete pos.p;
        return ans;
    }

    iterator erase(const_iterator first, const_iterator second) {
        while (first != second) {
            first = erase(first);
        }
        return first;
    }

    iterator splice(const_iterator pos, list &other, const_iterator first, const_iterator second) {
        auto *r = second.p;

        second.p->l->r = pos.p;
        pos.p->l->r = first.p;

        first.p->l->r = r;
        r = pos.p->l;

        pos.p->l = second.p->l;
        second.p->l = first.p->l;
        first.p->l = r;
        return iterator(pos.get());
    }

    iterator begin() {
        return iterator(tail->r);
    }

    iterator end() {
        return iterator(tail);
    }

    const_iterator begin() const {
        return const_iterator(tail->r);
    }

    const_iterator end() const {
        return const_iterator(tail);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    void swap(list<T> &a) noexcept {
        std::swap(a.tail, tail);
    }
};

#endif //LIST_LIST_H
