# Project 1 - Map

In this project, we will implement a map data structure like `std::map`. This data structure will also be very useful in the next project.

The *map* data structure is also known as a *dictionary* or *hash map*. As C++ learners, we should be aware that the widely used `std::map` is a part of the C++ Standard Template Library (STL).

It has the following properties:

- It is a collection of key-value pairs.
  - Each key is unique.
  - Each key maps to one single value.
- We search for / insert / delete a value by it's key.
  - It has `log(n)` time complexity for insertion, deletion, and search operations.
- It is ordered by the keys.
- It is implemented using a *self-balancing* binary search tree (BST) called a *<font color=red>red</font>-black tree*.

`std::map` is a very powerful data structure, but it is also complex. 

Thus, in this project, we will first implement a simpler version of the map data structure using a binary search tree. You can then continue to  implement the `std::map`-like data structure using a <font color=red>red</font>-black tree or other self-balancing trees.

## Work Breakdown

The project is scored out of 100 points. The distribution of points is as follows:

- **Stage 1** - Implement a simple map using BST (60 points)
  - `Map` class, ADT definition (Code given)
  - `BSTNode`, `BSTMap` implementation (60 points)
    - Class definition using templates (Code given)
    - `insert` & `erase` method (20 points)
    - `find`, `[]`, `at` (20 points)
    - `size`, `empty`, `clear` (10 points)
    - Tests for functions above (10 points)
- **Stage 2** (optional) - Balanced BST (+40 points)
  - Implement one kind of balanced BST (+20 points)
  - Fully test whether your balanced BST works as expected (+20 points)
- **Stage 3** (optional) - `BSTIterator` implementation based on Stage 1 (up to +35 points)
  - Bidirectional Iterator `BSTIterator` class (+10 points)
  - Edit your `BSTMap` to support iterator (+10 points)
  - Choose either implementation:
    - Implementation using `parent` pointer (+0 points)
    - Implementation without `parent` pointer yet still fast (+10 points)
  - Tests for iterator (+5 points)
- **Bonus** (up to +10 points)
  - Meet the auto-grader compatibility (5 points)
  - Code readability (5 points)
  - Citation of AI-generated code (-25 points if NOT cited properly)

With basic implementation of only `BSTNode` and `BSTMap`, you can get up to 70 points in this project(60 points + 10 bonus points in case good code quality). 

You can then choose to finish the optional parts to add bonus points to your score of this project. 

There are two projects in this course, contributing 50% of your final grade.

In your final grade, the project will be weighted by the following formula:

```py
proj1_score_raw = int(stage1_score + stage2_score + stage3_score + bonus_score)

if proj1_score_raw <= 100:
    proj1_score = proj1_score_raw * 25 / 100    # 25% in final
else:   # proj1_score_raw > 100
    proj1_score = 25 + sqrt(proj1_score_raw - 100)

proj_final_grade = min(50, proj1_score + proj2_score)
```

There will be a 10% penalty on `proj1_score` for each day the project is late.

## Notice on auto-grading

The project WILL be auto-graded in some parts. The auto-grader will check the correctness of your implementation and the compatibility with the auto-grader. In order to meet the auto-grader requirements and get the 5 bonus points, you should follow the instructions given in the project description. 

You would't need to care about the implementation of the auto-grader as long as you follow the instructions. 

If you have any questions about the instructions, please don't hesitate to ask TA, either by e-mail or on WeChat.

## Notice on AI-assisted coding

You can use any AI-assisted coding tools to help you write the code. However, the AI-assistants WILL give you similar solutions to the ones that are already available online. 

DUPLICATED SOLUTIONS WILL BE PENALIZED IF NOT CITED PROPERLY. 

If you are using AI Chatbots (like, ChatGPT), please provide reports with all your prompt and results as citation. 

Tab-Enter coding results (for example, using Github Copilot) are considered as badly cited code. Tab-Enter coding-assistants usually provide code that is already available online (either searchable by Google, or in public repositories on Github). If you use Tab-Enter coding, you should provide a proper source for the code you used (for example, the original webpage / repository as citation).

If you are not sure whether your citation is proper, please ask TA.

## Stage 1 - Simple map using BST

In this part, you will implement a simple map using a binary search tree (BST) data structure.

First download the project files from Canvas. The project files include the following:

