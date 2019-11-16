// Compilation: c++ gmp_int_profile.cpp -o gmp_int_profile -lgmp

#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

#include <gmp.h>

#include "timer.h"

const int NUM_ELEMENTS = 50000;
const uint64_t TOTAL_ELEMENTS = 100000000;

// Generate random strings of ints with upto 38 digits
void generate_random_numbers(vector<string>& vec) {

  char str[256];

  gmp_randstate_t state;
  gmp_randinit_default(state);
  
  mpz_t num;
  mpz_init(num);

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    // Use uniform distribution
    mpz_urandomb(num, state, 126); // 2^126 - 1 is 38 digits
    mpz_get_str(str, 10, num);
    vec.push_back(string(str));
  }

  mpz_clear(num);
  gmp_randclear(state);
}

void computation(const vector<mpz_t>& int_vec,
                 void (*function)(mpz_t, const mpz_t, const mpz_t)) {

  mpz_t result;
  mpz_init(result);

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      function(result, int_vec[i], int_vec[j]);
    }
  }

  mpz_clear(result);
}

void to_string(const vector<mpz_t>& int_vec) {

  char buf[256];

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpz_get_str(buf, 10, int_vec[i]);
  }

}

void gmp_int_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<mpz_t> int_vec(TOTAL_ELEMENTS);

  // Initialize the ints with the randomly generated strings
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpz_init_set_str(int_vec[i], string_vec[i].c_str(), 10);
    assert(int_vec[i][0]._mp_size <= 2);
  }

  // Perform addition
  timer.start("GMP Int Addition");
  computation(int_vec, mpz_add);
  timer.stop("GMP Int Addition");
    
  // Perform subtraction
  timer.start("GMP Int Subtraction");
  computation(int_vec, mpz_sub);
  timer.stop("GMP Int Subtraction");
    
  // Perform Multiplication
  timer.start("GMP Int Multiplication");
  computation(int_vec, mpz_mul);
  timer.stop("GMP Int Multiplication");
    
  // Perform Division
  timer.start("GMP Int Division");
  computation(int_vec, mpz_tdiv_q);
  timer.stop("GMP Int Division");

  // Perform string conversion
  timer.start("GMP Int String conversion");
  to_string(int_vec);
  timer.stop("GMP Int String conversion");

  // Clear the ints
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpz_clear(int_vec[i]);
  }

}

int main(int argc, char *argv[]) {

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  gmp_int_profiling(string_vec);

}
