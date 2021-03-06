#include "algs/algs.h"
#include "ast.h"
#include "goto_elimination.h"

namespace backward {
backward::SignalHandling sh;
}  // namespace backward

using namespace std;

std::string parseArgs(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./out path" << std::endl;
    exit(1);
  }

  return argv[1];
}

int main(int argc, const char *argv[]) {
  string fn = parseArgs(argc, argv);
  std::cout << argv[1] << std::endl;
  ios_base::sync_with_stdio(false);

  list<string> s = algs::io::readFileToList(fn);
  /* for (const auto &l : s) std::cout << l << std::endl; */

  ast::Stmt::ptr st = ast::StatementFactory::gen_statement(s);
  /* std::cout << *st << std::endl; */
  /* dbg("POTATO"); */

  st->dump("/tmp/original.cc");

  st = goto_elimination::eliminateGoto(move(st), ostream(0));

  st->dump("/tmp/changed.cc");

  std::cout << *st << std::endl;

  system("g++ /tmp/original.cc -o /tmp/original");
  std::cout << "Original output:" << std::endl;
  system("/tmp/original");

  std::cout << *st << std::endl;
  system("g++ /tmp/changed.cc -o /tmp/changed");
  std::cout << "Changed output:" << std::endl;
  system("/tmp/changed");

  return 0;
}
