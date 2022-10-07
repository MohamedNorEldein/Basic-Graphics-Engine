//
// Created by m7mdn on 7/25/2022.
//
#include <DoubleLinkedList.h>
namespace DoubleLinkedList {
//double linked list used to implement queue


    _list::_list() : head(nullptr), tail(nullptr) {
    }

    _list::_list(size_t size, short array_length) : head(nullptr), tail(nullptr) {
    }

    Node *_list::get_head() {
        return head;
    }

    Node *_list::get_tail() {
        return tail;
    }

    void _list::_push(unsigned char *data, size_t size, short array_length) {
        if (head == nullptr) {
            head = new DoubleLinkedList::Node;
            head->len = 0;
            head->data = new unsigned char[array_length * size];
            head->next = 0;
            head->back = 0;
            tail = head;

            __BUFFER::_push_head(head, data, size, array_length);
        } else {
            try {
                __BUFFER::_push_head(head, data, size, array_length);
            } catch (ArrayIsFilled &) {
//                std::cout << "new buffer is instantiated of length = " << array_length << std::endl;
//                nh = new DoubleLinkedList::Node{0, new unsigned char[array_length * size],head, nullptr};

                Node *nh = new DoubleLinkedList::Node;
                nh->len = 0;
                nh->data = new unsigned char[array_length * size];
                nh->next = head;
                nh->back = 0;

                head->back = nh;
                head = nh;
                __BUFFER::_push_head(head, data, size, array_length);
            }
        }
    }

    unsigned char *_list::_pop(size_t size, short array_length) {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else if (head != tail) {
            if (tail->len != 0) {
                tail->len--;
                tail->data += size;
                return tail->data - size;

            } else {

                tail = tail->back;
                delete[] (tail->next->data - array_length * size);
                delete tail->next;

                tail->next = nullptr;

                return _pop(size, array_length);
            }
        } else {
            try {
                return __BUFFER::_pop_tail(tail, size, array_length);
            } catch (ArrayIsEmpty &) {
                throw ListIsEmpty();
            }
        }
    }

    int delete_node(Node *head, Node *tail = nullptr) {

        if (head == tail) {
            return 0;
        } else {
            delete[] (head->data);
            delete_node(head->next, tail);
            delete head;
            return 0;
        }
    }

    _list::~_list() {
        delete_node(head);
    }
//------------------------------------------------------------------------------------
// double linked ring

    _ring::_ring(size_t size) : size(size), head(nullptr) {
    }

    _ring::~_ring() {
        if (head == nullptr) {
            return;
        }
        delete_node(head->next, head);
        delete[] head->data;
        delete head;
    }

    void _ring::_push_front(unsigned char *data) {
        if (head == nullptr) {
            head = new Node;
            head->len = 0;
            head->data = new unsigned char[size];

            head->next = head;
            head->back = head;
            __BUFFER::_push_head(head, data, size, 1);
        } else {
            Node *node = new Node;
            node->len = 0;
            node->data = new unsigned char[size];
            node->next = head->next;
            node->back = head;

            head->next = node;
            head->next->next->back = head->next;
            __BUFFER::_push_head(head->next, data, size, 1);
        }
    }


    unsigned char *_ring::_go_front() {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else {
            head = head->next;
            return head->data;
        }
    }

    unsigned char *_ring::_go_back() {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else {
            head = head->back;
            return head->data;
        }
    }


    unsigned char *_ring::_pop_front() {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else if (head == head->next) {
            unsigned char *h = head->data;
            head = nullptr;
            delete head;

            return h;
        } else {
            unsigned char *item;
            item = head->next->data;
            head->next = head->next->next;
            delete head->next->back;
            head->next->back = head;

            return item;
        }
    }

    unsigned char *_ring::_pop_back() {
        if (head == nullptr) {
            throw ListIsEmpty();
        } else if (head == head->back) {
            unsigned char *h = head->data;
            delete head;
            head = nullptr;
            return h;
        } else {
            unsigned char *item;
            item = head->back->data;
            head->back = head->back->back;
            delete head->back->next;
            head->back->next = head;

            return item;
        }
    }

}
