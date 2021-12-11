#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  std::unordered_map<std::string, int> dict = {};

  int n;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    std::string in;
    int value;

    std::cin >> value >> in;

    auto search = dict.find(in);

    if (value == 1) {
      if (search != dict.end()) {
        search->second += 1;
      } else {
        dict.insert({in, 1});
      }
    } else if (value == 2) {
      if (search != dict.end()) {
        std::cout << search->second << "\n";
      } else {
        std::cout << -1 << "\n";
      }
    }
  }
}
