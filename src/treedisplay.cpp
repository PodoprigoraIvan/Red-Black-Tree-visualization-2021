#include "tree.h"
#include <Windows.h>

#define WIDTH 1920
#define HEIGHT 1080

float r = 30;

void append_string_to_text(sf::Text& text, std::string s) {
    std::string tmp = text.getString();
    tmp += s;
    text.setString(tmp);
}

void display_tree(sf::RenderWindow& window, sf::Text& log, sf::Text& number, RBTree& tree) {
    sf::RectangleShape background(sf::Vector2f(WIDTH, HEIGHT));
    window.clear();
    window.draw(background);
    window.draw(log);
    window.draw(number);
    draw_subtree(window, tree.root, tree.nil, 0, WIDTH, 100);
    window.display();
}

void display_tree(sf::RenderWindow& window, sf::Text& log, RBTree& tree) {
    sf::RectangleShape background(sf::Vector2f(WIDTH, HEIGHT));
    window.clear();
    window.draw(background);
    window.draw(log);
    draw_subtree(window, tree.root, tree.nil, 0, WIDTH, 100);
    window.display();
}

void display_tree_with_waiting(sf::RenderWindow& window, sf::Text& log, RBTree& tree) {
    display_tree(window, log, tree);
    sf::sleep(sf::seconds(3.f));
}


void draw_subtree(sf::RenderWindow& window, Node* root, Node* nil, float start_x, float end_x, float y) {
    if (root == nullptr) return;
    float radius = r;
    sf::CircleShape circle(radius);
    if (root->observing) {
        circle.setOutlineThickness(radius / 4);
        circle.setOutlineColor(sf::Color(13, 122, 28));
    }
    float x = (end_x + start_x) / 2;
    circle.move(x - radius, y - radius);
    circle.setFillColor(sf::Color::Black);
    if (root->color)
        circle.setFillColor(sf::Color(190, 0, 0));
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(x, y);
    line[0].color = sf::Color::Blue;
    float x_left = (start_x + x) / 2;
    float x_right = (x + end_x) / 2;
    line[1].position = sf::Vector2f(x_left, y + 4 * radius);
    line[1].color = line[0].color;
    if (root->left != nil) {
        window.draw(line);
    }
    line[1].position = sf::Vector2f(x_right, y + 4 * radius);
    if (root->right != nil)
        window.draw(line);
    window.draw(circle);

    sf::Font font;
    double font_size = radius;
    font.loadFromFile("Montserrat-Regular.ttf");
    sf::Text text("", font, font_size);
    text.setFillColor(sf::Color::Green);
    text.setPosition(x - font_size / 2, y - font_size / 2);
    text.setString(std::to_string(root->value));
    window.draw(text);
    if (root->left != nil)
        draw_subtree(window, root->left, nil, start_x, x, y + 4 * radius);
    if (root->right != nil)
        draw_subtree(window, root->right, nil, x, end_x, y + 4 * radius);
}

Node* RBTree::find_element_and_display(int val, sf::RenderWindow& window, sf::Text& log) {
    log.setString("Begin finding element with value " + std::to_string(val));
    display_tree_with_waiting(window, log, *this);
    Node* p = root;
    Node* q = nil;
    while (p != nullptr && p != nil) {
        q = p;
        q->observing = true;
        if (p->value < val) {
            append_string_to_text(log, "\n" + std::to_string(val) + " > " + std::to_string(p->value) + " -> continue finding in right subtree");
            display_tree_with_waiting(window, log, *this);
            p = p->right;
            q->observing = false;
        }
        else if (p->value > val) {
            append_string_to_text(log, "\n" + std::to_string(val) + " < " + std::to_string(p->value) + " -> continue finding in left subtree");
            display_tree_with_waiting(window, log, *this);
            p = p->left;
            q->observing = false;
        }
        else if (p->value == val) {
            append_string_to_text(log, "\nElement found");
            display_tree_with_waiting(window, log, *this);
            q->observing = false;
            return p;
        }
    }
    append_string_to_text(log, "\nElement not found");
    display_tree_with_waiting(window, log, *this);
    return nullptr;
}

