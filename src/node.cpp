#include "node.h"

Node::Node(int given_val, bool given_color, Node* given_parent, Node* given_left, Node* given_right) :
    value(given_val),
    color(given_color),
    parent(given_parent),
    left(given_left),
    right(given_right),
    observing(false) {}