- `Map.h` - `Map` ADT. You should not modify this file.
- `BSTNode.h` - `BSTNode` class definition, to be implemented and be used in `BSTMap`.
- `BSTMap.h` - `BSTMap` class definition, to be implemented.
- `main.cpp` - The main file for testing your implementation. Write your tests in this file.
- `CMakelists.txt` - CMake file for building the project.

Now have a close look at the `Map.h` file. It defines the `Map` ADT, which is the interface for the map data structure. In this project, whatever `XXXMap` class (no matter `BSTMap`, `RBTreeMap`, `AVLTreeMap`, or etc.) you implement should inherit from the `Map` class and implement the methods defined in it.

Note: there are some slightly difference between these three functions that might be confusing in the `Map` ADT:

- `at` - This function returns the reference to the value associated with the given key. If the key does not exist in the map, it should throw an std::out_of_range exception. `at` is usually used when you are sure that the key exists in the map, or you want to manually handle the exception.
- `[]` - This function returns the value associated with the given key. If the key does not exist in the map, it should insert a new key-value pair with the given key and a default value, and return the reference to the value. `[]` is usually used for insertion or updating the value of a key.
- `find` - This function returns a pointer (or in stage 3, an iterator) to the key-value pair with the given key. If the key does not exist in the map, it should return a `nullptr` (or in stage 3, an iterator to the `end` of the map). `find` is usually used when you are not sure whether the key exists in the map.

You should then implement the `BSTNode` and `BSTMap` classes in the `BSTNode.h` and `BSTMap.h` files, respectively. 

Keep `BSTNode` simple. You can define it as either a template class or a template struct. (They are basically same things in C++ except the default access control difference.) We don't need to implement any methods in `BSTNode`, but you should define the member variables and constructors properly.

`BSTMap` should be a template class. It's where we implement the map functionalities. It holds a pointer to the root node of the BST, so that its member functions can access the inner BSTNodes. You should implement the methods defined in the `Map` ADT in the `BSTMap` class (the declaration of the methods are already given in the template).

After implementing the `BSTMap` class, you should write tests in the `main.cpp` file for all the methods you implemented. Note that you should demonstrate that your implementation works correctly by tests, and you should also test the edge cases. Buggy implementations will be penalized.

When you finish this part, have a backup of the project folder. When you submit your code, mark this folder as `Stage 1 - BSTNode BSTMap`.

## Stage 2 - Balanced BST (optional)

Before getting started in this part, first have a copy of your current project folder as a backup (this part will soon be marked as folder `Stage 2 - Balanced BST`). It is recommended to start this part only after you have finished the previous parts.

What's the problem of using a simple BST? Why do we need a balanced BST?

If we insert `n` keys in order in a simple BSTMap, the tree will be skewed, and the height of the tree will be `n`. In this case, the time complexity of the operations will be `O(n)`.

To solve this problem, we need to use a balanced BST. A balanced BST is a tree that is balanced in such a way that the height of the tree is `log(n)`, where `n` is the number of nodes in the tree. This ensures that the time complexity of the operations is `O(log(n))`.

There are several types of balanced BSTs, such as AVL trees, <font color=red>red</font>-black trees, splay trees, etc. In this project, you can choose one of them to implement.

You should implement your balanced BST in a new `XXXMap.h` file (for example, RBTree Map as `RBTreeMap.h`). If needed, you can also implement a new `XXXNode.h` file (for example, RBTreeNode as `RBTreeNode.h`).

You should then write tests in the `main.cpp` file to demonstrate that your balanced BST works correctly. Besides normal tests for the map functionalities, <u>you should also demonstrate by testing that the inner structure of your balanced BST is correct and works as expected</u>. After that, you can also show that the time complexity of the operations is `O(log(n))`. 

You might need to expose some of the internal details of the balanced BST in order to write tests for it. You can do this by adding some public methods (`getter`s) to the balanced BST class. 

When you finish this part, have a backup of the project folder. When you submit your code, mark this folder as `Stage 2 - Balanced BST`.

## Stage 3 -  BSTIterator (optional)

Why do we need an iterator for the map?

We can use the iterator to traverse the map in order (in this case, an in-order traversal of BST), or find the next or previous key-value pair of a given key by using the `++`/`--` operator. This is useful when we want to print the map in order, or when we want to find the next key-value pair of a given key.

