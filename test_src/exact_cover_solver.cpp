#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE sudoku_solver_tests
#include "../src/exact_cover_solver.hpp"
#include <boost/test/unit_test.hpp>

using boost::coroutines::coroutine;
using dancing_links::exact_cover_solver;
using std::vector;

BOOST_AUTO_TEST_CASE(exact_cover_tester) {
  vector<vector<size_t>> puzzle {
    { 2, 4, 5 },
    { 0, 3, 6 },
    { 1, 2, 5 },
    { 0, 3 },
    { 1, 6 },
    { 3, 4, 6 },
    { 0 },
  };
  exact_cover_solver ecs(7, puzzle.cbegin(), puzzle.cend());
  coroutine<vector<vector<size_t>>>::pull_type solution_source(
      bind(&exact_cover_solver::solve, &ecs, std::placeholders::_1));
  vector<vector<vector<size_t>>> solutions;
  for (auto s : solution_source)
    solutions.push_back(s);
  vector<vector<vector<size_t>>> expected {
    { { 1, 2, 5 }, { 4, 6, 3 }, { 0 } },
    { { 1, 6 }, { 2, 4, 5 }, { 3, 0 } },
  };
  BOOST_CHECK(solutions == expected);
}
