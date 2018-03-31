#include "big_integer.h"

#include <string>
#include <stdexcept>
#include <functional>
#include <algorithm>

typedef unsigned int digit_type;
typedef unsigned long long long_digit_type;

const size_t BASE_LEN = 32;
const digit_type MX_DIGIT = UINT32_MAX;

using std::vector;

digit_type digit_cast(int x) {
    return static_cast<digit_type>(x & MX_DIGIT);
}

long_digit_type long_digit_cast(int x) {
    return static_cast<long_digit_type>(x & MX_DIGIT);
}

big_integer::big_integer() : sign(false), data(1, 0) {
}

big_integer::big_integer(big_integer const &other) : sign(other.sign), data(other.data) {
    normalize();
}

big_integer::big_integer(int a) : data(0) {
    sign = 0;
    if (a < 0) {
        sign = 1;
    }
    data.push_back(digit_cast(llabs(a)));
}

big_integer::big_integer(unsigned int a) : sign(0), data(1, a) {

}

big_integer::big_integer(bool sign, std::vector<unsigned int> const &d) {
    data = d;
    this->sign = sign;
    normalize();
}

big_integer::big_integer(std::string const &str) {
    sign = (str[0] == '-');
    size_t start_pos = 0;
    if (str[0] == '-' || str[0] == '+') {
        start_pos++;
    }

    big_integer res;
    for (size_t i = start_pos; i < str.length(); i++) {
        res *= 10;
        res += str[i] - '0';
    }

    if (sign) {
        res = -res;
    }

    swap(res);
}

big_integer::~big_integer() {
    data.clear();
    sign = 0;
}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer o(other);
    swap(o);
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    return *this = *this + rhs;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this = *this - rhs;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    return *this = *this * rhs;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return *this = *this & rhs;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return *this = *this | rhs;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return *this = *this ^ rhs;
}

big_integer &big_integer::operator<<=(int rhs) {
    return *this = *this << rhs;
}

