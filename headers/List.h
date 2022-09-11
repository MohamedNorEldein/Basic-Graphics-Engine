//
// Created by m7mdn on 7/25/2022.
//

#include <iostream>

#ifndef DATASTRUCTURES_CPP_LIST_H
#define DATASTRUCTURES_CPP_LIST_H

class InvalidIndex : std::exception {};

class EmptyList : std::exception {};

std::string space(int n);

/*
 * a list class based on tree structure, in ordered manner.
 * the class is templated as templates works for all types in the same way
 * thus saving speed and run time by increasing the build time.
 */



template<typename Type, int count = 10>
class Node {
public:
    int startCount, height;
    Type data[count];
    Node* left, * right;
};

#define node Node<Type,count>

template<typename Type, int count=10>
class List {
    // private types   
   

private:
    node* root;
    int length;

private:
    static int getCount() {
        return count;
    }

    static node* find_node(node* parent, int q) {
        if (q > parent->startCount)
            return find_node(parent->right, q);
        if (q < parent->startCount)
            return find_node(parent->left, q);
        return parent;
    }

    static Type &find_data(node* parent, int r) {
        return parent->data[r];
    }

    static int height(node* parent) {
        if (parent == nullptr) { return -1; }

        return parent->height;
    }

    static void set_height(node* parent) {
        if (parent == nullptr) { return; }

        parent->height = 1 + max(height(parent->right), height(parent->left));
    }

    static node* right_r(node* x) {
        node* y = x->right;
        node* b = y->left;
        y->left = x;
        x->right = b;
        set_height(b);
        set_height(x);

        return y;
    }

    static node* left_r(node* y) {
        node* x = y->left;
        node* b = x->right;
        y->left = b;
        x->right = y;
        set_height(b);
        set_height(y);
        return x;
    }

    static int balance(node* parent) {
        if (parent == nullptr)
            return 0;
        return height(parent->left) - height(parent->right);
    }

    static node* avl_function(node* parent) {

        if (balance(parent) > 1) {
            if (balance(parent->left) < 0) {
                parent->left = right_r(parent->left);
                parent = left_r(parent);
                set_height(parent);
                return parent;
            }
            parent = left_r(parent);
            set_height(parent);
            return parent;
        } else if (balance(parent) < -1) {
            if (balance(parent->right) > 0) {
                parent->right = (left_r(parent->right));
                parent = right_r(parent);
                set_height(parent);
                return parent;
            }
            parent = right_r(parent);
            set_height(parent);
            return parent;
        } else {
            set_height(parent);
            return parent;
        }
    }

    static node* push_s(node* parent, Type data) {
        if (parent == nullptr) {
            node* p = new node{0};
            return p;
        }
        if (parent->right == nullptr) {
            parent->right = new node{parent->startCount + 1, 0, {0}, nullptr, nullptr};
            parent->right->data[0] = data;
            return parent;
        }
        parent->right = push_s(parent->right, data);
        return avl_function(parent);
    }

    static void deleteNode(node* ptr) {
        if (ptr == nullptr)
            return;
        deleteNode(ptr->right);
        deleteNode(ptr->left);
        delete ptr;
    }

    static node* _pop(node* parent) {
        if (parent == nullptr) {
            throw EmptyList();
        }
        if (parent->right == nullptr) {
            delete parent->right;
            parent->right = nullptr;
            return parent;
        }
        parent->right = _pop(parent->right);
        return avl_function(parent);
    }

    static void print_data(node* parent) {
        std::cout << parent->startCount << ':';
        for (int i = 0; i < count; ++i) {
            std::cout << parent->data[i] << ' ';
        }
        std::cout << "\n";
    }

    static void print_node(node* parent) {
        if (!parent)
            return;

        print_node(parent->left);
        std::cout << space(40 - 5 * parent->height);
        print_data(parent);

        print_node(parent->right);

    }

public:

    List() :  length(0) {
        root=0;
    }

    ~List() {
        deleteNode(root);
    }

    Type &operator[](int i) {
        if (i < 0)
            throw InvalidIndex();
        if (i > length)
            throw InvalidIndex();

        return find_data(find_node(root, (i / count)), i % count);
    }

    void push(Type data) {
//    std::cout<<length<<'\n';
        if (length % count == 0) {

            root = avl_function(push_s(root, data));
            length += 1;
            return;
        }
        find_node(root, length / count)->data[length % count] = data;
        length++;
    }

    Type pop() {
        if (length % count == 0) {

            root = avl_function(_pop(root));
            length--;
            return find_node(root, length / count)->data[length % count];
        }

        length--;
        return find_node(root, length / count)->data[length % count];
    }


    void print() {
        print_node(root);
    }

    unsigned int Length() {
        return length;
    }

    template<typename Type, int count>
    friend class ListIterator;
};


template<typename Type, int count >
class ListIterator {
    node** nodeArray;
    int index,j,k,length;
public:
    ListIterator(List<Type, count>& list) {
        if (count != list.getCount()) {
            std::cout << "error\n";

        }
        length = list.length;
        nodeArray = new node * [(unsigned int)(2 * log(list.length / count)) + 1];
        //    std::cout << 2 * log(list.length / count) << '\n';
        index = 0;
        j = 0;
        setNode(list.root);
    }

    ~ListIterator() {
        delete nodeArray;
    }
private:
    void setNode(node* parent) {
        if (parent==0)
            return;
        nodeArray[j] = parent;
        j++;
        setNode(parent->left);
    }

    void nextNode() {
        // pop head
        j--;

        // push head-> right
        setNode(nodeArray[j]->right);
    }
public:
    void operator++() {
        ++k;
        if (index < count-1) {
            index++;
            return;
        }
        nextNode();
        index = 0;
    }

    Type get() {
        return nodeArray[j-1]->data[index];
    }

    unsigned int getIndex() {
        return k;
    }

    Type& operator()() {
        return nodeArray[j - 1]->data[index];
    }
 
};



#endif //DATASTRUCTURES_CPP_LIST_H
