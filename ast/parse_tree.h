#pragma once

#include "expression.h"
#include "statement.h"

namespace statement {
typedef unordered_map<string, pair<Expr, string>> ParseTree;
pair<ParseTree, string> gen_parse_tree(list<string> s);
ostream& operator<<(ostream& os, const statement::ParseTree& t);
}  // namespace statement
