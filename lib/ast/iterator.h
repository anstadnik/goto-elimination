#pragma once
#include "expression.h"
#include "statement.h"

namespace ast {
class Stmt::Iterator {
 private:
  bool ensure_not_end();

 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Expr;
  using pointer = list<Expr>::iterator;
  using reference = value_type&;

  Iterator() = default;
  Iterator(const pointer& it) : it(it){};
  Iterator(pointer&& it) : it(it){};
  Iterator(const Iterator&) = default;
  Iterator(Iterator&&) = default;
  Iterator& operator=(const Iterator&) = default;
  Iterator& operator=(Iterator&&) = default;
  Iterator& operator=(const pointer& it) {
    this->it = it;
    return *this;
  };
  Iterator& operator=(pointer&& it) {
    this->it = it;
    return *this;
  }

  reference operator*() const { return *it; }
  pointer operator->() { return it; }
  const pointer operator->() const { return it; }

  // Prefix increment
  Iterator& operator++();

  // Postfix increment
  Iterator operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const Iterator& a, const Iterator& b) {
    return a.it == b.it;
  };
  friend bool operator!=(const Iterator& a, const Iterator& b) {
    return a.it != b.it;
  };

 private:
  pointer it;
  stack<pointer> history;
};
template <class T>
Stmt::Iterator Stmt::find_type() {
  return std::find_if(this->begin(), this->end(), [](const Expr& expr) {
    return holds_alternative<T>(expr.contents);
  });
}

}  // namespace ast
