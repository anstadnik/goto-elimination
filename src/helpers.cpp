#include "header.hpp"

std::string parseArgs(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./out path" << std::endl;
    return 0;
  }

  return argv[1];
}

void print_parse_tree(statement::ParseTree t) {
  vector<string> keys(t.size());

  auto key_selector = [](auto pair) { return pair.first; };
  transform(t.begin(), t.end(), keys.begin(), key_selector);
  sort(keys.begin(), keys.end());

  for (auto i : keys)
    /* std::cout << string(t[i].first) + "goto " + */
    /*                  (t[i].second.size() ? t[i].second : "terminate") + "\n"; */
  std::cout << visit([](auto&& expr) { return string(expr); }, t[i].first) +
                   "goto " +
                   (t[i].second.size() ? t[i].second : "terminate") + "\n";
}
