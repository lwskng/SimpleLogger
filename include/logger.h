//
// Created by lwskng on 9/2/22.
//

#pragma once

#include "timestamp.h"
#include "colors.h"

#include <chrono>
#include <ostream>
#include <syncstream>

#ifndef SIMPLELOGGER_LOGGER_H
#define SIMPLELOGGER_LOGGER_H

namespace dev_tools {

    using namespace std::string_view_literals;

    template <typename Duration = Milliseconds>
    class Logger {
    public:
        explicit Logger(std::string id, std::ostream& os, bool use_colors = false)
                : id_(id)
                , os_(os)
                , use_colors_ (use_colors)
        {}

        template <typename... Args>
        void LogMessage(Args... args) const {
            LogFunc([... args = std::forward<Args>(args)](std::ostream &os) {
                ((os << args << " "), ...);
            });
        }

        template<typename Fn, typename... Args>
        void LogFunc(Fn fn, Args... args) const {
            Timestamp<Duration> timestamp;
            std::osyncstream sync_os_(os_);
            if (use_colors_) {
                sync_os_ << FGRN(id_) << "> "sv << BOLD(FYEL(timestamp)) << ": "sv;
            } else {
                sync_os_ << id_ << "> "sv << timestamp << ": "sv;
            }
            fn(sync_os_, std::forward<Args>(args)...);
            sync_os_ << std::endl;
        }
    private:
        std::string id_;
        std::ostream& os_;
        bool use_colors_;
    };

}//!namespace

#endif //SIMPLELOGGER_LOGGER_H
