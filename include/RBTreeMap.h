#ifndef RB_TREE_MAP_H
#define RB_TREE_MAP_H

#include <algorithm>
#include <cassert>
#include <iostream>

#include "Map.h"
#include "log.h"

using std::cerr;
using std::cout;
using std::swap;

using K = int;
using V = int;
template<typename K, typename V>
class RBTreeNode {
public:
    K key;
    V value;
    RBTreeNode* ch[2];
    RBTreeNode* pa;
    // RBTreeNode* prev;
    bool red; // 0 is black, 1 is red
    RBTreeNode(const K& key, const V& value, const bool red): key(key), value(value), red(red) {
        this->ch[0] = this->ch[1] = this->pa = nullptr;
    }
    RBTreeNode(const K& key, const V& value, RBTreeNode* pa, RBTreeNode* const nil, const bool red):
        key(key),
        value(value),
        pa(pa),
        red(red) {
        this->ch[0] = this->ch[1] = nil;
    }
};

template<typename K, typename V>
class RBTreeMap: public Map<K, V> {
private:
    RBTreeNode<K, V>* root;
    RBTreeNode<K, V>* nil;
    int tsize = 0;

    struct FindInfo {
        RBTreeNode<K, V>* me;
        RBTreeNode<K, V>* pa;
        int d;
    };
    FindInfo find_info(const K& key) {
        auto* me = this->root;
        auto* pa = this->nil;
        int d = -1;
        while (me != nil) {
            if (me->key == key) {
                break;
            }
            pa = me;
            me = me->ch[d = key > me->key];
        }
        return { me, pa, d };
    }

    void rotate(RBTreeNode<K, V>* me) {
        assert(this->root != me);
        RBTreeNode<K, V>* pa = me->pa;
        RBTreeNode<K, V>* papa = pa->pa;
        int d = pa->ch[1] == me;
        RBTreeNode<K, V>* ch = me->ch[!d];

        // 1
        me->ch[!d] = pa;
        pa->pa = me;

        // 2 但是 ch 不一定存在，但是改 nil 属性可能也没事吧
        pa->ch[d] = ch;
        ch != this->nil && (ch->pa = pa);

        // 3 但是 papa 不一定存在
        me->pa = papa;
        if (papa != this->nil) {
            int dpa = papa->ch[1] == pa;
            papa->ch[dpa] = me;
        }

        // root
        if (pa == this->root) {
            this->root = me;
        }
    }

    RBTreeNode<K, V>* insert_returning_pointer(K key, V value) {
        // 1. 没有节点
        if (this->root == this->nil) {
            // 根节点是黑色
            cerr << "RBTreeMap::insert: insert root\n";
            this->root = new RBTreeNode<K, V>(key, value, this->nil, this->nil, false);
            this->tsize++;
            return this->root;
        }
        // 2. 能找到 key
        auto [me, pa, d] = this->find_info(key);
        if (me != this->nil) {
            cerr << "RBTreeMap::insert: key exists, change value\n";
            me->value = value;
            return me;
        }
        cerr << "RBTreeMap::insert: insert new node [" << key << "]\n";
        // 插入新节点
        auto* retval = me = new RBTreeNode<K, V>(key, value, pa, this->nil, true);
        this->tsize++;
        pa->ch[d] = me;
        // 不可更新 nil 父亲
        while (me->pa->red) {
            cerr << "    RBTreeMap::insert: managing [" << me->key << "]\n";
            // 4. 插入节点的父亲为红色（危）
            // * 根节点是黑色，所以父亲不是根，所以爷爷存在
            // * 爷爷肯定是黑色
            auto* pa = me->pa;
            auto* papa = pa->pa;
            int d = pa->ch[1] == me;
            int dpa = papa->ch[1] == pa;
            auto* uncle = papa->ch[!dpa];
            // 4.1 父亲和叔叔是红色
            if (uncle->red) {
                // 变换颜色
                pa->red = uncle->red = false;
                // 若爷爷不是根，则爷爷保持黑的话这棵子树比其他子树多了一层黑，故爷爷变红
                if (papa != this->root) {
                    papa->red = true;
                }
                me = papa;
                continue;
            }
            // 4.2 叔叔是黑色
            // 4.2.1 d == dpa 类型
            if (d == dpa) {
                pa->red = false;
                papa->red = true;
                this->rotate(pa);
            } else {
                this->rotate(me);
                me->red = false;
                papa->red = true;
                this->rotate(me);
            }
            break;
        }
        return retval;
    }

    RBTreeNode<K, V>* next(RBTreeNode<K, V>* me) {
        if (me->ch[1] != this->nil) {
            me = me->ch[1];
            while (me->ch[0] != this->nil) {
                me = me->ch[0];
            }
            return me;
        }
        while (me != root && me != me->pa->ch[0]) {
            me = me->pa;
        }
        return me->pa; // [todo] if me->pa == nil then return end()
    }

public:
    RBTreeMap() {
        this->nil = new RBTreeNode<K, V>(K(), V(), false);
        this->root = this->nil;
    }
    void insert(K key, V value) {
        this->insert_returning_pointer(key, value);
    }

