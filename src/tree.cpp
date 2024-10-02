#include <queue>
#include <string>
#include "tree.h"

RBTree::RBTree() {
    root = nullptr;
    nil = new Node(0, false, nullptr, nullptr, nullptr);
}

RBTree::~RBTree() {
    if (root == nullptr) {
        delete nil;
        return;
    }
    std::queue<Node*> q;
    q.push(root);
    while (q.size() != 0) {
        Node* node = q.front();
        if (node->left != nil) q.push(node->left);
        if (node->right != nil) q.push(node->right);
        q.pop();
        delete node;
    }
    delete nil;
}

void RBTree::left_rotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RBTree::right_rotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nil)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

Node* RBTree::find_element(int val) {
    Node* p = root;
    while (p != nullptr && p != nil) {
        if (p->value < val)
            p = p->right;
        else if (p->value > val)
            p = p->left;
        else if (p->value == val)
            return p;
    }
    return nullptr;
}

void RBTree::add_element(int val) {
    Node* cur = new Node(val, true, nil, nil, nil);
    if (root == nullptr) {
        root = cur;
        root->color = false;
        return;
    }
    else {
        Node* p = root;
        Node* q = nil;
        while (p != nil) {
            q = p;
            if (p->value <= cur->value)
                p = p->right;
            else
                p = p->left;
        }
        cur->parent = q;

        if (q->value <= cur->value)
            q->right = cur;
        else
            q->left = cur;

        while (cur->parent->color == true) {
            if (cur->parent == cur->parent->parent->left) {
                if (cur->parent->parent->right->color == true) {
                    cur->parent->color = false;
                    cur->parent->parent->right->color = false;
                    cur->parent->parent->color = true;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->right) {
                        cur = cur->parent;
                        left_rotate(cur);
                    }
                    cur->parent->color = false;
                    cur->parent->parent->color = true;
                    right_rotate(cur->parent->parent);
                }
            }
            else {
                if (cur->parent->parent->left->color == true) {
                    cur->parent->color = false;
                    cur->parent->parent->left->color = false;
                    cur->parent->parent->color = true;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->left) {
                        cur = cur->parent;
                        right_rotate(cur);
                    }
                    cur->parent->color = false;
                    cur->parent->parent->color = true;
                    left_rotate(cur->parent->parent);
                }
            }
        }
        root->color = false;
        nil->parent = nullptr;
        nil->right = nullptr;
        nil->left = nullptr;
    }
}

void RBTree::transplant(Node* u, Node* v) {
    if (root == u)
        if (v == nil)
            root = nullptr;
        else
            root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void RBTree::delete_element(int val) {
    Node* to_delete = find_element(val);
    if (to_delete == nil || to_delete == nullptr) return;
    Node* y = to_delete;
    bool y_original_color = y->color;
    Node* x;
    if (to_delete == root && to_delete->left == nil && to_delete->right == nil) {
        root = nullptr;
        delete to_delete;
        return;
    }
    if (to_delete->left == nil) {
        x = to_delete->right;
        transplant(to_delete, to_delete->right);
    }
    else if (to_delete->right == nil) {
        x = to_delete->left;
        transplant(to_delete, to_delete->left);
    }
    else {
        y = to_delete->right;
        while (y->left != nil) y = y->left;
        y_original_color = y->color;
        x = y->right;
        if (y->parent == to_delete) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = to_delete->right;
            y->right->parent = y;
        }
        transplant(to_delete, y);
        y->left = to_delete->left;
        y->left->parent = y;
        y->color = to_delete->color;
    }
    delete to_delete;
    if (y_original_color == false) {
        Node* brother;
        while (x != root && x->color == false) {
            if (x == x->parent->left) {
                brother = x->parent->right;
                if (brother->color == true) {
                    brother->color = false;
                    x->parent->color = true;
                    left_rotate(x->parent);
                    brother = x->parent->right;
                }
                if (brother->left->color == false && brother->right->color == false) {
                    brother->color = true;
                    x = x->parent;
                }
                else {
                    if (brother->right->color == false) {
                        brother->left->color = false;
                        brother->color = true;
                        right_rotate(brother);
                        brother = x->parent->right;
                    }

                    brother->color = x->parent->color;
                    x->parent->color = false;
                    brother->right->color = false;
                    left_rotate(x->parent);
                    if (x->parent->parent != nil)
                        if (x->parent->parent->parent == nil) root = x->parent->parent;
                    x = root;
                }
            }
            else {
                brother = x->parent->left;
                if (brother->color == true) {
                    brother->color = false;
                    x->parent->color = true;
                    right_rotate(x->parent);
                    if (x->parent->parent != nil)
                        if (x->parent->parent->parent == nil) root = x->parent->parent;
                    brother = x->parent->left;
                }
                if (brother->left->color == false && brother->right->color == false) {
                    brother->color = true;
                    x = x->parent;
                }
                else {
                    if (brother->left->color == false) {
                        brother->right->color = false;
                        brother->color = true;
                        left_rotate(brother);
                        brother = x->parent->left;
                    }

                    brother->color = x->parent->color;
                    x->parent->color = false;
                    brother->left->color = false;
                    right_rotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = false;
    }
}