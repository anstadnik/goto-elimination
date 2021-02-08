#include "header.hpp"
#include <iterator>


#define BACKWARD_HAS_DW 1
#include "3rd_party/backward.hpp"

namespace backward {
backward::SignalHandling sh;
}  // namespace backward

using namespace std;

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);

  /* string fn = ParseArgs(argc, argv); */
  const char* dummy[] = {"app", "../inputs/complicated.txt", NULL};
  string fn = parseArgs(2, dummy);
  /* string input = read_file_to_string(fn); */
  list<string> s = readFileToList(fn);
  statement::Statement::ptr st = statement::statementFactory(s);

  return 0;
}
