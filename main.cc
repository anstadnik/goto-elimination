#include "algs/algs.h"
#include "ast.h"
#include "goto_elimination.h"

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
  (void)argc, (void)argv;
  /* const char *dummy[] = {"app", "inputs/indirectly_related.txt", NULL}; */
  /* const char *dummy[] = {"app", "inputs/complicated.txt", NULL}; */
  /* const char *dummy[] = {"app", "inputs/working_complicated.txt", NULL}; */
  const char *dummy[] = {"app", "inputs/normal.txt", NULL};
  /* const char *dummy[] = {"app", "inputs/count_to_10.txt", NULL}; */
  string fn = parseArgs(2, dummy);
  list<string> s = readFileToList(fn);
  /* for (const auto &l : s) std::cout << l << std::endl; */

  ast::Stmt::ptr st = ast::StatementFactory::gen_statement(s);
  std::cout << *st << std::endl;
  dbg("POTATO");

  /* st->dump("/tmp/original.cc"); */

  /* st = goto_elimination::eliminateGoto(move(st)); */

  /* st->dump("/tmp/changed.cc"); */

  /* std::cout << *st << std::endl; */

  /* system("g++ /tmp/original.cc -o /tmp/original"); */
  /* std::cout << "Original output:" << std::endl; */
  /* system("/tmp/original"); */

  /* std::cout << *st << std::endl; */
  /* system("g++ /tmp/changed.cc -o /tmp/changed"); */
  /* std::cout << "Changed output:" << std::endl; */
  /* system("/tmp/changed"); */

  return 0;
}
