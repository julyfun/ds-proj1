//
// Created by [NAME] on [DATE].
//

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "BSTMap.h"
#include "RBTreeMap.h"
// TODO: include your own balanced BST Map implementation here

using std::cerr;
using std::cout;

bool eq(double a, double b) {
    return std::abs(a - b) < 1e-6;
}
void test_insert() {
    std::cout << "test test_insert ...\n";
    BSTMap<int, std::string> map;
    map.insert(2, "Lei Jun");
    map.erase(2);
    map.dfs_print();
    map.insert(10, "Messi");
    map.dfs_print();
    map.insert(3, "Jobs");
    map.dfs_print();
    map.insert(-1, "Si Jia");
    map.insert(-1, "Si Jia II");
    map.insert(-1, "Si Jia III");
    map.dfs_print();
    map.erase(10);
    map.dfs_print();
    map.erase(3);
    map.dfs_print();
    map.erase(-1);
    map.erase(-1);
    map.erase(-1);
    std::vector<std::pair<int, std::string>> to_insert = {
        { 17, "Q" }, { 18, "R" }, { 19, "S" }, { 20, "T" }, { 21, "U" }, { 3, "C" },  { 4, "D" },
        { 5, "E" },  { 6, "F" },  { 7, "G" },  { 8, "H" },  { 9, "I" },  { 10, "J" }, { 11, "K" },
        { 22, "V" }, { 23, "W" }, { 24, "X" }, { 25, "Y" }, { 26, "Z" }, { 1, "A" },  { 2, "B" },
        { 12, "L" }, { 13, "M" }, { 14, "N" }, { 15, "O" }, { 16, "P" },
    };
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(to_insert.begin(), to_insert.end(), g);
    for (auto t: to_insert) {
        map.insert(t.first, t.second);
    }
    map.erase(10);
    map.erase(2);
    map.erase(3);
    map.erase(17);
    map.dfs_print();
    std::cout << "destructing\n";
}

void test_other_thing() {
    std::cout << "test test_other_thing ...\n";
    Map<std::string, double>& map = *new BSTMap<std::string, double>();
    map.insert("Si Jia", 1.0);
    map.insert("Theo", 100.0);
    map.insert("Ye Heng", 20000.0);
    map.insert("Amo", 100.0);
    // test for []
    map["Si Jia"] = 2.0;
    std::cout << map["Si Jia"] << std::endl;

    map.erase("Si Jia");
    map["Si Jia"] = 2.0;
    std::cout << map["Si Jia"] << std::endl;

    // test for at
    map.at("Si Jia") *= 3.0;
    std::cout << map.at("Si Jia") << std::endl;
    try {
        auto& ref = map.at("None"); // should throw an exception
        ref = 1.0;
    } catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception\n";
    }

    // test find
    {
        auto* it = map.find("Si Jia");
        assert(eq(*it, 6.0));
    }
    {
        auto* it = map.find("None");
        assert(it == nullptr);
    }
    // test size
    assert(map.size() == 4);
    map.erase("None");
    assert(map.size() == 4);
    map.erase("Si Jia");
    assert(map.size() == 3);

    // test clear & empty
    map.clear();
    assert(map.empty());
    map.insert("Theo", 1e9);
    map.insert("Theo", 2e9);
    assert(eq(*map.find("Theo"), 2e9));
    map.clear();
    assert(map.empty());
}

void test_rbtree_insert() {
    cout << "test test_rbtree_insert ...\n";
    RBTreeMap<int, std::string> map;
    for (int i = 1; i <= 10; i++) {
        map.insert(i, std::to_string(i));
    }
    map.print_tree();
}

void test_rbtree_erase() {
    cout << "test test_rbtree_erase ...\n";
    RBTreeMap<int, std::string> map;
    for (int i = 1; i <= 10; i++) {
        map.insert(i, std::to_string(i));
    }
    for (int i = 1; i <= 10; i += 2) {
        map.erase(i);
    }
    map.print_tree();
}

void test_rbtree_at_index_find() {
    cout << "test test_rbtree_at_index_find ...\n";
    RBTreeMap<int, std::string> map;
    for (int i = 1; i <= 10; i++) {
        map.insert(i, std::to_string(i));
    }
    for (int i = 1; i <= 10; i += 2) {
        map.erase(i);
    }
    // at
    map.at(2) = "2.1";
    cout << "map.at(2): " << map.at(2) << "\n";
    try {
        cout << "map.at(1): " << map.at(1) << "\n";
    } catch (std::out_of_range& e) {
        cout << e.what() << "\n";
    }
    // []
    cout << "map[2]: " << map[2] << "\n";
    map[1] = "1";
    cout << "map[1]: " << map[1] << "\n";
    // find
    cout << "*map.find(2): " << *map.find(2) << "\n";
    cout << "map.find(3): " << nullptr << "\n";
}

void test_rbtree_size_empty_clear() {
    cout << "test test_rbtree_size_empty_clear ...\n";
    RBTreeMap<int, std::string> map;
    assert(map.empty());
    for (int i = 1; i <= 10; i++) {
        map.insert(i, std::to_string(i));
    }
    assert(map.size() == 10);
    assert(!map.empty());
    map.clear();
    assert(map.empty());
    assert(map.size() == 0);
}

void test_rbtree_bench() {
    RBTreeMap<int, float> map;
    for (int i = 1; i <= 1e6; i++) {
        map.insert(i, i * 10.0);
    }
    cout << "map.size(): " << map.size() << "\n";
    cout << "map.get_depth(): " << map.get_depth() << "\n";
}

void test_rbtree_iterator() {
    RBTreeMap<int, float> map;
    for (int i = 1; i <= 1e1; i++) {
        map.insert(i, i * 10.0);
    }
    RBTreeMap<int, float>::iterator i = map.begin();
    i++;
    (*i).second = 999;
    i--;
    try {
        i--; // should throw an exception
    } catch (std::out_of_range& e) {
        cout << e.what() << "\n";
    }
    cout << "all elements in map:\n";
    for (auto [key, value]: map) {
        cout << key << " " << value << "\n";
    }
    auto it = map.find_by_key(5);
    cout << "map.find(5): " << it->first << " " << it->second << "\n";
}

int main() {
    test_rbtree_insert();
    test_rbtree_erase();
    test_rbtree_at_index_find();
    test_rbtree_size_empty_clear();
    test_rbtree_bench();
    test_rbtree_iterator();
    return 0;
}
