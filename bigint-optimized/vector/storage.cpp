//
// Created by Andrew Berlin on 4/23/18.
//

#include "storage.h"

storage::storage() :
    _size(0),
    _is_big(false),
    active_data(data.small_data) {}

storage::~storage() {
    if (is_big()) {
        data.big_data.~big_obj();
    }
}

storage::storage(storage const &a) : _size(a.size()) {
    if (a.is_big()) {
        new(&data.big_data) big_obj(a.data.big_data);
        active_data = data.big_data.storage_ptr.get();
        _is_big = true;
    } else {
        for (size_t i = 0; i < SMALL_OBJ_SIZE; i++) {
            data.small_data[i] = a.data.small_data[i];
        }
        active_data = data.small_data;
        _is_big = false;
    }
}

storage::storage(size_t size, digit_type const &a) : storage() {
    ensure_capacity(size);
    if (a == 0) {
        memset(active_data, 0, size * sizeof(digit_type));
        _size = size;
    } else {
        for (size_t i = 0; i < size; i++) {
            push_back(a);
        }
    }
}

storage::storage(size_t size) : storage(size, 0) {

}

storage &storage::operator=(storage a) {
    swap(a);
    return *this;
}

bool operator==(storage const& a, storage const& b) {
    if (a.size() != b.size()) {
        return false;
    }
    return memcmp(a.active_data, b.active_data, a.size() * sizeof(digit_type)) == 0;
}

digit_type &storage::operator[](size_t ind) {
    detach();
    return *(active_data + ind);
}

digit_type const &storage::operator[](size_t ind) const {
    return *(active_data + ind);
}

digit_type const &storage::back() const {
    return (*this)[size() - 1];
}

bool storage::empty() const {
    return _size == 0;
}

size_t storage::size() const {
    return _size;
}

void storage::ensure_capacity(size_t n) {
    if (get_capacity() < n) {
        size_t new_capacity = n != 0 ? n * 2 : SMALL_OBJ_SIZE;
        if (_is_big || new_capacity > SMALL_OBJ_SIZE) {
            if (!is_big()) {
                to_big();
            }
            digit_type *temp = copy_data(active_data, size(), new_capacity);
//            data.big_data.storage_ptr = std::make_shared<digit_type> (*get_data());
            data.big_data.storage_ptr.reset(temp, storage::deleter());
            data.big_data.capacity = new_capacity;
            _is_big = true;
            active_data = data.big_data.storage_ptr.get();
        }
    }
}

void storage::to_big() {
    new(&data.big_data) big_obj(copy_data(active_data, size(), get_capacity()), size());
    active_data = data.big_data.storage_ptr.get();
    _is_big = true;
}

void storage::push_back(digit_type value) {
    ensure_capacity(size() + 1);
    (*this)[size()] = value;
    _size++;
}

void storage::pop_back() {
    detach();
    _size--;
}

void storage::swap(storage &a) noexcept {
    using std::swap;
    if (is_big() && a.is_big()) {
        swap(data.big_data, a.data.big_data);
        active_data = data.big_data.storage_ptr.get();
        a.active_data = a.data.big_data.storage_ptr.get();
    } else if (!is_big() && !a.is_big()) {
        for (size_t i = 0; i < SMALL_OBJ_SIZE; i++) {
            swap(data.small_data[i], a.data.small_data[i]);
        }
        active_data = data.small_data;
        a.active_data = a.data.small_data;
    } else if (!is_big() && a.is_big()) {
        swap_data(a.data, data);
        active_data = data.big_data.storage_ptr.get();
        a.active_data = a.data.small_data;
    } else {
        swap_data(data, a.data);
        active_data = data.small_data;
        a.active_data = a.data.big_data.storage_ptr.get();
    }
    swap(a._is_big, _is_big);
    swap(a._size, _size);
}

void storage::swap_data(container &big_container, container &small_container) {
    digit_type temp[SMALL_OBJ_SIZE] = {};

    for (size_t i = 0; i < SMALL_OBJ_SIZE; i++) {
        temp[i] = small_container.small_data[i];
    }
    new(&small_container.big_data) big_obj(big_container.big_data);
    big_container.big_data.~big_obj();
    for (size_t i = 0; i < SMALL_OBJ_SIZE; i++) {
        big_container.small_data[i] = temp[i];
    }
}

void storage::detach() {
    if (is_big() && !data.big_data.storage_ptr.unique()) {
        data.big_data.storage_ptr.reset(copy_data(active_data, size(), get_capacity()),
                                         storage::deleter());
        active_data = data.big_data.storage_ptr.get();
    }
}

bool storage::is_big() const {
    return _is_big;
}

size_t storage::get_capacity() {
    if (is_big()) {
        return data.big_data.capacity;
    }
    return SMALL_OBJ_SIZE;
}

digit_type *copy_data(digit_type *data, size_t size, size_t new_size) {
    auto *res = new digit_type[new_size * sizeof(digit_type)];
    memcpy(res, data, size * sizeof(digit_type));
    return res;

}