    /// 删除节点。ref: https://ysw1912.github.io/post/cc++/rbtree_delete/
    void erase(K key) {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            cerr << "RBTreeMap::erase: key not found\n";
            return;
        }
        cerr << "RBTreeMap::erase: erase [" << key << "]\n";
        // G1: me 两个孩子
        if (me->ch[0] != this->nil && me->ch[1] != this->nil) {
            auto* nxt = this->next(me);
            swap(me->key, nxt->key);
            swap(me->value, nxt->value);
            me = nxt;
            pa = me->pa;
            // 转化为 G2，但是只有 me 有效
        }
        // [drop] d
        // [using] me, pa
        // 注意根什么时候换了 byd
        // G2: me 没有孩子或者一个孩子
        // ch 替代 me 社会关系（包括是否是根）
        auto* ch = me->ch[me->ch[1] != this->nil];
        if (ch != this->nil) {
            ch->pa = me->pa;
        }
        if (this->root == me) {
            this->root = ch; // root 可能就是要变成 nil 的
        } else {
            me->pa->ch[me->pa->ch[1] == me] = ch;
        }
        // 不管有没有孩子都要判断平衡情况
        // G2.1 删除的节点是红色
        if (me->red) {
            delete me;
            this->tsize--;
            return;
        }
        // G2.2 删除的节点是黑色，则必有兄弟 w
        delete me;
        this->tsize--;
        auto* x = ch;
        auto* p = pa;
        while (true) {
            // case 1: x 根或者 x 红色
            if (x == this->root || x->red) {
                x->red = false;
                return;
            }
            // x 红色已处理，下面 x 黑色，其中 w 必存在
            // [using] x, p
            int xd = x == p->ch[1];
            auto* w = p->ch[!xd]; // p 是不是 nil <=> me 是不是根 <=> 处理过了
            // [using] x, p, xd, w
            // case 2: 兄弟红色
            if (w->red) {
                swap(p->red, w->red);
                this->rotate(w);
                w = p->ch[!xd];
                // x, w, p, xd 正确，转变为 case 3
            }
            // [using] x, p, xd, w
            // case 3: 兄弟黑色
            // case 3.1 w->ch[] 均为黑色
            if (!w->ch[0]->red && !w->ch[1]->red) {
                // case 3.1.1 p 黑色
                if (!p->red) {
                    w->red = true;
                    // 全体叶子少 1 黑
                    x = p;
                    p = x->pa;
                    continue;
                }
                // case 3.1.2 p 红色 [end]
                swap(w->red, p->red);
                return;
            }
            // case 3.2 w->ch[] 有红色
            // case 3.2.1 w->ch[!xd] 黑色, w->ch[xd] 红色
            if (!w->ch[!xd]->red && w->ch[xd]->red) {
                RBTreeNode<K, V>* wch[2] = { w->ch[0], w->ch[1] };
                this->rotate(wch[xd]);
                swap(w->red, wch[xd]->red);
                w = p->ch[!xd];
                // 进入 3.2.2 情况
            }
            // case 3.2.2 w->ch[!xd] 红色, w->ch[xd] 任意色 [end]
            // 双黑已经处理
            this->rotate(w);
            swap(p->red, w->red); // 不确定 p 的颜色，交换就完事了
            w->ch[!xd]->red = false;
            return;
        }
    }

    // ref: https://www.cnblogs.com/crazymakercircle/p/16320430.html#autoid-h3-7-1-0
    // ref: https://zhuanlan.zhihu.com/p/530685542

    V* find(K key) {
        auto [me, pa, d] = this->find_info(key);
        return me == this->nil ? nullptr : &me->value;
    }

    V& operator[](K key) {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            auto where = this->insert_returning_pointer(key, V());
            return (V&)(where->value);
        }
        return (V&)(me->value);
    }

    V& at(K key) {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            throw std::out_of_range("RBTreeMap::at: key not found");
        }
        return (V&)(me->value);
    }

    int size() {
        return this->tsize;
    }

    bool empty() {
        return this->tsize == 0;
    }

    void clear() {
        while (!this->empty()) {
            this->erase(this->root->key);
        }
    }

    ~RBTreeMap() {
        this->clear();
    }

public:
    void dfs_print() { // very useful, but not available for parent pointer
        cout << "{\n";
        if (this->root != this->nil) {
            this->dfs(this->root, 1, this->root->pa, -1);
        }
        cout << "}\n";
    }

private:
    void dfs(RBTreeNode<K, V>* me, int depth, RBTreeNode<K, V>* pa, int from) {
        if (pa != me->pa) {
            cout << "me: " << me->key << " in pa: " << pa->key << " stored pa: " << me->pa->key
                 << "\n";
            cout.flush();
            assert(pa == me->pa); // 顺带检验一下
        }
        cout << log::indent_of_depth(depth) << me->pa->key
             << (from == -1 ? "" : (from == 0 ? "L" : "R")) << "[" << me->key
             << (me->red ? "R" : "B") << ": " << me->value << " "
             << "]"
             << "(depth " << depth << ")";
        bool has_child = me->ch[0] != this->nil || me->ch[1] != this->nil;
        if (has_child) {
            cout << " {\n";
        } else {
            cout << "\n";
        }
        if (me->ch[0] != this->nil) {
            this->dfs(me->ch[0], depth + 1, me, 0);
        }
        if (me->ch[1] != this->nil) {
            this->dfs(me->ch[1], depth + 1, me, 1);
        }
        if (has_child) {
            cout << log::indent_of_depth(depth) << "}\n";
        }
    }

public:
    void dfs_print_sequence() {
        if (this->root == this->nil) {
            cout << "RBTreeMap::dfs_print_sequence: empty\n";
            return;
        }
        this->dfs_print_inorder(this->root);
        cout << "\n";
    }

private:
    void dfs_print_inorder(RBTreeNode<K, V>* me) {
        if (me->ch[0] != this->nil) {
            this->dfs_print_inorder(me->ch[0]);
        }
        cout << me->key << " ";
        if (me->ch[1] != this->nil) {
            this->dfs_print_inorder(me->ch[1]);
        }
    }
};

#endif
