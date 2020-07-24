#ifndef EXACT_COVER_SOLVER_H
#define EXACT_COVER_SOLVER_H

#include <boost/coroutine/all.hpp>
#include <vector>

namespace dancing_links {

using boost::coroutines::coroutine;
using std::array;
using std::vector;

class exact_cover_solver {
  struct header_t;
  struct column_t;
  struct node_t;

  struct header_t {
    column_t *l, *r;
  };
  struct column_t {
    column_t *l, *r;
    node_t *u, *d;
    size_t size;
  };
  struct node_t {
    node_t *l, *r, *u, *d;
    column_t* col;
  };

  header_t header;
  vector<column_t> columns;
  vector<node_t*> solution;

  void _attach_column(column_t* pivot);

  void _detach_column(column_t* pivot);

  column_t* _get_smallest_column();

  void _solve(coroutine<vector<node_t*>>::push_type& callback);

protected:
  exact_cover_solver(size_t n_col);

  template <class I> void add_row(I lo, I hi);

  template <class T> void add_row(T t);

public:
  template <class I> exact_cover_solver(size_t n_col, I lo, I hi);

  ~exact_cover_solver();

  void solve(coroutine<vector<vector<size_t>>>::push_type& callback);
};

} // namespace dancing_links

#endif
