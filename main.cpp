#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>

#include "include/logger.h"

using namespace std::string_literals;

auto some_func = [](std::ostream &os, int a) {
    auto res = a * a;
    os << res;
    os << " some internal results of "s;
    os << std::string(__PRETTY_FUNCTION__ );
    os << " work "s;
    os << (25.12-a);
    return res;
};

int main () {

    dev_tools::Logger logger("v 0.1, side branch"s, std::clog);
    logger.LogMessage("Logger created"s);

    const int thread_size = 100;
    logger.LogMessage("Making a vector of threads with size of"s, thread_size);
    std::vector<std::jthread> v(thread_size);

    for (int i = 0; i != thread_size/2; ++i) {
        v[i] = std::jthread([i, &logger](){
            unsigned int delay = rand()%100+1;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            logger.LogMessage("thread id is: "s, i, "some record"s, delay);
        });
    }

    for (int i = thread_size/2; i != thread_size; ++i) {
        v[i] = std::jthread([i, &logger](){
            unsigned int delay = rand()%100+1;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            logger.LogFunc(some_func, (std::cout, delay));
        });
    }

    return 0;
}