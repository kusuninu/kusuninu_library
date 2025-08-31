#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {
template<class T = long long> struct SternBrocotTree {
  public:
    struct Node {
        T p, q, r, s, d;
        Node left() { return Node{p, q, p + r, q + s, d + 1}; }
        Node right() { return Node{p + r, q + s, r, s, d + 1}; }
        Node kleft(T k) { return Node{p, q, p * k + r, q * k + s, d + k}; }
        Node kright(T k) { return Node{p + r * k, q + s * k, r, s, d + k}; }
        Node parent() {
            if(p < r) return Node{p, q, r - p, s - q, d - 1};
            return Node{p - r, q - s, r, s, d - 1};
        }
        std::pair<T, T> value() { return std::make_pair(p + r, q + s); }

        T nume() { return p + r; }

        T domi() { return q + s; }

        bool operator==(const Node& rhs) {
            return p == rhs.p && q == rhs.q && r == rhs.r && s == rhs.s;
        }
        bool operator!=(const Node& rhs) { return !(*this == rhs); }
    };

    Node root = Node{0, 1, 1, 0, 0};

    std::vector<T> encode_path(T a, T b) {
        assert(0 < a && 0 < b);
        std::vector<T> res;
        while(b != 0) {
            res.push_back(a / b);
            a %= b;
            swap(a, b);
        }
        res.back()--;
        return res;
    }

    Node decode_path(const std::vector<T>& V) {
        Node nd = root;
        for(bool c = true; T x: V) {
            if(c) nd = nd.kright(x);
            if(!c) nd = nd.kleft(x);
            c = !c;
        }
        return nd;
    }

    Node fraction_to_node(T a, T b) {
        bool c = true;
        Node nd = root;
        while(b != 0) {
            if(c) nd = nd.kright(a / b);
            if(!c) nd = nd.kleft(a / b);
            a %= b;
            swap(a, b);
            c = !c;
        }
        nd = nd.parent();
        return nd;
    }

    Node lca(T a, T b, T c, T d) {
        assert(0 < a && 0 < b);
        Node nd = root;
        bool flag = true;
        while(b != 0 && d != 0) {
            T p = min((a - 1) / b, (c - 1) / d);
            if(flag) nd = nd.kright(p);
            if(!flag) nd = nd.kleft(p);
            if(a / b != c / d) break;
            a %= b;
            swap(a, b);
            c %= d;
            swap(c, d);
            flag = !flag;
        }
        return nd;
    }

    //max fraction x such that F(x) = true && denominator and numerator <= N
    template<class F> Node max_search(F f, T N) { Node nd = root; }
};
}//namespace kusu_lib