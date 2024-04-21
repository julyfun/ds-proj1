#ifndef RB_TREE_MAP_H
#define RB_TREE_MAP_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

#include "Map.h"
#include "RBTreeNode.h"
#include "log.h"

using std::cerr;
using std::cout;
using std::max;
using std::swap;

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
            this->root = new RBTreeNode<K, V>(key, value, this->nil, this->nil, false);
            this->tsize++;
            return this->root;
        }
        // 2. 能找到 key
        auto [me, pa, d] = this->find_info(key);
        if (me != this->nil) {
            me->value = value;
            return me;
        }
        // 插入新节点
        auto* retval = me = new RBTreeNode<K, V>(key, value, pa, this->nil, true);
        this->tsize++;
        pa->ch[d] = me;
        // 不可更新 nil 父亲
        while (me->pa->red) {
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

    static RBTreeNode<K, V>* next(RBTreeNode<K, V>* me) {
        if (!is_nil(me->ch[1])) {
            me = me->ch[1];
            while (!is_nil(me->ch[0])) {
                me = me->ch[0];
            }
            return me;
        }
        while (!is_nil(me->pa) && me != me->pa->ch[0]) {
            me = me->pa;
        }
        return me->pa; // [todo] if me->pa == nil then return end()
    }

    static RBTreeNode<K, V>* prev(RBTreeNode<K, V>* me) {
        if (!is_nil(me->ch[0])) {
            me = me->ch[0];
            while (!is_nil(me->ch[1])) {
                me = me->ch[1];
            }
            return me;
        }
        while (!is_nil(me->pa) && me != me->pa->ch[1]) {
            me = me->pa;
        }
        if (is_nil(me->pa)) {
            throw std::out_of_range("RBTreeMap::prev: no previous element.");
        }
        return me->pa; // [todo] if me->pa == nil then return throw an error
    }

    RBTreeNode<K, V>* first() {
        auto* me = this->root;
        while (!is_nil(me->ch[0])) {
            me = me->ch[0];
        }
        return me;
    }

public:
    RBTreeMap() {
        this->nil = new RBTreeNode<K, V>();
        this->root = this->nil;
    }
    void insert(K key, V value) override {
        this->insert_returning_pointer(key, value);
    }

    /// 删除节点。ref: https://ysw1912.github.io/post/cc++/rbtree_delete/
    void erase(K key) override {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            return;
        }
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

    V* find(K key) override {
        auto [me, pa, d] = this->find_info(key);
        return me == this->nil ? nullptr : &me->value;
    }

    V& operator[](K key) override {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            auto where = this->insert_returning_pointer(key, V());
            return (V&)(where->value);
        }
        return (V&)(me->value);
    }

    V& at(K key) override {
        auto [me, pa, d] = this->find_info(key);
        if (me == this->nil) {
            throw std::out_of_range("RBTreeMap::at: key not found");
        }
        return (V&)(me->value);
    }

    int size() override {
        return this->tsize;
    }

    bool empty() override {
        return this->tsize == 0;
    }

    void clear() override {
        while (!this->empty()) {
            this->erase(this->root->key);
        }
    }

    ~RBTreeMap() {
        this->clear();
    }

public:
    int get_depth() {
        return this->dfs_depth(this->root);
    }

private:
    int dfs_depth(RBTreeNode<K, V>* me) {
        if (me == this->nil) {
            return 0;
        }
        return max(this->dfs_depth(me->ch[0]), this->dfs_depth(me->ch[1])) + 1;
    }

public:
    void print_tree() { // very useful, but not available for parent pointer
        cout << "{\n";
        if (this->root != this->nil) {
            this->dfs_print_check(this->root, 1, this->root->pa, -1);
        }
        cout << "}\n";
    }

private:
    void dfs_print_check(RBTreeNode<K, V>* me, int depth, RBTreeNode<K, V>* pa, int from) {
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
            this->dfs_print_check(me->ch[0], depth + 1, me, 0);
        }
        if (me->ch[1] != this->nil) {
            this->dfs_print_check(me->ch[1], depth + 1, me, 1);
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

public:
    struct iterator {
    private:
        RBTreeNode<K, V>* me;
        RBTreeNode<K, V>* root;

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<const K&, V&>;
        using pointer = value_type*;
        using reference = value_type&;
        // delete these is ok

        iterator(RBTreeNode<K, V>* me, RBTreeNode<K, V>* root): me(me), root(root) {}

        std::pair<const K&, V&> operator*() {
            return { this->me->key, this->me->value };
        }

        /// should return a fake pointer
        // std::pair<const K&, V&> operator->() {
        //     return { this->me->key, this->me->value };
        // }
        // 不支持，若要返回指针，必须有人持有该变量，则需要将 Node 中的类型改为实际的 pair (is there tuple?)
        // 实际上不应该新生成一个 pair 实例，而是 key 和 value 的引用的捆绑
        // 如何不创建新实例而返回引用的捆绑呢？

        std::shared_ptr<std::pair<const K&, V&>> operator->() {
            return std::make_shared<std::pair<const K&, V&>>(this->operator*());
            // holy sh
        }

        iterator operator++() {
            // this->me = RBTreeMap::next(this->me); // with pa ptr
            if (!is_nil(this->me->ch[1])) {
                this->me = this->me->ch[1];
                while (!is_nil(this->me->ch[0])) {
                    this->me = this->me->ch[0];
                }
            } else {
                auto* last_left = new RBTreeNode<K, V>(); // nil
                auto* cur = this->root;
                while (!(cur == this->me)) {
                    int d = this->me->key > cur->key;
                    if (d == 0) {
                        last_left = cur;
                    }
                    cur = cur->ch[d];
                }
                this->me = last_left;
            }
            return *this; // wtf returning a std::pair???
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        /// invalid for nil (which is a fake end)
        iterator operator--() {
            // this->me = RBTreeMap::prev(this->me);
            if (!is_nil(this->me->ch[0])) {
                this->me = this->me->ch[0];
                while (!is_nil(this->me->ch[1])) {
                    this->me = this->me->ch[1];
                }
            } else {
                auto* last_right = new RBTreeNode<K, V>(); // nil
                auto* cur = this->root;
                while (!(cur == this->me)) {
                    int d = this->me->key > cur->key;
                    if (d == 1) {
                        last_right = cur;
                    }
                    cur = cur->ch[d];
                }
                this->me = last_right;
            }
            if (is_nil(this->me)) {
                throw std::out_of_range("RBTreeMap::Iterator::operator-- out of range");
            }
            return *this;
        }
        iterator operator--(int) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return this->me == other.me
                || (is_nil(this->me) && is_nil(other.me)); // 没错可以访问同类的 private 变量
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const iterator& other) const {
            if (is_nil(this->me) && is_nil(other->me)) {
                return false;
            }
            if (is_nil(this->me) != is_nil(other->me)) {
                return is_nil(this->me) < is_nil(other->me);
            }
            return this->key < other->key;
        }
    };

public:
    iterator begin() {
        return iterator(this->first(), this->root);
    }

    iterator end() {
        return iterator(this->nil, this->root);
    }

    iterator find_by_key(const K& key) {
        auto [me, pa, d] = this->find_info(key);
        return iterator(me, this->root);
    }
};

#endif
