#pragma once
#include "ast.h"

namespace goto_elimination {
/*****************
 *  Definitions  *
 *****************/

/* Definition 1
 * The offset of a goto or label statement is n if the statement is the nth goto
 * or label statement that appears in the source program relative to the
 * beginning of the program. */
size_t offset(const ast::Expr& e);

/* Definition 2
 * The level of a label or a goto statement is m if the label or
 * the goto statement is nested inside exactly m loop, switch, or if/else
 * statements. */
size_t level(const ast::Expr& e);

/* Definition 3
 * A label statement and a goto statement are siblings if there
 * exists some statement sequence, stmt 1; ... ; stmt n, such that the label
 * statement corresponds to some stmt i and the goto statement corresponds to
 * some stmt j in the statement sequence. */
bool siblings(const ast::Stmt::Iterator& g, const ast::Stmt::Iterator& l);

/* Definition 4
 * A label statement and a goto statement are directly-related if
 * there exists some statement sequence, stmt 1; ... ; stmt n, such that either
 * the label or goto statements corresponds to some stmt i and the matching goto
 * or label statement is nested inside some stmt j in the statement sequence. */
bool directly_related(const ast::Stmt::Iterator& g,
                      const ast::Stmt::Iterator& l);

/* Definition 5
 * A label statement and a goto statement are indirectly-related if
 * they appear in the same procedure body, but they are neither siblings nor
 * directly related. */
bool indirectly_related(const ast::Stmt::Iterator& g,
                        const ast::Stmt::Iterator& l);

/*********************
 *  Transformations  *
 *********************/

ast::Stmt::Iterator move_outward(ast::Stmt::Iterator it);
ast::Stmt::Iterator move_inward(ast::Stmt::Iterator it, ast::Stmt::Iterator parent);
ast::Stmt::Iterator lift(ast::Stmt::Iterator it, ast::Stmt::Iterator target);
ast::Stmt::Iterator eliminate(ast::Stmt::Iterator it);

/*************
 *  Helpers  *
 *************/

ast::Stmt* get_common_parent(const ast::Stmt::Iterator& a,
                             const ast::Stmt::Iterator& b);

ast::Stmt::ptr eliminateGoto(ast::Stmt::ptr stmt);

}  // namespace goto_elimination
