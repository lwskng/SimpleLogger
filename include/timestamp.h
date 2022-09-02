//
// Created by lwskng on 9/2/22.
//

#pragma once

#include "date.h"

#include <ctime>
#include <chrono>
#include <ratio>
#include <cstdint>
#include <iosfwd>

#ifndef SIMPLELOGGER_TIMESTAMP_H
#define SIMPLELOGGER_TIMESTAMP_H

namespace dev_tools {

        //todo: add is_convertible to duration

        using Clock = std::chrono::system_clock;
        using HiResClock = std::chrono::high_resolution_clock;

        using Microseconds = std::chrono::microseconds;
        using Milliseconds = std::chrono::milliseconds;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using FiveMinutes = std::chrono::duration<long, std::ratio<300>>;
        using ThirtySeconds = std::chrono::duration<long, std::ratio<30>>;


        //todo: add SFINAE / concept to check Duration is a time constructable


        template <typename Duration>
        constexpr bool IsMicroseconds (Duration) {
            return std::is_same_v<Duration, Microseconds>;
        }
        template <typename Duration>
        constexpr bool IsMilliseconds (Duration) {
            return std::is_same_v<Duration, Milliseconds>;
        }
        template <typename Duration>
        constexpr bool IsSeconds (Duration) {
            return std::is_same_v<Duration, Seconds>;
        }
        template <typename Duration>
        constexpr bool IsMinutes (Duration) {
            return std::is_same_v<Duration, Minutes>;
        }


        template<class Duration>
        using TimePoint = std::chrono::time_point<Clock, Duration>;

        template<class Duration>
        struct Timestamp {
            Timestamp() : time_point(std::chrono::floor<Duration>(Clock::now()))
            {}

            TimePoint<Duration> time_point;

            std::string toString () const {
                std::string output;
                output.reserve(24);
                output += date::format("%F", time_point);
                output += ' ';
                output += date::format("%T", time_point);
                return output;
            }
        };

        template <typename Duration>
        struct TimestampHasher {
            size_t operator () (const Timestamp<Duration>& t) const {
                return t.time_point.time_since_epoch().count();
            }
        };

        template<class Duration>
        bool operator==(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs) {
            return lhs.time_point == rhs.time_point;
        }
        template<class Duration>
        bool operator!=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
            return not (rhs==lhs);
        }
        template<class Duration>
        bool operator < (const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
            return lhs.time_point < rhs.time_point;
        }
        template<class Duration>
        bool operator >(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
            return not (rhs < lhs);
        }
        template<class Duration>
        bool operator<=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
            return not (rhs > lhs);
        }
        template<class Duration>
        bool operator>=(const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs) {
            return not (lhs<rhs);
        }

        template<class Duration>
        std::ostream& operator<<(std::ostream& os, const Timestamp<Duration>& timestamp){
            os << timestamp.toString();
            return os;
        }

        template<class Duration>
        Timestamp<Duration> fromString(std::string input, const std::string& input_fmt){
            Timestamp<Duration> timestamp;
            std::stringstream ss(std::move(input));
            date::from_stream(ss, input_fmt.c_str(), timestamp.time_point);
            return timestamp;
        }

        template<class Duration>
        Timestamp<Duration> operator - (const Timestamp<Duration>& lhs, const Timestamp<Duration>& rhs){
            Timestamp<Duration> result;
            result.time_point -= std::chrono::duration_cast<Duration>(lhs.time_point - rhs.time_point);
            return result;
        }

        //todo: WIP, unfinished
        template<class Duration>
        Timestamp<Duration> operator + ([[maybe_unused]] const Timestamp<Duration>& lhs, [[maybe_unused]] const Timestamp<Duration>& rhs){
            Timestamp<Duration> result;
//		result.time_point += std::chrono::duration_cast<Duration>(lhs.time_point + rhs.time_point);
            return result;
        }

}//!namespace

#endif //SIMPLELOGGER_TIMESTAMP_H
