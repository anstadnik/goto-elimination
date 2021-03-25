#include <iterator>

#define BACKWARD_HAS_DW 1
#include "algs/algs.h"
#include "ast.h"
#include "goto_elimination.h"

/* namespace backward { */
/* backward::SignalHandling sh; */
/* }  // namespace backward */

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
  icecream::ic.include_context(true);

  /* string fn = ParseArgs(argc, argv); */
  (void)argc, (void)argv;
  /* const char *dummy[] = {"app", "inputs/indirectly_related.txt", NULL}; */
  const char *dummy[] = {"app", "inputs/complicated.txt", NULL};
  string fn = parseArgs(2, dummy);
  list<string> s = readFileToList(fn);
  /* for (const auto &l : s) std::cout << l << std::endl; */

  ast::Stmt::ptr st = ast::StatementFactory::gen_statement(s);
  std::cout << *st << std::endl;

  /* st = goto_elimination::eliminateGoto(move(st)); */

  /* goto_elimination::move_outward(st->find("P")); */
  goto_elimination::move_inward(st->find("E"), st->find("O"));
  std::cout << *st << std::endl;

  return 0;
}
