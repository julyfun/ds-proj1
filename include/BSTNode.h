//
// Created by [NAME] on [DATE].
//

#ifndef PROJECT_MAP_BSTNODE_H
#define PROJECT_MAP_BSTNODE_H

/**
 * @brief Node class for binary search tree.
 * @tparam K The type of the key.
 * @tparam V The type of the value.
 */
template<typename K, typename V>
class BSTNode { // or struct BSTNode if you prefer
    // TODO: member variables
    // TODO: constructor
public:
    K key;
    V value;
    BSTNode* ch[2];
    BSTNode* pa;
    BSTNode(const K& key, const V& value) {
        this->key = key;
        this->value = value;
        this->ch[0] = this->ch[1] = this->pa = nullptr;
    }
};

#endif //PROJECT_MAP_BSTNODE_H
