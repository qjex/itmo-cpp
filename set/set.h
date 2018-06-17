//
// Created by Andrew Berlin on 6/17/18.
//

#ifndef SET_SET_H
#define SET_SET_H

#include <iterator>

template<typename T>
struct set;

template<typename T>
void swap(set<T> &a, set<T> &b) noexcept {
    std::swap(a.fake, b.fake);
    if (b.root()->l != nullptr) {
        b.root()->l->parent = &b.fake;
    }
    if (a.root()->l != nullptr) {
        a.root()->l->parent = &a.fake;
    }
}

template<typename T>
struct set {
private:
    struct base_set_node {
        base_set_node *l, *r, *parent;

        base_set_node() {
            l = nullptr;
            r = nullptr;
            parent = nullptr;
        }

        ~base_set_node() {
        }
    };

    struct set_node : base_set_node {
        T data;

        set_node(T const &data) : data(data) {
        }
    };

    mutable base_set_node fake;

    base_set_node *root() const {
        return &fake;
    }

    void erase_impl(base_set_node* v) {
        base_set_node *p = v->parent;
        if (v->l && v->r) {
            auto nxt = minimum(v->r);
            if (!nxt->l && !nxt->r) {
                if (nxt->parent->l == nxt) {
                    nxt->parent->l = nullptr;
                } else {
                    nxt->parent->r = nullptr;
                }
            } else if (!nxt->l && nxt->r) {
                if (nxt->parent->l == nxt) {
                    nxt->parent->l = nxt->r;
                } else {
                    nxt->parent->r = nxt->r;
                }
                nxt->r->parent = nxt->parent;
            } else if (nxt->l && !nxt->r) {
                if (nxt->parent->l == nxt) {
                    nxt->parent->l = nxt->l;
                } else {
                    nxt->parent->r = nxt->l;
                }
                nxt->l->parent = nxt->parent;
            }

            if (v->parent->l == v) {
                v->parent->l = nxt;
            } else {
                v->parent->r = nxt;
            }
            if (v->r) {
                v->r->parent = nxt;
            }
            if (v->l) {
                v->l->parent = nxt;
            }

            nxt->parent = v->parent;
            nxt->l = v->l;
            nxt->r = v->r;
        } else {
            if (v->l == nullptr && v->r == nullptr) {
                if (p->l == v) {
                    p->l = nullptr;
                } else {
                    p->r = nullptr;
                }
            } else {
                if (v->r == nullptr) {
                    if (v == p->l) {
                        p->l = v->l;
                    } else {
                        p->r = v->l;
                    }
                    v->l->parent = p;
                } else if (v->l == nullptr) {
                    if (v == p->l) {
                        p->l = v->r;
                    } else {
                        p->r = v->r;
                    }
                    v->r->parent = p;
                }
            }
        }
        v->parent = nullptr;
        v->r = nullptr;
        v->l = nullptr;
        delete static_cast<set_node *>(v);
    }

    base_set_node *find(base_set_node *v, T const &val) const {
        if (v == nullptr) {
            return v;
        }
        T const &x = static_cast<set_node *>(v)->data;
        if (!(val < x) && !(x < val)) {
            return v;
        }
        if (val < x) {
            return find(v->l, val);
        } else {
            return find(v->r, val);
        }
    }

    static base_set_node *minimum(base_set_node *v) {
        if (v->l == nullptr) {
            return v;
        }
        return minimum(v->l);
    }

    static base_set_node *maximum(base_set_node *v) {
        if (v->r == nullptr) {
            return v;
        }
        return maximum(v->r);
    }

    static base_set_node *next(base_set_node *v) {
        if (v->r != nullptr) {
            return minimum(v->r);
        }
        auto y = v->parent;
        while (y != nullptr && v == y->r) {
            v = y;
            y = y->parent;
        }
        return y;
    }

    static base_set_node *prev(base_set_node *v) {
        if (v->l != nullptr) {
            return maximum(v->l);
        }
        auto y = v->parent;
        while (y != nullptr && v == y->l) {
            v = y;
            y = y->parent;
        }
        return y;
    }

