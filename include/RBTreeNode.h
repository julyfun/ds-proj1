#ifndef RB_TREE_NODE_H
#define RB_TREE_NODE_H

template<typename K, typename V>
class RBTreeNode {
public:
    K key; // 支持这玩意的 const 很难，要交换；而且又要同时支持 ch 和 pa 的 const，不可能嗷
    V value;
    RBTreeNode* ch[2];
    RBTreeNode* pa;
    bool red; // 0 is black, 1 is red
    RBTreeNode(): key(K()), value(V()), ch { nullptr, nullptr }, pa(nullptr), red(false) {};
    RBTreeNode(const K& key, const V& value, RBTreeNode* pa, RBTreeNode* const nil, const bool red):
        key(key),
        value(value),
        pa(pa),
        red(red) {
        this->ch[0] = this->ch[1] = nil;
    }
};

template<typename K, typename V>
bool is_nil(RBTreeNode<K, V>* ptr) {
    return ptr->pa == nullptr;
}

#endif
