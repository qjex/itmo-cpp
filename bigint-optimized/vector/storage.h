//
// Created by Andrew Berlin on 4/23/18.
//

#ifndef BIGINT_OPTIMIZED_VECTOR_H
#define BIGINT_OPTIMIZED_VECTOR_H

#include <cstring>
#include <type_traits>
#include <memory>

typedef unsigned int digit_type;

struct storage {
    storage();
    storage(storage const &v);
    storage(size_t size, digit_type const &a);
    explicit storage(size_t size);
    ~storage();

    friend bool operator==(storage const &a, storage const &b);

    storage &operator=(storage a);
    digit_type &operator[](size_t ind);
    digit_type const &operator[](size_t ind) const;

    digit_type const &back() const;

    bool empty() const;
    size_t size() const;

    void push_back(digit_type value);
    void pop_back();

    void swap(storage &a) noexcept;

private:
    static const size_t SMALL_OBJ_SIZE = 4;

    size_t _size = 0;
    bool _is_big = false;
    digit_type *active_data = nullptr;

    struct big_obj {
        std::shared_ptr<digit_type> storage_ptr;
        size_t capacity;

        big_obj() {
            capacity = 0;
            storage_ptr = nullptr;
        }

        big_obj(digit_type *a, size_t capacity) :
            storage_ptr(a), capacity(capacity) {}
    };

    union container {
        digit_type small_data[SMALL_OBJ_SIZE];
        big_obj big_data;

        ~container() {}
        container() {}
    } data;

    struct deleter {
        void operator()(digit_type const *p) {
            delete[] p;
        }
    };

private:
    void to_big();
    void detach();
    bool is_big() const;
    size_t get_capacity();
    void swap_data(container &big_container, container &small_container);
    void ensure_capacity(size_t n);
};

bool operator==(storage const &a, storage const &b);
digit_type *copy_data(digit_type *data, size_t size, size_t new_size);

#endif //BIGINT_OPTIMIZED_VECTOR_H
