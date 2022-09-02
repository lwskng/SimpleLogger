#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>

#include "include/logger.h"

using namespace std::string_literals;

//dummy func 1
auto some_func = [](std::ostream &os, size_t thread_id, int a) {
    auto res = a * a;
    os << "thread id is: " << thread_id;
    os << " some internal results of "s;
    os << std::string(__PRETTY_FUNCTION__ );
    os << " work "s;
    os << 42.42-res;
    return res;
};

//dummy func 2
auto another_func = [](std::ostream &os, size_t thread_id, int a, double b) {
    auto res = a * b;
    os << "thread id is: " << thread_id;
    os << ", important msg from "s;
    os << std::string(__PRETTY_FUNCTION__ );
    os << ", result is: "s;
    os << res;
    return res;
};


int main () {

    //make instance
    dev_tools::Logger logger("v 0.1, side branch"s, std::cout, true);
    logger.LogMessage("Logger created"s);

    //make threads
    const int thread_size = 100;
    logger.LogMessage("Making a vector of threads with size of"s, thread_size);
    std::vector<std::jthread> v(thread_size);

    //lambda to generate a prn
    const auto make_argument = []() {
        unsigned int delay = rand()%100+1;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        return delay;
    };

    //log some arbitrary messages
    for (int t_id = 0; t_id != thread_size / 2; ++t_id) {
        v[t_id] = std::jthread([t_id, &logger, &make_argument](){
            auto delay = make_argument();
            logger.LogMessage("thread id is:"s, t_id, "some record"s, delay);
        });
    }

    //log dummy func 1
    for (int t_id = thread_size / 2; t_id != thread_size * 3 / 4; ++t_id) {
        v[t_id] = std::jthread([t_id, &logger, &make_argument](){
            auto arg = make_argument();
            logger.LogFunc(some_func, t_id, arg);
        });
    }

    //log dummy func 2
    for (int t_id = thread_size * 3 / 4; t_id != thread_size; ++t_id) {
        v[t_id] = std::jthread([t_id, &logger, &make_argument](){
            auto arg = make_argument();
            logger.LogFunc(another_func, t_id, arg, 42.42);
        });
    }


    return 0;
}
