// Compilation: c++ gmp_int_profile.cpp -o gmp_int_profile -lgmp

#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <stdlib.h>
#include <bitset>
using namespace std;

#include "timer.h"

using  int128_t = __int128;
using uint128_t = unsigned __int128;

const int NUM_ELEMENTS = 50000;
const uint64_t TOTAL_ELEMENTS = 100000000;

#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)                                                                                                                                                
#ifndef __builtin_expect
#define __builtin_expect(x, expected_value) (x)
#endif
#endif

#ifndef LIKELY
#define LIKELY(x)   __builtin_expect((x),1)
#define UNLIKELY(x) __builtin_expect((x),0)
#endif

struct uint128_pod {
  uint64_t lo;
  uint64_t hi;
};

/*
int128_t atoint128_t(std::string const & in)
{
    int128_t res = 0;
    size_t i = 0;
    bool sign = false;

    if (in[i] == '-')
    {
        ++i;
        sign = true;
    }

    if (in[i] == '+')
    {
        ++i;
    }

    for (; i < in.size(); ++i)
    {
        const char c = in[i];
        if (not std::isdigit(c)) 
            throw std::runtime_error(std::string("Non-numeric character: ") + c);
        res *= 10;
        res += c - '0';
    }

    if (sign)
    {
        res *= -1;
    }

    return res;
}*/


// Generate random strings of ints with upto 38 digits
/*
void generate_random_numbers(vector<string>& vec) {

  char str[256];

  ::srand(42);
  int32_t r;
  int128_t r128;

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    r = ::rand();
    r128 = r * r * r * r;
    sprintf('      
    mpz_get_str(str, 10, num);
    vec.push_back(string(str));
  }

}*/
void computation(const vector<int128_t>& int_vec, char op)
{
  int128_t r = 1;
  switch(op)
  {
    case '+':
      for (int i = 0; i < NUM_ELEMENTS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
          r = int_vec[i] + int_vec[j];
        }
      }
      break;
    case '-':
      for (int i = 0; i < NUM_ELEMENTS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
          r = int_vec[i] - int_vec[j];
        }
      }
      break;
    case '*':
      for (int i = 0; i < NUM_ELEMENTS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
          r = int_vec[i] * int_vec[j];
        }
      }
      break;
    case '/':
      for (int i = 0; i < NUM_ELEMENTS; i++) {
        for (int j = 0; j < NUM_ELEMENTS; j++) {
          r = int_vec[i] / int_vec[j];
        }
      }
      break;
  }

  if (r == 1.5) {
    std::cout << "null" << std::endl;
  }

}


static inline int digits10(uint128_t x)
{
    if (x < 10ULL)
        return 1;
    if (x < 100ULL)
        return 2;
    if (x < 1000ULL)
        return 3;

    if (x < 1000000000000ULL)
    {
        if (x < 100000000ULL)
        {
            if (x < 1000000ULL)
            {
                if (x < 10000ULL)
                    return 4;
                else
                    return 5 + (x >= 100000ULL);
            }

            return 7 + (x >= 10000000ULL);
        }

        if (x < 10000000000ULL)
            return 9 + (x >= 1000000000ULL);

        return 11 + (x >= 100000000000ULL);
    }

    return 12 + digits10(x / 1000000000000ULL);
}

static const char digits[201] = "00010203040506070809"                                                                                                                                                          
                                "10111213141516171819"
                                "20212223242526272829"
                                "30313233343536373839"
                                "40414243444546474849"
                                "50515253545556575859"
                                "60616263646566676869"
                                "70717273747576777879"
                                "80818283848586878889"
                                "90919293949596979899";

static inline char * outTwoDigits(char * p, uint8_t value)
{
    memcpy(p, &digits[value * 2], 2);
    p += 2;
    return p;
}

static inline char * writeUIntText(uint128_t x, char * p)
{
    int len = digits10(x);
    auto pp = p + len;
    while (x >= 100)
    {
        const auto i = x % 100;
        x /= 100;
        pp -= 2;
        outTwoDigits(pp, i);
    }
    if (x < 10)
        *p = '0' + x;
    else
        outTwoDigits(p, x);
    return p + len;                                                                                                                                                                                             
}
static inline char * writeLeadingMinus(char * pos)
{
    *pos = '-';
    return pos + 1;
}

static inline char * writeSIntText(int128_t x, char * pos)
{
    static const int128_t min_int128 = int128_t(0x8000000000000000ll) << 64;

    if (UNLIKELY(x == min_int128))
    {
        memcpy(pos, "-170141183460469231731687303715884105728", 40);
        return pos + 40;
    }

    if (x < 0)
    {
        x = -x;
        pos = writeLeadingMinus(pos);
    }
    return writeUIntText(static_cast<uint128_t>(x), pos);
}

