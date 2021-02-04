#include "header.hpp"

using namespace std;

namespace statement
{
  Statement::Statement(list<string> p) {
    if (!p.front().starts_with("graph "))
      throw runtime_error("The first line should be \'graph\'");
    p.pop_front();
  
    for (const auto & s : p) {
      auto [left, right, cond] = splitConnection(s);
      /* std::cout << left << " " << right << std::endl; */
      /* std::cout << s; */
    }
  }
  
  Statement::~Statement() {}
} /* program */ 
