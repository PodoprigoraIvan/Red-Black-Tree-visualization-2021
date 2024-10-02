#pragma once

class Node {
public:
    int value;
    bool color;
    Node* parent;
    Node* left;
    Node* right;
    bool observing;

    Node(int given_val, bool given_color, Node* given_parent, Node* given_left, Node* given_right);
};