big_integer &big_integer::operator>>=(int rhs) {
    return *this = *this >> rhs;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer res(*this);
    if (res.length() == 1 && res.get_digit(0) == 0)
        return res;
    res.sign ^= 1;
    return res;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer operator+(big_integer a, big_integer const &b) {
    if (a.is_negative() == b.is_negative()) {
        vector<digit_type> data = a.get_data();
        bool carry = 0;
        for (size_t i = 0; i < std::max(a.length(), b.length()) || carry; i++) {
            if (i == data.size()) {
                data.push_back(0);
            }
            long_digit_type cur = static_cast<long_digit_type > (b.get_digit(i)) + carry + data[i];
            data[i] = cur & MX_DIGIT;
            carry = cur > MX_DIGIT;
        }
        return big_integer(a.is_negative(), data);
    }
    return a - (-b);
}

big_integer operator-(big_integer a, big_integer const &b) {
    if (a.is_negative() == b.is_negative()) {
        if (abs(a) >= abs(b)) {
            vector<digit_type> data = a.get_data();
            bool carry = 0;
            for (size_t i = 0; i < b.length() || carry; i++) {
                long_digit_type cur = static_cast<long_digit_type > (b.get_digit(i)) + carry;
                carry = cur > data[i];
                data[i] -= (cur & MX_DIGIT);

            }
            return big_integer(a.is_negative(), data);
        }
        return -(b - a);
    }
    return a + (-b);
}

big_integer operator*(big_integer a, big_integer const &b) {
    vector<digit_type> data(a.length() + b.length());

    for (size_t i = 0; i < a.length(); i++) {
        digit_type carry = 0;
        for (size_t j = 0; carry || j < b.length(); j++) {
            long_digit_type cur = data[i + j] + long_digit_cast(a.get_digit(i)) * b.get_digit(j) + carry;
            data[i + j] = (cur & MX_DIGIT);
            carry = (cur >> BASE_LEN);
        }
    }

    return big_integer(a.is_negative() ^ b.is_negative(), data);
}

std::pair<big_integer, big_integer> big_integer::div_mod_operation(big_integer const &b) {
    if (b.length() == 1 && b.get_digit(0) == 0) {
        throw std::runtime_error("Divison by zero");
    }

    bool res_sign = (*this).is_negative() ^b.is_negative();
    big_integer abs_a(abs(*this));
    big_integer abs_b(abs(b));
    if (abs_a < abs_b) {
        return {0, *this};
    }

    size_t n = abs_a.length();
    size_t m = abs_b.length();

    if (m == 1) {
        return {big_integer(res_sign, ((*this) / b.get_digit(0)).get_data()), big_integer((*this).is_negative(), {abs_a % b.get_digit(0)})};
    }

    digit_type f = digit_cast(((long_digit_type) MX_DIGIT + 1) / ((long_digit_type) b.get_digit(b.length() - 1) + 1));

    big_integer r = abs_a * f;
    big_integer d = abs_b * f;

    vector<digit_type> ans(r.length() - d.length() + 1);

    big_integer h;
    long_digit_type d1 = d.get_digit(d.length() - 1);

    for (int k = r.length() - 1; k > static_cast<int>(r.length() - d.length()); --k) {
        h <<= BASE_LEN;
        h += r.get_digit(k);
    }
    for (size_t k = r.length() - d.length() + 1; k--;) {
        h <<= BASE_LEN;
        h += r.get_digit(k);

        long_digit_type r2 = h.get_digit(h.length() - 1);
        if (h.length() > d.length()) {
            r2 *= ((long_digit_type )MX_DIGIT + 1);
            r2 += h.get_digit(h.length() - 2);
        }
        long_digit_type qt = std::min(r2 / d1, (long_digit_type) MX_DIGIT);
        big_integer dq = d * (digit_type)(qt & MX_DIGIT);
        while (h < dq) {
            qt--;
            dq -= d;
        }
        ans[k] = qt & MX_DIGIT;
        h -= dq;
    }
    return {big_integer(res_sign, ans), h};
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    auto tmp = div_mod_operation(rhs).first;
    swap(tmp);
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    auto tmp = div_mod_operation(rhs).second;
    swap(tmp);
    return *this;
}

big_integer operator/(big_integer a, big_integer const &b) {
    a /= b;
    return a;
}

big_integer operator%(big_integer a, big_integer const &b) {
    a %= b;
    return a;
}

big_integer apply_two_complement(big_integer const &a, big_integer const &b,
                                 const std::function<digit_type(digit_type, digit_type)> &operation) {
    vector<digit_type> data_a = a.is_negative() ? get_inverted_data(a.get_data()) : a.get_data();
    vector<digit_type> data_b = b.is_negative() ? get_inverted_data(b.get_data()) : b.get_data();

    for (size_t i = 0; i < std::max(data_b.size(), data_a.size()); i++) {
        if (i == data_a.size()) {
            data_a.push_back(a.is_negative() ? MX_DIGIT : 0);
        }
        data_a[i] = (i < b.length() ? operation(data_a[i], data_b[i]) : 0);
    }
    bool res_sign = (data_a.back() >> (BASE_LEN - 1)) > 0;
    if (data_a.size() == 1 && data_a[0] == 0) {
        res_sign = 0;
    }
    return big_integer(res_sign, res_sign ? get_inverted_data(data_a) : data_a);
}

big_integer operator&(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [](digit_type x, digit_type y) -> digit_type {
        return x & y;
    });
}

big_integer operator|(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [](digit_type x, digit_type y) -> digit_type {
        return x | y;
    });
}

big_integer operator^(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [](digit_type x, digit_type y) -> digit_type {
        return x ^ y;
    });
}

big_integer operator<<(big_integer a, int b) {
    if (b == 0) {
        return a;
    }

    size_t rem = b % BASE_LEN;
    size_t cnt = b / BASE_LEN;
    size_t size = a.length() + cnt + 1;
    vector<digit_type> data(size);

    for (size_t i = cnt; i < size; i++) {
        data[i] = (a.get_digit(i - cnt) << rem);
        if (i != cnt) {
            data[i] |= static_cast<long_digit_type >(a.get_digit(i - cnt - 1)) >> (BASE_LEN - rem);
        }
    }
    return big_integer(a.is_negative(), data);

}