void RBTree::add_element_and_display(int val, sf::RenderWindow& window, sf::Text& log) {
    log.setString("Begin adding element with value " + std::to_string(val));
    display_tree_with_waiting(window, log, *this);
    Node* cur = new Node(val, true, nil, nil, nil);
    if (root == nullptr) {
        root = cur;
        root->color = false;
        append_string_to_text(log, "\nTree is empty, root is " + std::to_string(val));
        root->observing = true;
        display_tree_with_waiting(window, log, *this);
        root->observing = false;
        return;
    }
    else {
        append_string_to_text(log, "\nSearching place for element with value " + std::to_string(val));
        display_tree_with_waiting(window, log, *this);
        Node* p = root;
        Node* q = nil;
        while (p != nil) {
            q = p;
            q->observing = true;
            display_tree_with_waiting(window, log, *this);
            if (p->value <= cur->value) {
                append_string_to_text(log, "\n" + std::to_string(val) + " >= " + std::to_string(p->value) + " -> going right subtree");
                display_tree_with_waiting(window, log, *this);
                p = p->right;
            }
            else {
                append_string_to_text(log, "\n" + std::to_string(val) + " < " + std::to_string(p->value) + " -> going left subtree");
                display_tree_with_waiting(window, log, *this);
                p = p->left;
            }
            q->observing = false;
        }
        cur->parent = q;

        if (q->value <= cur->value) {
            append_string_to_text(log, "\n" + std::to_string(val) + " >= " + std::to_string(q->value) + " -> new element now is right child");
            q->right = cur;
            cur->observing = true;
            display_tree_with_waiting(window, log, *this);
            cur->observing = false;
        }
        else {
            append_string_to_text(log, "\n" + std::to_string(val) + " < " + std::to_string(q->value) + " -> new element now is left child");
            q->left = cur;
            cur->observing = true;
            display_tree_with_waiting(window, log, *this);
            cur->observing = false;
        }

        while (cur->parent->color == true) {
            cur->observing = true;
            append_string_to_text(log, "\nOur element is red and his parent is red, fixing it");
            display_tree_with_waiting(window, log, *this);
            if (cur->parent == cur->parent->parent->left) {
                if (cur->parent->parent->right->color == true) {
                    cur->parent->color = false;
                    cur->parent->parent->right->color = false;
                    cur->parent->parent->color = true;
                    append_string_to_text(log, "\nOur element have red uncle\nChanging colors of parent, uncle and grandparent, continue with grandparent");
                    display_tree_with_waiting(window, log, *this);
                    cur->observing = false;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->right) {
                        append_string_to_text(log, "\nOur element dont have red uncle\nParent is left child, our element is right child\nLeft rotate for parent");
                        cur->observing = false;
                        cur = cur->parent;
                        cur->observing = true;
                        display_tree_with_waiting(window, log, *this);
                        left_rotate(cur);
                    }
                    cur->parent->color = false;
                    cur->parent->parent->color = true;
                    append_string_to_text(log, "\nOur element dont have red uncle\nChanging colors of parent and grandparent\nRight rotate for grandparent");
                    display_tree_with_waiting(window, log, *this);
                    right_rotate(cur->parent->parent);
                }
            }
            else {
                if (cur->parent->parent->left->color == true) {
                    cur->parent->color = false;
                    cur->parent->parent->left->color = false;
                    cur->parent->parent->color = true;
                    append_string_to_text(log, "\nOur element have red uncle\nChanging colors of parent, uncle and grandparent, continue with grandparent");
                    display_tree_with_waiting(window, log, *this);
                    cur->observing = false;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->left) {
                        append_string_to_text(log, "\nOur element dont have red uncle\nParent is right child, our element is left child\nRight rotate for parent");
                        cur->observing = false;
                        cur = cur->parent;
                        cur->observing = true;
                        display_tree_with_waiting(window, log, *this);
                        right_rotate(cur);
                    }
                    cur->parent->color = false;
                    cur->parent->parent->color = true;
                    append_string_to_text(log, "\nOur element dont have red uncle\nChanging colors of parent and grandparent\nLeft rotate for grandparent");
                    display_tree_with_waiting(window, log, *this);
                    left_rotate(cur->parent->parent);
                }
            }
            display_tree_with_waiting(window, log, *this);
            cur->observing = false;
        }
        root->color = false;
        append_string_to_text(log, "\nParent is not red, adding complete\nMaking root black");
    }
}

