#include "header.hpp"
#include <iterator>

using namespace std;

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);

  /* string fn = ParseArgs(argc, argv); */
  const char* dummy[] = {"app", "../inputs/complicated.txt", NULL};
  string fn = parseArgs(2, dummy);
  /* string input = read_file_to_string(fn); */
  auto s = readFileToList(fn);
  program::Program p(s);

  return 0;
}
