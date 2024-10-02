#include <iostream>
#include "../src/tree.h"

int count = 0;

void test1(){
    bool fail_flag = false;
    RBTree tree;
    if (tree.find_element(1) != nullptr) {
        std::cout << "Failed test in file " << __FILE__ << " on line " << __LINE__ << '\n';
        count++;
    }
}

void test2(){
    bool fail_flag = false;
    RBTree tree;
    tree.add_element(1);
    tree.add_element(2);
    tree.add_element(8);
    tree.add_element(5);
    if (tree.find_element(5) == nullptr) fail_flag = true;
    tree.delete_element(5);
    if (tree.find_element(5) != nullptr) fail_flag = true;
    if (fail_flag) {
        std::cout << "Failed test in file " << __FILE__ << " on line " << __LINE__ << '\n';
        count++;
    }
}

void test3(){
    bool fail_flag = false;
    RBTree tree;
    tree.add_element(1);
    tree.add_element(1);
    tree.add_element(1);
    tree.delete_element(1);
    tree.delete_element(1);
    if (tree.find_element(1) == nullptr) fail_flag = true;
    tree.delete_element(1);
    if (tree.find_element(1) != nullptr) fail_flag = true;
    if (fail_flag) {
        std::cout << "Failed test in file " << __FILE__ << " on line " << __LINE__ << '\n';
        count++;
    }
}

void test4(){
    bool fail_flag = false;
    RBTree tree;
    tree.add_element(5);
    tree.add_element(4);
    tree.add_element(3);
    tree.add_element(2);
    tree.add_element(10);
    tree.delete_element(4);
    tree.delete_element(10);
    tree.delete_element(5);
    tree.add_element(4);
    if (tree.find_element(4) == nullptr) fail_flag = true;
    if (tree.find_element(5) != nullptr) fail_flag = true;
    if (tree.find_element(10) != nullptr) fail_flag = true;
    if (fail_flag) {
        std::cout << "Failed test in file " << __FILE__ << " on line " << __LINE__ << '\n';
        count++;
    }
}

int main(){
    test1();
    test2();
    test3();
    test4();
    if (count == 0)
        std::cout << "#################\nPassed all tests!\n#################\n";
    return 0;
}