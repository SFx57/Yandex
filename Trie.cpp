// Trie implementation

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Trie {
 public:
  static constexpr int64_t kNull = -1;
  struct Node {
    Node() = default;
    Node(int64_t parent, char symbol) : parent(parent), symbol(symbol) {}

    bool HasChild(char symbol) {
      return children.find(symbol) != children.end();
    }

    std::unordered_map<char, int64_t> children;
    int64_t parent{-1};
    char symbol{};
    bool is_terminal{false};
  };

  Trie() : nodes_(1) {}

  int64_t Add(const std::string& key) {
    int64_t now = 0;
    for (auto symbol : key) {
      if (!nodes_[now].HasChild(symbol)) {
        nodes_.emplace_back(now, symbol);
        nodes_[now].children[symbol] = nodes_.size() - 1;
      }
      now = nodes_[now].children[symbol];
    }
    nodes_[now].is_terminal = true;
    return now;
  }

  Node& Get(int64_t index) { return nodes_.at(index); }

  size_t NodeCount() { return nodes_.size(); }

 private:
  std::vector<Node> nodes_;
};

class AhoKorasikAutomaton {
 public:
  AhoKorasikAutomaton(const std::vector<std::string>& patterns)
      : patterns_(patterns) {
    int32_t cnt = 0;
    for (auto pattern : patterns) {
      pattern_mapper_[trie_.Add(pattern)] = cnt++;
    }
    suffix_links_.assign(trie_.NodeCount(), Trie::kNull);
    terminal_links_.assign(trie_.NodeCount(), Trie::kNull);
  }

  std::vector<std::string> MakeStep(char symbol) {
    state_ = CalcStep(state_, symbol);
    return ExtractMatches();
  }

  void ResetState() { state_ = 0; }

 private:
  std::vector<std::string> ExtractMatches() {
    std::vector<std::string> ans;
    size_t vert = state_;
    while (vert != 0) {
      auto& node = trie_.Get(vert);
      if (node.is_terminal) {
        ans.push_back(patterns_[pattern_mapper_[vert]]);
      }
      vert = GetTerminalLink(vert);
    }
    return ans;
  }

  int64_t CalcStep(int64_t node, char symbol) {
    auto& vert = trie_.Get(node);
    if (vert.HasChild(symbol)) {
      return vert.children[symbol];
    }
    if (node == 0) {
      return 0;
    }
    return CalcStep(GetSuffixLink(node), symbol);
  }

  int64_t GetTerminalLink(int64_t node) {
    if (terminal_links_[node] != Trie::kNull) {
      return terminal_links_[node];
    }
    if (node == 0 || trie_.Get(node).parent == 0) {
      terminal_links_[node] = 0;
      return 0;
    }
    int64_t vert = GetSuffixLink(node);
    if (vert != 0 && !trie_.Get(vert).is_terminal) {
      vert = GetTerminalLink(vert);
    }
    terminal_links_[node] = vert;
    return vert;
  }

  int64_t GetSuffixLink(int64_t node) {
    if (suffix_links_[node] != Trie::kNull) {
      return suffix_links_[node];
    }
    if (node == 0 || trie_.Get(node).parent == 0) {
      suffix_links_[node] = 0;
      return 0;
    }
    auto& vert = trie_.Get(node);
    suffix_links_[node] = CalcStep(GetSuffixLink(vert.parent), vert.symbol);
    return suffix_links_[node];
  }
  std::vector<int64_t> suffix_links_;
  std::vector<int64_t> terminal_links_;

  std::unordered_map<int64_t, size_t> pattern_mapper_;

  int64_t state_{0};
  Trie trie_;
  std::vector<std::string> patterns_;
};

class Matcher {
 public:
  Matcher(const std::vector<std::string>& patterns) : automaton_(patterns) {}

  std::vector<std::pair<size_t, std::string>> FindMatches(
      const std::string& text) {
    automaton_.ResetState();
    std::vector<std::pair<size_t, std::string>> matches;
    for (size_t cnt = 0; cnt < text.size(); ++cnt) {
      auto step_matches = automaton_.MakeStep(text[cnt]);
      if (!step_matches.empty()) {
        for (auto& match : step_matches) {
          size_t pos = static_cast<size_t>(
              cnt - static_cast<int64_t>(match.length()) + 1);
          matches.emplace_back(pos, match);
        }
      }
    }
    return matches;
  }

 private:
  AhoKorasikAutomaton automaton_;
};

void FindAnswer(std::vector<std::string>& match_strings,
                std::vector<std::pair<size_t, std::string>>& matches) {
  std::unordered_map<std::string, std::vector<int64_t>> ans;
  for (auto& pair : matches) {
    ans[pair.second].push_back(pair.first + 1);
  }
  for (auto& string : match_strings) {
    std::cout << ans[string].size();
    if (ans[string].empty()) {
      std::cout << "\n";
      continue;
    }
    for (auto elem : ans[string]) {
      std::cout << " " << elem;
    }
    std::cout << "\n";
  }
}

int main() {
  std::string text;
  std::cin >> text;
  size_t num;
  std::cin >> num;
  std::vector<std::string> match_strings(num);
  for (size_t cnt = 0; cnt != num; ++cnt) {
    std::cin >> match_strings[cnt];
  }
  Matcher matcher{match_strings};

  auto matches = matcher.FindMatches(text);
  FindAnswer(match_strings, matches);
}
