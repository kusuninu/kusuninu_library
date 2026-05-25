#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<typename R, typename T>
concept trie_input_range =
    std::ranges::input_range<R> && std::convertible_to<std::ranges::range_value_t<R>, T>;

template<typename Key = char,
         int CharSize = 26,// >0: vector, -1: unordered_map
         Key Base = 'a'>
struct Trie {

    struct Node {
        int id;
        int parent;
        Key last_key;
        std::conditional_t<(CharSize > 0), std::vector<int>, std::unordered_map<Key, int>> next;
        int word_count = 0;
        int common_count = 0;
        int last_word_id = -1;

        Node(int id, int parent, Key last_key) : id(id), parent(parent), last_key(last_key) {
            if constexpr(CharSize > 0) { next.assign(CharSize, -1); }
        }

        //if there is no, return -1
        int& get_child_ref(Key k) {
            if constexpr(CharSize > 0) {
                assert(Base <= k && k < Base + CharSize);
                return next[static_cast<size_t>(k - Base)];
            }
            else { return next.try_emplace(k, -1).first->second; }
        }

        //if there is no, return -1
        int find_child_id(Key k) const {
            if constexpr(CharSize > 0) {
                assert(Base <= k && k < Base + CharSize);
                return next[static_cast<size_t>(k - Base)];
            }
            else {
                auto it = next.find(k);
                return it == next.end() ? -1 : it->second;
            }
        }
    };

    std::vector<Node> nodes;

    std::vector<int> pre_word_id;

    Trie() { nodes.emplace_back(0, -1, Key{}); }// root

    int size() const { return nodes.size(); }
    int all_word_count() const { return nodes[0].common_count; }

    int move(int id, Key k) const { return nodes[id].find_child_id(k); }

    template<trie_input_range<Key> Range>
    int insert(const Range& seq, int word_id = -1) {
        if(word_id == -1) { word_id = all_word_count(); }
        int node_idx = 0;
        for(Key k: seq) {
            int& child_idx = nodes[node_idx].get_child_ref(k);
            if(child_idx == -1) {
                nodes.emplace_back(nodes.size(), node_idx, k);
                child_idx = (int)nodes.size() - 1;
            }
            nodes[node_idx].common_count++;
            node_idx = child_idx;
        }
        nodes[node_idx].common_count++;
        nodes[node_idx].word_count++;
        pre_word_id.push_back(nodes[node_idx].last_word_id);
        nodes[node_idx].last_word_id = word_id;
        return node_idx;
    }

    int insert(const char* seq, int word_id = -1) { return insert(std::string_view(seq), word_id); }

    template<trie_input_range<Key> Range>
    int find(const Range& seq) const {
        int node_idx = 0;
        for(Key k: seq) {
            int child_idx = move(node_idx, k);
            if(child_idx == -1) return -1;
            node_idx = child_idx;
        }
        return node_idx;
    }

    int find(const char* seq) const { return find(std::string_view(seq)); }

    template<trie_input_range<Key> Range>
    int count(const Range& seq) const {
        int node_idx = find(seq);
        if(node_idx == -1) return 0;
        return nodes[node_idx].word_count;
    }

    int count(const char* seq) const { return count(std::string_view(seq)); }

    //@return the number of words that start with seq
    template<trie_input_range<Key> Range>
    int prefix_count(const Range& seq) const {
        int node_idx = find(seq);
        if(node_idx == -1) return 0;
        return nodes[node_idx].common_count;
    }

    int prefix_count(const char* seq) const { return prefix_count(std::string_view(seq)); }

    auto children(int id) const {
        if constexpr(CharSize > 0) {
            return std::views::iota(0, CharSize) |
                   std::views::filter([this, id](int i) { return nodes[id].next[i] != -1; }) |
                   std::views::transform([this, id](int i) {
                       return std::pair<Key, int>(static_cast<Key>(Base + i), nodes[id].next[i]);
                   });
        }
        else { return std::views::all(nodes[id].next); }
    }

    int parent(int id) const { return nodes[id].parent; }

    Key last_key(int id) const { return nodes[id].last_key; }

    bool is_leaf(int id) const { return nodes[id].word_count == nodes[id].common_count; }

    bool is_word_end(int id) const { return nodes[id].word_count != 0; }

    //@return the number of words which end at nodes[id]
    int word_count(int id) const { return nodes[id].word_count; }

    //@return the number of words which go through nodes[id]
    int common_count(int id) const { return nodes[id].common_count; }

    int last_word_id(int id) const { return nodes[id].last_word_id; }

    std::vector<int> all_word_id(int id) const {
        std::vector<int> res;
        int word_pos = last_word_id(id);
        while(word_pos != -1) {
            res.push_back(word_pos);
            word_pos = pre_word_id[word_pos];
        }
        return res;
    }

    std::vector<int> bfs_order() const {
        int l = 0;
        int r = 1;
        std::vector<int> res(size());
        for(; l < size(); l++) {
            for(auto [k, id]: children(res[l])) {
                res[r] = id;
                r++;
            }
        }
        return res;
    }

    std::vector<Key> restore_seq(int id) const {
        int node_idx = id;
        std::vector<Key> res;
        while(parent(node_idx) != -1) {
            res.push_back(last_key(node_idx));
            node_idx = parent(node_idx);
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};

}//namespace kusu_lib