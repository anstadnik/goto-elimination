#include "header.hpp"
#include <regex>

using namespace std;

namespace program
{
  Program::Program(list<string> p) {
    if (!p.front().starts_with("graph "))
      throw runtime_error("The first line should be \'graph\'");
    p.pop_front();
  
    for (const auto & s : p) {
      auto [left, right] = splitConnection(s);
      /* std::cout << left << " " << right << std::endl; */
      /* std::cout << s; */
    }
  }
  
  Program::~Program() {}
} /* program */ 