inline char * itoa(uint128_t i, char * p)
{
    return writeUIntText(i, p);
}

inline char * itoa(int128_t i, char * p)
{
    return writeSIntText(i, p);
}

inline void toString(uint128_t i, char *p)
{
  uint64_t div = 10000000000000000000ULL;
  size_t div_log = 19;
  uint128_pod *pod = reinterpret_cast<uint128_pod*>(&i);
  uint128_t high = i;
  uint128_t low;
  low = high % div;
  high /= div;
  uint128_t mid;
  mid = high % div;
  high /= div;

  uint128_pod *high_pod = reinterpret_cast<uint128_pod*>(&high);
  uint128_pod *mid_pod = reinterpret_cast<uint128_pod*>(&mid);
  uint128_pod *low_pod = reinterpret_cast<uint128_pod*>(&low);
  int printed_chars = 0;

  if (high_pod->lo != 0) {
    printed_chars = snprintf(p, div_log+1, "%ld", high_pod->lo);
    p += printed_chars; 
    printed_chars = snprintf(p, div_log+1, "%019ld", mid_pod->lo);
    p += printed_chars; 
  } else if (mid_pod->lo != 0) {
    printed_chars = snprintf(p, div_log+1, "%ld", mid_pod->lo);
    p += printed_chars; 
  }
  snprintf(p, div_log+1, "%019ld", low_pod->lo);

}

void to_string(const vector<int128_t>& int_vec) {

  char buf[256];

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    //itoa(int_vec[i], buf);
    toString(int_vec[i], buf);
    memset(buf, 0, 40);
  }

}

void native_int_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<int128_t> int_vec(TOTAL_ELEMENTS);
  int32_t r = rand();

  // This block contains extra output to compare the math operations results with
  // a different authoritative source.
  /*
  {
    char buf[256];
    int32_t s1 = 1804289383;
    int128_t v1 = (int128_t)s1*(int128_t)s1*(int128_t)s1*(int128_t)s1;
    itoa(v1, buf);
    std::cout << "int128 v1 " << buf << std::endl;
    memset(buf, 0, 256);
    toString(v1, buf);
    std::cout << "int128 v1 " << buf << std::endl;
    memset(buf, 0, 256);
    int32_t s2 = 1804289382;
    int128_t v2 = (int128_t)s2*(int128_t)s2*(int128_t)s2*(int128_t)s2;
    itoa(v2, buf);
    std::cout << "int128 v2 " << buf << std::endl;
    memset(buf, 0, 256);
    int128_t v3 = (int128_t)s2*(int128_t)s2*(int128_t)s2;
    int128_t sub = v1 - v2;
    int128_t div = v1 / v3;
    itoa(sub, buf);
    std::cout << "int128 sub " << buf << std::endl;
    memset(buf, 0, 256);
    itoa(div, buf);
    std::cout << "int128 div " << buf << std::endl;
    int64_t *r1 = (int64_t*)(&sub);
    int64_t *r2 = r1+1;
    int64_t *r3 = (int64_t*)(&div);
    int64_t *r4 = r3+1;
    std::cout << "r value " << r << std::endl;
    std::bitset<64> x(*r1);
    std::bitset<64> y(*r2);
    std::bitset<64> z(*r3);
    std::bitset<64> a(*r4);
    std::cout << "int128 sub left bin repr " << y << " int128 right value " << x << std::endl;
    uint128_pod *pod = reinterpret_cast<uint128_pod*>(&sub);
    std::cout << "int128 v1 hi dec " << pod->hi << " int128 lo dec " << pod->lo << std::endl;
    std::cout << "int128 div left bin repr " << a << " int128 right value " << z << std::endl;
  }
  */

  // Initialize the ints with the randomly generated strings
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    int128_t value = (int128_t)r*(int128_t)r*(int128_t)r*(int128_t)r;
    int_vec[i] = value;
    r = rand();
  }

  // Perform addition
  timer.start("Native Int Addition");
  computation(int_vec, '+');
  timer.stop("Native Int Addition");
    
  // Perform subtraction
  timer.start("Native Int Subtraction");
  computation(int_vec, '-');
  timer.stop("Native Int Subtraction");
    
  // Perform Multiplication
  timer.start("Native Int Multiplication");
  computation(int_vec, '*');
  timer.stop("Native Int Multiplication");
    
  // Perform Division
  timer.start("Native Int Division");
  computation(int_vec, '/');
  timer.stop("Native Int Division");

  // Perform string conversion
  timer.start("Native Int String conversion");
  to_string(int_vec);
  timer.stop("Native Int String conversion");


  // Clear the ints
  int_vec.clear();
}

int main(int argc, char *argv[]) {

  vector<string> string_vec;

  //random_numbers(string_vec);

  native_int_profiling(string_vec);

}