    base_set_node *lower_bound(base_set_node *v, T const &value) const {
        if (v == nullptr) {
            return v;
        }

        T const &z = static_cast<set_node *> (v)->data;

        if (!(z < value) && !(value < z)) {
            return v;
        }

        if (z < value) {
            return lower_bound(v->r, value);
        }

        auto nxt = lower_bound(v->l, value);
        if (nxt == nullptr) {
            return v;
        }
        T const &nxt_val = static_cast<set_node *> (nxt)->data;

        if (!(nxt_val < value)) {
            return nxt;
        }
        return v;
    }

    friend class iterator;

public:
    template<typename V>
    class set_iterator : public std::iterator<std::bidirectional_iterator_tag, V> {
        friend class set<T>;
    public:
        set_iterator(const set_iterator<V> &other) {
            p = other.p;
        }

        set_iterator() {
            p = nullptr;
        }

        V const *operator->() const {
            return &(static_cast<set_node *>(p)->data);
        }

        const set_iterator operator++(int) {
            set_iterator was(p);
            ++(*this);
            return was;
        }

        const set_iterator operator--(int) {
            set_iterator was(p);
            --(*this);
            return was;
        }

        V &operator*() const {
            return (static_cast<set_node *>(p))->data;
        }

        set_iterator &operator++() {
            p = next(p);
            return *this;
        }

        set_iterator &operator--() {
            p = prev(p);
            return *this;
        }

        bool operator!=(const set_iterator &other) const {
            return p != other.p;
        }

        bool operator==(const set_iterator &other) const {
            return p == other.p;
        }
    private:
        explicit set_iterator(base_set_node *p) : p(p) {

        }

        base_set_node *get() {
            return p;
        }
        base_set_node *p;
    };

    typedef set_iterator<const T> const_iterator;
    typedef const_iterator iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef const_reverse_iterator reverse_iterator;

    set() noexcept {
    }

    ~set() {
        clear();
    }

    set &operator=(const set &l) {
        set tmp(l);
        swap(tmp, *this);
        return *this;
    }

    set(const set &l) : set() {
        for (const auto &e : l) {
            insert(e);
        }
    }

    bool empty() const noexcept {
        return root()->l == nullptr;
    }

    void clear() noexcept {
        for (auto e = begin(); e != end();) {
            e = erase(e);
        }
    }

    std::pair<iterator, bool> insert(T const &value) {
        if (empty()) {
            root()->l = new set_node(value);
            root()->l->parent = root();
            return {const_iterator(root()->l), true};
        }

        base_set_node *v = root()->l;
        while (v != nullptr) {
            T const &z = static_cast<set_node *>(v)->data;

            if (z < value) {
                if (v->r != nullptr) {
                    v = v->r;
                } else {
                    v->r = new set_node(value);
                    v->r->parent = v;
                    return {const_iterator(v->r), true};
                }
            } else {
                if (value < z) {
                    if (v->l != nullptr) {
                        v = v->l;
                    } else {
                        v->l = new set_node(value);
                        v->l->parent = v;
                        return {const_iterator(v->l), true};
                    }
                } else {
                    return {const_iterator(v), false};
                }
            }
        }
        return {const_iterator(root()), false};
    }

    iterator erase(const_iterator pos) noexcept {
        iterator ans(pos);
        ++ans;
        erase_impl(pos.p);
        return ans;
    }

    const_iterator find(T const &value) const {
        auto res = find(root()->l, value);

        if (res == nullptr) {
            return const_iterator(root());
        }
        return const_iterator(res);
    }

    const_iterator lower_bound(T const &value) const{
        if (empty()) {
            return const_iterator(root());
        }
        auto res = lower_bound(root()->l, value);

        if (res != nullptr) {
            return const_iterator(res);
        }
        return const_iterator(root());
    }

    const_iterator upper_bound(T const &value) const {
        if (empty()) {
            return const_iterator(root());
        }

        auto v = find(value);
        if (v == end()) {
            return lower_bound(value);
        }

        return ++const_iterator(v);

    }

    const_iterator begin() const {
        auto v = root();
        while (v->l != nullptr) {
            v = v->l;
        }
        return const_iterator(v);
    }

    const_iterator end() const {
        return const_iterator(root());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    friend void swap<T>(set &lhs, set &rhs) noexcept;
};

#endif //SET_SET_H
