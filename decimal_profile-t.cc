#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
using namespace std;

#include <gmp.h>

#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>
#include <sql_string.h>
#include <my_decimal.h>

const int NUM_ELEMENTS = 50000;
const int SCALE = 8;
const uint64_t TOTAL_ELEMENTS = 100000000;

// Timer class used by this tdriver to output elapsed times, etc.
class Timer
{
public:
    void start(const string& message)
    {
        if (!fHeaderDisplayed)
        {
            header();
            fHeaderDisplayed = true;
        }

        gettimeofday(&fTvStart, 0);
        cout << timestr() << "          Start " << message << endl;
    }

    void stop(const string& message)
    {
        time_t now;
        time(&now);
        string secondsElapsed;
        getTimeElapsed(secondsElapsed);
        cout << timestr() << " " << secondsElapsed << " Stop  " << message << endl;
    }

    Timer() : fHeaderDisplayed(false) {}

private:

    struct timeval fTvStart;
    bool fHeaderDisplayed;

    double getTimeElapsed(string& seconds)
    {
        struct timeval tvStop;
        gettimeofday(&tvStop, 0);
        double secondsElapsed =
            (tvStop.tv_sec + (tvStop.tv_usec / 1000000.0)) -
            (fTvStart.tv_sec + (fTvStart.tv_usec / 1000000.0));
        ostringstream oss;
        oss << secondsElapsed;
        seconds = oss.str();
        seconds.resize(8, '0');
        return secondsElapsed;
    }

    string timestr()
    {
        struct tm tm;
        struct timeval tv;

        gettimeofday(&tv, 0);
        localtime_r(&tv.tv_sec, &tm);

        ostringstream oss;
        oss << setfill('0')
            << setw(2) << tm.tm_hour << ':'
            << setw(2) << tm.tm_min << ':'
            << setw(2) << tm.tm_sec	<< '.'
            << setw(6) << tv.tv_usec
            ;
        return oss.str();
    }

    void header()
    {
        cout << endl;
        cout << "Time            Seconds  Activity" << endl;
    }
};

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

void mdb_addition(const vector<my_decimal>& dec_vec) {

  my_decimal result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      my_decimal_add(0, &result, &dec_vec[i], &dec_vec[j]);
    }
  }

}

void mdb_subtraction(const vector<my_decimal>& dec_vec) {

  my_decimal result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      my_decimal_sub(0, &result, &dec_vec[i], &dec_vec[j]);
    }
  }

}

void mdb_multiplication(const vector<my_decimal>& dec_vec) {

  my_decimal result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      my_decimal_mul(0, &result, &dec_vec[i], &dec_vec[j]);
    }
  }

}

void mdb_division(const vector<my_decimal>& dec_vec) {

  my_decimal result;

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    for (int j = 0; j < NUM_ELEMENTS; j++) {
      my_decimal_div(0, &result, &dec_vec[i], &dec_vec[j], 0);
    }
  }

}

void mdb_to_string(const vector<my_decimal>& dec_vec) {

  String out_str;

  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    dec_vec[i].to_string_native(&out_str, 0, 0, 'X', 0);
  }

}

void mdb_profiling(const vector<string>& string_vec) {

  Timer timer;

  vector<my_decimal> dec_vec(TOTAL_ELEMENTS);

  const char *str;
  char *str_end;
  for (uint64_t i = 0; i < TOTAL_ELEMENTS; i++) {
    str = string_vec[i].c_str();
    str_end = (char *)(str + string_vec[i].size());
    str2my_decimal(0, str, &dec_vec[i], &str_end);
  }

  // Perform Addition
  timer.start("MDB Addition");
  mdb_addition(dec_vec);
  timer.stop("MDB Addition");

  // Perform Subtraction
  timer.start("MDB Subtraction");
  mdb_subtraction(dec_vec);
  timer.stop("MDB Subtraction");

  // Perform Multiplication
  timer.start("MDB Multiplication");
  mdb_multiplication(dec_vec);
  timer.stop("MDB Multiplication");

  // Perform Division
  timer.start("MDB Division");
  mdb_division(dec_vec);
  timer.stop("MDB Division");

  // Perform String Conversion
  timer.start("MDB String Conversion");
  mdb_to_string(dec_vec);
  timer.stop("MDB String Conversion");

}

int main (int argc, char *argv[]) {

  vector<string> string_vec;

  generate_random_numbers(string_vec);

  mdb_profiling(string_vec);

}