big_integer operator>>(big_integer a, int b) {
    if (b == 0) {
        return a;
    }

    size_t rem = b % BASE_LEN;
    size_t cnt_blocks = b / BASE_LEN;
    size_t size = cnt_blocks < a.length() ? a.length() - cnt_blocks : 0;
    vector<digit_type> data(size);

    for (size_t i = 0; i < size; i++) {
        data[i] = (static_cast<long_digit_type >(a.get_digit(i + cnt_blocks)) >> rem);
        data[i] |= (long_digit_cast(a.get_digit(i + cnt_blocks + 1)) << (BASE_LEN - rem));
    }
    return a.is_negative() ? ~big_integer(!a.is_negative(), data) : big_integer(a.is_negative(), data);
}

bool operator==(big_integer const &a, big_integer const &b) {
    return a.sign == b.sign && a.data == b.data;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign > b.sign;
    }

    if (a.length() != b.length()) {
        return a.length() < b.length();
    }

    for (size_t i = a.length(); i != 0; i--) {
        digit_type a_digit = a.get_digit(i - 1);
        digit_type b_digit = b.get_digit(i - 1);

        if (a_digit != b_digit) {
            return a_digit < b_digit;
        }
    }

    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(b < a);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

int operator%(big_integer a, int b) {
    digit_type carry = 0;

    for (size_t i = a.length(); i > 0; i--) {
        long_digit_type prev = (long_digit_cast(MX_DIGIT) + 1) * carry;
        long_digit_type cur = prev + a.get_digit(i - 1);

        carry = static_cast<digit_type>(cur % b);
    }
    return carry;
}

digit_type operator%(big_integer a, digit_type b) {
    digit_type carry = 0;

    for (size_t i = a.length(); i > 0; i--) {
        long_digit_type prev = (long_digit_cast(MX_DIGIT) + 1) * carry;
        long_digit_type cur = prev + a.get_digit(i - 1);

        carry = static_cast<digit_type>(cur % b);
    }
    return carry;
}

big_integer operator/(big_integer a, digit_type divisor) {
    size_t size = a.length();
    vector<digit_type> tmp(size);
    long_digit_type carry = 0;

    for (size_t ii = size; ii != 0; ii--) {
        size_t i = ii - 1;
        long_digit_type prev = (long_digit_cast(MX_DIGIT) + 1) * carry;
        long_digit_type cur = prev + a.get_digit(i);
        tmp[i] = static_cast<digit_type>(cur / divisor);
        carry = cur % divisor;
    }
    return big_integer(a.is_negative(), tmp);
}

big_integer operator/(big_integer a, int b) {
    if (b < 0) {
        return -a / digit_cast(llabs(b));
    }
    return a / digit_cast(b);
}

std::string to_string(big_integer const &a) {
    std::string result;
    big_integer abs_big = a;

    while (abs_big.length() != 1 || (abs_big.length() == 1 && abs_big.get_digit(0) != 0)) {
        digit_type t = abs_big % 10;
        abs_big = abs_big / 10;
        result.push_back('0' + t);
        abs_big.normalize();
    }

    while (!result.empty() && result.back() == '0') {
        result.pop_back();
    }

    if (result.empty()) {
        result.push_back('0');
    }

    if (a.is_negative()) {
        result.push_back('-');
    }
    reverse(result.begin(), result.end());
    return result;
}

size_t big_integer::length() const {
    return data.size();
}


std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

void big_integer::normalize() {
    while (!data.empty() && data.back() == 0) {
        data.pop_back();
    }
    if (data.empty()) {
        data.push_back(0);
        sign = 0;
    }
}

void big_integer::swap(big_integer &b) {
    std::swap(sign, b.sign);
    std::swap(data, b.data);
}

big_integer abs(big_integer const &b) {
    if (b.is_negative()) {
        return -b;
    }
    return b;
}

bool big_integer::is_negative() const {
    return sign;
}
unsigned int big_integer::get_digit(size_t pos) const {
    if (pos < data.size()) {
        return data[pos];
    } else {
        return 0;
    }
}
vector<digit_type> big_integer::get_data() const {
    return data;
}

std::vector<digit_type> get_inverted_data(std::vector<digit_type> data) {
    for (auto &x : data) {
        x = ~x;
    }
    return (big_integer(0, data) + 1).get_data();
}
