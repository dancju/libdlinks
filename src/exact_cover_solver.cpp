#include "exact_cover_solver.hpp"

namespace dancing_links {

void exact_cover_solver::_attach_column(column_t* pivot) {
  pivot->l->r = pivot->r->l = pivot;
  for (node_t* i = pivot->u; i != (void*)pivot; i = i->u)
    for (node_t* j = i->l; j != i; j = j->l) {
      j->u->d = j->d->u = j;
      j->col->size++;
    }
}

void exact_cover_solver::_detach_column(column_t* pivot) {
  pivot->l->r = pivot->r;
  pivot->r->l = pivot->l;
  for (node_t* i = pivot->d; i != (void*)pivot; i = i->d)
    for (node_t* j = i->r; j != i; j = j->r) {
      j->u->d = j->d;
      j->d->u = j->u;
      j->col->size--;
    }
}

exact_cover_solver::column_t* exact_cover_solver::_get_smallest_column() {
  column_t* res = (column_t*)header.r;
  for (column_t* i = header.r; i != (void*)&header; i = i->r)
    if (res->size > i->size)
      res = i;
  return res;
}

void exact_cover_solver::_solve(
    coroutine<vector<node_t*>>::push_type& callback) {
  if (header.r == (void*)&header) {
    callback(solution);
    return;
  }
  column_t* pivot_column = _get_smallest_column();
  _detach_column(pivot_column);
  for (node_t* i = pivot_column->d; i != (void*)pivot_column; i = i->d) {
    solution.push_back(i);
    for (node_t* j = i->r; j != i; j = j->r)
      _detach_column(j->col);
    _solve(callback);
    for (node_t* j = i->l; j != i; j = j->l)
      _attach_column(j->col);
    solution.pop_back();
  }
  _attach_column(pivot_column);
}

exact_cover_solver::exact_cover_solver(size_t n_col) :
    columns(n_col, column_t()) {
  header.l = header.r = (column_t*)&header;
  for (size_t j = 0; j < n_col; j++) {
    columns[j].l = header.l;
    columns[j].r = (column_t*)&header;
    columns[j].l->r = columns[j].r->l = &columns[j];
    columns[j].u = columns[j].d = (node_t*)&columns[j];
    columns[j].size = 0;
  }
}

template <class I> void exact_cover_solver::add_row(I lo, I hi) {
  node_t* first_node = new node_t();
  first_node->l = first_node->r = first_node;
  first_node->u = columns[*lo].u;
  first_node->d = (node_t*)&columns[*lo];
  first_node->u->d = first_node->d->u = first_node;
  first_node->col = &columns[*lo];
  first_node->col->size++;
  for (I j = lo + 1; j != hi; j++) {
    node_t* n = new node_t();
    n->l = first_node->l;
    n->r = first_node;
    n->l->r = n->r->l = n;
    n->u = columns[*j].u;
    n->d = (node_t*)&columns[*j];
    n->u->d = n->d->u = n;
    n->col = &columns[*j];
    n->col->size++;
  }
}

template <class T> void exact_cover_solver::add_row(T t) {
  add_row(t.begin(), t.end());
}
template void exact_cover_solver::add_row(array<int, 4>);

template <class I>
exact_cover_solver::exact_cover_solver(size_t n_col, I lo, I hi) :
    exact_cover_solver(n_col) {
  for (I i = lo; i != hi; i++)
    add_row(i->begin(), i->end());
}
template exact_cover_solver::exact_cover_solver(size_t,
    vector<vector<size_t>>::const_iterator,
    vector<vector<size_t>>::const_iterator);

exact_cover_solver::~exact_cover_solver() {
  for (auto i = columns.begin(); i != columns.end(); i++)
    for (node_t* j = i->d; j != (void*)&*i; j = j->d)
      delete j;
}

void exact_cover_solver::solve(
    coroutine<vector<vector<size_t>>>::push_type& callback) {
  coroutine<vector<node_t*>>::pull_type solutions(
      bind(&exact_cover_solver::_solve, this, std::placeholders::_1));
  for (vector<node_t*> solution_raw : solutions) {
    vector<vector<size_t>> solution_refined;
    for (node_t* i : solution_raw) {
      solution_refined.push_back(vector<size_t>());
      node_t* j = i;
      do {
        solution_refined.back().push_back(j->col - columns.data());
        j = j->r;
      } while (j != i);
    }
    callback(solution_refined);
  }
}

} // namespace dancing_links
