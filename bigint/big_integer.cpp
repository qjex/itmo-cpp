#include "big_integer.h"

#include <string>
#include <stdexcept>
#include <functional>

typedef unsigned int digit_type;
typedef unsigned long long long_digit_type;

const size_t BASE_LEN = 32;
const digit_type MX_DIGIT = UINT32_MAX;


using std::vector;

digit_type digit_cast(int x) {
    return static_cast<digit_type>(x);
}

big_integer::big_integer() : sign(false), data(1, 0){
}

big_integer::big_integer(int a) : data(0) {
    sign = 0;
    if (a < 0) {
        sign = 1;
    }
    data.push_back(digit_cast(llabs(a)));
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

big_integer &big_integer::operator/=(big_integer const &rhs) {
    return *this = *this / rhs;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return *this = *this % rhs;
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
            long_digit_type cur = data[i + j] + static_cast<long_digit_type>(a.get_digit(i)) * b.get_digit(j) + carry;
            data[i + j] = (cur & MX_DIGIT);
            carry = (cur >> BASE_LEN);
        }
    }

    return big_integer(a.is_negative() ^ b.is_negative(), data);
}

big_integer operator/(big_integer a, big_integer const &b) {
    return 1;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a - (a / b) * b;
}

big_integer apply_two_complement(big_integer const& a, big_integer const& b,
                                 const std::function<digit_type(digit_type, digit_type)> &operation) {
    vector<digit_type> data_a = a.is_negative() ? get_inverted_data(a.get_data()) : a.get_data();
    vector<digit_type> data_b = b.is_negative() ? get_inverted_data(b.get_data()) : b.get_data();

    for (size_t i = 0; i < data_b.size(); i++) {
        if (i == data_a.size()) {
            data_a.push_back(a.is_negative() ? MX_DIGIT : 0);
        }
        data_a[i] = operation(data_a[i], data_b[i]);
    }
    bool res_sign = (data_a.back() >> (BASE_LEN - 1)) > 0;
    if (data_a.size() == 1 && data_a[0] == 0) {
        res_sign = 0;
    }
    return big_integer(res_sign, res_sign ? get_inverted_data(data_a) : data_a);
}

big_integer operator&(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [] (digit_type x, digit_type y) -> digit_type {
        return x & y;
    });}

big_integer operator|(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [] (digit_type x, digit_type y) -> digit_type {
        return x | y;
    });
}

big_integer operator^(big_integer a, big_integer const &b) {
    return apply_two_complement(a, b, [] (digit_type x, digit_type y) -> digit_type {
        return x ^ y;
    });}

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
        data[i] |= (static_cast<long_digit_type>(a.get_digit(i + cnt_blocks + 1)) << (BASE_LEN - rem));
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

//TODO
digit_type operator%(big_integer const &first, int second) {
    big_integer abs_big(abs(first));
    size_t size = first.length();
    digit_type carry = 0;

    for (size_t i = size; i != 0; i--) {
        long_digit_type cur = abs_big.get_digit(i - 1) + carry * MX_DIGIT;

        carry = static_cast<digit_type>(cur % second);
    }
    return carry;
}

//TODO
template<typename E>
big_integer div_big_small(big_integer const &first, E second) {
    big_integer absFirst(abs(first));
    bool isNeg = first.is_negative() ^(second < 0 ? 1 : 0);
    long_digit_type div = abs(static_cast<long long>(second));
    size_t size = first.length();
    vector<digit_type> temp(size);
    long_digit_type carry = 0;
    for (size_t ii = size; ii != 0; ii--) {
        size_t i = ii - 1;
        long_digit_type cur = absFirst.get_digit(i) + carry * (1ll * MX_DIGIT + 1);
        temp[i] = static_cast<digit_type>(cur / div);
        carry = cur % div;
    }
    big_integer T(isNeg, temp);
    return T;
}

big_integer operator/(big_integer const &first, int second) {
    return div_big_small<int>(first, second);
}

big_integer operator/(big_integer const &first, digit_type second) {
    return div_big_small<digit_type>(first, second);
}

std::string to_string(big_integer const &a) {
    std::string result;
//    big_integer abs_big = abs(a);
//
//    while (abs_big.length() > 0) {
//        digit_type t = abs_big % BASE;
//        abs_big = abs_big / BASE;
//
//        for (int i = 0; i < BASE_LEN; i++) {
//            result.push_back('0' + t % 10);
//            t /= 10;
//        }
//        abs_big.normalize();
//    }
//
//    while (!result.empty() && result.back() == '0') {
//        result.pop_back();
//    }
//
//    if (result.empty()) {
//        result.push_back('0');
//    }
//
//    if (a.is_negative()) {
//        result.push_back('-');
//    }
//    reverse(result.begin(), result.end());
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
vector<digit_type > big_integer::get_data() const {
    return data;
}
std::vector<digit_type> get_inverted_data(std::vector<digit_type> data) {
    for (auto &x : data) {
        x = ~x;
    }
    return (big_integer(0, data) + 1).get_data();
}
