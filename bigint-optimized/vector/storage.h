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

    struct dynamic_storage {
        int ref_cnt = 0;
        size_t capacity;
        digit_type arr[];
    };

    struct big_obj {
        dynamic_storage *storage_ptr = nullptr;

        big_obj() {
            storage_ptr = new_dynamic_storage(0);
        }

        dynamic_storage *new_dynamic_storage(size_t capacity) {
            dynamic_storage* result = (dynamic_storage*) malloc(sizeof(dynamic_storage) + sizeof(digit_type) * capacity);
            result->ref_cnt = 1;
            result->capacity = capacity;
            return result;
        }

        big_obj(size_t capacity) {
            storage_ptr = new_dynamic_storage(capacity);
        }

        big_obj(digit_type *a, size_t size, size_t capacity) {
            storage_ptr = new_dynamic_storage(capacity);
            memcpy(storage_ptr->arr, a, size * sizeof(digit_type));
        }

        void update_capacity(size_t new_capacity) {
            auto ptr = new_dynamic_storage(new_capacity);
            memcpy(ptr->arr, storage_ptr->arr, storage_ptr->capacity * sizeof(digit_type));
            storage_ptr = ptr;
        }

        big_obj(big_obj const& other) {
            storage_ptr = other.quick_copy();
        }

        void detach() {
            storage_ptr->ref_cnt--;
            auto ptr = new_dynamic_storage(storage_ptr->capacity);
            memcpy(ptr->arr, storage_ptr->arr, storage_ptr->capacity * sizeof(digit_type));
            storage_ptr = ptr;
        }

        dynamic_storage * quick_copy()const {
            storage_ptr->ref_cnt++;
            return storage_ptr;
        }

        digit_type *get() {
            return storage_ptr->arr;
        }

        bool unique() {
            return storage_ptr->ref_cnt == 1;
        }
    };

    union container {
        digit_type small_data[SMALL_OBJ_SIZE];
        big_obj big_data;

        ~container() {}
        container() {}
    } data;

private:
    void to_big(size_t new_capacity);
    void detach();
    bool is_big() const;
    size_t get_capacity();
    void swap_data(container &big_container, container &small_container);
    void ensure_capacity(size_t n);
};

bool operator==(storage const &a, storage const &b);

#endif //BIGINT_OPTIMIZED_VECTOR_H
