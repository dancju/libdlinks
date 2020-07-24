#include "sudoku_solver.hpp"

namespace dancing_links {

sudoku_solver::sudoku_solver(int8_t* lo) : exact_cover_solver(4 * 9 * 9) {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      int8_t v = lo[9 * i + j];
      if (v)
        this->add_row(array<int, 4> {
            9 * i + j,
            81 + 9 * i + v - 1,
            81 * 2 + 9 * j + v - 1,
            81 * 3 + 9 * (i / 3 * 3 + j / 3) + v - 1,
        });
      else
        for (int k = 0; k < 9; k++)
          this->add_row(array<int, 4> {
              9 * i + j,
              81 + i * 9 + k,
              81 * 2 + j * 9 + k,
              81 * 3 + 9 * (i / 3 * 3 + j / 3) + k,
          });
    }
}

void sudoku_solver::solve(coroutine<array<int8_t, 81>>::push_type& callback) {
  coroutine<vector<vector<size_t>>>::pull_type solutions(
      bind(&exact_cover_solver::solve, (exact_cover_solver*)this,
          std::placeholders::_1));
  for (vector<vector<size_t>> solution_raw : solutions) {
    assert(solution_raw.size() == 81);
    array<int8_t, 81> solution_refined;
    for (vector<size_t> i : solution_raw) {
      assert(i.size() == 4);
      sort(i.begin(), i.end());
      solution_refined[i[0]] = i[1] % 9 + 1;
    }
    callback(solution_refined);
  }
}

} // namespace dancing_links
