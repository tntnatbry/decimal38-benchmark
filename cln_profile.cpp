// complilation: c++ cln_profile.cpp -o cln_profile -lcln -lgmp

#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

#include <gmp.h>

#include <cln/float.h>
#include <cln/lfloat_io.h>
using namespace cln;

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

void prepare_cln_strings(vector<string>& string_vec) {

  for (string& s : string_vec) {
    // _<num> gaurantees atleast num digits in
    // the mantissa (after the decimal point)
    s.append("_38");
  }

}

void cln_addition(const vector<cl_LF>& float_vec) {

  cl_LF result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = float_vec[i] + float_vec[j];
    }
  }

}

void cln_subtraction(const vector<cl_LF>& float_vec) {

  cl_LF result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = float_vec[i] - float_vec[j];
    }
  }

}

void cln_multiplication(const vector<cl_LF>& float_vec) {

  cl_LF result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = float_vec[i] * float_vec[j];
    }
  }

}

void cln_division(const vector<cl_LF>& float_vec) {

  cl_LF result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      result = float_vec[i] / float_vec[j];
    }
  }

}

void cln_to_string(const vector<cl_LF>& float_vec) {

  for (int i = 0; i < TOTAL_ELEMENTS; i++) {
    ostringstream ss;
    ss << float_vec[i];
  }

}

void cln_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<cl_LF> float_vec(TOTAL_ELEMENTS);

  for (int i = 0; i < TOTAL_ELEMENTS; i++) {
    float_vec[i] = string_vec[i].c_str();
  }

  // Perform Multiplication
  timer.start("CLN Addition");
  cln_addition(float_vec);
  timer.stop("CLN Addition");

  // Perform Subtraction
  timer.start("CLN Subtraction");
  cln_subtraction(float_vec);
  timer.stop("CLN Subtraction");

  // Perform Multiplication
  timer.start("CLN Multiplication");
  cln_multiplication(float_vec);
  timer.stop("CLN Multiplication");

  // Perform Division
  timer.start("CLN Division");
  cln_division(float_vec);
  timer.stop("CLN Division");

  // Perform String Conversion
  timer.start("CLN String Conversion");
  cln_to_string(float_vec);
  timer.stop("CLN String Conversion");

}

int main(int argc, char *argv[]) {

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  // Prepare data for CLN floats
  prepare_cln_strings(string_vec);

  cln_profiling(string_vec);

}
