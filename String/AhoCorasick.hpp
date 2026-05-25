#pragma once
#include <bits/stdc++.h>
#include "Trie.hpp"
namespace kusu_lib {
template<typename Key = char, int CharSize = 26, Key Base = 'a'>
struct AhoCorasick : public Trie<Key, CharSize, Base> {
  private:
    bool has_built = false;

  public:
    std::vector<int> _suffix_link;
    std::vector<int> _dict_link;

    void build() {
        has_built = true;
        _suffix_link.resize(this->size());
        _dict_link.resize(this->size());
        for(int p: this->bfs_order()) {
            if(p == 0) continue;
            int pos = this->parent(p);
            if(pos == 0) continue;
            Key k = this->last_key(p);
            while(pos != 0) {
                pos = _suffix_link[pos];
                int to = this->move(pos, k);
                if(to != -1) {
                    _suffix_link[p] = to;
                    break;
                }
            }
            if(this->is_word_end(_suffix_link[p])) _dict_link[p] = _suffix_link[p];
            else _dict_link[p] = _dict_link[_suffix_link[p]];
        }
    }

    int suffix_link(int id) const { return _suffix_link[id]; }

    int dict_link(int id) const { return _dict_link[id]; }

    bool contains_word(int id) const { return _dict_link[id] != 0 || this->is_word_end(id); }

    int transit(int id, Key k) const {
        while(true) {
            int to = this->move(id, k);
            if(to != -1) return to;
            if(id == 0) return 0;
            id = suffix_link(id);
        }
    }

    template<trie_input_range<Key> Range>
    std::vector<int> count_each_words(const Range& seq) const {
        std::vector<int> res(this->all_word_count());
        std::vector<int> cnt(this->size());
        int id = 0;
        for(auto k: seq) {
            id = transit(id, k);
            cnt[id]++;
        }
        auto order = this->bfs_order();
        for(int i = this->size() - 1; i > 0; i--) {
            for(auto word: this->all_word_id(order[i])) res[word] += cnt[order[i]];
            cnt[dict_link(order[i])] += cnt[order[i]];
        }
        return res;
    }

    template<trie_input_range<Key> Range>
    int count_all_words(const Range& seq) const {
        long long ans = 0;
        std::vector<int> cnt(this->size());
        int id = 0;
        for(auto k: seq) {
            id = transit(id, k);
            cnt[id]++;
        }
        auto order = this->bfs_order();
        for(int i = this->size() - 1; i > 0; i--) {
            ans += (long long)cnt[order[i]] * this->word_count(order[i]);
            cnt[dict_link(order[i])] += cnt[order[i]];
        }
        return ans;
    }

    std::vector<int> minimal_words() const {
        std::vector<int> res;
        std::queue<int> q;
        q.push(0);
        while(!q.empty()) {
            int id = q.front();
            q.pop();
            if(_dict_link[id] != 0) continue;
            if(this->is_word_end(id)) {
                res.push_back(this->last_word_id(id));
                continue;
            }
            for(auto [k, id]: this->children(id)) q.push(id);
        }
		return res;
    }
};
}//namespace kusu_lib