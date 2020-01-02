// Compilation: c++ int128_atoi_profile.cpp -o int128_atoi_profile -lgmp
/*
Run 1
Time            Seconds  Activity
14:44:12.481860          Start Naive Algorithm
14:44:12.481915 4.1008e- Stop  Naive Algorithm
14:44:12.481918          Start Reversed Double Dabble Algorithm
15:07:21.920116 1389.440 Stop  Reversed Double Dabble Algorithm

Run 2
Time            Seconds  Activity
16:06:51.888834          Start Naive Algorithm
16:06:51.888914 6.38962e Stop  Naive Algorithm
16:06:51.888918          Start Reversed Double Dabble Algorithm
16:27:51.688992 1259.800 Stop  Reversed Double Dabble Algorithm
*/

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

#include <gmp.h>

#include "timer.h"

const uint64_t TOTAL_ELEMENTS = 100000000;

using uint128_t = unsigned __int128;

struct uint128_pod
{
  uint64_t lo;
  uint64_t hi;
};

// Generate random strings of ints with upto 38 digits
void generate_random_numbers(vector<string>& vec) {

  char str[256];

  gmp_randstate_t state;
  gmp_randinit_default(state);
  
  mpz_t num;
  mpz_init(num);

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    // Use uniform distribution
    mpz_urandomb(num, state, 128); // 2^128 - 1 is 38 digits
    mpz_get_str(str, 10, num);
    vec.push_back(string(str));
  }

  mpz_clear(num);
  gmp_randclear(state);
}

// Naive Algorithm
void atoi_algo1(const vector<string>& strings) {
  uint128_t res;
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    res = 0;
    for (int j = 0; j < strings[i].size(); j++) {
      res = res*10 + strings[i][j] - '0';
    }
  }
}

// Reversed Double Dabble Algorithm
// Source: https://stackoverflow.com/questions/45608424/atoi-for-int128-t-type
// Reference: https://en.wikipedia.org/wiki/Double_dabble
void atoi_algo2(const vector<string>& strings) {
  uint128_t res;
  uint128_t mask;
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    int digits = strings[i].size();
    char *scratch = (char*) malloc(digits);
    for (int j = 0; j < digits; j++) {
      scratch[j] = strings[i][j] - '0';
    }
    int scanstart = 0;
    mask = 1;
    res = 0;
    while (scanstart < digits) {
      if (scratch[digits - 1] & 1)
        res |= mask;
      mask <<= 1;
      for (int j = digits - 1; j > scanstart; j--) {
        scratch[j] >>= 1;
        if (scratch[j-1] & 1)
          scratch[j] |= 8;
        if (scratch[j] > 7)
          scratch[j] -= 3;
      }
      scratch[scanstart] >>= 1;
      if (scratch[scanstart] > 7)
        scratch[scanstart] -= 3;
      while (scanstart < digits && !scratch[scanstart])
        scanstart++;
    }
    free(scratch);
  }
}

int main() {

  Timer timer;

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  // algo1
  timer.start("Naive Algorithm");
  atoi_algo1(string_vec);
  timer.stop("Naive Algorithm");
    
  // algo2
  timer.start("Reversed Double Dabble Algorithm");
  atoi_algo2(string_vec);
  timer.stop("Reversed Double Dabble Algorithm");

}