Adding iterators to the map will also make it compatible with the range-based for loops and the C++ Algorithms library: a collection of functions for searching, sorting, counting and manipulating containers, based on iterators. [Ref: https://internalpointers.com/post/writing-custom-iterators-modern-cpp]

Example of using range-based for loop with `std::map`:

```cpp
std::map<int, int> m;
m[1] = 2;
m[2] = 4;
m[3] = 6;

for (auto& [key, value] : m) {
    std::cout << key << " " << value << std::endl;
}
```

In this example, the range-based for loop will iterate over the map in order, and print the key-value pairs.

Example of using iterators to find the next key-value pair of a given key:

```cpp
std::map<int, int> m;
m[1] = 2;
m[2] = 4;
m[3] = 6;

auto it = m.find(2);
std::cout << it->first << " " << it->second << std::endl; // 2 4

it++;   // move to the next key-value pair
std::cout << it->first << " " << it->second << std::endl; // 3 6
```

In this example, we first find the key-value pair with key 2, and then use the `++` operator to find the next key-value pair.

Before getting started in this part, first have a copy of your current project folder as a backup (this part will soon be marked as folder `Stage 3 - Iterator`). Considering the complexity of this part, it is recommended to start this part only after you have finished the previous parts. 

You can now implement the `BSTIterator` class in a new file named `BSTIterator.h`, which is a bidirectional iterator for the `BSTMap`. You should also edit your `BSTMap` class to support the iterator. You can start your work based on either Stage 1 or Stage 2, since no balancing is needed in Stage 3.

Have a look at this reference for how to implement a custom iterator: [Writing Custom Iterators in Modern C++: https://internalpointers.com/post/writing-custom-iterators-modern-cpp], and you can also refer to the iterator of `std::map` or other sources for inspiration and compability. (Remember to cite properly if you refer to other sources.)

> **Even more optional:**
> 
> When implementing the `BSTIterator` class, you might want to add a `parent` pointer to the `BSTNode` class. This is because the iterator needs to know the parent node of the current node in order to move to the next or previous node.
> 
> If you want to challenge yourself and get even more points, you can choose to implement the `BSTIterator` without the `parent` pointer yet still fast (without increasing time complexity). There are several ways to do this, and you should now find inspirations from the search engines by yourself. Since it's very challenging to implement the `BSTIterator` without the `parent` pointer, it is advised to finish the other parts first.

After finishing the implementation of the `BSTIterator`, you should write tests for the iterator in the `main.cpp` file. You should demonstrate that your iterator works correctly by tests. You can also show the compatibility of your iterator with the range-based for loop and the C++ Algorithms library.

When you finish this part, have a backup of the project folder. When you submit your code, mark this folder as `Stage 3 - Iterator`.

## Submission Format

Please submit your project as a zip file. Code of each stage should be placed in separate folders. 

Keep all your citations in a single file named `References.md`. You should mark clearly which part of the code is generated by AI-assistants. 

- For ChatGPT users, you can "share" the chat dialogue as link and paste the link in the citation. 
- For ChatGPT Next users, you can export the chat dialogue as Markdown and paste the content in the citation.
- For Github Copilot users, since Copilot is designed to directly copy-paste code from other source repos, you should cite the original source of the code you used (for example, the original webpage / repository) in case you used Copilot to generate all the code.

DUPLICATED SOLUTIONS WILL BE PENALIZED IF NOT CITED PROPERLY.

No report is needed if you have followed the instructions properly. But if you have any special notes, comments or explanations, you can also include them in the `README.md` file.

If you have finished all stages, the final submission zip file should contain the following folders and files :

- `Stage 1 - BSTNode BSTMap/`
- `Stage 2 - Balanced BST/`
- `Stage 3 - Iterator/`
- `References.md`
- `README.md` (if necessary)

## Contact us

If you have any questions about the project, or you need some hints, please don't hesitate to contact TA, either by e-mail or on WeChat.

E-mail: wangsj2000@sjtu.edu.cn

WeChat: oreste-maplewithered

## References

- https://internalpointers.com/post/writing-custom-iterators-modern-cpp
- https://en.cppreference.com/w/cpp/container/map
- Github Copilot (for generating this guide automatically)