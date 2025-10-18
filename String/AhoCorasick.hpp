#pragma once
#include <bits/stdc++.h>
#include "Trie.hpp"
namespace kusu_lib {
template<typename Key, int CharSize, Key Base>
struct AhoCorasick : public Trie<Key, CharSize, Base> {
  private:
    bool has_built = false;

  public:
};
}//namespace kusu_lib