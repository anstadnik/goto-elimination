#pragma once

#include "3rd_party/icecream.hpp"
#include "io/io.hpp"
#include "statement.hpp"

string parseArgs(int argc, const char* argv[]);
void print_parse_tree(unordered_map<string, pair<statement::Expr::ptr, string>> t);
