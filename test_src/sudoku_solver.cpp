#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE sudoku_solver_tests
#include "../src/sudoku_solver.hpp"
#include <boost/test/unit_test.hpp>

using boost::coroutines::coroutine;
using dancing_links::sudoku_solver;
using std::array;
using std::vector;

BOOST_AUTO_TEST_CASE(sudoku_tester) {
  array<int8_t, 81> puzzle { //
    0, 6, 1, 0, 0, 9, 5, 2, 0, //
    8, 0, 0, 0, 0, 3, 4, 0, 0, //
    0, 0, 0, 0, 0, 0, 0, 0, 8, //
    9, 2, 0, 0, 5, 0, 0, 0, 0, //
    0, 5, 0, 0, 1, 0, 0, 4, 0, //
    0, 0, 0, 0, 3, 0, 0, 7, 5, //
    2, 0, 0, 0, 0, 5, 0, 0, 0, //
    0, 0, 8, 2, 0, 0, 0, 0, 1, //
    0, 3, 9, 7, 0, 0, 2, 5, 0
  };
  sudoku_solver ss(puzzle.data());
  coroutine<array<int8_t, 81>>::pull_type solution_source(
      bind(&sudoku_solver::solve, &ss, std::placeholders::_1));
  vector<array<int8_t, 81>> solutions;
  for (auto i : solution_source)
    solutions.push_back(i);
  vector<array<int8_t, 81>> expected {
    {
        4, 6, 1, 8, 7, 9, 5, 2, 3, //
        8, 9, 2, 5, 6, 3, 4, 1, 7, //
        3, 7, 5, 1, 2, 4, 6, 9, 8, //
        9, 2, 3, 4, 5, 7, 1, 8, 6, //
        7, 5, 6, 9, 1, 8, 3, 4, 2, //
        1, 8, 4, 6, 3, 2, 9, 7, 5, //
        2, 1, 7, 3, 4, 5, 8, 6, 9, //
        5, 4, 8, 2, 9, 6, 7, 3, 1, //
        6, 3, 9, 7, 8, 1, 2, 5, 4, //
    },
    {
        4, 6, 1, 8, 7, 9, 5, 2, 3, //
        8, 9, 5, 1, 2, 3, 4, 6, 7, //
        3, 7, 2, 5, 6, 4, 1, 9, 8, //
        9, 2, 3, 4, 5, 7, 8, 1, 6, //
        7, 5, 6, 9, 1, 8, 3, 4, 2, //
        1, 8, 4, 6, 3, 2, 9, 7, 5, //
        2, 1, 7, 3, 4, 5, 6, 8, 9, //
        5, 4, 8, 2, 9, 6, 7, 3, 1, //
        6, 3, 9, 7, 8, 1, 2, 5, 4, //
    },
  };
  BOOST_CHECK(solutions == expected);
}
