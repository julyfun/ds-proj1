# Work Breakdown

source: [GitHub](https://github.com/julyfun/github-test)

## Stage 1 (folder `Stage 1 - BSTNode BSTMap`)

### `insert` & `erase` method

- See `BSTMap::insert` and `BSTMap::erase` in `BSTMap.h`

### `find`, `[]`, `at`

- See `BSTMap::find`, `BSTMap::operator[]`, and `BSTMap::at` in `BSTMap.h`

### `size`, `empty`, `clear`

- See `BSTMap::size`, `BSTMap::empty`, and `BSTMap::clear` in `BSTMap.h`
- The `erase` method was called in the destructor, in order to avoid memory leakage

### Tests for functions above

- Run the tests by compiling `Stage 1 - BSTNode BSTMap` and run `./main`.

#### Tests for `insert` & `erase` method

- See `test_insert()` in `test.cpp`
- An debug method `dfs_print()` is implemented, so that the structure of the tree can be printed, including the key-value pairs and parent-child relations
- Tests included:
  - Insert an element and erase it
  - Insert with an existing key to update the value
  - erase a non-existing key, nothing will be done
  - insert tens of shuffled elements and print the tree

#### Tests for `fin`, `[]`, `at`, `size`, `empty`, `clear`

- See `test_other_thing()` in `test.cpp`
- The `at` method uses the `try-catch` syntax to catch the error when the key provided does not exist in the tree
- The `assert()` syntax is used to automatically detect error cases. Still most test results printed out for a quick look. **A test output will start by `test <test_name> ...\n`**
- Tests included:
  - Use a abstact-class reference to the child class
  - Use `[]` to modify an existing key-value
  - Insert new value via `[]`
  - Reference by `at()` and catch errors when the key does not exist
  - `find()` usage and returning `nullptr` when the key does not exist
  - Dereferencing a pointer returned by `find()`
  - `size()` and `empty()` and manual `clear()` usage
  - no `dfs_print()` for the abstact class, no tests are verified by `assert()` and `std::cout`

## Stage 2 (folder `Stage 2 - Balanced BST`)

- An Red-Black Tree class named `RBTreeMap` is implementd, it inherits the abstact class `Map`
- Referenced tutorial links is shown before the definition of certain methods

### Implement one kind of balanced BST

- See methods in the template class `RBTreeMap` in `RBTreeMap.h`
- The node class definitino is in the `RBTreeNode.h`
- Different cases in the `insert` and `erase` methods are shown by comments

### Fully test whether your balanced BST works as expected

- Run the tests by compiling `Stage 2 - Balanced BST` and run `./main`, this may take a few seconds

- Normal tests for the interfaces are implemented in different functions in `main.cpp`, generally covering all cases in `Stage - 1`:

```cpp
test_rbtree_insert()
test_rbtree_erase();
test_rbtree_at_index_find();
test_rbtree_size_empty_clear();
```

- Improved debugger function `print_tree()` and `print_sequences()` are implemented to print the tree structure and the sequences of the tree nodes, clearly showing that the order of keys are maintained
- **Benchmark tests**: in `test_rbtree_bench()`, $10^6$ elements are inserted in the tree. After that, the size and the depth is printed. In this case, $size = 1000000$ and $depth = 37$, the latter is in the magnitude of $O(\log n)$
- **Yet another benchmark tests**: an interesting test is in `test_two_trees_bench()`, it inserts `200000` elements in both `BSTMap` and `RBTreeMap`, and compare the time taken. The result (single thread on M1 chip):

```
BSTMap is inserting 200000 elements...
  it takes 7801ms
RBTreeMap is inserting 200000 elements...
  it takes 51ms
```

## Stage 3 (folder `Stage 3 - BSTIterator`)

### Bidirectional Iterator `BSTIterator` class

- See `RBTreeMap::iterator` struct in `RBTreeMap.h`
- It is bidirectional, as `++` and `--` are implemented (both postfix and prefix)
- Reloaded `!=` and `==`
- A new find method named `find_by_key()` returning an iterator to the key and `end()` to the end of map if the key doesn't exist

### Edit your `BSTMap` to support iterator

- `begin()` and `end()` are provided in the `RBTreeMap` class

### Implementation without `parent` pointer yet still fast

- No `parent` pointer are used in the iterator. The idea is to store pointers to both the current node and the root node. To find the successor of a node, first try if it has a right child:
  - If so, find the leftmost child of that node, which is the successor. The time complexity is $O(\log n)$ as the depth of the tree is in that magnitude.
  - If not, find the current node from the root, along the path, store the last "left-child" branch, where the parent is successor. For the same reason it is $O(\log n)$

### Tests for iterator

- Run the tests by compiling `Stage 3 - BSTIterator` and run `./main`

- See `test_rbtree_iterator()` in `main.cpp`, tests include:
  - Bidirection movement of the iterator
  - Dereferencing of an iterator
  - Catch the exception of `i--` when `i == begin()`
  - `find_by_key()` the new find method who returns iterator
  - Compatility with the range-based for loop `for (auto [key, value]: map)` and C++ algorithm library: `std::for_each()`, `std::find_if()`
