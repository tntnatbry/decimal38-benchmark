// Compilation: c++ mpfr_profile.cpp -o mpfr_profile -lmpfr -lgmp

#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

#include <mpfr.h>

#include "timer.h"

const int NUM_ELEMENTS = 50000;
const int SCALE = 8;
const uint64_t TOTAL_ELEMENTS = 100000000;

// Generate random strings of floats with
// max 30 digits before decimal point and 8 after
void generate_random_numbers(vector<string>& vec) {

  char str[256];
  string str2;

  gmp_randstate_t state;
  gmp_randinit_default(state);
  
  mpz_t num;
  mpz_init(num);

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    // Use uniform distribution
    mpz_urandomb(num, state, 99); // 2^99 - 1 is 30 digits
    mpz_get_str(str, 10, num);
    str2 = string(str);
    str2.append(".");
    mpz_urandomb(num, state, 26); // 2^26 - 1 is 8 digits
    mpz_get_str(str, 10, num);
    str2.append(string(str));
    // Ensure there are 8 digits after the decimal
    if (strlen(str) < SCALE)
      str2.append((SCALE - strlen(str)), '0');
    vec.push_back(str2);
  }

  mpz_clear(num);
  gmp_randclear(state);
}

void computation(const vector<mpfr_t>& float_vec,
                 int (*function)(mpfr_t, const mpfr_t, const mpfr_t, mpfr_rnd_t)) {

  mpfr_t result;
  mpfr_init2(result, 192);

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      function(result, float_vec[i], float_vec[j], MPFR_RNDN);
    }
  }

  mpfr_clear(result);
}

void to_string(const vector<mpfr_t>& float_vec) {

  char buf[256];

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpfr_sprintf(buf, "%.*Rf", SCALE, float_vec[i]);
  }

}

void mpfr_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<mpfr_t> float_vec(TOTAL_ELEMENTS);

  // Initialize the floats with the randomly generated strings
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpfr_init2(float_vec[i], 192); // Set the precision to 192 bits
    mpfr_set_str(float_vec[i], string_vec[i].c_str(), 10, MPFR_RNDN);
  }

  // Perform addition
  timer.start("MPFR Addition");
  computation(float_vec, mpfr_add);
  timer.stop("MPFR Addition");
    
  // Perform subtraction
  timer.start("MPFR Subtraction");
  computation(float_vec, mpfr_sub);
  timer.stop("MPFR Subtraction");
    
  // Perform Multiplication
  timer.start("MPFR Multiplication");
  computation(float_vec, mpfr_mul);
  timer.stop("MPFR Multiplication");
    
  // Perform Division
  timer.start("MPFR Division");
  computation(float_vec, mpfr_div);
  timer.stop("MPFR Division");

  // Perform string conversion
  timer.start("MPFR String conversion");
  to_string(float_vec);
  timer.stop("MPFR String conversion");

  // Clear the floats
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    mpfr_clear(float_vec[i]);
  }

}

int main(int argc, char *argv[]) {

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  mpfr_profiling(string_vec);

}
