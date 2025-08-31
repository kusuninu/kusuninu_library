#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long> struct ConvexHullTrick {
  private:
    struct Line {
        T first;
        T second;
        mutable std::function<const Line*()> next_line;
        bool is_query;
        Line(T a, T b) : first(a), second(b), is_query(false) {}
        Line(T a, T b, bool is_query) : first(a), second(b), is_query(is_query) {}
        bool operator<(const Line& rhs) const {
            if(is_query) {
                const Line* nex = rhs.next_line();
                if(nex == nullptr) return false;
                return (rhs.first - nex->first) * first + rhs.second - nex->second < 0;
            }
            if(rhs.is_query) {
                const Line* nex = next_line();
                if(nex == nullptr) return false;
                return (first - nex->first) * rhs.first + second - nex->second < 0;
            }
            return first < rhs.first;
        }
    };

    bool isMin;
    bool lineMonotone;
    bool queryMonotone;
    bool smallEnough;
    std::deque<Line> deq;
    std::set<Line> st;
    std::function<bool(const Line&, const T&)> comp = [](const Line& line, const T& x) {
        auto&& itr = line.next_line();
        if(itr == nullptr) return false;
        return (line.first - itr->first) * x + line.second - itr->second < 0;
    };
    int query_count = 0;
    int query_dir = 0;//0 -> unknown, 1 -> increase, 2 -> decrease
    T last_query = T(0);
    bool line_exist = false;

    bool is_useless(const Line& pre, const Line& p, const Line& nex) {
        if(smallEnough) {
            if(p.first == pre.first) return p.second <= pre.second;
            if(p.first == nex.first) return p.second <= nex.second;
            return (p.second - pre.second) * (nex.first - p.first) <=
                   (nex.second - p.second) * (p.first - pre.first);
        } else {
            if(p.first == pre.first) return p.second <= pre.second;
            if(p.first == nex.first) return p.second <= nex.second;
            return (long double)(p.second - pre.second) / (long double)(p.first - pre.first) <=
                   (long double)(nex.second - p.second) / (long double)(nex.first - p.first);
        }
    }

    T calc(Line line, T x) { return line.first * x + line.second; }

  public:
    //T is integer & |a*b|<2^63 => smallEnough
    ConvexHullTrick(bool isMin, bool lineMonotone = false, bool queryMonotone = false,
                    bool smallEnough = false)
        : isMin(isMin), lineMonotone(lineMonotone), queryMonotone(queryMonotone),
          smallEnough(smallEnough) {}

    void add_line(T a, T b) {
        line_exist = true;
        if(isMin) {
            a = -a;
            b = -b;
        }
        if(lineMonotone) {
            assert(deq.empty() || a <= deq.front().first || deq.back().first <= a);
            Line p(a, b);
            if(deq.empty() || deq.back().first <= a) {
                if(!deq.empty() && deq.back().first == a && b <= deq.back().second) return;
                while(1 < deq.size() && is_useless(deq[(int)deq.size() - 2], deq.back(), p)) {
                    deq.pop_back();
                }
                deq.emplace_back(a, b);
            } else {
                if(deq.front().first == a && b <= deq.front().second) return;
                while(1 < deq.size() && is_useless(p, deq.front(), deq[1])) { deq.pop_front(); }
                deq.emplace_front(a, b);
            }
        } else {
            Line p(a, b);
            auto&& itr = st.insert(p).first;
            if(itr != st.begin() && std::next(itr) != st.end() &&
               is_useless(*std::prev(itr), *itr, *std::next(itr))) {
                st.erase(itr);
            }
            itr->next_line = [=, this] {
                return (std::next(itr) == st.end() ? nullptr : &*std::next(itr));
            };
            auto&& itr1 = std::next(itr);
            if(itr1 != st.end()) {
                auto itr2 = std::next(itr1);
                while(itr2 != st.end() && is_useless(*itr, *itr1, *itr2)) {
                    st.erase(itr1);
                    itr1 = std::next(itr);
                    itr2 = std::next(itr1);
                }
            }
            if(itr != st.begin() && std::prev(itr) != st.begin()) {
                auto itr3 = std::prev(itr);
                auto itr4 = std::prev(itr3);
                while(is_useless(*itr4, *itr3, *itr)) {
                    st.erase(itr3);
                    if(itr4 == st.begin()) break;
                    itr3 = std::prev(itr);
                    itr4 = std::prev(itr3);
                }
            }
        }
    }

    T get(T x) {
        assert(line_exist);
        if(queryMonotone) {
            assert(query_dir == 0 || (query_dir == 1 && last_query <= x) ||
                   (query_dir == 2 && x <= last_query));
            if(1 <= query_count && query_dir == 0) {
                if(last_query < x) query_dir = 1;
                else if(x < last_query)
                    query_dir = 2;
            }
        }
        last_query = x;
        query_count++;
        if(lineMonotone && queryMonotone) {
            if(query_dir == 0 || query_dir == 1) {
                while(1 < deq.size() && calc(deq.front(), x) < calc(deq[1], x)) deq.pop_front();
                return calc(deq.front(), x) * (isMin ? -1 : 1);
            } else {
                while(1 < deq.size() && calc(deq[(int)deq.size() - 2], x) > calc(deq.back(), x))
                    deq.pop_back();
                return calc(deq.back(), x) * (isMin ? -1 : 1);
            }
        } else if(lineMonotone) {
            int l = -1;
            int r = (int)deq.size() - 1;
            while(r - l > 1) {
                int m = (l + r) >> 1;
                if(calc(deq[m], x) < calc(deq[m + 1], x)) l = m;
                else
                    r = m;
            }
            return calc(deq[r], x) * (isMin ? -1 : 1);
        } else {
            if(queryMonotone) {
                if(query_dir == 0 || query_dir == 1) {
                    while(comp(*st.begin(), x)) st.erase(st.begin());
                    return calc(*st.begin(), x) * (isMin ? -1 : 1);
                } else {
                    while(1 < st.size() && !comp(*std::prev(std::prev(st.end())), x)) {
                        st.erase(std::prev(st.end()));
                    }
                    return calc(*st.rbegin(), x) * (isMin ? -1 : 1);
                }
            } else {
                Line p(x, x, true);
                return calc(*st.lower_bound(p), x) * (isMin ? -1 : 1);
            }
        }
    }
};
}//namespace kusu_lib