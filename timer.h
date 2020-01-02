#ifndef H_TIMER
#define H_TIMER

#include <sys/time.h>
#include <sstream>
#include <iomanip>

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
        std::cout << timestr() << "          Start " << message << std::endl;
    }

    void stop(const string& message)
    {
        time_t now;
        time(&now);
        string secondsElapsed;
        getTimeElapsed(secondsElapsed);
        std::cout << timestr() << " " << secondsElapsed << " Stop  " << message << std::endl;
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
        std::ostringstream oss;
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

        std::ostringstream oss;
        oss << std::setfill('0')
            << setw(2) << tm.tm_hour << ':'
            << setw(2) << tm.tm_min << ':'
            << setw(2) << tm.tm_sec << '.'
            << setw(6) << tv.tv_usec
            ;
        return oss.str();
    }

    void header()
    {
        std::cout << std::endl;
        std::cout << "Time            Seconds  Activity" << std::endl;
    }
};
#endif
