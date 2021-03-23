#pragma once

#include "expression.h"
#include "statement.h"

namespace ast {
typedef unordered_map<string, pair<Expr, string>> ParseTree;
pair<ParseTree, string> gen_parse_tree(list<string> s);
ostream& operator<<(ostream& os, const ast::ParseTree& t);
}  // namespace ast
