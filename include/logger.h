//
// Created by lwskng on 9/2/22.
//

#pragma once

#include "timestamp.h"

#include <chrono>
#include <ostream>
#include <mutex>
#include <syncstream>

#ifndef SIMPLELOGGET_LOGGER_H
#define SIMPLELOGGET_LOGGER_H

namespace dev_tools {

    using namespace std::string_view_literals;

    template <typename Duration = Milliseconds>
    class Logger {
    public:
        explicit Logger(std::string id, std::ostream& os)
                : id_(id)
                , os_(os)
        {}

        template <typename... Args>
        void LogMessage(Args... args) const {
            LogFunc([... args = std::forward<Args>(args)](std::ostream &os) {
                ((os << args << " "), ...);
            });
        }

        template<typename Fn, typename... Args>
        void LogFunc(Fn &&fn, Args... args) const {
            Timestamp<Duration> timestamp;
            std::osyncstream sync_os_(os_);
            sync_os_ << id_ << "> "sv << timestamp << ": "sv;
            fn(sync_os_, args...);
            sync_os_ << std::endl;
        }
    private:
        std::string id_;
        std::ostream& os_;
    };

}//!namespace

#endif //SIMPLELOGGET_LOGGER_H
