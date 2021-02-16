#include <iterator>

#define BACKWARD_HAS_DW 1
#include "goto_elimination.h"
#include "statement.h"

#include "algs/algs.h"

namespace backward {
backward::SignalHandling sh;
}  // namespace backward

using namespace std;

std::string parseArgs(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./out path" << std::endl;
    return 0;
  }

  return argv[1];
}

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);

  /* string fn = ParseArgs(argc, argv); */
  const char *dummy[] = {"app", "inputs/complicated.txt", NULL};
  string fn = parseArgs(2, dummy);
  list<string> s = readFileToList(fn);
  statement::Stmt st = statement::statementFactory(s);
  /* st = eliminateGoto(st); */
  /* std::cout << std::endl; */
  std::cout << st << std::endl;

  for (auto& expr : st) {
    std::cout << expr << std::endl;
  }

  return 0;
}
