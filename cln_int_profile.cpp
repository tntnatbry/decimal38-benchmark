// Compilation: c++ cln_int_profile.cpp -o cln_int_profile -lcln -lgmp
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

#include <gmp.h>

#include <cln/integer.h>
#include <cln/integer_io.h>
using namespace cln;

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

void cln_addition(const vector<cl_I>& int_vec) {

  cl_I result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = int_vec[i] + int_vec[j];
    }
  }

}

void cln_subtraction(const vector<cl_I>& int_vec) {

  cl_I result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = int_vec[i] - int_vec[j];
    }
  }

}

void cln_multiplication(const vector<cl_I>& int_vec) {

  cl_I result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = int_vec[i] * int_vec[j];
    }
  }

}

void cln_division(const vector<cl_I>& int_vec) {

  cl_I result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      try {
        result = exquo(int_vec[i], int_vec[j]);
      }
      catch (exquo_exception e) {
        // suppress the exception
      }
    }
  }

}

void cln_to_string(const vector<cl_I>& int_vec) {

  for (int i = 0; i < TOTAL_ELEMENTS; i++) {
    ostringstream ss;
    ss << int_vec[i];
  }

}

void cln_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<cl_I> int_vec(TOTAL_ELEMENTS);

  for (int i = 0; i < TOTAL_ELEMENTS; i++) {
    int_vec[i] = string_vec[i].c_str();
  }

  // Perform Addition
  timer.start("CLN Int Addition");
  cln_addition(int_vec);
  timer.stop("CLN Int Addition");

  // Perform Subtraction
  timer.start("CLN Int Subtraction");
  cln_subtraction(int_vec);
  timer.stop("CLN Int Subtraction");

  // Perform Multiplication
  timer.start("CLN Int Multiplication");
  cln_multiplication(int_vec);
  timer.stop("CLN Int Multiplication");

  // Perform Division
  //timer.start("CLN Int Division");
  //cln_division(int_vec);
  //timer.stop("CLN Int Division");

  // Perform String Conversion
  timer.start("CLN Int String Conversion");
  cln_to_string(int_vec);
  timer.stop("CLN Int String Conversion");

}

int main(int argc, char *argv[]) {

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  cln_profiling(string_vec);

}
