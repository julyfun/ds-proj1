#ifndef PROJECT_MAP_BSTMAP_H
#define PROJECT_MAP_BSTMAP_H

#include <iostream>
#include <stdexcept>

#include "BSTNode.h"
#include "Map.h"
#include "log.h"

/**
 * @brief Map implemented by binary search tree.
 * @tparam K The type of the key.
 * @tparam V The type of the value.
 */
template<typename K, typename V>
class BSTMap: public Map<K, V> {
private:
    BSTNode<K, V>* root;
    std::size_t tsize = 0;

    BSTNode<K, V>* find_along(BSTNode<K, V>* sub, int along) {
        auto cur = sub;
        while (cur->ch[along] != nullptr) {
            cur = cur->ch[along];
        }
        return cur;
    }

    BSTNode<K, V>* real_find(K key) {
        auto cur = this->root;
        while (cur != nullptr) {
            if (key == cur->key) {
                return cur;
            }
            cur = cur->ch[key > cur->key];
        }
        return nullptr;
    }

public:
    /** member functions to override **/
    BSTMap() {
        this->root = nullptr;
    }

    void insert(K key, V value) override {
        // 重复的 key 进行更新
        if (auto ptr = this->real_find(key); ptr != nullptr) {
            ptr->value = value;
            return;
        }

        auto cur = this->root;
        BSTNode<K, V>* parent = nullptr; // 准备更新父亲的孩子
        int from = -1; // 0: left, 1: right
        while (cur != nullptr) { // find child
            parent = cur;
            cur = cur->ch[from = (key > cur->key)];
        }
        // 创建新节点 and 维护树结构
        auto new_node_ptr = new BSTNode<K, V>(key, value);
        new_node_ptr->pa = parent;
        if (parent != nullptr) {
            parent->ch[from] = new_node_ptr;
        } else {
            this->root = new_node_ptr;
        }
        this->tsize++;
    }

    void erase(K key) override {
        auto ptr = this->real_find(key);
        if (ptr == nullptr) {
            return;
        }
        if (ptr->ch[0] == nullptr && ptr->ch[1] == nullptr) {
            // 如果有父亲，要改变父亲的儿子
            auto pa = ptr->pa;
            if (pa != nullptr) {
                pa->ch[pa->ch[1] == ptr] = nullptr;
            } else {
                // 这就是只有一个元素的情况
                this->root = nullptr;
            }
            this->tsize--;
            delete ptr;
            return;
        }
        for (int i = 0; i <= 1; i++) {
            if (auto ch = ptr->ch[i]; ch != nullptr) {
                auto pa = ptr->pa;
                // pa 换儿子
                if (pa != nullptr) {
                    pa->ch[pa->ch[1] == ptr] = ch;
                } else { // 变根
                    this->root = ch;
                }
                // ch 换爸爸
                ch->pa = pa; // 后面不需要这个信息
                // ch 的反向儿子接到另一边
                // - 如果有 bro，ch->[i ^ 1] 要接到 bro_left->ch[i]
                if (ptr->ch[i ^ 1] != nullptr) {
                    if (ch->ch[i ^ 1] != nullptr) {
                        auto bro_left = this->find_along(ptr->ch[i ^ 1], i);
                        bro_left->ch[i] = ch->ch[i ^ 1];
                        ch->ch[i ^ 1]->pa = bro_left;
                    }
                    // 有 bro 还需要建立 ch 和 bro 的双边
                    ptr->ch[i ^ 1]->pa = ch;
                    ch->ch[i ^ 1] = ptr->ch[i ^ 1];
                }
                // - 如果 ptr 另一边那么啥也不用干
                delete ptr;
                break;
            }
        }
        this->tsize--;
    }

    V* find(K key) override {
        auto cur = this->root;
        while (cur != nullptr) {
            if (key == cur->key) {
                return &cur->value;
            }
            cur = cur->ch[key > cur->key];
        }
        return nullptr;
    }

    V& operator[](K key) override {
        auto ptr = this->real_find(key);
        if (ptr == nullptr) {
            this->insert(key, V());
            ptr = this->real_find(key);
        }
        return (V&)(ptr->value); // 修改 value 不需要监视
    }

    V& at(K key) override {
        auto ptr = this->real_find(key);
        if (ptr == nullptr) {
            throw std::out_of_range("BSTMap::at: exception, out of range");
        }
        return (V&)(ptr->value);
    }

    int size() override {
        return this->tsize;
    }

    bool empty() override {
        return this->tsize == 0;
    }

    void clear() override { // make it O(n)
        // No i'm too lazy
        while (!this->empty()) {
            this->erase(this->root->key);
        }
    }

    void dfs_print() { // very useful, but not available for parent pointer
        std::cerr << "{\n";
        if (this->root != nullptr) {
            this->dfs(this->root, 1, -1);
        }
        std::cerr << "}\n";
    }
    void dfs(BSTNode<K, V>* cur, int depth, int from) {
        std::cerr << log::indent_of_depth(depth) << (from == -1 ? "" : (from == 0 ? "L" : "R"))
                  << "[" << cur->key << ": " << cur->value << "]";
        bool has_child = cur->ch[0] != nullptr || cur->ch[1] != nullptr;
        if (has_child) {
            std::cerr << " {\n";
        } else {
            std::cerr << "\n";
        }
        if (cur->ch[0] != nullptr) {
            this->dfs(cur->ch[0], depth + 1, 0);
        }
        if (cur->ch[1] != nullptr) {
            this->dfs(cur->ch[1], depth + 1, 1);
        }
        if (has_child) {
            std::cerr << log::indent_of_depth(depth) << "}\n";
        }
    }

    ~BSTMap() {
        std::cerr << "BSTMap::~BSTMap: destructing BSTMap\n";
        this->clear(); // 不需要额外重置 root，在 erase 只剩一个节点的时候会做
    }
};

#endif // PROJECT_MAP_BSTMAP_H
