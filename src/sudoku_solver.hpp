#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "exact_cover_solver.hpp"

namespace dancing_links {

class sudoku_solver : exact_cover_solver {
public:
  sudoku_solver(int8_t* lo);

  void solve(coroutine<array<int8_t, 81>>::push_type& callback);
};

} // namespace dancing_links

#endif
