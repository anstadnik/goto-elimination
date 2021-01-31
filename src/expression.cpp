#include "header.hpp"

namespace program {
Expression::Expression(const string label) : label(move(label)) {}

Arbitrary::Arbitrary(const string label, const string code)
    : Expression(label), code(code) {}

Assignment::Assignment(const string label, const string var, const string op)
    : Expression(move(label)), var(move(var)), op(move(op)) {}
/* WhileLoop::WhileLoop(const string label, const string var, const string op)
 */
/*     : Expression(move(label)), var(move(var)), op(move(op)) {} */

Expression::~Expression(){};
Arbitrary::~Arbitrary(){};
Assignment::~Assignment(){};
WhileLoop::~WhileLoop(){};

ostream& operator<<(std::ostream& os, program::Expression::ptr a) {
  os << a->label;
  return os;
}
}  // namespace program
