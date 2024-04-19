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
// TODO: include your own balanced BST Map implementation here

bool eq(double a, double b) {
    return std::abs(a - b) < 1e-6;
}
void test_insert() {
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
    for (auto t: to_insert) {
        map.insert(t.first, t.second);
    }
    map.dfs_print();
    map.erase(10);
    map.erase(2);
    map.erase(3);
    map.dfs_print();
    map.erase(17);
    map.dfs_print();
    std::cerr << "destructing\n";
}

void test_other_thing() {
    std::cerr << "running test_other_thing ...\n";
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
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception\n";
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

int main() {
    test_insert();
    test_other_thing();

    return 0;
}
