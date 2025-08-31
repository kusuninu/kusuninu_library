#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
struct Timer {
  private:
    std::chrono::system_clock::time_point begin;
    std::chrono::system_clock::time_point now;
    std::chrono::system_clock::time_point temp;

  public:
    Timer() {
        begin = std::chrono::system_clock::now();
        now = begin;
    }
    double get_ms() {
        now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count();
    }

    double get_lap() {
        temp = std::chrono::system_clock::now();
        auto ans = temp - now;
        now = temp;
        return std::chrono::duration_cast<std::chrono::milliseconds>(ans).count();
    }
};
}// namespace kusu_lib