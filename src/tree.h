#pragma once
#include <SFML/Graphics.hpp>
#include "node.h"

class RBTree {
public:
    Node* root;
    Node* nil;
    RBTree();
    ~RBTree();
    void add_element(int val);
    void add_element_and_display(int val, sf::RenderWindow& window, sf::Text& log);
    Node* find_element(int val);
    Node* find_element_and_display(int val, sf::RenderWindow& window, sf::Text& log);
    void transplant(Node* u, Node* v);
    void delete_element(int val);
    void delete_element_and_display(int val, sf::RenderWindow& window, sf::Text& log);
    void left_rotate(Node* x);
    void right_rotate(Node* x);
};

extern float r;
void draw_subtree(sf::RenderWindow& window, Node* root, Node* nil, float start_x, float end_x, float y);
void display_tree(sf::RenderWindow& window, sf::Text& log, RBTree& tree);
void display_tree(sf::RenderWindow& window, sf::Text& log, sf::Text& number, RBTree& tree);
void display_tree_with_waiting(sf::RenderWindow& window, sf::Text& log, sf::Text& number, RBTree& tree);