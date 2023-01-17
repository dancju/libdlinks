This is a C++ library of [Dancing Links](https://en.wikipedia.org/wiki/Dancing_Links). It contains solvers for the [exact cover problem (ECP)](https://en.wikipedia.org/wiki/Exact_cover) and [Sudoku](https://en.wikipedia.org/wiki/Dancing_Links). It features a generator pattern when generating puzzle solutions with the help of [Boost.Coroutine](http://www.boost.org/libs/coroutine).

## Usage

### Exact cover problem

```cpp
vector<vector<size_t>> puzzle{
    {2, 4, 5}, {0, 3, 6}, {1, 2, 5}, {0, 3}, {1, 6}, {3, 4, 6}, {0},
};
exact_cover_solver ecs{7, puzzle.cbegin(), puzzle.cend()};
coroutine<vector<vector<size_t>>>::pull_type solutions{
    bind(&exact_cover_solver::solve, &ecs, std::placeholders::_1)};
for (vector<vector<size_t>>& s : solutions) {
  for (vector<size_t>& i : s) {
    for (size_t j : i)
      cout << j << ' ';
    cout << " / ";
  }
  cout << endl;
}
```

```
1 2 5  / 4 6 3  / 0  / 
1 6  / 2 4 5  / 3 0  / 
```

### Sudoku

```cpp
array<int8_t, 81> puzzle{
    0, 6, 1, 0, 0, 9, 5, 2, 0,  //
    8, 0, 0, 0, 0, 3, 4, 0, 0,  //
    0, 0, 0, 0, 0, 0, 0, 0, 8,  //
    9, 2, 0, 0, 5, 0, 0, 0, 0,  //
    0, 5, 0, 0, 1, 0, 0, 4, 0,  //
    0, 0, 0, 0, 3, 0, 0, 7, 5,  //
    2, 0, 0, 0, 0, 5, 0, 0, 0,  //
    0, 0, 8, 2, 0, 0, 0, 0, 1,  //
    0, 3, 9, 7, 0, 0, 2, 5, 0,
};
sudoku_solver ss{puzzle.data()};
coroutine<array<int8_t, 81>>::pull_type solutions{
    bind(&sudoku_solver::solve, &ss, std::placeholders::_1)};
for (array<int8_t, 81>& s : solutions) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++)
      cout << (int)s[9 * i + j] << ' ';
    cout << endl;
  }
  cout << endl;
}
```

```
4 6 1 8 7 9 5 2 3 
8 9 2 5 6 3 4 1 7 
3 7 5 1 2 4 6 9 8 
9 2 3 4 5 7 1 8 6 
7 5 6 9 1 8 3 4 2 
1 8 4 6 3 2 9 7 5 
2 1 7 3 4 5 8 6 9 
5 4 8 2 9 6 7 3 1 
6 3 9 7 8 1 2 5 4 

4 6 1 8 7 9 5 2 3 
8 9 5 1 2 3 4 6 7 
3 7 2 5 6 4 1 9 8 
9 2 3 4 5 7 8 1 6 
7 5 6 9 1 8 3 4 2 
1 8 4 6 3 2 9 7 5 
2 1 7 3 4 5 6 8 9 
5 4 8 2 9 6 7 3 1 
6 3 9 7 8 1 2 5 4 

```

## TODO

- switch from Boost.Coroutine to await & yield when C++20 is supported
- coverage test
- visulisation
- the support of more puzzles that could reduce to ECP, such as [Pentomino](https://en.wikipedia.org/wiki/Pentomino), [Polycube](https://en.wikipedia.org/wiki/Polycube), and the [n queens puzzle](https://en.wikipedia.org/wiki/Eight_queens_puzzle)
