#pragma once
#include <bits/stdc++.h>
namespace kusu_lib {

template<typename Key,
         int CharSize = -1,// >0: vector, -1: unordered_map
         Key Base = Key{}>
struct Trie {
  private:
    // ===== Node Policy =====
    struct FixedVectorNode {
        int id;
        int parent;
        Key last_key;
        std::vector<int> next;
        int word_count = 0;
        int common_count = 0;
        bool is_leaf = true;

        FixedVectorNode(int id, int parent, Key last_key)
            : id(id), parent(parent), last_key(last_key), next(CharSize, -1) {}

        int& get_child_ref(Key k) {
            assert(Base <= k && k < Base + CharSize);
            return next[static_cast<size_t>(k - Base)];
        }

        int find_child_id(Key k) const {
            assert(Base <= k && k < Base + CharSize);
            return next[static_cast<size_t>(k - Base)];
        }

        std::vector<std::pair<Key, int>> get_children() const {
            std::vector<std::pair<Key, int>> res;
            for(int i = 0; i < CharSize; i++) {
                if(next[i] != -1) res.emplace_back(static_cast<Key>(Base + i), next[i]);
            }
            return res;
        }
    };

    struct UnorderedMapNode {
        int id;
        int parent;
        Key last_key;
        std::unordered_map<Key, int> next;
        int word_count = 0;
        int common_count = 0;
        bool is_leaf = true;

        UnorderedMapNode(int id, int parent, Key last_key)
            : id(id), parent(parent), last_key(last_key) {}

        int& get_child_ref(Key k) {
            auto [it, inserted] = next.try_emplace(k, -1);
            return it->second;
        }

        int find_child_id(Key k) const {
            auto it = next.find(k);
            if(it == next.end()) return -1;
            return it->second;
        }

        std::vector<std::pair<Key, int>> get_children() const {
            std::vector<std::pair<Key, int>> res;
            res.reserve(next.size());
            for(auto [k, v]: next) { res.emplace_back(k, v); }
            return res;
        }
    };
    using Node = std::conditional_t<(CharSize > 0), FixedVectorNode, UnorderedMapNode>;

    std::vector<Node> nodes;

  public:
    Trie() { nodes.emplace_back(-1, -1, Key{}); }// root

    int size() { return nodes.size(); }
    int all_word_count() { return nodes[0].common_count; }

    template<std::ranges::random_access_range R>
    requires std::same_as<std::ranges::range_value_t<R>, Key>
    int insert(const R& seq) {
        int node_idx = 0;
        for(Key k: seq) {
            int& child_idx = nodes[node_idx].get_child_ref(k);
            if(child_idx == -1) {
                nodes.emplace_back(nodes.size(), node_idx, k);
                child_idx = (int)nodes.size() - 1;
                nodes[node_idx].is_leaf = false;
            }
            nodes[node_idx].common_count++;
            node_idx = child_idx;
        }
        nodes[node_idx].common_count++;
        nodes[node_idx].word_count++;
        return node_idx;
    }

    template<std::ranges::random_access_range R>
    requires std::same_as<std::ranges::range_value_t<R>, Key>
    int count(const R& seq) const {
        int node_idx = 0;
        for(Key k: seq) {
            int child_idx = nodes[node_idx].find_child_id(k);
            if(child_idx == -1) return 0;
            node_idx = child_idx;
        }
        return nodes[node_idx].word_count;
    }

    template<std::ranges::random_access_range R>
    requires std::same_as<std::ranges::range_value_t<R>, Key>
    int search_id(const R& seq) const {
        int node_idx = 0;
        for(Key k: seq) {
            int child_idx = nodes[node_idx].find_child_id(k);
            if(child_idx == -1) return -1;
            node_idx = child_idx;
        }
        return node_idx;
    }

    std::vector<std::pair<Key, int>> children(int id) const { return nodes[id].get_children(); }

    int parent(int id) const { return nodes[id].parent; }

    bool is_leaf(int id) const { return nodes[id].is_leaf; }

    //@return the number of words which end at nodes[id]
    int word_count(int id) const { return nodes[id].word_count; }

    //@return the number of words which go through nodes[id]
    int common_count(int id) const { return nodes[id].common_count; }

    std::vector<Key> restore_seq(int id) const {
        int node_idx = id;
        std::vector<Key> res;
        while(nodes[node_idx].parent != -1) {
            res.push_back(nodes[node_idx].last_key);
            node_idx = nodes[node_idx].parent;
        }
        std::reverse(res.begin(), res.end());
        return res;
    }

    //@return the number of words that start with seq
    template<std::ranges::random_access_range R>
    requires std::same_as<std::ranges::range_value_t<R>, Key>
    int starts_with(const R& seq) const {
        int node_idx = 0;
        for(Key k: seq) {
            int child_idx = nodes[node_idx].find_child_id(k);
            if(child_idx == -1) return 0;
            node_idx = child_idx;
        }
        return nodes[node_idx].common_count;
    }
};

}//namespace kusu_lib