void RBTree::delete_element_and_display(int val, sf::RenderWindow& window, sf::Text& log) {
    Node* to_delete = find_element_and_display(val, window, log);
    if (to_delete == nil || to_delete == nullptr) return;
    Node* y = to_delete;
    bool y_original_color = y->color;
    Node* x;
    if (to_delete == root && to_delete->left == nil && to_delete->right == nil) {
        log.setString("Tree only have one element, deleting it");
        display_tree_with_waiting(window, log, *this);
        root = nullptr;
        delete to_delete;
        return;
    }
    to_delete->observing = true;
    log.setString("");
    if (to_delete->left == nil) {
        log.setString("Removing element dont have left child.\nTransplant removing element and its right child");
        display_tree_with_waiting(window, log, *this);
        x = to_delete->right;
        transplant(to_delete, to_delete->right);
        if (x == nil)
            append_string_to_text(log, "\nSuccessor X is nil child");
        else {
            append_string_to_text(log, "\nSuccessor X is element with value " + std::to_string(x->value));
            x->observing = true;
        }
        display_tree_with_waiting(window, log, *this);
        x->observing = false;
    }
    else if (to_delete->right == nil) {
        log.setString("Removing element dont have right child.\nTransplant removing element and its left child");
        display_tree_with_waiting(window, log, *this);
        x = to_delete->left;
        transplant(to_delete, to_delete->left);
        if (x == nil)
            append_string_to_text(log, "\nSuccessor X is nil child");
        else {
            append_string_to_text(log, "\nSuccessor X is element with value " + std::to_string(x->value));
            x->observing = true;
        }
        display_tree_with_waiting(window, log, *this);
        x->observing = false;
    }
    else {
        y = to_delete->right;
        while (y->left != nil) y = y->left;
        y->observing = true;
        log.setString("Removing element heve both children,\nfinding the element with next value after removing element.\nIts element with value " + std::to_string(y->value)+". Lets call it Y");
        display_tree_with_waiting(window, log, *this);
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
        append_string_to_text(log, "\nElement with value " + std::to_string(to_delete->value) + " removed.\nNow the element Y with next value is on its place");
        display_tree_with_waiting(window, log, *this);
        y->observing = false;

        if (x == nil)
            append_string_to_text(log, "\nSuccessor X is nil right child of Y");
        else {
            append_string_to_text(log, "\nSuccessor X is element with value " + std::to_string(x->value));
            x->observing = true;
        }
        display_tree_with_waiting(window, log, *this);
        x->observing = false;
    }
    
    to_delete->observing = false;
    delete to_delete;

    bool end_flag = false;
    if (y_original_color == false) {
        append_string_to_text(log, "\nRemoved element was black or element with next value was black.\nFixing tree.");
        display_tree_with_waiting(window, log, *this);
        Node* brother;
        while (x != root && x->color == false) {
            if (x == nil) {
                append_string_to_text(log, "\nSuccessor element X is nil child of element with value " + std::to_string(x->parent->value));
                x->parent->observing = true;
                display_tree_with_waiting(window, log, *this);
                x->parent->observing = false;
            }
            else {
                append_string_to_text(log, "\nSuccessor element X is element with value " + std::to_string(x->value));
                x->observing = true;
                display_tree_with_waiting(window, log, *this);
                x->observing = false;
            }
            if (x == x->parent->left) {
                brother = x->parent->right;
                if (brother->color == true) {
                    brother->observing = true;
                    append_string_to_text(log, "\nCase 1: Brother of successor X is red, X - left child");
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    brother->color = false;
                    x->parent->color = true;
                    append_string_to_text(log, "\nMaking brother black and parent red.\nLeft rotate for parent");
                    x->parent->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    left_rotate(x->parent);
                    display_tree_with_waiting(window, log, *this);
                    x->parent->observing = false;
                    brother = x->parent->right;
                }
                if (brother->left->color == false && brother->right->color == false) {
                    append_string_to_text(log, "\nCase 2: Brother is black, both of brothers children are black");
                    brother->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    append_string_to_text(log, "\nMaking brother red");
                    brother->color = true;
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    append_string_to_text(log, "\nGoing for fixing to X parent");
                    x = x->parent;
                    x->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    x->observing = false;
                }
                else {
                    if (brother->right->color == false) {
                        append_string_to_text(log, "\nCase 3: X - left child, brother is black,\nbrothers left child is red, right child is black");
                        brother->observing = true;
                        display_tree_with_waiting(window, log, *this);
                        append_string_to_text(log, "\nMaking brother red and its left child black.\nRight rotate for brother");
                        display_tree_with_waiting(window, log, *this);
                        brother->left->color = false;
                        brother->color = true;
                        right_rotate(brother);
                        display_tree_with_waiting(window, log, *this);
                        append_string_to_text(log, "\nBecause of rotation brother of X changed.\nNow going for case 4");
                        brother->observing = false;
                        brother = x->parent->right;
                        brother->observing = true;
                        display_tree_with_waiting(window, log, *this);
                    }
                    append_string_to_text(log, "\nCase 4: X - left child, brother is black, brothers right child is red");
                    display_tree_with_waiting(window, log, *this);
                    append_string_to_text(log, "\nMaking brothers color the same as parent color,\nmaking parent and brothers right child black.\nLeft rotate for parent");
                    brother->color = x->parent->color;
                    x->parent->color = false;
                    brother->right->color = false;
                    left_rotate(x->parent);
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    append_string_to_text(log, "\nRemoving complete");
                    end_flag = true;
                    display_tree_with_waiting(window, log, *this);
                    x = root;
                }
            }
            else {
                brother = x->parent->left;
                if (brother->color == true) {
                    brother->observing = true;
                    append_string_to_text(log, "\nCase 1: Brother of successor X is red, X - right child");
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    brother->color = false;
                    x->parent->color = true;
                    append_string_to_text(log, "\nMaking brother black and parent red\nRight rotate for parent");
                    x->parent->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    right_rotate(x->parent);
                    display_tree_with_waiting(window, log, *this);
                    x->parent->observing = false;
                    brother = x->parent->left;
                }
                if (brother->left->color == false && brother->right->color == false) {
                    append_string_to_text(log, "\nCase 2: Both of brothers children are black");
                    brother->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    append_string_to_text(log, "\nMaking brother red");
                    brother->color = true;
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    append_string_to_text(log, "\nGoing for fixing to X parent.");
                    x = x->parent;
                    x->observing = true;
                    display_tree_with_waiting(window, log, *this);
                    x->observing = false;
                }
                else {
                    if (brother->left->color == false) {
                        append_string_to_text(log, "\nCase 3: X - right child, brother is black,\nbrothers right child is red, left child is black");
                        brother->observing = true;
                        display_tree_with_waiting(window, log, *this);
                        append_string_to_text(log, "\nMaking brother red and its right child black\nLeft rotate for brother");
                        display_tree_with_waiting(window, log, *this);
                        brother->right->color = false;
                        brother->color = true;
                        left_rotate(brother);
                        display_tree_with_waiting(window, log, *this);
                        append_string_to_text(log, "\nBecause of rotation brother of X changed\nNow going for case 4");
                        brother->observing = false;
                        brother = x->parent->left;
                        brother->observing = true;
                        display_tree_with_waiting(window, log, *this);
                    }
                    append_string_to_text(log, "\nCase 4: X - right child, brother is black,\nbrothers left child is red");
                    display_tree_with_waiting(window, log, *this);
                    append_string_to_text(log, "\nMaking brothers color the same as parent color,\nmaking parent and brothers left child black\nRight rotate for parent");
                    brother->color = x->parent->color;
                    x->parent->color = false;
                    brother->left->color = false;
                    right_rotate(x->parent);
                    display_tree_with_waiting(window, log, *this);
                    brother->observing = false;
                    append_string_to_text(log, "\nRemoving complete.");
                    end_flag = true;
                    display_tree_with_waiting(window, log, *this);
                    x = root;
                }
            }
        }
        append_string_to_text(log, "\nX is red or X is root\nMaking succesor element X black");
        x->observing = true;
        x->color = false;
        display_tree_with_waiting(window, log, *this);
        x->observing = false;
    }
    if (end_flag == false)
        append_string_to_text(log, "\nRemoving complete");
}